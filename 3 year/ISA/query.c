/*
 * File: query.c
 * Author: xshevc01
 * Description: implementation of creating and parsing packets
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "parser.h"
#include "error.h"
#include "query.h"

unsigned char *create_dns_query(int rd, int reverse, int reqtype, const char *domain, size_t *query_length)
{
      // create a buffer to hold the dns query
      unsigned char *query = (unsigned char *)malloc(MAX_QUERY_LENGTH);
      if (query == NULL)
      {
            print_error_and_exit("could not allocate memory for query!");
      }

      set_query_header(query, rd);

      *query_length = set_query_question(reverse, reqtype, query, domain);
      // for (unsigned char *i = query; i < query + 40; i++) {
      //       printf("0x%02x ", (unsigned char)*i); // %02x ensures two-digit hexadecimal representation
      // }
      return query;
}

uint16_t set_dns_flags(int qr, int opcode, int aa, int tc, int rd, int ra, int z, int ad, int cd, int rcode)
{
      uint16_t flags = 0;
      flags |= (rd & 0x01) << 8;      // Recursion Desired
      flags |= (tc & 0x01) << 9;      // Truncated
      flags |= (aa & 0x01) << 10;     // Authoritative Answer
      flags |= (opcode & 0x0F) << 11; // Purpose of Message (4 bits)
      flags |= (qr & 0x01) << 15;     // Query/Response Flag
      flags |= (rcode & 0x0F);        // Response Code (4 bits)
      flags |= (cd & 0x01) << 5;      // Checking Disabled
      flags |= (ad & 0x01) << 6;      // Authenticated Data
      flags |= (z & 0x01) << 7;       // Reserved
      flags |= (ra & 0x01);           // Recursion Available
      return htons(flags);
}

void set_query_header(unsigned char *query, int rd)
{
      DNSheader *dns_header = (DNSheader *)query;
      dns_header->id = (uint16_t)htons(getpid());
      dns_header->flags = set_dns_flags(0, 0, 0, 0, rd, 0, 0, 0, 0, 0);
      dns_header->qdcount = htons(1); // number of questions
      dns_header->ancount = 0;
      dns_header->nscount = 0;
      dns_header->arcount = 0;

      return;
}

size_t set_query_question(int reverse, int reqtype, unsigned char *query, const char *domain)
{
      // calculate the length of the DNS query header
      size_t header_length = sizeof(DNSheader);

      // pointer to the current position in the query buffer
      unsigned char *question = query + header_length;
      unsigned char *current_ptr = question;

      current_ptr = convert_and_set_domain(current_ptr, domain, reverse);
      if (current_ptr == NULL)
      {
            free(query);
            print_error_and_exit("invalid format of IPv4 address!");
      }

      uint16_t qtype = htons(TYPE_A);
      if (reverse)
      {
            qtype = htons(TYPE_PTR);
      }
      else if (reqtype)
      {
            qtype = htons(TYPE_AAAA);
      }

      uint16_t qclass = htons(CLASS_IN);

      // copy the query type and class to the query buffer
      memcpy(current_ptr, &qtype, sizeof(uint16_t));
      current_ptr += sizeof(uint16_t);

      memcpy(current_ptr, &qclass, sizeof(uint16_t));
      current_ptr += sizeof(uint16_t);

      return current_ptr - query;
}

int is_ipv4(const char *address)
{
      struct sockaddr_in sa;
      return inet_pton(AF_INET, address, &(sa.sin_addr)) != 0;
}

unsigned char *convert_and_set_domain(unsigned char *current_ptr, const char *domain, int reverse)
{
      // received IPv4 address firstly needs to be reversed and end with in-addr.arpa
      char address[32];
      if (reverse)
      {
            if (!is_ipv4(domain))
            {
                  return NULL;
            }
            char *octets[4];
            char *token = strtok((char *)domain, ".");
            for (int i = 0; i < 4; i++)
            {
                  octets[i] = token;
                  token = strtok(NULL, ".");
            }
            sprintf(address, "%s.%s.%s.%s.in-addr.arpa", octets[3], octets[2], octets[1], octets[0]);
            domain = address;
      }

      // www.fit.vut.cz -> 3www3fit3vut2cz0
      char *token = strtok((char *)domain, ".");
      while (token != NULL)
      {
            // length of the current label
            size_t label_len = strlen(token);
            *current_ptr = (unsigned char)label_len;
            current_ptr++;
            // copy the  label
            memcpy(current_ptr, token, label_len);
            current_ptr += label_len;
            // move to the next label
            token = strtok(NULL, ".");
      }
      // terminate the domain name with a zero-length label
      *current_ptr = 0x00;
      current_ptr++;
      return current_ptr;
}

void parse_dns_response(unsigned char *response, size_t query_length)
{
      DNSheader *dns_header = parse_dns_header(response);
      if (dns_header == NULL)
      {
            return;
      }
      unsigned char *answers = parse_dns_question(response, dns_header);
      unsigned char *authority = parse_dns_answer(response, answers, dns_header);
      unsigned char *additional = parse_dns_authority(response, authority, dns_header);
      parse_dns_additional(response, additional, dns_header);
      return;
}

DNSheader *parse_dns_header(unsigned char *response)
{
      DNSheader *dns_header = (DNSheader *)response;
      // extract the Response Code (RCODE) from the header flags
      uint16_t rcode = (ntohs(dns_header->flags) & 0x000f);
      if (rcode != 0)
      {
            switch (rcode)
            {
            case 1:
                  print_warning("Format error - The name server was unable to interpret the query!");
                  break;

            case 2:
                  print_warning("Server failure - The name server was unable to process this query due to a problem with the name server!");
                  break;

            case 3:
                  print_warning("Name Error - The domain name referenced in the query does not exist!");
                  break;

            case 4:
                  print_warning("Not Implemented - The name server does not support the requested kind of query!");
                  break;

            case 5:
                  print_warning("Refused - The name server refuses to perform the specified operation for policy reasons!");
                  break;

            default:
                  print_warning("Received an error from server!");
                  break;
            }
            // return NULL;
      }

      // extract header flags
      uint16_t flags = ntohs(dns_header->flags);
      int aa = (flags >> 10) & 0x01;
      int ra = (flags >> 7) & 0x01;
      int tc = (flags >> 9) & 0x01;

      printf("Authoritative: ");
      if (aa)
      {
            printf("Yes, ");
      }
      else
      {
            printf("No, ");
      }

      printf("Recursive: ");
      if (ra)
      {
            printf("Yes, ");
      }
      else
      {
            printf("No, ");
      }

      printf("Truncated: ");
      if (tc)
      {
            printf("Yes\n");
      }
      else
      {
            printf("No\n");
      }
      return dns_header;
}

unsigned char *parse_dns_question(unsigned char *response, DNSheader *dns_header)
{
      unsigned char *question = response + sizeof(DNSheader);
      uint16_t qtype = 0;
      uint16_t qclass = 0;
      int to_skip = 0;
      printf("Question section (%d)\n", ntohs(dns_header->qdcount));

      // iterate through all questions
      for (int i = 0; i < ntohs(dns_header->qdcount); i++)
      {
            // Extract QNAME
            unsigned char *name = read_domain_name(question, response, &to_skip);
            question += to_skip;

            // extract QTYPE and QCLASS
            memcpy(&qtype, question, sizeof(uint16_t));
            question += sizeof(uint16_t);

            memcpy(&qclass, question, sizeof(uint16_t));
            question += sizeof(uint16_t);

            // print QNAME
            printf("\t%s, ", name);

            // print QTYPE
            switch (ntohs(qtype))
            {
            case TYPE_A:
                  printf("A, ");
                  break;

            case TYPE_PTR:
                  printf("PTR, ");
                  break;

            case TYPE_AAAA:
                  printf("AAAA, ");
                  break;

            default:
                  break;
            }

            // print QCLASS
            switch (ntohs(qclass))
            {
            case CLASS_IN:
                  printf("IN\n");
                  break;

            default:
                  printf("\n");
                  break;
            }

            free(name);
      }
      return question;
}

unsigned char *parse_dns_answer(unsigned char *response, unsigned char *answer, DNSheader *dns_header)
{
      uint16_t type = 0;
      uint16_t class = 0;
      uint32_t ttl = 0;
      uint16_t rdlength = 0;
      int to_skip = 0;
      printf("Answer section (%d)\n", ntohs(dns_header->ancount));

      // iterate through all answers
      for (int i = 0; i < ntohs(dns_header->ancount); i++)
      {
            // extract NAME
            unsigned char *name = read_domain_name(answer, response, &to_skip);
            unsigned char *rdata = NULL;
            answer += to_skip;

            // extract TYPE, CLASS, TTL, RDLENGTH
            memcpy(&type, answer, sizeof(uint16_t));
            answer += sizeof(uint16_t);

            memcpy(&class, answer, sizeof(uint16_t));
            answer += sizeof(uint16_t);

            memcpy(&ttl, answer, sizeof(uint32_t));
            answer += sizeof(uint32_t);

            memcpy(&rdlength, answer, sizeof(uint16_t));
            answer += sizeof(uint16_t);

            // extract RDATA
            if (ntohs(type) == TYPE_A || ntohs(type) == TYPE_AAAA)
            {
                  // IPv4 or IPv6 address
                  rdata = (unsigned char *)malloc(ntohs(rdlength));
                  for (int j = 0; j < ntohs(rdlength); j++)
                  {
                        rdata[j] = answer[j];
                  }
                  rdata[ntohs(rdlength)] = '\0';
                  answer = answer + ntohs(rdlength);
            }
            else
            {
                  // not an address
                  rdata = read_domain_name(answer, response, &to_skip);
                  answer += to_skip;
            }

            // print NAME
            printf("\t%s, ", name);

            // print TYPE
            switch (ntohs(type))
            {
            case TYPE_A:
                  printf("A, ");
                  break;

            case TYPE_CNAME:
                  printf("CNAME, ");
                  break;

            case TYPE_PTR:
                  printf("PTR, ");
                  break;

            case TYPE_AAAA:
                  printf("AAAA, ");
                  break;

            default:
                  break;
            }

            // print CLASS
            switch (ntohs(class))
            {
            case CLASS_IN:
                  printf("IN, ");
                  break;

            default:
                  break;
            }

            // print TTL
            printf("%u, ", ntohl(ttl));

            // print RDATA depending on TYPE
            long *p = (long *)rdata;
            struct sockaddr_in a;
            struct in6_addr aaaa;

            switch (ntohs(type))
            {
            case TYPE_A:
                  a.sin_addr.s_addr = (*p);
                  printf("%s\n", inet_ntoa(a.sin_addr));
                  break;

            case TYPE_CNAME:
                  printf("%s\n", rdata);
                  break;

            case TYPE_PTR:
                  printf("%s\n", rdata);
                  break;

            case TYPE_AAAA:
                  // copy the raw binary data to an in6_addr structure
                  memcpy(&aaaa, rdata, sizeof(struct in6_addr));
                  char ip6str[INET6_ADDRSTRLEN];
                  // convert the in6_addr structure to a human-readable IPv6 address string
                  if (inet_ntop(AF_INET6, &aaaa, ip6str, sizeof(ip6str)) != NULL)
                  {
                        printf("%s\n", ip6str);
                  }
                  else
                  {
                        printf("Error converting IPv6 address\n");
                  }
                  break;

            default:
                  printf("%s\n", rdata);
                  break;
            }

            free(name);
            free(rdata);
      }

      return answer;
}

unsigned char *parse_dns_authority(unsigned char *response, unsigned char *authority, DNSheader *dns_header)
{
      uint16_t type = 0;
      uint16_t class = 0;
      uint32_t ttl = 0;
      uint16_t rdlength = 0;
      int to_skip = 0;
      printf("Authority section (%d)\n", ntohs(dns_header->nscount));

      // iterate through all authorities
      for (int i = 0; i < ntohs(dns_header->nscount); i++)
      {
            // extract NAME
            unsigned char *name = read_domain_name(authority, response, &to_skip);
            authority += to_skip;

            // extract TYPE, CLASS, TTL, RDLENGTH
            memcpy(&type, authority, sizeof(uint16_t));
            authority += sizeof(uint16_t);

            memcpy(&class, authority, sizeof(uint16_t));
            authority += sizeof(uint16_t);

            memcpy(&ttl, authority, sizeof(uint32_t));
            authority += sizeof(uint32_t);

            memcpy(&rdlength, authority, sizeof(uint16_t));
            authority += sizeof(uint16_t);

            // extract RDATA
            unsigned char *rdata = read_domain_name(authority, response, &to_skip);
            authority += to_skip;

            // print NAME
            printf("\t%s, ", name);

            // print TYPE
            switch (ntohs(type))
            {
            case TYPE_NS:
                  printf("NS, ");
                  break;

            case TYPE_SOA:
                  printf("SOA, ");
                  break;

            case TYPE_DS:
                  printf("DS, ");
                  break;

            case TYPE_DNSKEY:
                  printf("DNSKEY, ");
                  break;

            default:
                  break;
            }

            // print CLASS
            switch (ntohs(class))
            {
            case CLASS_IN:
                  printf("IN, ");
                  break;

            default:
                  break;
            }

            // print TTL
            printf("%u, ", ntohl(ttl));

            // print RDATA, twice for SOA (primary server name + responsible person name)
            printf("%s", rdata);

            if (ntohs(type) == TYPE_SOA)
            {
                  unsigned char *rdata2 = read_domain_name(authority, response, &to_skip);
                  printf(" %s\n", rdata2);
                  free(rdata2);
            }
            else
            {
                  printf("\n");
            }

            free(rdata);
            free(name);
      }

      return authority;
}

void parse_dns_additional(unsigned char *response, unsigned char *additional, DNSheader *dns_header)
{
      uint16_t type = 0;
      uint16_t class = 0;
      uint32_t ttl = 0;
      uint16_t rdlength = 0;
      int to_skip = 0;
      printf("Additional section (%d)\n", ntohs(dns_header->arcount));

      // iterate through all additionals
      for (int i = 0; i < ntohs(dns_header->arcount); i++)
      {
            // extract NAME
            unsigned char *name = read_domain_name(additional, response, &to_skip);
            unsigned char *rdata = NULL;
            additional += to_skip;

            // extract TYPE, CLASS, TTL, RDLENGTH
            memcpy(&type, additional, sizeof(uint16_t));
            additional += sizeof(uint16_t);

            memcpy(&class, additional, sizeof(uint16_t));
            additional += sizeof(uint16_t);

            memcpy(&ttl, additional, sizeof(uint32_t));
            additional += sizeof(uint32_t);

            memcpy(&rdlength, additional, sizeof(uint16_t));
            additional += sizeof(uint16_t);

            // extract RDATA
            if (ntohs(type) == TYPE_A || ntohs(type) == TYPE_AAAA)
            {
                  // IPv4 or IPv6 address
                  int data_length = ntohs(rdlength);
                  rdata = (unsigned char *)malloc(data_length);
                  for (int j = 0; j < data_length; j++)
                  {
                        rdata[j] = additional[j];
                  }
                  rdata[data_length] = '\0';
                  additional += data_length;
            }
            else
            {
                  // not an address
                  rdata = read_domain_name(additional, response, &to_skip);
                  additional += to_skip;
            }

            // print NAME
            printf("\t%s, ", name);

            // print TYPE
            switch (ntohs(type))
            {
            case TYPE_A:
                  printf("A, ");
                  break;

            case TYPE_NS:
                  printf("NS , ");
                  break;

            case TYPE_MX:
                  printf("MX , ");
                  break;

            case TYPE_AAAA:
                  printf("AAAA, ");
                  break;

            default:
                  break;
            }

            // print CLASS
            switch (ntohs(class))
            {
            case CLASS_IN:
                  printf("IN, ");
                  break;

            default:
                  break;
            }

            // print TTL
            printf("%u, ", ntohl(ttl));

            // print RDATA depending on TYPE
            long *p = (long *)rdata;
            struct sockaddr_in a;
            struct in6_addr aaaa;

            switch (ntohs(type))
            {
            case TYPE_A:
                  a.sin_addr.s_addr = (*p);
                  printf("%s\n", inet_ntoa(a.sin_addr));
                  break;
            case TYPE_AAAA:
                  // copy the raw binary data to an in6_addr structure
                  memcpy(&aaaa, rdata, sizeof(struct in6_addr));
                  char ip6str[INET6_ADDRSTRLEN];
                  // convert the in6_addr structure to a human-readable IPv6 address string
                  if (inet_ntop(AF_INET6, &aaaa, ip6str, sizeof(ip6str)) != NULL)
                  {
                        printf("%s\n", ip6str);
                  }
                  else
                  {
                        printf("Error converting IPv6 address\n");
                  }
                  break;

            default:
                  printf("%s\n", rdata);
                  break;
            }

            free(name);
            free(rdata);
      }

      return;
}

// inspired by https://www.binarytides.com/dns-query-code-in-c-with-linux-sockets/
unsigned char *read_domain_name(unsigned char *reader, unsigned char *response, int *to_skip)
{
      unsigned int p = 0, jumped = 0, offset;

      *to_skip = 1;
      unsigned char *name = (unsigned char *)malloc(256);

      name[0] = '\0';

      while (*reader != 0)
      {
            if (*reader >= 192)
            {
                  // handle pointer encoding, where the top 2 bits are set
                  // calculate the offset using the next byte and shift the bits
                  offset = (*reader) * 256 + *(reader + 1) - 49152; // 49152 = 11000000 00000000
                  reader = response + offset - 1;
                  jumped = 1;
            }
            else
            {
                  // read individual labels and build the domain name
                  name[p++] = *reader;
            }

            reader++;

            if (jumped == 0)
            {
                  *to_skip = *to_skip + 1;
            }
      }

      name[p] = '\0';
      if (jumped == 1)
      {
            *to_skip = *to_skip + 1;
      }

      // convert the domain name to the human-readable format
      for (int i = 0; i < (int)strlen((const char *)name); i++)
      {
            p = name[i];
            for (int j = 0; j < (int)p; j++)
            {
                  name[i] = name[i + 1];
                  i++;
            }
            name[i] = '.';
      }

      return name;
}