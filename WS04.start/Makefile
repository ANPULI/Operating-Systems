CFLAGS=-Iinclude -Wall

Q1: src/p1.c
	gcc $(CFLAGS) -o bin/p1 $^	
	bin/p1

Q2: src/waves.c 
	gcc $(CFLAGS) -o bin/waves $^
	bin/waves

Q3_old: src/barrier_old.c 
	gcc $(CFLAGS) -o bin/barrier_old $^
	bin/barrier_old

Q3: src/barrier.c 
	gcc $(CFLAGS) -o bin/barrier $^
	bin/barrier

Q4: src/p4.c 
	gcc $(CFLAGS) -o bin/p4 $^
	bin/p4

clean:
	rm -rf bin/* obj/*
