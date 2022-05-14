#
# Makefile
# Solution IJC-DU2, 17.4.2022
# Author: Aleksandr Shevchenko, FIT
# Compiled: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
#

CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -Wextra
CFLAGSSH = -std=c11 -pedantic -Wall -Wextra -fPIC
OBJLIB = error.o htab_bucket_count.o htab_clear.o htab_erase.o\
		htab_find.o htab_for_each.o htab_free.o htab_hash_function.o\
		htab_init.o htab_lookup_add.o htab_pair_print.o htab_resize.o htab_size.o io.o
OBJLIBSH = error_sh.o htab_bucket_count_sh.o htab_clear_sh.o htab_erase_sh.o\
		htab_find_sh.o htab_for_each_sh.o htab_free_sh.o htab_hash_function_sh.o\
		htab_init_sh.o htab_lookup_add_sh.o htab_pair_print_sh.o htab_resize_sh.o htab_size_sh.o io_sh.o
BIN = tail wordcount wordcount-dynamic
LIB = libhtab.a libhtab.so

all: $(BIN)

#
# libraries:
#
libhtab.a: $(OBJLIB)
	ar rcs $@ $^
libhtab.so: $(OBJLIBSH)
	$(CC) $^ -shared -o $@

#
# .o files
#
error.o: error.c
	$(CC) $(CFLAGS) -c $< -o $@
htab_bucket_count.o: htab_bucket_count.c htab.h htab_private.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_clear.o: htab_clear.c htab.h htab_private.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_erase.o: htab_erase.c htab.h htab_private.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_find.o: htab_find.c htab.h htab_private.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_for_each.o: htab_for_each.c htab.h htab_private.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_free.o: htab_free.c htab.h htab_private.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_hash_function.o: htab_hash_function.c htab.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_init.o: htab_init.c htab.h htab_private.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_lookup_add.o: htab_lookup_add.c htab.h htab_private.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_pair_print.o: htab_pair_print.c htab.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_resize.o: htab_resize.c htab.h htab_private.h
	$(CC) $(CFLAGS) -c $< -o $@
htab_size.o: htab_size.c htab.h htab_private.h
	$(CC) $(CFLAGS) -c $< -o $@
io.o: io.c
	$(CC) $(CFLAGS) -c $< -o $@
tail.o: tail.c error.h
	$(CC) $(CFLAGS) -c $< -o $@
wordcount.o: wordcount.c htab.h io.h error.h
	$(CC) $(CFLAGS) -c $< -o $@

#
# sh.o files
#
error_sh.o: error.c
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_bucket_count_sh.o: htab_bucket_count.c htab.h htab_private.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_clear_sh.o: htab_clear.c htab.h htab_private.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_erase_sh.o: htab_erase.c htab.h htab_private.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_find_sh.o: htab_find.c htab.h htab_private.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_for_each_sh.o: htab_for_each.c htab.h htab_private.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_free_sh.o: htab_free.c htab.h htab_private.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_hash_function_sh.o: htab_hash_function.c htab.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_init_sh.o: htab_init.c htab.h htab_private.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_lookup_add_sh.o: htab_lookup_add.c htab.h htab_private.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_pair_print_sh.o: htab_pair_print.c htab.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_resize_sh.o: htab_resize.c htab.h htab_private.h
	$(CC) $(CFLAGSSH) -c $< -o $@
htab_size_sh.o: htab_size.c htab.h htab_private.h
	$(CC) $(CFLAGSSH) -c $< -o $@
io_sh.o: io.c
	$(CC) $(CFLAGSSH) -c $< -o $@

#
# bin files:
#
tail: tail.o error.o
	$(CC) $(CFLAGS) $^ -o $@
wordcount: wordcount.o io.o error.o libhtab.a
	$(CC) $(CFLAGS) -static wordcount.o io.o error.o -o $@ -L. -lhtab
wordcount-dynamic: wordcount.o io.o error.o libhtab.so
	$(CC) $(CLAGS) -Wl,-rpath,. wordcount.o io.o error.o -o $@ -L. -lhtab
#
# other instructions:
#
clean:
	rm -f $(BIN) $(LIB) *.o *.zip
zip:
	zip xshevc01.zip *.c *.cc *.h Makefile