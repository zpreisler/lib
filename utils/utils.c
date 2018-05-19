#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "utils.h"
char *prog;
void warn(char *s,...){
	va_list ap;
	va_start(ap,s);
	fprintf(stderr,"[%sWARNING%s] ",YELLOW,RESET);
	if(prog)fprintf(stderr,"%s: ",prog);
	vfprintf(stderr,s,ap);
	fputc('\n',stderr);
	va_end(ap);
}
void error(char *s,...){
	va_list ap;
	va_start(ap,s);
	fprintf(stderr,"[%sERROR%s] ",RED,RESET);
	if(prog)fprintf(stderr,"%s: ",prog);
	vfprintf(stderr,s,ap);
	fputc('\n',stderr);
	va_end(ap);
	exit(1);
}
void p_error(char *s,...){
	va_list ap;
	va_start(ap,s);
	fprintf(stderr,"[%sERROR%s] ",RED,RESET);
	if(prog)fprintf(stderr,"%s: ",prog);
	vfprintf(stderr,s,ap);
	perror("");
	va_end(ap);
	exit(1);
}
void *alloc(size_t size){
	void *p=malloc(size);
	if(p==NULL)error("Can not allocate %ld bytes\n",(long)size);
	return p;
}
void align_alloc(void **memptr,size_t alignment,size_t size){
	if(posix_memalign(memptr,alignment,size))error("Can not align allocate %ld bytes\n",(long)size);
}
FILE *open_file2(char *name,char *ext,char *opt){
	int i=strlen(name);
	FILE *f;
	if((f=fopen(strcat(name,ext),opt))==NULL){
		warn("can not open file \"%s\"",name);
		*(name+i)='\0';
		return NULL;
	}
	else{
		fprintf(stdout,"%s [%sopened%s]\n",name,GREEN,RESET);
		*(name+i)='\0';
		return f;
	}
}
FILE *open_file2s(char *name,char *ext,char *opt){
	int i=strlen(name);
	FILE *f;
	if((f=fopen(strcat(name,ext),opt))==NULL){
		warn("can not open file \"%s\"",name);
		*(name+i)='\0';
		return NULL;
	}
	else{
		//fprintf(stdout,"%s [%sopened%s]\n",name,GREEN,RESET);
		*(name+i)='\0';
		return f;
	}
}
FILE *open_file(char *name,char *opt){
	FILE *f;
	if((f=fopen(name,opt))==NULL){
		warn("can not open file \"%s\"",name);
		return NULL;
	}
	else{
	  	fprintf(stdout,"%s [%sopened%s]\n",name,GREEN,RESET);
		return f;
	}
}
int close_file(FILE *f){
	if(fclose(f)==EOF){
		warn("can not close file");
		return 1;
	}
	else return 0;
}
int uwrite(const void *ptr,size_t size,size_t n,FILE *stream){
	size_t count;
	count=fwrite(ptr,size,n,stream);
	if(count!=n)p_error("uwrite: %d %d",count,n);
	return 0;
}
int uread(void *ptr,size_t size,size_t n,FILE *stream){
	size_t count;
	if(!stream)p_error("uread: ");
	count=fread(ptr,size,n,stream);
	if(count!=n)p_error("uread: ");
	return 0;
}
int ufscanf(FILE *file,const char *format, ...){
	va_list ap;
	va_start(ap,format);
	fscanf(file,format,ap);
	va_end(ap);
	if(errno)p_error("ufscanf:");
	return 0;
}
