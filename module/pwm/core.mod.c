#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x1f3e07f7, "module_layout" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x8260686f, "bitmap_find_next_zero_area" },
	{ 0xff4a4852, "seq_open" },
	{ 0x4abdb61c, "seq_printf" },
	{ 0xe93e49c3, "devres_free" },
	{ 0x62b72b0d, "mutex_unlock" },
	{ 0xf803fe39, "bitmap_set" },
	{ 0x9cf43cb3, "debugfs_create_file" },
	{ 0xfeb1775d, "seq_read" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x27e1a049, "printk" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0xe16b893b, "mutex_lock" },
	{ 0xff5e1268, "devres_alloc" },
	{ 0x5d5b5a16, "radix_tree_delete" },
	{ 0x7dad71a5, "module_put" },
	{ 0x644bf570, "devres_release" },
	{ 0xa3b6d335, "devres_add" },
	{ 0xff1e9dd8, "seq_list_start" },
	{ 0xd2c4dee1, "seq_lseek" },
	{ 0x37a0cba, "kfree" },
	{ 0x870bf928, "radix_tree_lookup" },
	{ 0xfa9dcea7, "seq_release" },
	{ 0xe7d4daac, "seq_list_next" },
	{ 0xf202c5cb, "radix_tree_insert" },
	{ 0xc6f2711f, "try_module_get" },
	{ 0xd92afabe, "bitmap_clear" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "7297533D888D44A053B9081");
