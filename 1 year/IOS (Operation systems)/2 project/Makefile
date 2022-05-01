#
# Makefile
# Solution IOS-project 2, 25.4.2022
# Author: Aleksandr Shevchenko, FIT
# Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
#

CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic -pthread -lrt
BIN = proj2

all: $(BIN)

#
# .o files:
#
message.o: message.c
	$(CC) $(CFLAGS) -c $< -o $@
parsing.o: parsing.c message.h proj2.h
	$(CC) $(CFLAGS) -c $< -o $@
process.o: process.c message.h proj2.h
	$(CC) $(CFLAGS) -c $< -o $@
proj2.o: proj2.c proj2.h parsing.h shared.h sem.h process.h message.h
	$(CC) $(CFLAGS) -c $< -o $@
sem.o: sem.c proj2.h
	$(CC) $(CFLAGS) -c $< -o $@
shared.o: shared.c proj2.h
	$(CC) $(CFLAGS) -c $< -o $@

#
# bin files:
#
proj2: proj2.o parsing.o shared.o sem.o process.o message.o
	$(CC) $(CFLAGS) $^ -o $@

#
# other instructions:
#
clean:
	rm -f $(BIN) *.o *.zip proj2.out
zip:
	zip proj2.zip *.c *.h Makefile