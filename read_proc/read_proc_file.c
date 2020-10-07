/* The Linux Channel
 * Code for Video Episode: 345 C-code to read a proc file in Linux user-space - Live Demo and Example
 * Author: Kiran Kankipati
 * Updated: 10-Jun-2018
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

FILE *fp;

#define MAXBUFLEN 1000000 
unsigned char buf[MAXBUFLEN]; int buflen=0;


void main(void)
{   int i=0;
	 buflen=0;
	 fp = fopen("/proc/cpuinfo", "rb"); if(fp!=NULL) { while(!feof(fp)) { buflen = fread(buf, 1, MAXBUFLEN, fp); } fclose(fp); }


	 if(buflen>1) 
	 {	//raw (hex) dump
	 	for(i=0;i<buflen;i++) { printf("%02x ", buf[i]); if(i%8==0) { printf(" "); } if(i%16==0) { printf("\n"); } } printf("\n");
	 	
	 	//string (readable characters) dump
	 	//for(i=0;i<buflen;i++) { printf("%c", buf[i]); } printf("\n");
	 }

}