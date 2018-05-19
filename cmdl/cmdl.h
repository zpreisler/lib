#ifndef ARG_H
#define ARG_H
#define ARG_VOID 0
#define ARG_STRING 1
#define ARG_INT 2
#define ARG_LONG 4
#define ARG_FLOAT 8
#define ARG_DOUBLE 16
#define ARG_DVEC 32
#define ARG_FVEC 64
#define ARG_LFVEC 128
#define ARG_SWITCH 256
#define ARG_LIST 512
#define ARG_HARD_SPHERE 1024
#define ARG_SQUARE_WELL 2048
#define ARG_KERN_FRENKEL 4096
#define ARG_CPATCH 8192
#define ARG_DLIST 16384
typedef struct list{
	unsigned N,nt,ne;
	double scale,or;
	void *core;
	struct list *next;
}list;
typedef struct hard_sphere{
	struct hard_sphere *next;
	unsigned N;
	double sigma;
}hard_sphere;
typedef struct square_well{
	struct square_well *next;
	unsigned N;
	double sigma;
	double sigma_well;
}square_well;
typedef struct kern_frenkel{
	struct kern_frenkel *next;
	unsigned N;
	unsigned Nalloc;
	unsigned npatch;
	double sigma;
	double sigma_well; //sigma + patch range
	double patch_width;
	double sigma_patch;
	char ptype;
	char ctype;
	double w,wold;
	//double phi,phiold;
	void *p;
	void *patch;
	char gc; //grand canonical
	double mu;
	unsigned replica;
}kern_frenkel;
typedef struct cpatch{
	struct cpatch *next;
	unsigned N;
	unsigned npatch;
	double sigma;
	double sigma_patch; // patch diameter
	char gc; //grand canonical
	double mu;
}cpatch;
typedef struct llist{
	struct llist *next;
}llist;
typedef struct arg{
	char short_opt,*long_opt,*key; //short, long strings
	void *value;
	int flag;
	int mask;
	char *help;
}arg;
extern int read_string(FILE*,char*,int);
extern int cmdl_args(int,char**,int,arg*);
extern int file_args(FILE*,arg*,int);
extern int dump_args(FILE *f,arg *p,int n);
extern int print_args(FILE *f,arg *p,int n);
extern int usage(FILE*,arg*,int,char*);
#endif
