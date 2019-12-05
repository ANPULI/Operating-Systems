CC=gcc -Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

Q1: ${SRC}/p1.c
	${CC} -o ${OBJ}/1.o $^ -I${INC}
	@echo "##################################################"
	@${OBJ}/1.o
	@echo "##################################################"

Q2: ${SRC}/decoder.c
	${CC} -o ${OBJ}/decoder.o $^ -I${INC}
	@echo "##################################################"
	@${OBJ}/decoder.o
	@echo "##################################################"

Q3: ${SRC}/multi-converter.c ${SRC}/converters.c
	${CC} -o ${OBJ}/3.o $^ -I${INC}
	@echo "##################################################"
	# @${OBJ}/3.o CNY 100
	@${OBJ}/3.o USD 256
	@echo "##################################################"

clean:
	rm -f ${LIB}/* ${OBJ}/* ${BIN}/*
