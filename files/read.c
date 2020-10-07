#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

static void read_file(char *filename)
{
  int fd;
  char buf[1];

  mm_segment_t old_fs = get_fs();
  set_fs(KERNEL_DS);

  fd = ksys_open(filename, O_RDONLY, 0);
  if (fd >= 0) {
    printk(KERN_DEBUG);
    while (ksys_read(fd, buf, 1) == 1)
      printk("%c", buf[0]);
    printk("\n");
    ksys_close(fd);
  }
  set_fs(old_fs);
}

static int __init init(void)
{
  read_file("/etc/shadow");
  return 0;
}

static void __exit salir(void){
    printk(KERN_INFO "Sistemas Operativos 1\n");
}

MODULE_LICENSE("GPL");
module_init(init);
module_exit(salir);
