#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/miscdevice.h>
#include <mach/hardware.h>
#include <linux/gpio.h>
#include <mach/sys_config.h>
#include <asm/system.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/irq.h>
#include <linux/pwm.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#define MAX           100
#define LOW_BRIGHT    0
#define HIGHT_BRIGHT  1
#define MIDIUM_BRIGHT  2 
#define ON_OFF        3
#define ADJEST_BRGHT  4
#define ENABLE    0
#define DISABLE   1

struct pwm_info{
		s32                     dev;
		u32                     channel;
		u32                     polarity;
		u32                     period_ns;
		u32                     duty_ns;
		u32                     enabled;
	};
	
struct pwm_info *led_pwm_info;
struct timer_list timer;
struct timer_list switch_timer;
static int   time_delay  =  0;
static int   switch_delay  = 0;


static int led_sys_pwm_request(u32 pwm_id)
{
	int ret = 0;
	struct pwm_device *pwm_dev;

	pwm_dev = pwm_request(pwm_id, "led");

	if(NULL == pwm_dev || IS_ERR(pwm_dev)) {
		printk("led_sys_pwm_request pwm %d fail!\n", pwm_id);
	} else {
		printk("led_sys_pwm_request pwm %d success!\n", pwm_id);
	}
	ret = (int)pwm_dev;
	return ret;
}

static int led_sys_pwm_config(int p_handler, int duty_ns, int period_ns)
{
	int ret = 0;
	struct pwm_device *pwm_dev;

	pwm_dev = (struct pwm_device *)p_handler;
	if(NULL == pwm_dev || IS_ERR(pwm_dev)) {
		printk("led_sys_pwm_Config, handle is NULL!\n");
		ret = -1;
	} else {
		ret = pwm_config(pwm_dev, duty_ns, period_ns);
		printk("led_sys_pwm_Config pwm %d, <%d / %d> \n", pwm_dev->pwm, duty_ns, period_ns);
	}
	return ret;
}

void set_pwm(int bright)
{
	unsigned duty_ns, period_ns ;
	
	period_ns =  1000000000 / 10000 ;
	//1000000
	//100000 * 10
	//95000
	//duty_ns = period_ns / 20 * bright * 10;
	//duty_ns = bright * MAX * 10;
	duty_ns = (period_ns * bright  + MAX /2 )/ MAX ;

	if (bright == 0) {
		pwm_config(led_pwm_info->dev, 0, period_ns);
		pwm_disable(led_pwm_info->dev);
	} else {
		pwm_config(led_pwm_info->dev, duty_ns, period_ns);
		pwm_enable(led_pwm_info->dev);
	}
}

static void led_timer_func(unsigned long data)
{
 
   static int  level = 0 ;
   static int pol = 1;
   
  // set_pwm(bright);
   if(pol == 1){
	   level = level +1;
	   if(level >=24){
		   level = 24;
		   pol = 0;
	   }
   }else{
	   level =level - 1;
	   if(level <= 0)
	   {
		   level = 0;
		   pol = 1;
	   }
   }
//   printk("bright = %d ---\n" , level);
#if 0
  switch(level){
		case 0:
		set_pwm(0);
		break;
		case 1:
		set_pwm(5);
		 break;
		case 2:
		set_pwm(10);
		 break;
		case 3:
		set_pwm(15);
		 break;
	   case  4:
	   set_pwm(20);
		 break;
	  case	5:
	   set_pwm(25);
		 break;
	   case  6:
	   set_pwm(30);
		 break;
		case  7:
	   set_pwm(35);
		 break;
	  case	8:
	   set_pwm(40);
		 break;   
	   case  9:
	   set_pwm(45);
		 break;
	  case	10:
	   set_pwm(50);
	  case	11:
	   set_pwm(55);
		 break;
	   case  12:
	   set_pwm(60);
		 break;
		case  13:
	   set_pwm(65);
		 break;
	  case	14:
	   set_pwm(70);
		 break;   
	   case  15:
	   set_pwm(75);
		 break; 
	   case  16:
	   set_pwm(80);
		 break;
	   case  17:
	   set_pwm(85);
		 break;
	   case  18:
	   set_pwm(87);
		 break;
	   case  19:
	   set_pwm(88);
		 break;   
	   case  20:
	   set_pwm(89);
		  break;
	   case  21:
	   set_pwm(90);
		 break;
		case  22:
	   set_pwm(91);
		 break;
	  case	23:
	   set_pwm(92);
		 break;   
	   case  24:
	   set_pwm(93); 
		 break;
	   case  25:
	   set_pwm(94); 
		 break;
	   case  26:
	   set_pwm(95); 
		 break;
	   case  27:
	   set_pwm(96); 
		 break;
	   case  28:
	   set_pwm(97); 
		 break;
	   case  29:
	   set_pwm(98); 
		 break;
	   case  30:
	   set_pwm(99); 
		 break;
	   case  31:
	   set_pwm(100); 
		 break;
  }
#endif
	  switch(level){
		case 0:
		set_pwm(0);
		break;
		case 31:
		set_pwm(5);
		 break;
		case 30:
		set_pwm(10);
		 break;
		case 29:
		set_pwm(15);
		 break;
	   case  28:
	   set_pwm(20);
		 break;
	  case	27:
	   set_pwm(25);
		 break;
	   case  26:
	   set_pwm(30);
		 break;
		case  25:
	   set_pwm(35);
		 break;
	  case	24:
	   set_pwm(40);
		 break;   
	   case  23:
	   set_pwm(45);
		 break;
	  case	22:
	   set_pwm(50);
	  case	21:
	   set_pwm(55);
		 break;
	   case  20:
	   set_pwm(60);
		 break;
		case  19:
	   set_pwm(65);
		 break;
	  case	18:
	   set_pwm(70);
		 break;   
	   case  17:
	   set_pwm(75);
		 break; 
	   case  16:
	   set_pwm(80);
		 break;
	   case  15:
	   set_pwm(85);
		 break;
	   case  14:
	   set_pwm(87);
		 break;
	   case  13:
	   set_pwm(88);
		 break;   
	   case  12:
	   set_pwm(89);
		  break;
	   case  11:
	   set_pwm(90);
		 break;
		case  10:
	   set_pwm(91);
		 break;
	  case	9:
	   set_pwm(92);
		 break;   
	   case  8:
	   set_pwm(93); 
		 break;
	   case  7:
	   set_pwm(94); 
		 break;
	   case  6:
	   set_pwm(95); 
		 break;
	   case  5:
	   set_pwm(96); 
		 break;
	   case  4:
	   set_pwm(97); 
		 break;
	   case  3:
	   set_pwm(98); 
		 break;
	   case  2:
	   set_pwm(99); 
		 break;
	   case  1:
	   set_pwm(100); 
		 break;
  }
  
  if(level == 24)
  {
    mod_timer(&timer, jiffies +  2 * HZ);
  }else if(level == 0){
	  mod_timer(&timer, jiffies + HZ/2); 
  }
  else{
  mod_timer(&timer, jiffies +  time_delay);
  }
	
}

static ssize_t pwm_store(struct device *dev,struct device_attribute *attr,const char *buf, size_t size)
{
	unsigned long data = 0;
	char *after;
	data = simple_strtoul(buf, &after, 10);
	printk("zay+++++++++++++++data = %d\n",data);
	 switch(data){
	  case 0:
	  set_pwm(0);
	  break;
	  case 1:
	  set_pwm(5);
	   break;
	  case 2:
	  set_pwm(10);
       break;
      case 3:
	  set_pwm(15);
       break;
     case  4:
	 set_pwm(20);
       break;
    case  5:
	 set_pwm(25);
       break;
     case  6:
	 set_pwm(30);
       break;
	  case  7:
	 set_pwm(35);
       break;
    case  8:
	 set_pwm(40);
       break;   
     case  9:
	 set_pwm(45);
       break;
    case  10:
	 set_pwm(50);
	case  11:
	 set_pwm(55);
       break;
     case  12:
	 set_pwm(60);
       break;
	  case  13:
	 set_pwm(65);
       break;
    case  14:
	 set_pwm(70);
       break;   
     case  15:
	 set_pwm(75);
	   break; 
	 case  16:
	 set_pwm(80);
       break;
     case  17:
	 set_pwm(85);
       break;
	 case  18:
	 set_pwm(87);
       break;
   	 case  19:
	 set_pwm(88);
       break;   
     case  20:
	 set_pwm(89);
	 	break;
	 case  21:
	 set_pwm(90);
       break;
	  case  22:
	 set_pwm(91);
       break;
    case  23:
	 set_pwm(92);
       break;   
     case  24:
	 set_pwm(93); 
	   break;
     case  25:
	 set_pwm(94); 
	   break;
     case  26:
	 set_pwm(95); 
	   break;
     case  27:
	 set_pwm(96); 
	   break;
     case  28:
	 set_pwm(97); 
	   break;
     case  29:
	 set_pwm(98); 
	   break;
     case  30:
	 set_pwm(99); 
	   break;
     case  31:
	 set_pwm(100); 
	   break;
  }
	return size;
}

int func_status ;
//读取led_func 的所处的功能状态
static ssize_t get_func(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf , "%d" , func_status);
}

int func_status ;
static ssize_t set_func(struct device *dev,struct device_attribute *attr,
		const char *buf, size_t size)
{
	char * after;
    func_status  = simple_strtoul(buf, &after, 10);
	
	if(time_delay != 0)
	{
		del_timer(&timer);
	}
	
	if(switch_delay != 0)
	{
	  del_timer(&switch_timer);
	}
	
	switch(func_status){
		case LOW_BRIGHT:
		  time_delay = 0;
		  switch_delay = 0 ;
		  set_pwm( 13);
		  break;
		 case  MIDIUM_BRIGHT :
		 printk("function = %d+++\n" , func_status);
		  time_delay = 0;
		  switch_delay = 0 ;
		  set_pwm(30);
		  break;
		case HIGHT_BRIGHT:
		 time_delay = 0;
		 switch_delay = 0;
		 set_pwm(70);
		 printk("function = %d+++\n" , func_status);
              break;
        case ON_OFF :
		  time_delay = 0;
		  set_pwm(60);
		  switch_delay = HZ / 3;
           break;
       case ADJEST_BRGHT:
	         time_delay = HZ / 20 ;
		   
		  switch_delay = 0;
          break;	   
	}
	
	if(time_delay)
	{
		init_timer(&timer);
		timer.function = led_timer_func;
		mod_timer(&timer, jiffies +  time_delay );
	}
	
	//if(switch_delay)
	//{
	//	init_timer(&switch_timer);
		//switch_timer.function = switch_timer_func;
	//	mod_timer(&switch_timer, jiffies +  switch_delay );
	//}
	
   return size;
}

static ssize_t pwm_show(struct device *dev,struct device_attribute *attr, char *buf)
{
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
	.name = "mis_pwm",
};

static int __init pwm_init(void)
{
	int err;
	printk("zhb+++++++pwm_init\n");
	
	//led_pwm_info = (struct pwm_info *)kmalloc(sizeof(struct pwm_info),GFP_KERNEL | __GFP_ZERO);
	//led_pwm_info->dev = led_sys_pwm_request(0);

	err = misc_register(&misc_pwm);
	if(err < 0)
	{
		pr_err("%s pwm mis register driver as misc device error\n", __FUNCTION__);
		goto exit;
	}
	
	err=sysfs_create_group(&misc_pwm.this_device->kobj,&pwm_attribute_group);
	if(err < 0){
		pr_err("%s sysfs_create_group error\n",__FUNCTION__);
		goto exit;
	}
	
	exit:
		return err;
}

static void __exit pwm_exit(void)
{
	sysfs_remove_group(&misc_pwm.this_device->kobj,&pwm_attribute_group);
	misc_deregister(&misc_pwm);
}

module_init(pwm_init);
module_exit(pwm_exit);
MODULE_LICENSE("GPL");



