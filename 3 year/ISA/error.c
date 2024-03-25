/*
 * File: error.c
 * Author: xshevc01
 * Description: implementation of printing errors and warnings
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void print_warning(const char *message)
{
      fprintf(stderr, "Warning: %s\n", message);
      return;
}

void print_error_and_exit(const char *message)
{
      fprintf(stderr, "Error: %s\n", message);
      exit(EXIT_FAILURE);
      return;
}