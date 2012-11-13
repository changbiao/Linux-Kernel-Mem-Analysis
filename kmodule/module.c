#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/linkage.h>

#define STORAGE 1024
#define BUF_LEN_I 256
#define BUF_LEN (1024*256)

/* Name of the proc file */
static char MODNAME[] = "mem_hurst";

struct my_mem_info
{
	unsigned int record;
	int pid;
	unsigned long address;
	unsigned long error_code;
};



/* Size of the buffer */

extern void (*STUB_do_page_fault_injection)(unsigned long, struct task_struct *, unsigned long);
void do_page_fault_injection(unsigned long address, struct task_struct *tsk, unsigned long error_code);

static unsigned int i = 0;
static int flag = 0;
struct my_mem_info data_store[STORAGE];

void do_page_fault_injection(unsigned long address, struct task_struct *tsk, unsigned long error_code)
{
	if(flag)
	{
		return;
	}
	int bit2 = error_code & 0x4 >> 2;
	if(!bit2)
	{
		return;
	}
	
	int pos = i % STORAGE;
	struct my_mem_info mi;
	mi.record = i;
	mi.pid = tsk->pid;
	mi.address = address;
	mi.error_code = error_code;
	data_store[pos] = mi;
	i++;

}

static int iteration = 0;
static int get_info(char *sys_buffer, char **my_buffer, off_t file_pos, int my_buffer_length)
{
    flag = 1;
    static char buffer[BUF_LEN];
    static int len;
    len = 0;
    int start = (iteration)*10;
    int finish =  (iteration+1)*10;

    printk(KERN_DEBUG "found offset to be  %d\n", file_pos);
    printk(KERN_DEBUG "found initial len to be  %d\n", len);

    unsigned int n = i;
    if(i > STORAGE)
    {
       n = STORAGE;
    }
    if(finish > n)
    {
	finish = n;
    }
     

    int j;
    memset(buffer, '\0', BUF_LEN);
    for(j = start; j < finish; j++)
    {
	int this_len = 0;
	char buffer_i[BUF_LEN_I];
	memset(buffer_i, '\0', BUF_LEN_I);
	unsigned int record = data_store[j].record;
	int pid_ = data_store[j].pid;
	unsigned long address_ = data_store[j].address;
	unsigned long error_code_ = data_store[j].error_code;
	int bit0 = error_code_ & 0x1;
	int bit1 = error_code_ & 0x2 >> 1;
	int bit2 = error_code_ & 0x4 >> 2;
	int bit3 = error_code_ & 0x8 >> 3;
	int bit4 = error_code_ & 0x10 >> 4;
        this_len = snprintf(buffer_i, BUF_LEN_I, "record: %u, pid: %d, address: %lu, bit0: %d, bit1: %d, bit2: %d, bit3: %d, bit4: %d\n", record, pid_, address_, bit0, bit1, bit2, bit3, bit4);
	strncat(buffer, buffer_i, BUF_LEN_I);
	len = len+this_len;
    }  

    printk(KERN_DEBUG "printing  %d to %d\n", start, finish);
    printk(KERN_DEBUG "found len to be  %d\n", len);

    *my_buffer = buffer;

    flag = 0;
    if(finish >= STORAGE || finish >= n)
    {
	iteration = 0;
        return 0;
    }
    iteration = iteration + 1;
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


