all: caseConverter bellower frank caseInverter yeller checker

CC=gcc
CFLAGS=-I. -std=gnu99
DEPS = caseConverter.h, caseInverter.h

%.o: %.c $(DEPS)
	$(CC) -c -o  $@ $< $(CFLAGS)

caseConverter: caseConverter.o SockAddrsEqual.o
	gcc -o caseConverter -std=gnu99 caseConverter.o SockAddrsEqual.o -I.

bellower: bellower.o SockAddrsEqual.o 
	gcc -o bellower -std=gnu99 bellower.o SockAddrsEqual.o -I.

frank: frank.o SockAddrsEqual.o
	gcc -o frank -std=gnu99 frank.o SockAddrsEqual.o -I.

caseInverter: caseInverter.o AddressUtility.o 
	gcc -o caseInverter -std=gnu99 caseInverter.o AddressUtility.o -I.

yeller: yeller.o AddressUtility.o
	gcc -o yeller -std=gnu99 yeller.o AddressUtility.o -I.

checker: checker.o AddressUtility.o
	gcc -o checker -std=gnu99 checker.o AddressUtility.o -I.

clean:
	rm *o


