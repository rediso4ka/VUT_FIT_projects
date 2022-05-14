#
# Makefile
# Řešení IJC-DU1, 20.3.2022
# Autor: Aleksandr Shevchenko, FIT
# Přeloženo: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
#

CC = gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra
BIN = primes primes-i steg-decode

all: $(BIN)

#
# .o files s makry:
#
primes.o: primes.c bitset.h eratosthenes.h error.h
	$(CC) $(CFLAGS) -c primes.c -o primes.o
bitset.o: bitset.c bitset.h
	$(CC) $(CFLAGS) -c bitset.c -o bitset.o
eratosthenes.o: eratosthenes.c bitset.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes.o
error.o: error.c
	$(CC) $(CFLAGS) -c error.c -o error.o
ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c ppm.c -o ppm.o
steg-decode.o: steg-decode.c ppm.h error.h bitset.h eratosthenes.h
	$(CC) $(CFLAGS) -c steg-decode.c -o steg-decode.o

#
# .o s inline funkcemi:
#
primes-i.o: primes.c bitset.h eratosthenes.h error.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c primes.c -o primes-i.o
bitset-i.o: bitset.c bitset.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c bitset.c -o bitset-i.o
eratosthenes-i.o: eratosthenes.c bitset.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c eratosthenes.c -o eratosthenes-i.o

#
# bin files:
#
primes: primes.o bitset.o eratosthenes.o error.o
	$(CC) $(CFLAGS) primes.o bitset.o eratosthenes.o error.o -o primes
primes-i: primes-i.o bitset-i.o eratosthenes-i.o error.o
	$(CC) $(CFLAGS) -DUSE_INLINE primes-i.o bitset-i.o eratosthenes-i.o error.o -o primes-i
steg-decode: steg-decode.o ppm.o error.o bitset.o eratosthenes.o
	$(CC) $(CFLAGS) steg-decode.o ppm.o error.o bitset.o eratosthenes.o -o steg-decode

#
# dalsi prikazy:
#
clean:
	rm -f $(BIN) *.o *.zip

zip:
	zip xshevc01.zip *.c *.h Makefile

run: primes primes-i
	./primes
	./primes-i