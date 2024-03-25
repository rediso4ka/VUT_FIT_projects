#
# File: Makefile
# Author: xshevc01
#

CC = gcc
CFLAGS = -Wall -pedantic
SRC = dns.c parser.c error.c network.c query.c

all: dns

dns: dns.c
	$(CC) $(CFLAGS) -o dns $(SRC)

run: dns
	./dns

clean:
	rm -f dns

test: dns
	chmod +x dns_test.sh
	./dns_test.sh