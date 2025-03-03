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
	{ 0x23c03251, "i2c_register_driver" },
	{ 0x639a2613, "device_destroy" },
	{ 0x9a3d88fa, "class_destroy" },
	{ 0x51ad2efc, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x4c8110b1, "sysfs_remove_group" },
	{ 0xd4925fdd, "devm_kmalloc" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x4332afa5, "sysfs_create_group" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xf6d7acc3, "cdev_init" },
	{ 0xf5193ee8, "cdev_add" },
	{ 0x8f4e90d7, "class_create" },
	{ 0x75f343e5, "device_create" },
	{ 0x2c6697b9, "_dev_info" },
	{ 0x846d2c82, "_dev_err" },
	{ 0xa9329c5c, "i2c_del_driver" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0xc440aa6f, "i2c_transfer_buffer_flags" },
	{ 0xf9a482f9, "msleep" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x96848186, "scnprintf" },
	{ 0xdcb764ad, "memset" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x126bac03, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:shtc3");
MODULE_ALIAS("of:N*T*Csensirion,shtc3");
MODULE_ALIAS("of:N*T*Csensirion,shtc3C*");

MODULE_INFO(srcversion, "855D4921A91FA30631479B3");
