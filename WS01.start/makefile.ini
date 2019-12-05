CC=gcc -Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all: runstack


${OBJ}/stack_array.o: ${INC}/stack.h ${SRC}/stack_array.c
	${CC} -c -o ${OBJ}/stack_array.o ${SRC}/stack_array.c -I${INC}

${OBJ}/stack_test.o: ${INC}/stack.h ${SRC}/stack_test.c
	${CC} -c -o ${OBJ}/stack_test.o ${SRC}/stack_test.c -I${INC}

${BIN}/stack_test: ${OBJ}/stack_test.o ${OBJ}/stack_array.o
	${CC} -o ${BIN}/stack_test ${OBJ}/stack_test.o ${OBJ}/stack_array.o


runstack: ${BIN}/stack_test
	echo "Testing Stack"
	${BIN}/stack_test

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
