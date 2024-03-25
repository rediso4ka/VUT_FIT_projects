/*
 * File: parser.h
 * Author: xshevc01
 * Description: header file for argument parser,
 *              including constants from the task
 */

#ifndef PARSER_H
#define PARSER_H

// Default values from the task
#define DEFAULT_RD 0
#define DEFAULT_PTR 0          // not reversed
#define DEFAULT_REQUEST_TYPE 0 // A
#define DEFAULT_PORT 53

// Max length of query
#define MAX_QUERY_LENGTH 1024

/*
 * Function: parse_args
 * Description: parses input arguments
 * Parameters:
 *   - argc:      number of input arguments
 *   - argv:      input arguments
 *   - rd:        specify recursion if needed
 *   - reverse:   specify reverse if needed
 *   - reqtype:   specify aaaa if needed
 *   - server:    server to send request
 *   - port:      port to which send request
 *   - domain:    requested address
 * Return:
 *   - nothing
 */
void parse_args(int argc, char *argv[], int *rd, int *reverse,
                int *reqtype, char **server, int *port, char **domain);

/*
 * Function: print_help
 * Description: prints help message and exits the program
 * Return:
 *   - nothing
 */
void print_help();

#endif