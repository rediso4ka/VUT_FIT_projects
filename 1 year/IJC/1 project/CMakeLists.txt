cmake_minimum_required(VERSION 3.21)
project(IJC_1 C)

set(CMAKE_C_STANDARD 11)

add_executable(IJC_1 bitset.h eratosthenes.c bitset.c eratosthenes.h error.h error.c ppm.c ppm.h primes.c steg-decode.c)
