CC=gcc -Wall
AR=ar -cr
INSTALL=install -m 400
GIT=git

all: utils.o libutils.a
	#$(GIT) commit -a -m "build"`$(GIT) rev-list --count HEAD`
git:
	$(GIT) commit -a -m "build"`$(GIT) rev-list --count HEAD`
utils.o: utils.c
	$(CC) -c utils.c
libutils.a: utils.o
	$(AR) libutils.a utils.o
clean:
	rm -f utils.o lizutils.a
install:
	$(INSTALL) libutils.a $(HOME)/lib
	$(INSTALL) utils.h $(HOME)/include
