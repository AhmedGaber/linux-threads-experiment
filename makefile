CC=gcc
CFLAGS = -c

all: matMultp

matMultp: parser.o main.o
	$(CC) ­pthread parser.o main.o ­o matMultp

parser: parser.c parser.h
	$(CC) parser.c -o parser.o

main: main.c
	$(CC) main.c -o main.o

clean:
	rm  parser.o main.o matMultp
