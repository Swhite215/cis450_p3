CFLAGS= -g
LDFLAGS= #-lsocket -lnsl
LDFLAGS1= -lpthread #-lsocket -lnsl -lpthread
CC=gcc

all: tc

# To make an executable
tc: tc.o 
	$(CC) $(LDFLAGS) -o tc tc.o
 
# To make an object from source
.c.o:
	$(CC) $(CFLAGS) -c $*.c

# clean out the dross
clean:
	-rm tc *.o core 

