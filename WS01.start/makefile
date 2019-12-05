CC=gcc -Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

# all: archive_list runstack

all: archive_fifo runfifo
libstack: archive_array
stacksize: libstack changesize runstack 
newlibstack: clean archive_list
Q5: newlibstack runstack
libfifo: clean archive_fifo
Q6: libfifo runfifo

${OBJ}/stack_array.o: ${INC}/stack.h ${SRC}/stack_array.c
	${CC} -c -o ${OBJ}/stack_array.o ${SRC}/stack_array.c -I${INC}

${OBJ}/stack_test.o: ${INC}/stack.h ${SRC}/stack_test.c
	# DEFINE STACK SIZE 10
	# ${CC} -D STACK_SIZE=10 -c -o ${OBJ}/stack_test.o ${SRC}/stack_test.c -I${INC}
	${CC} -c -o ${OBJ}/stack_test.o ${SRC}/stack_test.c -I${INC}

${OBJ}/fifo_test.o: ${INC}/fifo.h ${SRC}/fifo_test.c
	${CC} -c -o ${OBJ}/fifo_test.o ${SRC}/fifo_test.c -I${INC}

# USING STATIC LIBRARY
${BIN}/fifo_test: ${OBJ}/fifo_test.o ${LIB}/libfifo.a
	${CC} -o ${BIN}/fifo_test ${OBJ}/fifo_test.o -L${LIB} -lfifo

${BIN}/stack_test: ${OBJ}/stack_test.o ${LIB}/libstack.a
	${CC} -o ${BIN}/stack_test ${OBJ}/stack_test.o -L${LIB} -lstack

# ${BIN}/stack_test: ${OBJ}/stack_test.o ${OBJ}/stack_array.o
# 	${CC} -o ${BIN}/stack_test ${OBJ}/stack_test.o ${OBJ}/stack_array.o

${OBJ}/list_impl.o: ${INC}/list.h ${SRC}/list_impl.c
	${CC} -c -o ${OBJ}/list_impl.o ${SRC}/list_impl.c -I${INC}

${OBJ}/stack_list.o: ${INC}/stack.h ${INC}/list.h ${SRC}/stack_list.c
	${CC} -c -o ${OBJ}/stack_list.o ${SRC}/stack_list.c -I${INC}

${OBJ}/fifo_list.o: ${INC}/fifo.h ${INC}/list.h ${SRC}/fifo_list.c
	${CC} -c -o ${OBJ}/fifo_list.o ${SRC}/fifo_list.c -I${INC}

changesize: ${INC}/stack.h ${SRC}/stack_test.c
	${CC} -D STACK_SIZE=10 -c -o ${OBJ}/stack_test.o ${SRC}/stack_test.c -I${INC}

archive_array: ${OBJ}/stack_array.o
	echo "Archiving array"
	ar rcs ${LIB}/libstack.a ${OBJ}/stack_array.o

archive_list: ${OBJ}/list_impl.o ${OBJ}/stack_list.o
	echo "Archiving list"
	ar rcs ${LIB}/libstack.a ${OBJ}/list_impl.o ${OBJ}/stack_list.o

archive_fifo: ${OBJ}/list_impl.o ${OBJ}/fifo_list.o
	echo "Archiving fifo"
	ar rcs  ${LIB}/libfifo.a ${OBJ}/list_impl.o ${OBJ}/fifo_list.o

runstack: ${BIN}/stack_test
	echo "Testing Stack"
	${BIN}/stack_test

runfifo: ${BIN}/fifo_test
	echo "Testing FIFO"
	${BIN}/fifo_test

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*


test:
	# gcc ${SRC}/stack_list.c ${SRC}/list_impl.c ${SRC}/main.c -I${INC} -o test
	gcc ${SRC}/stack_list.c ${SRC}/list_impl.c ${SRC}/stack_test.c -I${INC} -o test
	# gcc -g ${SRC}/stack_list.c ${SRC}/list_impl.c -I${INC} -o test
	./test
	rm test