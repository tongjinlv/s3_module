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
#define PWM_NUM 2
#define PWM_REG_NUM 3

typedef unsigned int __u32;

__u32 record_reg[PWM_REG_NUM];
struct sunxi_pwm_cfg {
    script_item_u *list;
};

struct sunxi_pwm_chip {
    struct pwm_chip chip;
};

struct sunxi_pwm_cfg pwm_cfg[4];

__u32 pwm_active_sta[4] = {1, 0, 0, 0};
__u32 pwm_pin_count[4] = {0};
static struct gpio_config pwm_port_cfg[10];
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
void sunxi_pwm_get_sys_config(int pwm, struct sunxi_pwm_cfg *sunxi_pwm_cfg)
{
    char primary_key[25];
    script_item_u   val;
    script_item_value_type_e  type;

    sprintf(primary_key, "pwm%d_para", pwm);
    sunxi_pwm_cfg->list = NULL;
    type = script_get_item(primary_key, "pwm_used", &val);
    if((SCIRPT_ITEM_VALUE_TYPE_INT == type) && (val.val == 1)) {
        pwm_pin_count[pwm] = script_get_pio_list(primary_key, &sunxi_pwm_cfg->list);
        if(pwm_pin_count[pwm] == 0)
            pr_warn("get pwm%d gpio list fail!\n", pwm);
    }
	pr_warn("pwm_pin_count=%d\n", pwm_pin_count[pwm]);
}
static int sunxi_pwm_set_polarity(struct pwm_chip *chip, struct pwm_device *pwm, enum pwm_polarity polarity)
{
    __u32 temp;

    temp = sunxi_pwm_read_reg(0);
    if(polarity == PWM_POLARITY_NORMAL) {
        pwm_active_sta[pwm->pwm] = 1;
        if(pwm->pwm == 0)
            temp |= 1 << 5;
        else
            temp |= 1 << 20;
        }else {
            pwm_active_sta[pwm->pwm] = 0;
            if(pwm->pwm == 0)
                temp &= ~(1 << 5);
            else
                temp &= ~(1 << 20);
            }

	sunxi_pwm_write_reg(0, temp);

    return 0;
}
static int sunxi_pwm_config()
{
   int duty_ns=1000;
   int period_ns=193747;
    __u32 pre_scal[11][2] = {{15, 1}, {0, 120}, {1, 180}, {2, 240}, {3, 360}, {4, 480}, {8, 12000}, {9, 24000}, {10, 36000}, {11, 48000}, {12, 72000}};
    __u32 freq=1000;
    __u32 pre_scal_id = 0;
    __u32 entire_cycles = 256;
    __u32 active_cycles = 192;
    __u32 entire_cycles_max = 65536;
    __u32 temp;
    entire_cycles = 24000000 / freq / pre_scal[pre_scal_id][1];

    while(entire_cycles > entire_cycles_max) {
        pre_scal_id++;

        if(pre_scal_id > 10)
            break;

        entire_cycles = 24000000 / freq / pre_scal[pre_scal_id][1];
        }

    if(period_ns < 5*100*1000)
        active_cycles = (duty_ns * entire_cycles + (period_ns/2)) /period_ns;
    else if(period_ns >= 5*100*1000 && period_ns < 6553500)
        active_cycles = ((duty_ns / 100) * entire_cycles + (period_ns /2 / 100)) / (period_ns/100);
    else
        active_cycles = ((duty_ns / 10000) * entire_cycles + (period_ns /2 / 10000)) / (period_ns/10000);

    temp = sunxi_pwm_read_reg(0);

    if(0 == 0)
        temp = (temp & 0xfffffff0) |pre_scal[pre_scal_id][0];
    else
        temp = (temp & 0xfff87fff) |pre_scal[pre_scal_id][0];

    sunxi_pwm_write_reg(0, temp);

    sunxi_pwm_write_reg((0+ 1)  * 0x04, ((entire_cycles - 1)<< 16) | active_cycles);

    printk("PWM _TEST: duty_ns=%d, period_ns=%d, freq=%d, per_scal=%d, period_reg=0x%x\n", duty_ns, period_ns, freq, pre_scal_id, temp);


    return 0;
}
static int sunxi_pwm_enable()
{
    __u32 temp;
    int i;
    __u32 ret = 0;
    char pin_name[255];
    __u32 config;

    for(i = 0; i < pwm_pin_count[0]; i++) {
        if(!IS_AXP_PIN(pwm_cfg[0].list[i].gpio.gpio)) {
            sunxi_gpio_to_name(pwm_cfg[0].list[i].gpio.gpio, pin_name);
            config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, pwm_cfg[0].list[i].gpio.mul_sel);
            pin_config_set(SUNXI_PINCTRL, pin_name, config);
        }
        else {
            pr_warn("this is axp pin!\n");
        }

        gpio_free(pwm_cfg[0].list[i].gpio.gpio);
    }
    temp = sunxi_pwm_read_reg(0);
    if(0 == 0) {
        temp |= 1 << 4;
        temp |= 1 << 6;
        } else {
            temp |= 1 << 19;
            temp |= 1 << 21;
            }

    sunxi_pwm_write_reg(0, temp);
    return 0;
}

static void sunxi_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
    __u32 temp;
    int i;
    __u32 ret = 0;
    char pin_name[255];
    __u32 config;

    for(i = 0; i < pwm_pin_count[pwm->pwm]; i++) {
        ret = gpio_request(pwm_cfg[pwm->pwm].list[i].gpio.gpio, NULL);
        if(ret != 0) {
            pr_warn("pwm gpio %d request failed!\n", pwm_cfg[pwm->pwm].list[i].gpio.gpio);
        }
        if(!IS_AXP_PIN(pwm_cfg[pwm->pwm].list[i].gpio.gpio)) {
            sunxi_gpio_to_name(pwm_cfg[pwm->pwm].list[i].gpio.gpio, pin_name);
            config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0x7);
            pin_config_set(SUNXI_PINCTRL, pin_name, config);
        }
        else {
            pr_warn("this is axp pin!\n");
        }

        gpio_free(pwm_cfg[pwm->pwm].list[i].gpio.gpio);
    }
    temp = sunxi_pwm_read_reg(0);
    if(pwm->pwm == 0) {
        temp &= ~(1 << 4);
        temp &= ~(1 << 6);
        } else {
            temp &= ~(1 << 19);
            temp &= ~(1 << 21);
            }
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
    gpio_set_value(pwm_port_cfg[0].gpio,0);
    return 0;
}
static ssize_t pwm_write(struct file *filp, const char __user *buf, size_t size, loff_t *offset) 
{ 
    printk("write[%d]=%d\r\n",buf[0],buf[1]); 
    gpio_set_value(pwm_port_cfg[buf[0]].gpio,buf[1]);
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
struct pwm_device *pwm_dev_2;
struct pwm_device *pwm_dev_3;
static int __init pwm_init(void)
{
    int i=0;
	char pin_name[255];
	pwm_config(pwm_dev_2,200000,500000);
    printk("pwm_init\r\n");
	for(i=0; i<PWM_NUM; i++) {
        sunxi_pwm_get_sys_config(i, &pwm_cfg[i]);
    }
	gpio_direction_output(36,1);
	sunxi_pwm_config();
	sunxi_pwm_enable();
    misc_register(&pwm_dev);
    return 0;
}

static void __exit pwm_exit(void)
{
    printk("pwm_exit\r\n");
    misc_deregister(&pwm_dev);//注销设备
    return;
}


module_init(pwm_init);
module_exit(pwm_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sanby");
