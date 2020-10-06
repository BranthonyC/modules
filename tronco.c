#include <linux/module.h>              /* Todos los modulos lo necesitan */
#include <linux/kernel.h>               /* Ofrece la macro KERN_INFO */
#include <linux/init.h>                   /* Ofrece las macros de inicio y fin */

static int __init troncostart(void)
{
        printk(KERN_INFO "Que pasa tronco!\n");
        /*
         * Si no se devuelve 0 significa que initmodule ha fallado y no ha podido cargarse.
         */
        return 0;
}
static void __exit troncocleanup(void)
{
        printk(KERN_INFO "Me voy dando el piro!\n");
}

/*
 * Indicamos cuales son las funciones de inicio y fin
 */
moduleinit(troncostart);
moduleexit(troncocleanup);
/*
 * Documentacion del modulo
 */
MODULELICENSE("GPL");
MODULEAUTHOR("Genbeta Dev - https://www.genbetadev.com");
MODULEDESCRIPTION("Este modulo no vale para absolutamente nada");
MODULESUPPORTEDDEVICE("eltiodelavara");