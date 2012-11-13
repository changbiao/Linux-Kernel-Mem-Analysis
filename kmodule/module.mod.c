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
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0xf1a3d2a2, "struct_module" },
	{ 0x25da070, "snprintf" },
	{ 0x1b7d4074, "printk" },
	{ 0xeaa5c7ae, "create_proc_entry" },
	{ 0xa8c9cad6, "STUB_do_page_fault_injection" },
	{ 0x7338aaf7, "remove_proc_entry" },
	{ 0x33bd0086, "proc_root" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "55C17CB7BC4ACD8291DC0A5");
