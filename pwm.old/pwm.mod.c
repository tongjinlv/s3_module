#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x1f3e07f7, "module_layout" },
	{ 0xb6a3a1d5, "misc_deregister" },
	{ 0x4480a049, "sysfs_remove_group" },
	{ 0xbd738ed9, "sysfs_create_group" },
	{ 0xd7565690, "misc_register" },
	{ 0x7d11c268, "jiffies" },
	{ 0xc8fd727e, "mod_timer" },
	{ 0x132a7a5b, "init_timer_key" },
	{ 0x27e1a049, "printk" },
	{ 0xcca27eeb, "del_timer" },
	{ 0x20000329, "simple_strtoul" },
	{ 0x3facb673, "pwm_enable" },
	{ 0x67ce6821, "pwm_disable" },
	{ 0xd0535cca, "pwm_config" },
	{ 0x91715312, "sprintf" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "8D506FEDFFC12FFC68AF80D");
