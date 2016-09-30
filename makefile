all: caseConverter bellower frank
CC=gcc
CFLAGS=-I. -std=gnu99
DEPS = practical.h

%.o: %.c $(DEPS)
	$(CC) -c -o  $@ $< $(CFLAGS)

caseConverter: caseConverter.o DieWithMessage.o TCPServerUtility.o AddressUtility.o
	gcc -o caseConverter -std=gnu99 caseConverter.o DieWithMessage.o TCPServerUtility.o AddressUtility.o -I.

CC=gcc
CFLAGS=-I. -std=gnu99
DEPS = practical.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bellower: bellower.o DieWithMessage.o TCPClientUtility.o 
	gcc -o bellower -std=gnu99 bellower.o DieWithMessage.o TCPClientUtility.o -I.

CC=gcc
CFLAGS=-I. -std=gnu99
DEPS = practical.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

frank: frank.o DieWithMessage.o TCPClientUtility.o
	gcc -o frank -std=gnu99 frank.o DieWithMessage.o TCPClientUtility.o -I.
clean:
	rm *o


