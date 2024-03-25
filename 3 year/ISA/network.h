/*
 * File: network.h
 * Author: xshevc01
 * Description: header file for working with socket
 *              and sending / receiving packets
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <netinet/in.h>

/*
 * Function: dns_request_body
 * Description: main body for connection establishment and sending / receiving packages
 * Parameters:
 *   - rd:        specify recursion if needed
 *   - reverse:   specify reverse if needed
 *   - reqtype:   specify aaaa if needed
 *   - server:    server to send request
 *   - domain:    requested address
 *   - port:      port to which send request
 * Return:
 *   - nothing
 */
void dns_request_body(int rd, int reverse, int reqtype, char *server, const char *domain, int port);

/*
 * Function: create_udp_socket
 * Description: creates a UDP socket
 * Parameters:
 *   - reqtype: specify aaaa if needed
 * Return:
 *   - socket file descriptor
 */
int create_udp_socket(int reqtype);

/*
 * Function: setup_dns_server4
 * Description: sets up DNS server info for ipv4
 * Parameters:
 *   - reqtype:   specify aaaa if needed
 *   - server:    server to send request
 *   - port:      port to which send request
 * Return:
 *   - structure sockaddr_in, which will be used to connect to a server
 */
struct sockaddr_in setup_dns_server4(int reqtype, char *server, int port);

/*
 * Function: setup_dns_server6
 * Description: sets up DNS server info for IPv6
 * Parameters:
 *   - reqtype:   specify aaaa if needed
 *   - server:    server to send request
 *   - port:      port to which send request
 * Return:
 *   - structure sockaddr_in6, which will be used to connect to a server
 */
struct sockaddr_in6 setup_dns_server6(int reqtype, char *server, int port);

/*
 * Function: get_server_ip
 * Description: used by setup_dns_server4/6, gets IP of a server from its name
 * Parameters:
 *   - reqtype:   specify aaaa if needed
 *   - server:    server to send request
 * Return:
 *   - IP of the server
 */
char *get_server_ip(int reqtype, char *server);

/*
 * Function: send_dns_query
 * Description: sends the created query to the specified server
 * Parameters:
 *   - reqtype:         specify aaaa if needed
 *   - sockfd:          socker file descriptor
 *   - query:           query to be sent
 *   - query_length:    length of the query
 *   - server_addr4:    sockaddr_in structure created before in setup_dns_server4
 *   - server_addr6:    sockaddr_in6 structure created before in setup_dns_server6
 * Return:
 *   - nothing
 */
void send_dns_query(int reqtype, int sockfd, unsigned char *query, size_t query_length, struct sockaddr_in server_addr4, struct sockaddr_in6 server_addr6);

/*
 * Function: receive_dns_response
 * Description: receives a response from the server
 * Parameters:
 *   - sockfd:          socker file descriptor
 *   - server_addr:     sockaddr_in structure created before in setup_dns_server
 *   - query:           query to free memory in case of error
 * Return:
 *   - response from the server
 */
unsigned char *receive_dns_response(int sockfd, struct sockaddr_in server_addr, unsigned char *query);

#endif