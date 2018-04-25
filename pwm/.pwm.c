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
#define BIT(x) 1<<x
typedef unsigned int __u32;

static __u32 sunxi_pwm_read_reg(__u32 offset)
{
    __u32 value = 0;
    value = sys_get_wvalue(SUNXI_PWM_VBASE + offset);
    return value;
}

static __u32 sunxi_pwm_write_reg(__u32 offset, __u32 value)
{
    sys_put_wvalue(SUNXI_PWM_VBASE + offset, value);

    return 0;
}


int pwm_open(struct inode *pstInode, struct file *pstFile)
{
    printk("pwm_open\r\n");
    return 0;
}

long pwm_ioctl(struct file *pstFile, unsigned int uiCmd, unsigned long ulArgc)
{
    printk("pwm_ioctl\r\n");
    return -1;
}

static ssize_t pwm_read(struct file *filp, char __user *buf, size_t size, loff_t *offset) 
{
    printk("read%d\r\n",buf[0]);
    return 0;
}
static ssize_t pwm_write(struct file *filp, const char __user *buf, size_t size, loff_t *offset) 
{ 
    printk("write[%d]=%d\r\n",buf[0],buf[1]); 
    return 0; 
} 
static struct file_operations pwm_ops =
{
    .owner = THIS_MODULE,
    .open = pwm_open,
    .read =pwm_read,
    .write=pwm_write,
    .unlocked_ioctl=pwm_ioctl,
};
static struct miscdevice pwm_dev = {
    .minor            = MISC_DYNAMIC_MINOR,
    .name            ="mypwm",
    .fops            = &pwm_ops,
};
static int __init pwm_init(void)
{
    int a=0;
    int i=0;
    while(a<0x5ff)
    {
	char pin_name[255];
    printk("pwm_init\r\n");
    __u32 config;
    sunxi_gpio_to_name(36, pin_name);
    printk("pin_name=%s\r\n",pin_name);
    config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 2);
    pin_config_set(SUNXI_PINCTRL, pin_name, config);
    sunxi_gpio_to_name(37, pin_name);
    printk("pin_name=%s\r\n",pin_name);
    config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 2);
    pin_config_set(SUNXI_PINCTRL, pin_name, config);
    //sunxi_pwm_write_reg(0,0x0);
    __u32 reg2=0x05ff0000+a;
    sys_put_wvalue(SUNXI_PWM_VBASE+8,reg2);//PWM0
    sys_put_wvalue(SUNXI_PWM_VBASE+4,reg2);//PWM1
    __u32 reg1=0x280050;//
    printk("pwm_reg[%d]=%x\r\n",0,reg1);
    sys_put_wvalue(SUNXI_PWM_VBASE+0,reg1);
    /*__u32 reg0=sunxi_pwm_read_reg(0);
    if(reg0&BIT(29))printk("PWM1 busy\n");
    if(reg0&BIT(28))printk("PWM0 busy\n");
    if(reg0&BIT(24))printk("PWM1 24MHZ\n");
    if(reg0&BIT(23))printk("PWM1 output start\n");
    if(reg0&BIT(22))printk("PWM1 pulse mode\n");
    if(reg0&BIT(21))printk("PWM1 geting special clock\n");
    if(reg0&BIT(20))printk("PWM1 action state H\n");
    if(reg0&BIT(19))printk("PWM1 enable\n");
    printk("PWM1 prescal=%d%d%d%d\n",!!(reg0&BIT(18)),!!(reg0&BIT(17)),!!(reg0&BIT(16)),!!(reg0&BIT(15)));
    if(reg0&BIT(9))printk("PWM0 24MHZ\n");
    if(reg0&BIT(8))printk("PWM0 output start\n");
    if(reg0&BIT(7))printk("PWM0 pulse mode\n");
    if(reg0&BIT(6))printk("PWM0 geting special clock\n");
    if(reg0&BIT(5))printk("PWM0 action state H\n");
    if(reg0&BIT(4))printk("PWM0 enable\n");
    printk("PWM0 prescal=%d%d%d%d\n",!!(reg0&BIT(3)),!!(reg0&BIT(2)),!!(reg0&BIT(1)),!!(reg0&BIT(0)));
    printk("pwm_reg[%d]=%x\r\n",0,sunxi_pwm_read_reg(0));
    printk("PWM0 %d/%d\n",sunxi_pwm_read_reg(8)>>16,sunxi_pwm_read_reg(8)&0xffff);
    printk("pwm_reg[%d]=%x\r\n",8,sunxi_pwm_read_reg(8));
    printk("PWM1 %d/%d\n",sunxi_pwm_read_reg(4)>>16,sunxi_pwm_read_reg(4)&0xffff);
    printk("pwm_reg[%d]=%x\r\n",4,sunxi_pwm_read_reg(4));*/
    msleep(1);
    a+=10;
    }
    misc_register(&pwm_dev);
    return 0;
}
/*
[  955.768764] pwm_exit
[  955.800805] pwm_init
[  955.803461] pin_name=PB4
[  955.806454] PWM1 geting special clock
[  955.810789] PWM1 enable
[  955.813696] PWM1 prescal=0000
[  955.817075] PWM0 geting special clock
[  955.821272] PWM0 enable
[  955.824075] PWM0 prescal=1111
[  955.827538] pwm_reg[0]=28005f
[  955.831021] PWM0 2399/960
[  955.834014] pwm_reg[4]=95f03c0
[  955.837590] PWM1 479/377
[  955.840704] pwm_reg[8]=1df0179
*/
static void __exit pwm_exit(void)
{
    printk("pwm_exit\r\n");
    char pin_name[255];
    printk("pwm_init\r\n");
    __u32 config;
    sunxi_gpio_to_name(36, pin_name);
    printk("pin_name=%s\r\n",pin_name);
    config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 7);
    pin_config_set(SUNXI_PINCTRL, pin_name, config);
    sunxi_gpio_to_name(37, pin_name);
    printk("pin_name=%s\r\n",pin_name);
    config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 7);
    pin_config_set(SUNXI_PINCTRL, pin_name, config);
    misc_deregister(&pwm_dev);//注销设备
    return;
}


module_init(pwm_init);
module_exit(pwm_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sanby");
