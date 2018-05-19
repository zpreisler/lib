CC=gcc -Wall
AR=ar -cr
INSTALL=install -m 400
GIT=git

all: cmdl.o libcmdl.a
	#$(GIT) commit -a -m "build"`$(GIT) rev-list --count HEAD`
git:
	$(GIT) commit -a -m "build"`$(GIT) rev-list --count HEAD`
cmdl.o: cmdl.c
	$(CC) -c cmdl.c -I $(HOME)/include -L $(HOME)/lib -lutils
libcmdl.a: cmdl.o
	$(AR) libcmdl.a cmdl.o 
clean:
	rm -f cmdl.o libcmdl.a
install:
	$(INSTALL) libcmdl.a $(HOME)/lib
	$(INSTALL) cmdl.h $(HOME)/include

test:
	$(CC) -g -DPROG cmdl.c -L $(HOME)/lib -I $(HOME)/include/ -lutils
