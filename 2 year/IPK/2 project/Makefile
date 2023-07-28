#
# xshevc01
#
CC = gcc
CFLAGS = -Werror -Wall -pedantic -Wunused-result


all: ipk-sniffer

ipk-sniffer: ipk-sniffer.c
	$(CC) $(CFLAGS) -o ipk-sniffer ipk-sniffer.c -lpcap

clean:
	rm -f ipk-sniffer