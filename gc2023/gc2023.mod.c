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
	{ 0x4f81f02d, "cci_dev_exit_helper" },
	{ 0xe766812d, "cci_dev_init_helper" },
	{ 0xd7565690, "misc_register" },
	{ 0x17a142df, "__copy_from_user" },
	{ 0x9d669763, "memcpy" },
	{ 0x98082893, "__copy_to_user" },
	{ 0x791ab099, "v4l2_ctrl_query_fill" },
	{ 0xc5ae0182, "malloc_sizes" },
	{ 0xa78f630d, "cci_dev_probe_helper" },
	{ 0x8836fca7, "kmem_cache_alloc" },
	{ 0xb6b0fcdb, "v4l2_chip_ident_i2c_client" },
	{ 0xc314ea33, "vfe_get_standby_mode" },
	{ 0x4e8b1cb0, "cci_read_a8_d8" },
	{ 0xfb9324fe, "vfe_set_pmu_channel" },
	{ 0x6e6b9f1e, "vfe_gpio_set_status" },
	{ 0x6004e994, "cci_unlock" },
	{ 0x54b2490a, "vfe_set_mclk" },
	{ 0xf291ebc2, "vfe_set_mclk_freq" },
	{ 0x27a23634, "vfe_gpio_write" },
	{ 0x76f00564, "cci_lock" },
	{ 0x12a38747, "usleep_range" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xf9a482f9, "msleep" },
	{ 0x37a0cba, "kfree" },
	{ 0xfb9ff2e4, "cci_dev_remove_helper" },
	{ 0x27e1a049, "printk" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xc1d310f0, "cci_write_a8_d8" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=cci,vfe_subdev";

MODULE_ALIAS("i2c:gc2023_mipi_2lane");

MODULE_INFO(srcversion, "D3B9E7EF15872AEFD79A9F3");
