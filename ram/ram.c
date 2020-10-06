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
     si_meminfo(&inf);
     long total_memoria = (inf.totalram * 4);
     long memoria_libre = (inf.freeram * 4);
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
     long memoria_utilizada = total_memoria-memoria_libre;
     seq_printf(archivo, "             Memoria Total: \t  %8lu MB           \n",total_memoria);
     seq_printf(archivo, "             Memoria Libre: \t  %8lu MB           \n",memoria_libre);
     seq_printf(archivo, "          Memoria Utilizada: \t  %8lu %%         \n",(memoria_utilizada * 100)/total_memoria);
     
     
     seq_printf(archivo, "********************************************************\n");
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
    proc_create("memo_201503699",0,NULL,&operaciones);   
    printk(KERN_INFO "201503699\n200815342\n");
    return 0;
}

static void __exit salir(void){
    remove_proc_entry("memo_201503699",NULL);
    printk(KERN_INFO "Sistemas Operativos 1\n");
}

module_init(iniciar);
module_exit(salir);