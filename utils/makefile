CC=gcc -Wall
AR=ar -cr
INSTALL=install -m 400
GIT=git

all: utils.o libutils.a

utils.o: utils.c
	$(CC) -c utils.c
libutils.a: utils.o
	$(AR) libutils.a utils.o
clean:
	rm -f utils.o lizutils.a
install:
	mkdir -p $(HOME)/lib
	$(INSTALL) libutils.a $(HOME)/lib
	mkdir -p $(HOME)/include
	$(INSTALL) utils.h $(HOME)/include
