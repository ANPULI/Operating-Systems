CFLAGS=-Iinclude -Wall

FILE=tasks4

all: build run

Q1: build RR

Q2: build MFQ

Q3: build IORR

build: clean
	mkdir obj
	gcc $(CFLAGS) -c -o obj/policies.o src/sched-policies.c	
	gcc $(CFLAGS) -c -o obj/simulator.o src/sched-simulator.c
	mkdir bin
	gcc $(CFLAGS) -o bin/sched-simulator obj/simulator.o obj/policies.o

run:
	bin/sched-simulator $(FILE) FCFS
#	bin/sched-simulator $(FILE) SJF 1>sched-run-sjf.txt
#	bin/sched-simulator $(FILE) SRTF 1>sched-run-srtf.txt
	# bin/sched-simulator $(FILE) RR 4 1>sched-run-rr.txt
	# bin/sched-simulator $(FILE) MFQ 3
	# bin/sched-simulator $(FILE) IORR 3
	cat *.txt

RR:
	bin/sched-simulator $(FILE) RR 4

MFQ:
	bin/sched-simulator $(FILE) MFQ 3

IORR:
	bin/sched-simulator $(FILE) IORR 4

clean:
	rm -rf bin obj sched-run-*.txt
