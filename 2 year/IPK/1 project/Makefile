CC = gcc
# CFLAGS = -g -std=c11 -pedantic -Wall -Wextra
CFLAGS = -Wunused-result
# CFLAGS = 
BIN = ipkcpc

all: $(BIN)

ipkcpc.o: ipkcpc.c
	$(CC) $(CFLAGS) -c ipkcpc.c -o ipkcpc.o

ipkcpc: ipkcpc.o
	$(CC) $(CFLAGS) ipkcpc.o -o ipkcpc


clean:
	rm -f $(BIN) *.o *.zip

run_tcp: ipkcpc
	./ipkcpc -h localhost -p 2023 -m tcp

run_udp: ipkcpc
	./ipkcpc -h localhost -p 2023 -m udp