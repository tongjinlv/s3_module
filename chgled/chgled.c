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
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/wakelock.h>

#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/slab.h>

#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/input.h>
#include <linux/mfd/axp-mfd.h>
#include <asm/div64.h>

#include <mach/sys_config.h>
#include <linux/arisc/arisc.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#include </root/SDK/build_s3evb/linux-3.4/drivers/power/axp_power/axp-cfg.h>
#include </root/SDK/build_s3evb/linux-3.4/drivers/power/axp_power/axp22-sply.h>
#include </root/SDK/build_s3evb/linux-3.4/drivers/power/axp_power/axp19/axp19-sply.h>

#endif
typedef unsigned int __u32;


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
    printk("chgled set tags: %d %d %d\n",tag1,tag2,tag3);
    return size;
}
static ssize_t pwm_show(struct device *dev,struct device_attribute *attr, char *buf)
{
     printk("matata d\n");
    return 0;
}

static DEVICE_ATTR(func, S_IRUGO|S_IWUSR|S_IWGRP,
		get_func, set_func);	
		
static struct attribute *pwm_ctl_attributes[] = {
	&dev_attr_func.attr,
	NULL
};

static struct attribute_group pwm_attribute_group = {
	.name = "chgled",
	.attrs =pwm_ctl_attributes,
};
static struct miscdevice misc_pwm={
	.minor = MISC_DYNAMIC_MINOR,
	.name = "misc_led",
};

static int __init chgled_init(void)
{
	printk("module chgled_init\n");
	misc_register(&misc_pwm);
	sysfs_create_group(&misc_pwm.this_device->kobj,&pwm_attribute_group);
}

static void __exit chgled_exit(void)
{
    printk("module chgled_exit\n");
   
	sysfs_remove_group(&misc_pwm.this_device->kobj,&pwm_attribute_group);
	misc_deregister(&misc_pwm);
}

module_init(chgled_init);
module_exit(chgled_exit);
MODULE_LICENSE("GPL");



