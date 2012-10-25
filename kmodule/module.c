#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/linkage.h>

#define BUF_LEN 128

/* Name of the proc file */
static char MODNAME[] = "mem_hurst";

unsigned int record_;
int pid_;
unsigned long address_;
unsigned long error_code_;



/* Size of the buffer */

extern void (*STUB_do_page_fault_injection)(unsigned long, struct task_struct *, unsigned long);
void do_page_fault_injection(unsigned long address, struct task_struct *tsk, unsigned long error_code);

unsigned int i = 0;

void do_page_fault_injection(unsigned long address, struct task_struct *tsk, unsigned long error_code)
{
	record_ = i;
	pid_ = tsk->pid;
	address_ = address;
	error_code_ = error_code;
	i++;
}





static unsigned int last = 0;
static int get_info(char *sys_buffer, char **my_buffer, off_t file_pos, int my_buffer_length)
{

	static char buffer[BUF_LEN];
	static int len = 0;
	if(last == 0 || last != record_)
	{
		int bit0 = error_code_ & 0x1;
		int bit1 = error_code_ & 0x2 >> 1;
		int bit2 = error_code_ & 0x4 >> 2;
		int bit3 = error_code_ & 0x8 >> 3;
		int bit4 = error_code_ & 0x10 >> 4;
		len = sprintf(buffer, "record: %u pid: %d address: %lu bit0: %d bit1: %d bit2: %d bit3: %d bit4: %d\n", record_, pid_, address_, bit0, bit1, bit2, bit3, bit4);
		last = record_;
	}
	*my_buffer = buffer;
	return len;
}

int __init init_module(void)
{
	create_proc_info_entry(MODNAME, S_IFREG | S_IRUGO, &proc_root, get_info);
	STUB_do_page_fault_injection=&(do_page_fault_injection);
	return 0;
}

void __exit cleanup_module(void)
{
	remove_proc_entry(MODNAME, &proc_root);
	STUB_do_page_fault_injection=NULL;
}


