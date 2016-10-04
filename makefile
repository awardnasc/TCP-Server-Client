all: caseConverter bellower frank

CC=gcc
CFLAGS=-I. -std=gnu99
DEPS = caseconverter.h

%.o: %.c $(DEPS)
	$(CC) -c -o  $@ $< $(CFLAGS)

caseConverter: caseConverter.o SockAddrsEqual.o
	gcc -o caseConverter -std=gnu99 caseConverter.o SockAddrsEqual.o -I.

bellower: bellower.o SockAddrsEqual.o 
	gcc -o bellower -std=gnu99 bellower.o SockAddrsEqual.o -I.

frank: frank.o SockAddrsEqual.o
	gcc -o frank -std=gnu99 frank.o SockAddrsEqual.o -I.

clean:
	rm *o


