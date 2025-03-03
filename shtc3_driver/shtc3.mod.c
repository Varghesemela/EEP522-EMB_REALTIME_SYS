#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xcf435ce7, "device_destroy" },
	{ 0x6b8f7feb, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xfa31ad9, "sysfs_remove_group" },
	{ 0xdde25e1, "class_create" },
	{ 0x48d62cc5, "i2c_register_driver" },
	{ 0x378b3561, "class_destroy" },
	{ 0x122c3a7e, "_printk" },
	{ 0xf61c822d, "devm_kmalloc" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x10a5cd, "sysfs_create_group" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xfde7dc9b, "cdev_init" },
	{ 0x282d49c2, "cdev_add" },
	{ 0xd4ae550c, "device_create" },
	{ 0x2f19d425, "_dev_info" },
	{ 0xae383b88, "_dev_err" },
	{ 0x2b99903d, "i2c_del_driver" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0xbfb17dd8, "i2c_transfer_buffer_flags" },
	{ 0xf9a482f9, "msleep" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x96848186, "scnprintf" },
	{ 0xdcb764ad, "memset" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x773354b7, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:shtc3");
MODULE_ALIAS("i2c:shtc3_dome");
MODULE_ALIAS("of:N*T*Csensirion,shtc3");
MODULE_ALIAS("of:N*T*Csensirion,shtc3C*");

MODULE_INFO(srcversion, "F1486AFFBD61087B925BBE3");
