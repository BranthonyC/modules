#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/module.h>
#include <linux/kernel.h>	
#include <linux/init.h>		
#include <linux/sched.h>    
#include <linux/sched/signal.h>
#include <linux/unistd.h>      
#include <linux/ktime.h>
#include <linux/sysinfo.h>

#define BUFSIZE 150

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brandon Antony Chitay Coutiño");
MODULE_DESCRIPTION("Porcentaje de uso de del CPU.");


u64 uptime;
u64 total_time;
u64 seconds;
u64 cpu_usage;

struct task_struct *task; 
struct task_struct *task_child;
struct list_head *list;


 
static int write_proc_file(struct seq_file * proc_file,void *v){
	uptime = 350735.47;
	seq_printf(proc_file, "{\n\"procesos\":[\n");
	for_each_process( task ){            /*    for_each_process() MACRO for iterating through each task in the os located in linux\sched\signal.h    */
		total_time = task->utime + task->stime;
		seconds = task->utime - (task->start_time/100);
		cpu_usage = 100 * ((total_time/100)/seconds);
		if(task->utime>0){
			seq_printf(proc_file, "{\"Name\": \"%s\", \"Utime\": %lli, \"Stime\": %lli, \"Start_Time\":%lli  },\n",
			task->comm, task->utime, task->stime, task->start_time);/*    log parent id/executable name/state */
		}
    }
	seq_printf(proc_file, "]\n}\n");
	return 0;
    //
}

static int on_open(struct inode *inode, struct file *file){
    return single_open(file, write_proc_file,NULL);
}

static struct file_operations processes = 
{
    .open = on_open,
    .read = seq_read
};

static int __init on_start(void){
    proc_create("cpu_usage",0,NULL,&processes);
    return 0;
}

static void __exit on_exit(void){
    remove_proc_entry("cpu_usage",NULL);
}

module_init(on_start);
module_exit(on_exit);
