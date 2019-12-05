CFLAGS=-Iinclude -Wall -lrt -lpthread
BIN = bin
INC = include
LIB = lib
OBJ = obj
SRC = src
TXT = txt

q1: src/q1.c
	gcc -o bin/q1 $^ $(CFLAGS) 
	bin/q1 txt/file1 txt/file2 txt/file3 txt/file4

q2: src/q2.c
	gcc -o bin/q2 $^ $(CFLAGS) 
	bin/q2 txt/file1 txt/file2 txt/file3 txt/file4

q3: src/bounded_buffer.c
	gcc -o bin/bounded_buffer $^ $(CFLAGS) 
	bin/bounded_buffer 2 5

upper: src/upper.c
	gcc -o bin/upper $^	$(CFLAGS) 
	bin/upper txt/file1

clean:
	rm -rf bin/* obj/*
	rm -rf txt/*.UPPER.txt
