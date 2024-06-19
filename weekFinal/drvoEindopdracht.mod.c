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
	{ 0x3df0823f, "class_destroy" },
	{ 0xf55bc851, "platform_driver_unregister" },
	{ 0xa3287a5d, "i2c_put_adapter" },
	{ 0x7b0a8b0e, "device_create_file" },
	{ 0x7c1318e3, "device_create" },
	{ 0x6c058549, "kmem_cache_alloc_trace" },
	{ 0xfa75b452, "kmalloc_caches" },
	{ 0xde72f9f, "i2c_new_device" },
	{ 0x73e20c1c, "strlcpy" },
	{ 0x5f754e5a, "memset" },
	{ 0x73e1d3e2, "i2c_get_adapter" },
	{ 0xccdf3fc8, "of_property_read_variable_u32_array" },
	{ 0xc4021e53, "_dev_err" },
	{ 0xb81960ca, "snprintf" },
	{ 0xd286d2a6, "i2c_transfer_buffer_flags" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0x1ee793bd, "i2c_smbus_write_byte" },
	{ 0x11a13e31, "_kstrtol" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x728d218, "i2c_unregister_device" },
	{ 0x37a0cba, "kfree" },
	{ 0xaf87d05c, "device_destroy" },
	{ 0x761270f0, "device_remove_file" },
	{ 0x7c32d0f0, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

