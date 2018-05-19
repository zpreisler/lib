#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include "utils.h"
#include "cmdl.h"
char *get_arg(arg p,char *argv[],int *i){
	int j,k;
	char *err,*s=argv[*i];
	char c;
	list *l;
	kern_frenkel *kf=NULL;
	switch(p.flag){
		case ARG_VOID: break;
		case ARG_STRING: strcpy((char*)p.value,s); break;
		case ARG_INT:
							*(int*)p.value=strtol(s,&err,10);
							if(*err)error("not an integer '%s'",s);
							break;
		case ARG_LONG:
							*(unsigned long long int*)p.value=strtoul(s,&err,10);
							if(*err)error("not a unsigned long integer '%s'",s);
							break;
		case ARG_FLOAT:
							*(float*)p.value=strtod(s,&err);
							if(*err)error("not a float '%s'",s);
							break;
		case ARG_DOUBLE:
							*(double*)p.value=strtod(s,&err);
							if(*err)error("not a double '%s'",s);
							break;
		case ARG_DVEC:
							for(j=0;j<p.mask-1;(*i)++,j++){
								*((int*)p.value+j)=strtod(argv[*i],&err);
								if(*err)error("not an integer '%s'",argv[*i]);
								if(!argv[*i+1])error("missing argument '-%c' '--%s'",p.short_opt,p.long_opt);
								if(*argv[*i+1]=='*'){
									for(k=j++;j<p.mask;j++)*((int*)p.value+j)=*((int*)p.value+k);
									break;
								}
							}
							*((int*)p.value+j)=strtod(argv[*i],&err);
							if(*err)error("not a float '%s'",argv[*i]);
							break;
		case ARG_FVEC:
							for(j=0;j<p.mask-1;(*i)++,j++){
								*((float*)p.value+j)=strtod(argv[*i],&err);
								if(*err)error("not a float '%s'",argv[*i]);
								if(!argv[*i+1])error("missing argument '-%c' '--%s'",p.short_opt,p.long_opt);
								if(*argv[*i+1]=='*'){
									for(k=j++;j<p.mask;j++)*((float*)p.value+j)=*((float*)p.value+k);
									break;
								}
							}
							*((float*)p.value+j)=strtod(argv[*i],&err);
							if(*err)error("not a float '%s'",argv[*i]);
							break;
		case ARG_LFVEC:
							for(j=0;j<p.mask-1;(*i)++,j++){
								*((double*)p.value+j)=strtod(argv[*i],&err);
								if(*err)error("not a double '%s'",argv[*i]);
								if(!argv[*i+1])error("missing argument '-%c' '--%s'",p.short_opt,p.long_opt);
								if(*argv[*i+1]=='*'){
									for(k=j++;j<p.mask;j++)*((double*)p.value+j)=*((double*)p.value+k);
									break;
								}
							}
							*((double*)p.value+j)=strtod(argv[*i],&err);
							if(*err)error("not a double '%s'",argv[*i]);
							break;
		case ARG_SWITCH: *(int*)p.value^=p.mask; break;
		case ARG_LIST:
							l=(list*)alloc(sizeof(list));
							l->N=strtol(argv[(*i)++],&err,10);
							l->nt=strtol(argv[(*i)++],&err,10);
							l->ne=strtol(argv[(*i)++],&err,10);
							l->scale=strtod(argv[(*i)++],&err);
							l->or=strtod(argv[(*i)],&err);
							l->next=*(list**)p.value;
							*(list**)p.value=l;
							break;
		case ARG_HARD_SPHERE:
							kf=(kern_frenkel*)alloc(sizeof(kern_frenkel));
							kf->ptype=0;
							kf->npatch=0;
							kf->N=strtol(argv[(*i)],&err,10);
							if(*err)error("not an integer '%s'",argv[*i]);
							if(!argv[*i+1] || *argv[*i+1]=='-'){
								kf->sigma=1.0;
								fprintf(stderr,"'-%c' '--%s' [N, sigma] sigma not set, default sigma=%lf\n",
										p.short_opt,
										p.long_opt,
										kf->sigma);
								kf->next=*(kern_frenkel**)p.value;
								*(kern_frenkel**)p.value=kf;
								break;
							}
							else{
								kf->sigma=strtod(argv[++(*i)],&err);
								if(*err)error("not a double '%s'",argv[*i]);
								kf->next=*(kern_frenkel**)p.value;
								*(kern_frenkel**)p.value=kf;
								break;
							}
		case ARG_SQUARE_WELL:
							kf=(kern_frenkel*)alloc(sizeof(kern_frenkel));
							kf->ptype=1;
							kf->npatch=0;
							kf->N=strtol(argv[(*i)],&err,10);
							if(*err)error("not an integer '%s'",argv[*i]);
							if(!argv[*i+1] || *argv[*i+1]=='-'){
								kf->sigma=1.0;
								kf->sigma_well=1.4;
								fprintf(stderr,"'-%c' '--%s' [N, sigma, sigma_well] sigma and sigma_well not set, default sigma=%lf, sigma_well=%lf\n",
										p.short_opt,
										p.long_opt,
										kf->sigma,
										kf->sigma_well);
								kf->next=*(kern_frenkel**)p.value;
								*(kern_frenkel**)p.value=kf;
								break;
							}
							else{
								kf->sigma=strtod(argv[++(*i)],&err);
								if(*err)error("sigma not a double '%s'",argv[*i]);
							}
							if(!argv[*i+1] || *argv[*i+1]=='-'){
								kf->sigma_well=1.4;
								fprintf(stderr,"'-%c' '--%s' [N, sigma, sigma_well] sigma_well not set, default sigma_well=%lf\n",
										p.short_opt,
										p.long_opt,
										kf->sigma_well);
								kf->next=*(kern_frenkel**)p.value;
								*(kern_frenkel**)p.value=kf;
								break;
							}
							else{
								kf->sigma_well=strtod(argv[++(*i)],&err);
								if(*err)error("not a double '%s'",argv[*i]);
								kf->next=*(kern_frenkel**)p.value;
								*(kern_frenkel**)p.value=kf;
								break;
							}
		case ARG_KERN_FRENKEL:
							kf=(kern_frenkel*)alloc(sizeof(kern_frenkel));
							kf->ptype=2;
							kf->gc=0;
							kf->N=strtol(argv[(*i)],&err,10);
							if(*err)error("not an integer '%s'",argv[*i]);
							if(!argv[*i+1] || *argv[*i+1]=='-'){
								kf->npatch=1;
								kf->sigma=1.0;
								kf->sigma_well=1.4;
								kf->patch_width=0.333;
								fprintf(stderr,"'-%c' '--%s' [N, npatch, sigma, sigma_patch,patch_width] not set\ndefault npatch=%d, sigma=%lf, sigma_patch=%lf, patch_width=%lf\n",
										p.short_opt,
										p.long_opt,
										kf->npatch,
										kf->sigma,
										kf->sigma_well,
										kf->patch_width);
								kf->next=*(kern_frenkel**)p.value;
								*(kern_frenkel**)p.value=kf;
								break;
							}
							kf->npatch=strtol(argv[++(*i)],&err,10);
							if(*err)error("sigma not an integer '%s'",argv[*i]);

							kf->sigma=strtod(argv[++(*i)],&err);
							if(*err)error("sigma not a double '%s'",argv[*i]);

							kf->sigma_well=strtod(argv[++(*i)],&err);
							if(*err)error("not a double '%s'",argv[*i]);

							kf->patch_width=strtod(argv[++(*i)],&err);
							if(*err)error("not a double '%s'",argv[*i]);

							if(argv[(*i)+1]){
								c=*argv[(*i)+1];
							}
							else{
								c='0';
							}
							switch(c){
								case 'w':
									kf->ctype=c;
									(*i)++;
									kf->w=strtod(argv[++(*i)],&err);
									if(*err)error("not a double '%s'",argv[*i]);
									break;
								case 's':
									kf->ctype=c;
									(*i)++;
									kf->w=strtod(argv[++(*i)],&err);
									if(*err)error("not a double '%s'",argv[*i]);
									break;
								default:
									kf->ctype='0';
									kf->w=0.0;
							}

							if(argv[(*i)+1]){
								c=*argv[(*i)+1];
							}
							else{
								c='0';
							}
							switch(c){
								case 'u':
									kf->gc=1;
									(*i)++;
									kf->mu=strtod(argv[++(*i)],&err);
									if(*err)error("not a double '%s'",argv[*i]);
									break;
								default:
									kf->gc=0;
									kf->mu=0.0;
							}

							kf->next=*(kern_frenkel**)p.value;
							*(kern_frenkel**)p.value=kf;
							break;

		case ARG_DLIST:
							kf=*(kern_frenkel**)p.value;
							//printf("sopt %d %c lopt %s\n",p.short_opt,p.short_opt,p.long_opt);
							if(p.short_opt=='u'){
								while(kf!=NULL){
									printf("kf %d\n",kf->N);
									if(*argv[*i]=='x'){
										kf->gc=0;
									}
									else{
										kf->gc=1;
										kf->mu=strtod(argv[*i],&err);
										if(*err)error("not a double '%s'",argv[*i]);
									}
									if(kf->next!=NULL)(*i)++;
									kf=kf->next;
								}
							}
							if(p.short_opt=='w'){
								kf=*(kern_frenkel**)p.value;
								while(kf!=NULL){
									kf->patch_width=strtod(argv[*i],&err);
									if(*err)error("not a double '%s'",argv[*i]);
									if(kf->next!=NULL)(*i)++;
									kf=kf->next;
								}
							}
							if(p.short_opt=='P'){
								kf=*(kern_frenkel**)p.value;
								while(kf!=NULL){
									kf->w=strtod(argv[*i],&err);
									if(*err)error("not a double '%s'",argv[*i]);
									if(kf->next!=NULL)(*i)++;
									kf=kf->next;
								}
							}
							/**(double*)p.value=strtod(s,&err);
							if(*err)error("not a double '%s'",s);*/
							break;
		case ARG_CPATCH:
							kf=(kern_frenkel*)alloc(sizeof(kern_frenkel));
							kf->ptype=3;
							kf->N=strtol(argv[(*i)],&err,10);
							if(*err)error("not an integer '%s'",argv[*i]);
							if(!argv[*i+1] || *argv[*i+1]=='-'){
								kf->npatch=1;
								kf->sigma=1.0;
								kf->sigma_patch=0.1;
								fprintf(stderr,"'-%c' '--%s' [N, npatch, sigma, sigma_patch] not set\ndefault npatch=%d, sigma=%lf, sigma_patch=%lf\n",
										p.short_opt,
										p.long_opt,
										kf->npatch,
										kf->sigma,
										kf->sigma_patch);
								kf->next=*(kern_frenkel**)p.value;
								*(kern_frenkel**)p.value=kf;
								break;
							}
							kf->npatch=strtol(argv[++(*i)],&err,10);
							if(*err)error("sigma not an integer '%s'",argv[*i]);

							kf->sigma=strtod(argv[++(*i)],&err);
							if(*err)error("sigma not a double '%s'",argv[*i]);

							kf->sigma_patch=strtod(argv[++(*i)],&err);
							if(*err)error("not a double '%s'",argv[*i]);

							kf->next=*(kern_frenkel**)p.value;
							*(kern_frenkel**)p.value=kf;
							break;
	}
	if(errno)p_error("%s ",s);
	return err;
}
int cmdl_args(int argc,char *argv[],int n,arg *p){
	char *msg;
	int i,j;
	errno=0,prog=*argv;
	for(i=1;i<argc;i++){
		msg=argv[i];
		if(*msg++=='-'){
			if(*msg=='-'){
				for(j=0,++msg;j<n;j++){
					if(p[j].long_opt&&!strcmp(p[j].long_opt,msg)){
						if(++i==argc&&p[j].flag!=ARG_SWITCH)error("missing argument '--%s'",p[j].long_opt);
						get_arg(p[j],argv,&i);
						break;
					}
				}
				if(j==n)error("invalid option -- '%s'",msg);
			}
			else if(*msg=='\0')error("invalid option - ''");
			else{
				for(j=0;j<n;j++){
					if(p[j].short_opt==*msg){
						if(p[j].flag==ARG_SWITCH){
							get_arg(p[j],argv,&i);
							msg++;j=-1;
						}
						else{
							if(*++msg=='\0'){
							  	if(++i==argc)error("missing argument '-%c'",p[j].short_opt);
								get_arg(p[j],argv,&i);
								break;
							}
							else{
								*(argv+i)+=2;
								get_arg(p[j],argv,&i);
								break;
							}
						}
					}
				}
				if(j==n&&*msg)error("invalid option - '%c'",*msg);
			}
		}
	}	
	return 0;
}
#define COMMENT '#'
#define SIZE 512
int skip(FILE *f){
	int c;
	while(isspace(c=getc(f)));
	if(c==COMMENT){
		while((c=getc(f))!='\n');
		return skip(f);
	}
	return ungetc(c,f);
}
int read_string(FILE *f,char *s,int n){
	int c,i;
	if(!s||n<0)return -1;
	skip(f);
	for(i=0;i<n&&(c=getc(f))!=EOF&&!isspace(c);i++,s++)*s=c;
	*s='\0';
	ungetc(c,f);
	return i;
}
char *get_farg(FILE *f,arg p){
	char buffer[SIZE],*err;
	int j,k;
	list *l;
	kern_frenkel *kf;
	char c;
	long int position;
	switch(p.flag){
		case ARG_VOID: break;
		case ARG_STRING: read_string(f,p.value,p.mask); break;
		case ARG_INT:
							read_string(f,buffer,SIZE);
							*(int*)p.value=strtol(buffer,&err,10);
							if(*err)error("not an integer '%s'",buffer);
							break;
		case ARG_LONG:
							read_string(f,buffer,SIZE);
							*(unsigned long long int*)p.value=strtoul(buffer,&err,10);
							if(*err)error("not a long '%s'",buffer);
							break;
		case ARG_FLOAT:
							read_string(f,buffer,SIZE);
							*(float*)p.value=strtod(buffer,&err);
							if(*err)error("not a float '%s'",buffer);
							break;
		case ARG_DOUBLE:
							read_string(f,buffer,SIZE);
							*(double*)p.value=strtod(buffer,&err);
							if(*err)error("not a double '%s'",buffer);
							break;
		case ARG_DVEC:
							for(j=0;j<p.mask;j++){
								read_string(f,buffer,SIZE);
								if(j>0&&*buffer=='*'){
									for(k=j-1;j<p.mask;j++)*((int*)p.value+j)=*((int*)p.value+k);
									break;
								}
								*((int*)p.value+j)=strtod(buffer,&err);
								if(*err)error("not an integer '%s'",buffer);
							}
							break;
		case ARG_FVEC:
							for(j=0;j<p.mask;j++){
								read_string(f,buffer,SIZE);
								if(j>0&&*buffer=='*'){
									for(k=j-1;j<p.mask;j++)*((float*)p.value+j)=*((float*)p.value+k);
									break;
								}
								*((float*)p.value+j)=strtod(buffer,&err);
								if(*err)error("not a float '%s'",buffer);
							}
							break;
		case ARG_LFVEC:
							for(j=0;j<p.mask;j++){
								read_string(f,buffer,SIZE);
								if(j>0&&*buffer=='*'){
									for(k=j-1;j<p.mask;j++)*((double*)p.value+j)=*((double*)p.value+k);
									break;
								}
								*((double*)p.value+j)=strtod(buffer,&err);
								if(*err)error("not a double '%s'",buffer);
							}
							break;
		case ARG_SWITCH: *(short*)p.value^=p.mask; break;
		case ARG_LIST:
							l=(list*)alloc(sizeof(list));
							read_string(f,buffer,SIZE);
							l->N=strtol(buffer,&err,10);
							read_string(f,buffer,SIZE);
							l->nt=strtol(buffer,&err,10);
							read_string(f,buffer,SIZE);
							l->ne=strtol(buffer,&err,10);
							read_string(f,buffer,SIZE);
							l->scale=strtod(buffer,&err);
							read_string(f,buffer,SIZE);
							l->or=strtod(buffer,&err);
							l->next=*(list**)p.value;
							*(list**)p.value=l;
							break;
		case ARG_HARD_SPHERE:
							kf=(kern_frenkel*)alloc(sizeof(kern_frenkel));
							kf->ptype=0;
							kf->npatch=0;
							read_string(f,buffer,SIZE);
							kf->N=strtol(buffer,&err,10);
							if(*err)error("%s [N, sigma] not an integer '%s'",p.key,buffer);
							read_string(f,buffer,SIZE);
							kf->sigma=strtod(buffer,&err);
							if(*err)error("%s [N, sigma] not a double '%s'",p.key,buffer);
							kf->next=*(kern_frenkel**)p.value;
							*(kern_frenkel**)p.value=kf;
							break;
		case ARG_SQUARE_WELL:
							kf=(kern_frenkel*)alloc(sizeof(kern_frenkel));
							kf->ptype=1;
							kf->npatch=0;
							read_string(f,buffer,SIZE);
							kf->N=strtol(buffer,&err,10);
							if(*err)error("%s [N, sigma,sigma_well] not an integer '%s'",p.key,buffer);
							read_string(f,buffer,SIZE);
							kf->sigma=strtod(buffer,&err);
							if(*err)error("%s [N, sigma,sigma_well] not a double '%s'",p.key,buffer);
							read_string(f,buffer,SIZE);
							kf->sigma_well=strtod(buffer,&err);
							if(*err)error("%s [N, sigma,sigma_well] not a double '%s'",p.key,buffer);
							kf->next=*(kern_frenkel**)p.value;
							*(kern_frenkel**)p.value=kf;
							break;
		case ARG_KERN_FRENKEL:
							kf=(kern_frenkel*)alloc(sizeof(kern_frenkel));
							kf->ptype=2;
							read_string(f,buffer,SIZE);
							kf->N=strtol(buffer,&err,10);
							if(*err)error("%s [N, npatch, sigma, sigma_patch, patch_width] not an integer '%s'",p.key,buffer);

							read_string(f,buffer,SIZE);
							kf->npatch=strtol(buffer,&err,10);
							if(*err)error("%s [N, npatch, sigma, sigma_patch, patch_width] not an integer '%s'",p.key,buffer);

							read_string(f,buffer,SIZE);
							kf->sigma=strtod(buffer,&err);
							if(*err)error("%s [N, sigma,sigma_patch] not a double '%s'",p.key,buffer);
							read_string(f,buffer,SIZE);
							kf->sigma_well=strtod(buffer,&err);
							if(*err)error("%s [N, sigma,sigma_patch] not a double '%s'",p.key,buffer);

							read_string(f,buffer,SIZE);
							kf->patch_width=strtod(buffer,&err);
							if(*err)error("%s [N, sigma,sigma_patch] not a double '%s'",p.key,buffer);

							position=ftell(f);
							j=read_string(f,buffer,SIZE);
							if(j==1){
								c=*buffer;
								//printf("c=%c\n",c);
							}
							else c='0';
							switch(c){
								case 'w':
									kf->ctype=c;
									read_string(f,buffer,SIZE);
									kf->w=strtod(buffer,&err);
									if(*err)error("not a double '%s'",buffer);
									break;
								case 's':
									kf->ctype=c;
									read_string(f,buffer,SIZE);
									kf->w=strtod(buffer,&err);
									if(*err)error("not a double '%s'",buffer);
									break;
								default:
									fseek(f,position,SEEK_SET);
									kf->ctype='0';
									kf->w=0.0;
							}

							position=ftell(f);
							j=read_string(f,buffer,SIZE);
							if(j==1){
								c=*buffer;
								//printf("c=%c\n",c);
							}
							else c='0';
							switch(c){
								case 'u':
									kf->gc=1;
									read_string(f,buffer,SIZE);
									kf->mu=strtod(buffer,&err);
									if(*err)error("not a double '%s'",buffer);
									break;
								default:
									fseek(f,position,SEEK_SET);
									kf->gc=0;
									kf->mu=0.0;
							}

							kf->next=*(kern_frenkel**)p.value;
							*(kern_frenkel**)p.value=kf;
							break;
		case ARG_CPATCH:
							kf=(kern_frenkel*)alloc(sizeof(kern_frenkel));
							kf->ptype=3;
							read_string(f,buffer,SIZE);
							kf->N=strtol(buffer,&err,10);
							if(*err)error("%s [N, npatch, sigma, sigma_patch] not an integer '%s'",p.key,buffer);

							read_string(f,buffer,SIZE);
							kf->npatch=strtol(buffer,&err,10);
							if(*err)error("%s [N, npatch, sigma, sigma_patch] not an integer '%s'",p.key,buffer);

							read_string(f,buffer,SIZE);
							kf->sigma=strtod(buffer,&err);
							if(*err)error("%s [N, sigma,sigma_patch] not a double '%s'",p.key,buffer);
							read_string(f,buffer,SIZE);
							kf->sigma_patch=strtod(buffer,&err);
							if(*err)error("%s [N, sigma,sigma_patch] not a double '%s'",p.key,buffer);

							kf->next=*(kern_frenkel**)p.value;
							*(kern_frenkel**)p.value=kf;
							break;
	}
	if(errno)p_error("%s ",buffer);
	return err;
}
int file_args(FILE *f,arg *p,int n){
	char buffer[SIZE];
	int i;
	errno=0;
	while(!feof(f)){
		if(read_string(f,buffer,SIZE)!=-1){
			for(i=0;i<n;i++){
				if(!strcmp(buffer,p[i].key)){
					get_farg(f,p[i]);
				}
			}
		}
	}	
	return 0;
}
int dump_args(FILE *f,arg *p,int n){
	int i,j;
	list *l;
	kern_frenkel *kf;
	for(i=0;i<n;i++){
		switch(p[i].flag){
			case ARG_VOID: break;
			case ARG_STRING: fprintf(f,"%s %s\n",p[i].key,(char*)p[i].value); break;
			case ARG_INT: fprintf(f,"%s %d\n",p[i].key,*(int*)p[i].value); break;
			case ARG_LONG:fprintf(f,"%s %Lu\n",p[i].key,*(unsigned long long int*)p[i].value); break;
			case ARG_FLOAT: fprintf(f,"%s %f\n",p[i].key,*(float*)p[i].value); break;
			case ARG_DOUBLE: fprintf(f,"%s %.16lf\n",p[i].key,*(double*)p[i].value); break;
			case ARG_DVEC: fprintf(f,"%s",p[i].key);
								for(j=0;j<p[i].mask;j++)fprintf(f," %d",*((int*)p[i].value+j)); fputc('\n',f);
								break;
			case ARG_FVEC: fprintf(f,"%s",p[i].key);
								for(j=0;j<p[i].mask;j++)fprintf(f," %f",*((float*)p[i].value+j)); fputc('\n',f);
								break;
			case ARG_LFVEC: fprintf(f,"%s",p[i].key);
								for(j=0;j<p[i].mask;j++)fprintf(f," %lf",*((double*)p[i].value+j)); fputc('\n',f);
								break;
			case ARG_SWITCH: fprintf(f,"%s %d\n",p[i].key,*(int*)p[i].value); break;
			case ARG_LIST: l=*(list**)p[i].value;
								while(l!=NULL){
									fprintf(f,"%s %d %d %d %lf %lf\n",p[i].key,l->N,l->nt,l->ne,l->scale,l->or);
									l=l->next;
								}
								break;
			case ARG_HARD_SPHERE:
								kf=*(kern_frenkel**)p[i].value;
								while(kf!=NULL){
									if(kf->ptype==0){
										fprintf(f,"%s %d %lf  [N, sigma]\n",
												p[i].key,
												kf->N,
												kf->sigma);
									}
									kf=kf->next;
								}
								break;
			case ARG_SQUARE_WELL:
								kf=*(kern_frenkel**)p[i].value;
								while(kf!=NULL){
									if(kf->ptype==1){
										fprintf(f,"%s %d %lf %lf  [N ,sigma, sigma_well]\n",
												p[i].key,
												kf->N,
												kf->sigma,
												kf->sigma_well);
									}
									kf=kf->next;
								}
								break;
			case ARG_KERN_FRENKEL:
								kf=*(kern_frenkel**)p[i].value;
								while(kf!=NULL){
									if(kf->ptype==2){
										switch(kf->ctype){
											case 's':
											case 'w':
												if(kf->gc){
													fprintf(f,"%s %d %d %.12lf %.12lf %.12lf %c %.12lf u %.12lf [N, npatch, sigma, sigma_patch, patch_width, w split, gc mu]\n",
															p[i].key,
															kf->N,
															kf->npatch,
															kf->sigma,
															kf->sigma_well,
															kf->patch_width,
															kf->ctype,
															kf->w,
															kf->mu);

												}
												else{
													fprintf(f,"%s %d %d %.12lf %.12lf %.12lf %c %.12lf [N, npatch, sigma, sigma_patch, patch_width]\n",
															p[i].key,
															kf->N,
															kf->npatch,
															kf->sigma,
															kf->sigma_well,
															kf->patch_width,
															kf->ctype,
															kf->w);
												}
												break;
											default:
												if(kf->gc){
													fprintf(f,"%s %d %d %.12lf %.12lf %.12lf u %.12lf [N, npatch, sigma, sigma_patch, patch_width, gc mu]\n",
															p[i].key,
															kf->N,
															kf->npatch,
															kf->sigma,
															kf->sigma_well,
															kf->patch_width,
															kf->mu);
												}
												else{
													fprintf(f,"%s %d %d %.12lf %.12lf %.12lf  [N, npatch, sigma, sigma_patch, patch_width]\n",
															p[i].key,
															kf->N,
															kf->npatch,
															kf->sigma,
															kf->sigma_well,
															kf->patch_width);
												}
										}
									}
									kf=kf->next;
								}
								break;
			case ARG_CPATCH:
								kf=*(kern_frenkel**)p[i].value;
								while(kf!=NULL){
									if(kf->ptype==3){
										fprintf(f,"%s %d %d %.3lf %.3lf  [N, npatch, sigma, sigma_patch]\n",
												p[i].key,
												kf->N,
												kf->npatch,
												kf->sigma,
												kf->sigma_patch);
									}
									kf=kf->next;
								}
								break;
		}
	}
	return 0;
}
#undef SIZE
int print_args(FILE *f,arg *p,int n){
	int i;
	for(i=0;i<n;i++){
		fprintf(f,"[-%c] [--%s] {%s} %s\n",p[i].short_opt,p[i].long_opt,p[i].key,p[i].help);
	}
	return 0;
}
int usage(FILE *f,arg *p,int n,char *prog){
	fprintf(f,"Usage: %s\n",prog);
	print_args(f,p,n);
	return 0;
}
#ifdef PROG
#define SIZE 512
int main(int argc,char *argv[]){
	FILE *f;
	int i=0,o=0;
	float fl=1.0;
	float fv[3]={0.0,0.0,0.0};
	char buff[256]="";
	list *l=NULL;
	arg cmdl[]={{'a',"aa","aa:",&i,ARG_INT,0,"integer value"},{'f',"fl","float:",&fl,ARG_FLOAT,0,"float value"},{'o',"O","opt",&o,ARG_SWITCH,1,"optimize"},
		{'s',"str","string:",buff,ARG_STRING,SIZE,"msg"},{'F',"fv","vec:",&fv,ARG_FVEC,3,"vector"},{'l',"list","list:",&l,ARG_LIST,0,"list"}};

	cmdl_args(argc,argv,6,cmdl);
	print_args(stdout,cmdl,6);
	/*printf("listp:%p\n",l);
	while(l!=NULL){
		printf("list: %d\n",l->N);
		l=l->next;
	}*/
	dump_args(stdout,cmdl,6);

	f=open_file("t","r");

	file_args(f,cmdl,6);
	dump_args(stdout,cmdl,6);
	/*printf("listp:%p\n",l);
	while(l!=NULL){
		printf("list: %d\n",l->N);
		l=l->next;
	}*/
	return 0;
}
#endif
