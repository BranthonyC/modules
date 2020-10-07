
/*  Mapfile Reading Module                  ::     code 2001 per mammon_
 *
 *    This just demonstrates how to read the values of given symbols from
 *    a system mapfile while in kernel mode. The code is largely a quick
 *    hack after an all-nighter, but is functional.
 *
 *    -- To compile:
 *       gcc -I/usr/src/linux/include -Wall -c mapfile_read`.c
 */
#define __KERNEL__
#define MODULE
#define LINUX

#if CONFIG_MODVERSIONS==1
 #define MODVERSIONS
 #include <linux/modversions.h>
#endif
#include <linux/kernel.h>		/* half of these might not even be needed */
#include <linux/module.h> 
#include <linux/init.h>  
#include <linux/slab.h>
#include <linux/unistd.h> 
#include <linux/sched.h> 
#include <linux/fs.h>
#include <linux/file.h>	
#include <linux/mm.h>



/* ============================================================== SYS_READ */
static int my_goddamn_read_actor(read_descriptor_t * desc, struct page *page, unsigned long offset, unsigned long size)
{
   char *kaddr;
   unsigned long left = 0, count = desc->count;

   if (size > count) 	size = count;

   kaddr = page->virtual;
	if (! kaddr )		return(0);
	memcpy( desc->buf, kaddr + offset, size);
   desc->count = count - size;
   desc->written += size;
   desc->buf += size;
   return size;
}

int my_goddamn_read(struct file *f, char * buf, size_t count){
	ssize_t ret;
   read_descriptor_t desc;

	if (! f || ! buf || ! count ) 	return -EBADF;
	if (! f->f_op || f->f_op->read != generic_file_read )    return -EINVAL;

	if (f->f_mode & FMODE_READ) {
		ret = locks_verify_area(FLOCK_VERIFY_READ, f->f_dentry->d_inode, f, 
				                  f->f_pos, count);
		if (!ret) {
			desc.written = 0;
			desc.count = count;
			desc.buf = buf;
			desc.error = 0;
			do_generic_file_read(f, &f->f_pos, &desc, my_goddamn_read_actor);
			ret = ( desc.written ) ? desc.written : desc.error;
		}
	}
   return ret;
}


/* ============================================================== SYMBOLS */
char *symbols[] = {
	"idt_table", "set_intr_gate", "set_trap_gate", "set_system_gate",
	"set_call_gate", "do_exit", "access_process_vm", "ptrace_readdata",
	"ptrace_writedata", "putreg", "getreg", "show_state", "show_task",
	"error_code", "do_debug", "do_int3", "do_nmi", NULL
};


/* check if this is one of the desired symbols */
int deal_with_symbol(char *name, unsigned long address) {
	int x;	
	for ( x = 0; symbols[x]; x++ ) {
		/* find first match */
		if (! strncmp( symbols[x], name, strlen(symbols[x]) )) {
			printk("found %s at %08lx\n",name, address);
			return(1);
		}
	}
	return(0);
}


/* ============================================================== MAPFILE */
/* fill 'name' and 'addr' with symbols from mapfile */
int get_symbol_from_buf(unsigned char *buf, int len, char *name, 
		                  unsigned long *addr) {
	int x, start;
	char addrbuf[12] = {0};

	/* first 8 chars are address */
	memcpy( addrbuf, buf, 8 );

	*addr = simple_strtoul(addrbuf, NULL, 16); 
	if (! *addr ) {
		/* we have a problem: advance to next \n and exit */
		for ( x = 0; x < len && buf[x] != '\n'; x++ )	
			;
		return(0); 
	}
	/* next 3 chars are pointless */
	start = x = 11;

	while( buf[x] != '\n' && buf[x] != 0 && x < len ) {
		x++;
	}
	if ( buf[x] != '\n' ) {
		/* we ran off the end of the buffer */
		return(0);
	}
	memcpy( name, &buf[start], x - start );
	return(x + 1);
}

#define MAPFILE_PAGE_SIZE 4096
/* take page from mapfile, search for symbols, return # bytes read before
 * final newline */
int do_mapfile_page(char *buf, char *cache){
	int x, last_n, tmpsize, pos = 0;
	unsigned long address;
	unsigned char name[64] = {0}, tmpbuf[128 + 64] = {0};

	if (! buf || ! cache ) return(0);

	if ( cache[0] ) {
		/* find last incomplete line in cache */
		for ( x = 0; x < 128; x++ )	
			if (cache[x] == '\n' ) last_n = x;

		tmpsize = 128 - last_n; 
		/* copy last incomplete line and a good helping of the next buffer */
		memcpy( tmpbuf, &cache[last_n + 1], tmpsize - 1 );
		for ( x = 0; x < MAPFILE_PAGE_SIZE && buf[x] != '\n'; x++ )	
			;
		memcpy( &tmpbuf[tmpsize], buf, x + 1 );
		tmpsize += x;

		/* get first symbol from cache */
		if ( get_symbol_from_buf( tmpbuf, tmpsize, name, &address ) ) {
			deal_with_symbol(name, address);
			memset(name, 0, 64);
		} /* else I guess we just live with it :P */

		/* ...and mark place in buf where we will start looking */
		pos = x + 1;
	}

	while (  pos < MAPFILE_PAGE_SIZE && 
			  (tmpsize = get_symbol_from_buf( &buf[pos], MAPFILE_PAGE_SIZE - pos, 
														 name, &address))  )      {
		/* compare against known symbols */
		deal_with_symbol(name, address);
		memset(name, 0, 64);
		pos += tmpsize;
	}
	return(1);
}

int read_mapfile( char *path ) {
	struct file *f;
	char *buf, cache[128] = {0};
	unsigned int size;

	buf = kmalloc( MAPFILE_PAGE_SIZE, GFP_KERNEL );
	if (! buf ) 	return(0);

	f = filp_open(path, O_RDONLY, 0);
	if ( f ) {
		if ( f->f_op->read != generic_file_read ) {
			filp_close(f, 0);	/* we have no locks, so need no owner param */
			return(0);
		}

		size = f->f_dentry->d_inode->i_size;
		while( my_goddamn_read(f, buf, MAPFILE_PAGE_SIZE) > 0 ) {
			do_mapfile_page( buf, cache );

			/* store the last 128 bytes of the buffer in cache */
			memcpy( cache, &buf[MAPFILE_PAGE_SIZE - 128], 128);
			memset( buf, 0, MAPFILE_PAGE_SIZE );
		}
		filp_close(f, 0);	/* we have no locks, so need no owner param */
	}
	kfree(buf);
	return(1);
}

/* ========================================================== MODULE INIT */
char *mapfile;
MODULE_PARM(mapfile, "s");

int __init init_dbg_mod(void){
   EXPORT_NO_SYMBOLS;
	
	if (! mapfile) mapfile = "/boot/System.map";
	read_mapfile( mapfile );

	return(1);
}

module_init(init_dbg_mod);

/* ================================================================== EOF */
