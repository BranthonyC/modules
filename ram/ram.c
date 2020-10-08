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
     memoria_utilizada = total_memoria-memoria_libre;
     seq_printf(archivo, "{\"memory_usage\": %8lu }",(memoria_utilizada * 100)/total_memoria);
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
    proc_create("memory_usage",0,NULL,&operaciones);   
    return 0;
}

static void __exit salir(void){
    remove_proc_entry("memory_usage",NULL);
}

module_init(iniciar);
module_exit(salir);