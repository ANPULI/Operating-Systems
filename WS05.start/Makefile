CFLAGS=-Iinclude -Wall -lrt -lpthread
BIN = bin
INC = include
LIB = lib
OBJ = obj
SRC = src

Q1_server: src/conversion_server.c src/converters.c
	gcc -o bin/conversion_server $^	$(CFLAGS) 
	bin/conversion_server server

Q1_client: src/conversion_client.c src/converters.c
	gcc -o bin/conversion_client $^	$(CFLAGS)
	bin/conversion_client server client_1 CNY 100
	bin/conversion_client server client_2 EUR 100
	bin/conversion_client server client_3 USD 100
	bin/conversion_client server client_4 GBP 100
	bin/conversion_client server client_5 JPY 100

Q2_1: src/dining_philosophers_1.c 
	gcc -o bin/dining_philosophers_1 $^ $(CFLAGS) 
	bin/dining_philosophers_1

Q2_2: src/dining_philosophers_2.c 
	gcc -o bin/dining_philosophers_2 $^ $(CFLAGS) 
	bin/dining_philosophers_2

Q2_3: src/dining_philosophers_3.c 
	gcc -o bin/dining_philosophers_3 $^ $(CFLAGS) 
	bin/dining_philosophers_3

clean:
	rm -rf bin/* obj/*

remove:
