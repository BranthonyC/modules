#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/module.h>
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/sched.h>    // informacion de procesos
#include <linux/sched/signal.h> //para recorrido de procesos
#include <linux/unistd.h>       /* for _syscallX macros/related stuff */
#include <linux/ktime.h>
#include <linux/sysinfo.h>

//#include < linux/fs.h>

#define BUFSIZE 150

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Escribir informacion de la memoria ram.");
MODULE_AUTHOR("Eddy Sirin - 201503699");



struct task_struct *task;//info de un proceso
struct task_struct *task_child;        /*    Structure needed to iterate through task children    */
struct list_head *list;            /*    Structure needed to iterate through the list in each task->children struct    */
// struct sysinfo *info;
u64 uptime;
u64 total_time;
u64 seconds;
int cpu_usage;

 
static int escribir_archivo(struct seq_file * archivo,void *v){
	uptime = 350735.47;
	seq_printf(archivo, "{\n\"procesos\":[\n");
	for_each_process( task ){            /*    for_each_process() MACRO for iterating through each task in the os located in linux\sched\signal.h    */
		total_time = task->utime + task->stime;
		seconds = task->utime - (task->start_time/100);
		cpu_usage = 100 * ((total_time/100)/seconds);
		if(task->utime>0){
			seq_printf(archivo, "{\"NOMBRE\": \"%s\", \"UTIME\": %lli, \"STIME\": %lli, \"STARTTIME\":%lli  },\n",
		}
		task->comm, task->utime, task->stime, task->start_time);/*    log parent id/executable name/state */
    }
	seq_printf(archivo, "]\n}\n");
     return 0;
    //
}

static int al_abrir(struct inode *inode, struct file *file){
    return single_open(file, escribir_archivo,NULL);
}

static struct file_operations operaciones = 
{
    .open = al_abrir,
    .read = seq_read
};

static int __init iniciar(void){
    proc_create("cpu_usage",0,NULL,&operaciones);   
    printk(KERN_INFO "Eddy Javier Sirin Hernandez\nCarlos Augusto Bautista Salguero\n");
    return 0;
}

static void __exit salir(void){
    remove_proc_entry("cpu_usage",NULL);
    printk(KERN_INFO "Sistemas Operativos 1\n");
}

module_init(iniciar);
module_exit(salir);
