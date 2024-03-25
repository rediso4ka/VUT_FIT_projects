/*
 * File: dns.c
 * Author: xshevc01
 * Description: main file
 */

#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "network.h"

/*
 * Function: main
 * Description: main body of dns program
 * Parameters:
 *   - argc: amount of input arguments
 *   - argv: input arguments
 * Return:
 *   - 0 if success
 */
int main(int argc, char *argv[])
{
      int rd = DEFAULT_RD;                //-r
      int reverse = DEFAULT_PTR;          //-x
      int reqtype = DEFAULT_REQUEST_TYPE; //-6
      char *server = NULL;                //-s
      char *domain = NULL;
      int port = DEFAULT_PORT; //-p

      parse_args(argc, argv, &rd, &reverse, &reqtype, &server, &port, &domain);

      dns_request_body(rd, reverse, reqtype, server, domain, port);

      return 0;
}