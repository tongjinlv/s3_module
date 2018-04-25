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
#define pwm_max 100




static void led_timer_func0(unsigned long data)
{
    static int count,pwm0_count,pwm0_add;
    mod_timer(&timer0, jiffies);
    __u32 reg2=pwm_max*51/50;
    reg2<<=16;
    if(pwm0_count<pwm_max*1/50)pwm0_add=1;
    if(pwm0_count>pwm_max)pwm0_add=0;
    if(pwm0_add)pwm0_count++;else pwm0_count--;
    switch(pwm0_mode)
    {
        case 0:reg2+=pwm_max;sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;
        case 1:reg2+=pwm0_count;sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;
        case 2:if(count++>40)count=0;if(count>20)reg2+=pwm_max;sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;
        case 3:if(pwm0_add)reg2+=0;else reg2+=pwm_max;sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;
        case 4:sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);break;
    }
}

static void led_timer_func1(unsigned long data)
{
    static int count,pwm0_count,pwm0_add;
    mod_timer(&timer1, jiffies);
    __u32 reg2=pwm_max*51/50;
    reg2<<=16;
    if(pwm0_count<pwm_max*1/50)pwm0_add=1;
    if(pwm0_count>pwm_max)pwm0_add=0;
    if(pwm0_add)pwm0_count++;else pwm0_count--;
    switch(pwm1_mode)
    {
        case 0:reg2+=pwm_max;sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;
        case 1:reg2+=pwm0_count;sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;
        case 2:if(count++>40)count=0;if(count>20)reg2+=pwm_max;sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;
        case 3:if(pwm0_add)reg2+=0;else reg2+=pwm_max;sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;
        case 4:sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);break;
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
    printk("matata %d %d\n",tag1,tag2);
    switch(tag1)
    {
        case 0:pwm0_mode=tag2;
        break;
        case 1:pwm1_mode=tag2;
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
	int err;
	printk("matata pwm_init\n");
    __u32 config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 2);
    pin_config_set(SUNXI_PINCTRL, "PB4", config);
    pin_config_set(SUNXI_PINCTRL, "PB5", config);
    sys_put_wvalue(SUNXI_PWM_VBASE+0,0x280050);
    pwm0_mode=0;
    pwm1_mode=0;
    init_timer(&timer0);
    init_timer(&timer1);
	timer0.function = led_timer_func0;
    timer1.function = led_timer_func1;
	mod_timer(&timer0, jiffies+10);
    mod_timer(&timer1, jiffies+10);
	misc_register(&misc_pwm);
	sysfs_create_group(&misc_pwm.this_device->kobj,&pwm_attribute_group);
}

static void __exit pwm_exit(void)
{
    printk("matata pwm_exit\n");
    del_timer(&timer0);
    del_timer(&timer1);
	sysfs_remove_group(&misc_pwm.this_device->kobj,&pwm_attribute_group);
	misc_deregister(&misc_pwm);
}

module_init(pwm_init);
module_exit(pwm_exit);
MODULE_LICENSE("GPL");



