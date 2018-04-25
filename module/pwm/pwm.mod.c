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
	{ 0xd7565690, "misc_register" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0xd0535cca, "pwm_config" },
	{ 0x6069c16e, "script_get_pio_list" },
	{ 0xbc601ad6, "script_get_item" },
	{ 0xfe990052, "gpio_free" },
	{ 0x1b68aba6, "pin_config_set" },
	{ 0x91715312, "sprintf" },
	{ 0x432fd7f6, "__gpio_set_value" },
	{ 0x27e1a049, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "42DF49764800E9422F53E56");
