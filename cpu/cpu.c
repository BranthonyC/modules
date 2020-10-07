#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/module.h>
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
//#include < linux/fs.h>

#define BUFSIZE 150

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Escribir informacion de la memoria ram.");
MODULE_AUTHOR("Eddy Sirin - 201503699");

struct sysinfo inf;
static int escribir_archivo(struct seq_file * archivo,void *v){
     long total_memoria = 0;
     long memoria_libre = 0;
     long memoria_utilizada= 0;
     si_meminfo(&inf);
     total_memoria = inf.totalram * 4;
     memoria_libre = inf.freeram * 4;
     seq_printf(archivo, "********************************************************\n");
     seq_printf(archivo, "***         Laboratorio Sistemas Operativos 1        ***\n");
     seq_printf(archivo, "***              Vacaciones Junio 2020               ***\n");
     seq_printf(archivo, "***     Eddy Javier Sirin Hernandez -- 201503699     ***\n");
     seq_printf(archivo, "***   Carlos Augusto Bautista Salguero -- 200815342  ***\n");
     seq_printf(archivo, "***                                                  ***\n");
     seq_printf(archivo, "***               Proyecto 1  -- Parte 1             ***\n");
     seq_printf(archivo, "***                 Modulo Memoria RAM               ***\n");
     seq_printf(archivo, "***                                                  ***\n");
     seq_printf(archivo, "********************************************************\n");
     seq_printf(archivo, "********************************************************\n");
     memoria_utilizada = total_memoria-memoria_libre;
     seq_printf(archivo, "             Memoria Total: \t  %8lu MB           \n",total_memoria);
     seq_printf(archivo, "             Memoria Libre: \t  %8lu MB           \n",memoria_libre);
     seq_printf(archivo, "          Memoria Utilizada: \t  %8lu %%         \n",(memoria_utilizada * 100)/total_memoria);
     
     
     seq_printf(archivo, "********************************************************\n");

     struct task_struct *task;

    for_each_process( task ){  
        seq_printf(archivo, "{%s [%d]}\n ", task->comm, task->pid);
    }

    stat("/proc/3889", {st_mode=S_IFDIR|0555, st_size=0, ...}) = 0
    open("/proc/3889/stat", O_RDONLY)       = 7
    read(7, "3889 (top) S 3854 3889 3854 3481"..., 1024) = 342

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
    proc_create("CPU_Brandon",0,NULL,&operaciones);
    return 0;
}

static void __exit salir(void){
    remove_proc_entry("CPU_Brandon",NULL);
    printk(KERN_INFO "Sistemas Operativos 1\n");
}

module_init(iniciar);
module_exit(salir);
