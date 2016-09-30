all: TCPCaseInverter bellower
CC=gcc
CFLAGS=-I. -std=gnu99
DEPS = practical.h

%.o: %.c $(DEPS)
	$(CC) -c -o  $@ $< $(CFLAGS)

TCPCaseInverter: TCPCaseInverter.o DieWithMessage.o TCPServerUtility.o AddressUtility.o
	gcc -o TCPCaseInverter -std=gnu99 TCPCaseInverter.o DieWithMessage.o TCPServerUtility.o AddressUtility.o -I.

CC=gcc
CFLAGS=-I. -std=gnu99
DEPS = practical.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bellower: bellower.o DieWithMessage.o TCPClientUtility.o 
	gcc -o bellower -std=gnu99 bellower.o DieWithMessage.o TCPClientUtility.o -I.

clean:
	rm *o


