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
	{ 0xcc5676f, "cdev_del" },
	{ 0xe050bb90, "class_destroy" },
	{ 0x7b533daa, "device_destroy" },
	{ 0xf2641322, "platform_device_unregister" },
	{ 0x100f5e04, "platform_driver_unregister" },
	{ 0xe0185383, "platform_driver_register" },
	{ 0x59e9de0f, "platform_device_register" },
	{ 0x4b5c4698, "device_create" },
	{ 0xb7ab848c, "__class_create" },
	{ 0x3b570441, "cdev_add" },
	{ 0xaef971c6, "cdev_init" },
	{ 0x596073c3, "cdev_alloc" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x6069c16e, "script_get_pio_list" },
	{ 0xbc601ad6, "script_get_item" },
	{ 0x1fcd0939, "pwmchip_add" },
	{ 0xed7b523b, "dev_set_drvdata" },
	{ 0x76792d0, "dev_err" },
	{ 0xc264c8e9, "devm_kzalloc" },
	{ 0xfe990052, "gpio_free" },
	{ 0x1b68aba6, "pin_config_set" },
	{ 0x91715312, "sprintf" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xff76a929, "pwmchip_remove" },
	{ 0x66aaf7de, "dev_get_drvdata" },
	{ 0x27e1a049, "printk" },
	{ 0x9d669763, "memcpy" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0x2196324, "__aeabi_idiv" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "7C1669D28807542903D8057");
