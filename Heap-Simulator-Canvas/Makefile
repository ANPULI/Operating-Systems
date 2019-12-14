CFLAGS=-Iinclude -Wall

#To run with a different memory allocation strategy, uncomment the corresponding line
# STRATEGY= FIRSTFIT
# STRATEGY= BESTFIT
STRATEGY= WORSTFIT

all: build run

build: clean
	mkdir obj
	gcc $(CFLAGS) -c -o obj/management.o src/memory-management.c
	gcc $(CFLAGS) -c -o obj/simulator.o src/memory-simulator.c -DALLOCATION_STRATEGY=$(STRATEGY)
	gcc $(CFLAGS) -c -o obj/simulation.o src/memory-simulation-run.c
	mkdir bin
	gcc $(CFLAGS) -o bin/memory-simulator obj/management.o obj/simulator.o obj/simulation.o

run:
	bin/memory-simulator

clean:
	rm -rf bin obj sched-run-*.txt