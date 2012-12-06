#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/linkage.h>
#include <linux/pagemap.h>

#define STORAGE 1024
#define BUF_LEN_I 512
#define BUF_LEN (1024*512)
#define LINES_PER_ITERATION 5

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

	if((error_code & (1<<2)) != 0x4)
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
static int lines_per_iteration = LINES_PER_ITERATION;
static int get_info(char *sys_buffer, char **my_buffer, off_t file_pos, int my_buffer_length)
{
    flag = 1;
    static char buffer[BUF_LEN];
    static int len;
    len = 0;
    int start = (iteration)*lines_per_iteration;
    int finish =  (iteration+1)*lines_per_iteration;

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
	int bit0 = 0;
	int bit1 = 0;
	int bit2 = 0;
	int bit3 = 0;
	int bit4 = 0;


	if(error_code_ & (1<<0))
	{
		bit0 = 1;
	}
	if(error_code_ & (1<<1))
	{
		bit1 = 1;
	}
	if(error_code_ & (1<<2))
	{
		bit2 = 1;
	}
	if(error_code_ & (1<<3))
	{
		bit3 = 1;
	}
	if(error_code_ & (1<<4))
	{
		bit4 = 1;
	}

	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;

	pgd = pgd_index(address_);
	pud = pud_offset(pgd, address_);
	pmd = pmd_offset(pud, address_);
//	pte = pte_offset_kernel(pmd, address_);
//
//	unsigned long pgd = pgd_index(address_);
//	unsigned long pud = pud_offset(&pgd, address_);
//	unsigned long pmd = pmd_offset(&pud, address_);
//	unsigned long pte = pte_offset_kernel(&pmd, address_);
//

	int pgd_i = (pgd >> 24);
	int pud_i = (pud >> 24);
	int pmd_i = (pmd >> 24);

        this_len = snprintf(buffer_i, BUF_LEN_I, "record: %u, pid: %d, address: %lu, bit0: %d, bit1: %d, bit2: %d, bit3: %d, bit4: %d, pgd: %d, pud: %d, pmd: %d\n", record, pid_, address_, bit0, bit1, bit2, bit3, bit4, pgd_i, pud_i, pmd_i);
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


