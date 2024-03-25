#
# File: Makefile
#

CC = g++
CFLAGS = -lsimlib -Wall -Wextra -pedantic -std=c++11 -Werror -pedantic-errors
SRC1 = pool.cpp
SRC2 = pool_after.cpp
SRC3 = pool_weekend.cpp
SRC4 = pool_after_improved.cpp

all: pool pool_after pool_weekend pool_after_improved

pool: pool.cpp
	$(CC) $(SRC1) -o pool $(CFLAGS)

pool_after: pool_after.cpp
	$(CC) $(SRC2) -o pool_after $(CFLAGS)

pool_weekend: pool_weekend.cpp
	$(CC) $(SRC3) -o pool_weekend $(CFLAGS)

pool_after_improved: pool_after_improved.cpp
	$(CC) $(SRC4) -o pool_after_improved $(CFLAGS)

run: pool pool_after pool_weekend pool_after_improved
	./pool
	./pool_after
	./pool_weekend
	./pool_after_improved

clean:
	rm -f pool pool_after pool_weekend pool_after_improved *.out