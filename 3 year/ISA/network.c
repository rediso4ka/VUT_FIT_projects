/*
 * File: network.c
 * Author: xshevc01
 * Description: implementation of creating socket
 *              and sending / receiving packets
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "network.h"
#include "error.h"
#include "query.h"
#include "parser.h"

void dns_request_body(int rd, int reverse, int reqtype, char *server, const char *domain, int port)
{
      int sockfd = create_udp_socket(reqtype);

      // setting up sockaddr structure for IPv4 or IPv6 depending on reqtype
      struct sockaddr_in server_addr4;
      struct sockaddr_in6 server_addr6;
      if (reqtype)
      {
            server_addr6 = setup_dns_server6(reqtype, server, port);
      }
      else
      {
            server_addr4 = setup_dns_server4(reqtype, server, port);
      }

      size_t query_length = 0;
      unsigned char *query = create_dns_query(rd, reverse, reqtype, domain, &query_length);

      send_dns_query(reqtype, sockfd, query, query_length, server_addr4, server_addr6);
      unsigned char *response = receive_dns_response(sockfd, server_addr4, query);

      parse_dns_response(response, query_length);

      free(query);
      free(response);
      close(sockfd);
}

int create_udp_socket(int reqtype)
{
      // creating a socket for IPv4 or IPv6 depending on reqtype
      int sockfd = socket((reqtype == 1) ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if (sockfd < 0)
      {
            print_error_and_exit("socket not created!");
      }
      return sockfd;
}

struct sockaddr_in setup_dns_server4(int reqtype, char *server, int port)
{
      // create a sockaddr_in structure to hold the server address information
      struct sockaddr_in server_addr4;
      memset(&server_addr4, 0, sizeof(server_addr4));

      // set the address family to AF_INET for IPv4
      server_addr4.sin_family = AF_INET;
      server_addr4.sin_port = htons(port);

      char *server_ip = get_server_ip(reqtype, server);

      // set the IPv4 address in network byte order
      server_addr4.sin_addr.s_addr = inet_addr(server_ip);
      free(server_ip);

      return server_addr4;
}

struct sockaddr_in6 setup_dns_server6(int reqtype, char *server, int port)
{
      // create a sockaddr_in6 structure to hold the server address information
      struct sockaddr_in6 server_addr6;
      memset(&server_addr6, 0, sizeof(server_addr6));

      // Set the address family to AF_INET6 for IPv6
      server_addr6.sin6_family = AF_INET6;
      server_addr6.sin6_port = htons(port);

      char *server_ip = get_server_ip(reqtype, server);

      // set the IPv6 address in network byte order
      if (inet_pton(AF_INET6, server_ip, &(server_addr6.sin6_addr)) != 1)
      {
            free(server_ip);
            print_error_and_exit("failed to set the server address for IPv6");
      }
      free(server_ip);

      return server_addr6;
}

char *get_server_ip(int reqtype, char *server)
{
      // determine the address family and the string length for IP address
      int af_inet = (reqtype == 1) ? AF_INET6 : AF_INET;
      int inet_addrstrlen = (reqtype == 1) ? INET6_ADDRSTRLEN : INET_ADDRSTRLEN;

      // set up hints for address resolution
      struct addrinfo hints, *res;
      memset(&hints, 0, sizeof(hints));
      hints.ai_family = af_inet;
      hints.ai_socktype = SOCK_DGRAM;

      // perform DNS resolution to obtain server addresses
      int result = getaddrinfo(server, NULL, &hints, &res);
      if (result != 0)
      {
            print_error_and_exit("could not receive IP address of domain!");
      }

      // iterate through the results to find an address with the desired family
      struct addrinfo *current = res;
      while (current != NULL)
      {
            if (current->ai_family == af_inet)
            {
                  break;
            }
            current = current->ai_next;
      }

      // if no matching address was found, free resources and exit
      if (current == NULL)
      {
            freeaddrinfo(res);
            print_error_and_exit("no address found with the desired family!");
      }

      // prepare variables to hold the received server addresses
      struct sockaddr_in6 received_server_addr6;
      struct sockaddr_in received_server_addr4;

      // extract the address information based on the address family
      if (af_inet == AF_INET6)
      {
            memcpy(&received_server_addr6, current->ai_addr, sizeof(struct sockaddr_in6));
      }
      else
      {
            memcpy(&received_server_addr4, current->ai_addr, sizeof(struct sockaddr_in));
      }

      freeaddrinfo(res);

      char *server_ip = (char *)malloc(inet_addrstrlen);
      if (server_ip == NULL)
      {
            print_error_and_exit("could not allocate memory for server ip!");
      }

      // convert the binary address to a text form and store it in server_ip
      if (af_inet == AF_INET6)
      {
            if (inet_ntop(af_inet, &received_server_addr6.sin6_addr, server_ip, inet_addrstrlen) == NULL)
            {
                  print_error_and_exit("error during converting IP addresses from binary to text form!");
            }
      }
      else
      {
            if (inet_ntop(af_inet, &received_server_addr4.sin_addr, server_ip, inet_addrstrlen) == NULL)
            {
                  print_error_and_exit("error during converting IP addresses from binary to text form!");
            }
      }

      return server_ip;
}

void send_dns_query(int reqtype, int sockfd, unsigned char *query, size_t query_length, struct sockaddr_in server_addr4, struct sockaddr_in6 server_addr6)
{
      // set timeout for receiving response
      struct timeval timeout;
      timeout.tv_sec = 5;
      timeout.tv_usec = 0;
      setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
      if (reqtype)
      {
            // send the query using an IPv6 address
            if (sendto(sockfd, query, query_length, 0, (struct sockaddr *)&server_addr6, sizeof(server_addr6)) < 0)
            {
                  free(query);
                  close(sockfd);
                  print_error_and_exit("could not send the query!");
            }
      }
      else
      {
            // send the query using an IPv4 address
            if (sendto(sockfd, query, query_length, 0, (struct sockaddr *)&server_addr4, sizeof(server_addr4)) < 0)
            {
                  free(query);
                  close(sockfd);
                  print_error_and_exit("could not send the query!");
            }
      }
      return;
}

unsigned char *receive_dns_response(int sockfd, struct sockaddr_in server_addr, unsigned char *query)
{
      unsigned char *response = (unsigned char *)malloc(MAX_QUERY_LENGTH);
      if (response == NULL)
      {
            print_error_and_exit("could not allocate memory for response!");
      }

      // receive the response
      int i = sizeof server_addr;
      ssize_t response_length = recvfrom(sockfd, (char *)response, MAX_QUERY_LENGTH, 0, (struct sockaddr *)&server_addr, (socklen_t *)&i);

      if (response_length < 0)
      {
            free(response);
            free(query);
            close(sockfd);
            print_error_and_exit("could not receive response from server!");
      }

      return response;
}