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

static struct gpio_config gpio_port_cfg[10];
int gpio_open(struct inode *pstInode, struct file *pstFile)
{
    printk("gpio_open\r\n");
    return 0;
}

long gpio_ioctl(struct file *pstFile, unsigned int uiCmd, unsigned long ulArgc)
{
    printk("gpio_ioctl\r\n");
    return -1;
}

static ssize_t gpio_read(struct file *filp, char __user *buf, size_t size, loff_t *offset) 
{
    int i;
	char pin_name[10],buf_name[10],pin_value;
	for(i=0;i<10;i++)
	{
		if(buf[i]=='/'||buf[i]==0)
		{
			pin_name[i]=0;
	        pin_value=buf[i+1];
			break;
		}
		pin_name[i]=buf[i];
	}
	for(i=0;i<500;i++)
	{
		sunxi_gpio_to_name(i, buf_name);
		if(strcmp(buf_name,pin_name)==0)break;
	}
	gpio_direction_input(i);
	pin_value=gpio_get_value(i);
	char value='L';
	if(pin_value)value='H';
//	printk("get [%s][%d]=[%c]\r\n",pin_name,i,value);
    return pin_value;
}

static ssize_t gpio_write(struct file *filp, const char __user *buf, size_t size, loff_t *offset) 
{ 
    int i;
	char pin_name[10],buf_name[10],pin_value;
	for(i=0;i<10;i++)
	{
		if(buf[i]=='/'||buf[i]==0)
		{
			pin_name[i]=0;
	        pin_value=buf[i+1];
			break;
		}
		pin_name[i]=buf[i];
	}
	for(i=0;i<500;i++)
	{
		sunxi_gpio_to_name(i, buf_name);
		if(strcmp(buf_name,pin_name)==0)break;
	}
	
//	printk("set [%s][%d]=[%c]\r\n",pin_name,i,pin_value);
	gpio_direction_output(i,pin_value=='H');
	gpio_set_value(i,pin_value=='H');
    return i; 
} 
static struct file_operations gpio_ops =
{
    .owner = THIS_MODULE,
    .open = gpio_open,
    .read =gpio_read,
    .write=gpio_write,
    .unlocked_ioctl=gpio_ioctl,
};
static struct miscdevice gpio_dev = {
    .minor            = MISC_DYNAMIC_MINOR,
    .name            ="mygpio",
    .fops            = &gpio_ops,
};
static int __init gpio_init(void)
{
   // printk("gpio_init\r\n");
	//gpio_write(0,"PE15|L",0,0);
	//printk("gpio=%d\r\n",gpio_read(0,"PE15|H",0,0));
    misc_register(&gpio_dev);
    return 0;
}

static void __exit gpio_exit(void)
{
    printk("gpio_exit\r\n");
    misc_deregister(&gpio_dev);//注销设备
    return;
}


module_init(gpio_init);
module_exit(gpio_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sanby");
