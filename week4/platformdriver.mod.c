#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
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
__used
__attribute__((section("__versions"))) = {
	{ 0x516e49f9, "module_layout" },
	{ 0xe89992a5, "__platform_driver_register" },
	{ 0xebdab104, "__class_create" },
	{ 0xf55bc851, "platform_driver_unregister" },
	{ 0x52073aa0, "driver_remove_file" },
	{ 0x7b0a8b0e, "device_create_file" },
	{ 0x7c1318e3, "device_create" },
	{ 0x6c058549, "kmem_cache_alloc_trace" },
	{ 0xfa75b452, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0x3df0823f, "class_destroy" },
	{ 0xaf87d05c, "device_destroy" },
	{ 0x761270f0, "device_remove_file" },
	{ 0x7c32d0f0, "printk" },
	{ 0x91715312, "sprintf" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0x20c55ae0, "sscanf" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

