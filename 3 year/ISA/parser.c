/*
 * File: parser.c
 * Author: xshevc01
 * Description: implementation of arguments parsing
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "error.h"

void parse_args(int argc, char *argv[], int *rd, int *reverse,
                int *reqtype, char **server, int *port, char **domain)
{
      // iterating through all input arguments
      for (int i = 1; i < argc; i++)
      {
            if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0 || argc > 8)
            {
                  print_help();
            }
            else if (strcmp(argv[i], "-r") == 0)
            {
                  *rd = 1;
            }
            else if (strcmp(argv[i], "-x") == 0)
            {
                  *reverse = 1;
            }
            else if (strcmp(argv[i], "-6") == 0)
            {
                  *reqtype = 1;
            }
            else if (strcmp(argv[i], "-s") == 0)
            {
                  if (i + 1 < argc)
                  {
                        *server = argv[i + 1];
                        i++;
                  }
                  else
                  {
                        print_error_and_exit("parameter -s requires server information!\n Try --help\n");
                  }
            }
            else if (strcmp(argv[i], "-p") == 0)
            {
                  if (i + 1 < argc)
                  {
                        *port = atoi(argv[i + 1]);
                        if (*port == 0)
                        {
                              print_error_and_exit("invalid port number!\n Try --help\n");
                        }
                        i++;
                  }
                  else
                  {
                        print_error_and_exit("parameter -p requires port number!\n Try --help\n");
                  }
            }
            else
            {
                  *domain = argv[i];
            }
      }

      if (*reverse && *reqtype)
      {
            print_error_and_exit("this version of program does not support flags -6 and -x at the same time!");
      }

      if (*server == NULL || *domain == NULL)
      {
            print_error_and_exit("server or address not given!\nTry --help\n");
      }
}

void print_help()
{
      printf("Usage: dns [-r] [-x] [-6] -s server [-p port] address\n\n");
      printf("Description:\n");
      printf("Perform DNS queries to resolve hostnames to IP addresses or retrieve other DNS information.\n\n");
      printf("Options:\n");
      printf("  -r            Enable recursion (Recursion Desired = 1), otherwise recursion is disabled.\n");
      printf("  -x            Perform a reverse DNS query instead of a direct query.\n");
      printf("  -6            Perform an AAAA record query instead of the default A record query.\n");
      printf("  -s server     Specify the IP address or domain name of the DNS server to send the query to.\n");
      printf("  -p port       Specify the port number for the DNS query (default is 53).\n");
      printf("  address       The address or hostname to query.\n\n");
      exit(0);
}