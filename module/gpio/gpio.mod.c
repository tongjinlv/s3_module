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
	{ 0x6c8d5ae8, "__gpio_get_value" },
	{ 0x65d6d0f0, "gpio_direction_input" },
	{ 0x432fd7f6, "__gpio_set_value" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x91715312, "sprintf" },
	{ 0x27e1a049, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "EB80B5F8F5D6FBDAC8B48D9");
