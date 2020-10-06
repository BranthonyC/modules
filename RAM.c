#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>

#include <linux/swap.h>
#include <linux/swapfile.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>

#define FileProc "RAM_201503911"
struct sysinfo i;



static int show_memory_stat(struct seq_file *f, void *v){
    si_meminfo(&i); 
    seq_printf(f,"Total:%8lu\n", (i.totalram));
    seq_printf(f,"Libre:%8lu\n", (i.freeram));
    return 0;
}

static int meminfo_proc_open(struct inode *inode, struct file *file){
    return single_open(file, show_memory_stat, NULL);
}

static const struct file_operations Meminfo_fops = {
    .owner = THIS_MODULE,
    .open = meminfo_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};


static int __init start_function(void)
{
    proc_create(FileProc, 0, NULL, &Meminfo_fops);
    printk(KERN_INFO "Archivo Creado: /proc/%s.\n", FileProc);
    return 0;
}

static void __exit clean_function(void)
{
    remove_proc_entry(FileProc, NULL); 
    printk(KERN_INFO "Modulo Eliminado.\n");
}
 
module_init(start_function);
module_exit(clean_function);
 
MODULE_AUTHOR("Juan Pablo Osuna de Leon");
MODULE_DESCRIPTION("Modulo para el consumo de Ram.");
MODULE_LICENSE("GPL");
