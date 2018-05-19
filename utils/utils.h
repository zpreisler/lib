#ifndef UTILS_H
#define UTILS_H
/*
#define RED "\e[031;1m"
#define GREEN "\e[032;1m"
#define YELLOW "\e[033;1m"
#define BLUE "\e[034;1m"
#define BLACK "\e[0m"
*/
#define BLACK "\e[0;30m"
#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define PURPLE "\e[0;35m"
#define CYAN "\e[0;36m"
#define WHITE "\e[0;37m"
#define BBLACK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGREEN "\e[1;32m"
#define BYELLOW "\e[1;33m"
#define BBLUE "\e[1;34m"
#define BPURPLE "\e[1;35m"
#define BCYAN "\e[1;36m"
#define BWHITE "\e[1;37m"
#define UBLACK "\e[4;30m"
#define URED "\e[4;31m"
#define UGREEN "\e[4;32m"
#define UYELLOW "\e[4;33m"
#define UBLUE "\e[4;34m"
#define UPURPLE "\e[4;35m"
#define UCYAN "\e[4;36m"
#define UWHITE "\e[4;37m"
#define BGBLACK "\e[40m"
#define BGRED "\e[41m"
#define BGGREEN "\e[42m"
#define BGYELLOW "\e[43m"
#define BGBLUE "\e[44m"
#define BGPURPLE "\e[45m"
#define BGCYAN "\e[46m"
#define BGWHITE "\e[47m"
#define RESET "\e[0m"
extern char *prog;
extern void *alloc(size_t);
extern void align_alloc(void **memptr,size_t alignment,size_t size);
extern void warn(char*,...);
extern void error(char*,...);
extern void p_error(char*,...);
extern FILE *open_file(char*,char*);
extern FILE *open_file2(char*,char*,char*);
extern FILE *open_file2s(char*,char*,char*);
extern int close_file(FILE*);
extern int uwrite(const void*,size_t,size_t,FILE*);
extern int uread(void*,size_t,size_t,FILE*);
extern int ufscanf(FILE *file, const char *format, ...);
#endif
