#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/module.h>
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/sched.h>    // informacion de procesos
#include <linux/sched/signal.h> //para recorrido de procesos
#include <studio.h>

//#include < linux/fs.h>

#define BUFSIZE 150

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Escribir informacion de la memoria ram.");
MODULE_AUTHOR("Eddy Sirin - 201503699");

struct task_struct *task;//info de un proceso
struct task_struct *task_child;        /*    Structure needed to iterate through task children    */
struct list_head *list;            /*    Structure needed to iterate through the list in each task->children struct    */
 
static int escribir_archivo(struct seq_file * archivo,void *v){
     
    
     seq_printf(archivo, "******************************************************************\n");
     seq_printf(archivo, "***               Laboratorio Sistemas Operativos 1            ***\n");
     seq_printf(archivo, "***                    Vacaciones Junio 2020                   ***\n");
     seq_printf(archivo, "***           Eddy Javier Sirin Hernandez -- 201503699         ***\n");
     seq_printf(archivo, "***        Carlos Augusto Bautista Salguero -- 200815342       ***\n");
     seq_printf(archivo, "***                                                            ***\n");
     seq_printf(archivo, "***                     Proyecto 1  -- Parte 1                 ***\n");
     seq_printf(archivo, "***                       Modulo Procesos CPU                  ***\n");
     seq_printf(archivo, "***                                                            ***\n");
     seq_printf(archivo, "******************************************************************\n");
     seq_printf(archivo, "******************************************************************\n");
     seq_printf(archivo, "                                                            \n");
  
    // for_each_process(task){
    //     pr_info("%s [%d]\n", task->comm, task->pid);
    //     seq_printf(archivo, "%s [%d]\n", task->comm, task->pid);/*    log parent id/executable name/state    */
    // }

    fp = fopen("/proc/stat", "r");
    char line[512];
    fgets(line, sizeof(line), fp);
    seq_printf(archivo, "%s\n", line);


    

    // seq_printf(archivo, "%s\n", line);



    //  for_each_process( task ){            /*    for_each_process() MACRO for iterating through each task in the os located in linux\sched\signal.h    */
    //     seq_printf(archivo, "{\"PADRE\": %d , \"PID\": %d , \"NOMBRE\": \"%s\" , \"STADO\": %ld }\n",task->pid,task->pid, task->comm, task->state);/*    log parent id/executable name/state    */
    //     list_for_each(list, &task->children){                        /*    list_for_each MACRO to iterate through task->children    */
 
    //         task_child = list_entry( list, struct task_struct, sibling );    /*    using list_entry to declare all vars in task_child struct    */
     
    //         seq_printf(archivo, "{\"PADRE\": %d , \"PID\": %d , \"NOMBRE\": \"%s\" , \"STADO\": %ld , \"CPU\": %u }\n",task->pid, /*    log child of and child pid/name/state    */
    //             task_child->pid, task_child->comm, task_child->state, task_child->cpu);
    //     }
        
    // }    
     seq_printf(archivo, "*******************************************************************************************\n");
     return 0;
    
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
    proc_create("cpu_201503699",0,NULL,&operaciones);   
    printk(KERN_INFO "Eddy Javier Sirin Hernandez\nCarlos Augusto Bautista Salguero\n");
    return 0;
}

static void __exit salir(void){
    remove_proc_entry("cpu_201503699",NULL);
    printk(KERN_INFO "Sistemas Operativos 1\n");
}

module_init(iniciar);
module_exit(salir);