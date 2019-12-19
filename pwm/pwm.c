#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <mach/hardware.h>
#include <mach/sys_config.h>
#include <mach/irqs.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include <linux/pwm.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/fs.h>
#include<linux/device.h>
#include<linux/pinctrl/pinconf.h>
#include <linux/pinctrl/pinconf-sunxi.h>
#include <linux/pinctrl/consumer.h>
#include "mach/platform.h"
#include "mach/sys_config.h"
#define sys_get_wvalue(n)   (*((volatile __u32 *)(n)))          /* word input */
#define sys_put_wvalue(n,c) (*((volatile __u32 *)(n))  = (c))   /* word output */
typedef unsigned int __u32;
struct timer_list timer0,timer1;
int pwm0_mode,pwm1_mode;
int pwm0_value,pwm1_value;
int pwm_max=100;
int PIN4_NUMBER,PIN5_NUMBER;
int gpio_name2pin(char *pin_name)
{
    int i;
    char buf_name[10];
	for(i=0;i<500;i++)
	{
		sunxi_gpio_to_name(i, buf_name);
		if(strcmp(buf_name,pin_name)==0)return i;
	}
    return i;
}
static void toggle_led0()
{
    __u32 reg2=pwm_max;
    reg2<<=16;
    if(sys_get_wvalue(SUNXI_PWM_VBASE+8)&0xffff)reg2+=0;else reg2+=pwm_max+10;sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);
}
static void toggle_led1()
{
    __u32 reg2=pwm_max;
    reg2<<=16;
    if(sys_get_wvalue(SUNXI_PWM_VBASE+4)&0xffff)reg2+=0;else reg2+=pwm_max+10;sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);
}

static void led_timer_func0(unsigned long data)
{
    static int count,pwm0_count,pwm0_add,toggle,count1;
    mod_timer(&timer0, jiffies);
    u32 reg2=pwm_max*51/50;
    reg2<<=16;
    if(pwm0_count<pwm_max*1/50)pwm0_add=1;
    if(pwm0_count>pwm_max)pwm0_add=0;
    if(pwm0_add)pwm0_count++;else pwm0_count--;
    switch(pwm0_mode)
    {
        case 0:reg2+=pwm_max+10;sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;//熄灭
        case 1:reg2+=pwm0_count;sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;//呼吸
        case 2:if(count++>40)count=0;if(count>20)reg2+=pwm_max;sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;//快闪
        case 3:if(pwm0_add)reg2+=0;else reg2+=pwm_max;sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;//慢闪
        case 4:sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;//长亮
        case 7:sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2+pwm0_value);if(pwm0_value<100)pwm0_value++;break;//褪色
        case 8:sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2+pwm0_value);if(pwm0_value>0)pwm0_value--;break;//逐渐明朗
        case 9:if(count1++>=pwm0_value){count1=0;toggle_led0();};break;//闪烁频率
		case 10:if(count++>100)count=0;
		     if(count>0&&count<10)reg2+=pwm_max;
			 if(count>10&&count<20)reg2+=0;
			 if(count>20&&count<30)reg2+=pwm_max;
			 if(count>30&&count<40)reg2+=0;
			  if(count>40)reg2+=pwm_max;
		     sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;//双闪
    }
}
static void led_set_func0()
{
    __u32 reg2=pwm_max*51/50;
    reg2<<=16;
    switch(pwm0_mode)
    {
        case 0:pwm_max=100;break;
        case 1:pwm_max=pwm0_value;break;
        case 2:pwm_max=100;break;
        case 3:pwm_max=100;break;
        case 4:pwm_max=100;break;
        case 5:toggle_led0();break;//翻转
        case 6:sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2+pwm0_value);break;//调亮度
        case 7:if(pwm0_value>100)pwm0_value=100;pwm0_value=100-pwm0_value;break;
        case 8:if(pwm0_value>100)pwm0_value=100;pwm0_value=100-pwm0_value;break;
    }
}
static void led_set_func1()
{
    __u32 reg2=pwm_max*51/50;
    reg2<<=16;
    switch(pwm1_mode)
    {
        case 0:pwm_max=100;break;
        case 1:pwm_max=pwm1_value;break;
        case 2:pwm_max=100;break;
        case 3:pwm_max=100;break;
        case 4:pwm_max=100;break;
        case 5:toggle_led1();break;//翻转
        case 6:sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2+pwm1_value);break;//调亮度
        case 7:if(pwm1_value>100)pwm1_value=100;pwm1_value=100-pwm1_value;break;
        case 8:if(pwm1_value>100)pwm1_value=100;pwm1_value=100-pwm1_value;break;
    }
}
static void led_timer_func1(unsigned long data)
{
    static int count,pwm0_count,pwm0_add,count1;
    mod_timer(&timer1, jiffies);
    __u32 reg2=pwm_max*51/50;
    reg2<<=16;
    if(pwm0_count<pwm_max*1/50)pwm0_add=1;
    if(pwm0_count>pwm_max)pwm0_add=0;
    if(pwm0_add)pwm0_count++;else pwm0_count--;
    switch(pwm1_mode)
    {
        case 0:reg2+=pwm_max+10;sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;
        case 1:reg2+=pwm0_count;sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;
        case 2:if(count++>40)count=0;if(count>20)reg2+=pwm_max;sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;
        case 3:if(pwm0_add)reg2+=0;else reg2+=pwm_max;sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;
        case 4:sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;
        case 7:sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2+pwm1_value);if(pwm1_value<100)pwm1_value++;break;//褪色
        case 8:sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2+pwm1_value);if(pwm1_value>0)pwm1_value--;break;//逐渐明朗
        case 9:if(count1++>=pwm1_value){count1=0;toggle_led1();};break;//闪烁频率
		case 10:if(count++>100)count=0;
		     if(count>0&&count<10)reg2+=pwm_max;
			 if(count>10&&count<20)reg2+=0;
			 if(count>20&&count<30)reg2+=pwm_max;
			 if(count>30&&count<40)reg2+=0;
			 if(count>40)reg2+=pwm_max;
		     sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;//双闪
    }
}

static ssize_t pwm_store(struct device *dev,struct device_attribute *attr,const char *buf, size_t size)
{
    printk("matata a\n");
    return 0;
}
static ssize_t get_func(struct device *dev,struct device_attribute *attr, char *buf)
{
    printk("matata b\n");
    return 0;
}
static ssize_t set_func(struct device *dev,struct device_attribute *attr,const char *buf, size_t size)
{
    char * after;
    int tag1  = simple_strtoul(buf, &after, 10);
    int tag2  = simple_strtoul(&buf[2], &after, 10);
    int tag3  = simple_strtoul(&buf[4], &after, 10);
    //printk("pwm set tags: %d %d %d\n",tag1,tag2,tag3);
    switch(tag1)
    {
        case 0:pwm0_mode=tag2;pwm0_value=tag3;
        led_set_func0();
        break;
        case 1:pwm1_mode=tag2;pwm1_value=tag3;
        led_set_func1();
        break;
        default:printk("please put 0 1\n");
        break;
    }
    return size;
}
static ssize_t pwm_show(struct device *dev,struct device_attribute *attr, char *buf)
{
     printk("matata d\n");
    return 0;
}
static DEVICE_ATTR(bright, S_IRUGO|S_IWUSR|S_IWGRP,
		pwm_show, pwm_store);
static DEVICE_ATTR(func, S_IRUGO|S_IWUSR|S_IWGRP,
		get_func, set_func);	
		
static struct attribute *pwm_ctl_attributes[] = {
	&dev_attr_bright.attr,
	&dev_attr_func.attr,
	NULL
};

static struct attribute_group pwm_attribute_group = {
	.name = "pwm",
	.attrs =pwm_ctl_attributes,
};
static struct miscdevice misc_pwm={
	.minor = MISC_DYNAMIC_MINOR,
	.name = "misc_pwm",
};

static int __init pwm_init(void)
{
	__u32 reg2=0;
	printk("module pwm_init1\n");
    __u32 config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 2);
    pin_config_set(SUNXI_PINCTRL, "PB4", config);
    pin_config_set(SUNXI_PINCTRL, "PB5", config);
    sys_put_wvalue(SUNXI_PWM_VBASE+0,0x280050);
    pwm0_mode=4;
    pwm1_mode=4;
    init_timer(&timer0);
    init_timer(&timer1);
	timer0.function = led_timer_func0;
    timer1.function = led_timer_func1;
	mod_timer(&timer0, jiffies+10);
    mod_timer(&timer1, jiffies+10);
	misc_register(&misc_pwm);
    PIN4_NUMBER=gpio_name2pin("PB4");
    PIN4_NUMBER=gpio_name2pin("PB4");
    reg2<<=16;
	sysfs_create_group(&misc_pwm.this_device->kobj,&pwm_attribute_group);
	return 0;
}

static void __exit pwm_exit(void)
{
    printk("module pwm_exit1\n");
    del_timer(&timer0);
    del_timer(&timer1);
	sysfs_remove_group(&misc_pwm.this_device->kobj,&pwm_attribute_group);
	misc_deregister(&misc_pwm);
}

module_init(pwm_init);
module_exit(pwm_exit);
MODULE_LICENSE("GPL");


//echo 0 9 1 > /sys/class/misc/misc_pwm/pwm/func 2> /dev/null

