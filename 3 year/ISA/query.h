/*
 * File: query.h
 * Author: xshevc01
 * Description: header file for creating and parsing packets
 */

#ifndef QUERY_H
#define QUERY_H

#include <stdint.h>

#define TYPE_A 1     // Address Record
#define TYPE_CNAME 5 // Canonical Name Record
#define TYPE_PTR 12  // Pointer Record
#define TYPE_AAAA 28 // IPv6 Address Record

// for additional and authority sections
#define TYPE_NS 2      // Name Server
#define TYPE_SOA 6     // Start of Authority
#define TYPE_MX 15     // Mail Exchanger
#define TYPE_DS 43     // Delegation Signer
#define TYPE_DNSKEY 48 // DNS Key

#define CLASS_IN 1 // Internet Addresses

// source: https://www.catchpoint.com/blog/how-dns-works
//  0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                      ID                       |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |QR|   Opcode  |AA|TC|RD|RA|Z |AD|CD|   RCODE   |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                    QDCOUNT                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                    ANCOUNT                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                    NSCOUNT                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                    ARCOUNT                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// using pragma to specify the alignment of structure members in memory
#pragma pack(push, 1)
typedef struct
{
      uint16_t id;
      uint16_t flags;
      uint16_t qdcount;
      uint16_t ancount;
      uint16_t nscount;
      uint16_t arcount;
} DNSheader;
#pragma pack(pop)

// source: https://mislove.org/teaching/cs4700/spring11/handouts/project1-primer.pdf
//  0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                                               |
// /                    NAME                       /
// /                                               /
// |                                               |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                    TYPE                       |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                    CLASS                      |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                     TTL                       |
// |                                               |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                  RDLENGTH                     |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// /                    RDATA                      /
// /                                               /
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
#pragma pack(push, 1)
typedef struct
{
      unsigned char *name;
      uint16_t type;
      uint16_t class;
      uint32_t ttl;
      uint16_t rdlength;
      unsigned char *rdata;
} DNSanswer;
#pragma pack(pop)

/*
 * Function: create_dns_query
 * Description: creates a DNS query
 * Parameters:
 *   - rd:              specify recursion if needed
 *   - reverse:         specify reverse if needed
 *   - reqtype:         specify aaaa if needed
 *   - domain:          requested address
 *   - query_length:    length of query to return
 * Return:
 *   - query
 */
unsigned char *create_dns_query(int rd, int reverse, int reqtype, const char *domain, size_t *query_length);

/*
 * Function: set_dns_flags
 * Description: used by set_query_header, sets flags values in the header
 * Parameters:
 *   - qr:        query / response
 *   - opcode:    kind of query
 *   - aa:        authoritative answer
 *   - tc:        truncation
 *   - rd:        recursion desired
 *   - ra:        recursion available
 *   - z:         reserved for future use
 *   - ad:        authentic data
 *   - cd:        checking disabled
 *   - rcode:     response code
 * Return:
 *   - created flags
 */
uint16_t set_dns_flags(int qr, int opcode, int aa, int tc, int rd, int ra, int z, int ad, int cd, int rcode);

/*
 * Function: set_query_header
 * Description: creates a DNS header in given query
 * Parameters:
 *   - query:     pointer to a query to construct
 *   - rd:        specify recursion if needed
 * Return:
 *   - nothing
 */
void set_query_header(unsigned char *query, int rd);

/*
 * Function: set_query_question
 * Description: creates a DNS question in given query
 * Parameters:
 *   - reverse:   specify reverse if needed
 *   - reqtype:   specify aaaa if needed
 *   - query:     pointer to a query to construct
 *   - domain:    requested address
 * Return:
 *   - query_length for create_dns_query
 */
size_t set_query_question(int reverse, int reqtype, unsigned char *query, const char *domain);

/*
 * Function: is_ipv4
 * Description: checks if given address is IPv4
 * Parameters:
 *  - address:   address to check
 * Return:
 * - 1 if IPv4, 0 otherwise
 */
int is_ipv4(const char *address);

/*
 * Function: convert_and_set_domain
 * Description: used by set_query_question, encode the domain name into the query
 * Parameters:
 *   - current_ptr:     pointer to a question position in query
 *   - domain:          requested address
 *   - reverse:         specify reverse if needed
 * Return:
 *   - new position of current_ptr
 */
unsigned char *convert_and_set_domain(unsigned char *current_ptr, const char *domain, int reverse);

/*
 * Function: parse_dns_response
 * Description: parses DNS response
 * Parameters:
 *   - response:        response received from the server
 *   - query_length:    length of sent query
 * Return:
 *   - nothing
 */
void parse_dns_response(unsigned char *response, size_t query_length);

/*
 * Function: parse_dns_header
 * Description: parses DNS header in DNS response
 * Parameters:
 *   - response:  response received from the server
 * Return:
 *   - pointer to a DNS header
 */
DNSheader *parse_dns_header(unsigned char *response);

/*
 * Function: parse_dns_question
 * Description: parses question section of DNS response
 * Parameters:
 *   - response:        response received from the server
 *   - dns_header:      DNS header received in parse_dns_header
 * Return:
 *   - pointer to an answers section
 */
unsigned char *parse_dns_question(unsigned char *response, DNSheader *dns_header);

/*
 * Function: parse_dns_answer
 * Description: parses answer section of DNS response
 * Parameters:
 *   - response:        response received from the server
 *   - answer:          pointer to answers section
 *   - dns_header:      DNS header received in parse_dns_header
 * Return:
 *   - pointer to an authority section
 */
unsigned char *parse_dns_answer(unsigned char *response, unsigned char *answer, DNSheader *dns_header);

/*
 * Function: parse_dns_authority
 * Description: parses authority section of DNS response
 * Parameters:
 *   - response:        response received from the server
 *   - authority:       pointer to authority section
 *   - dns_header:      DNS header received in parse_dns_header
 * Return:
 *   - pointer to an additional section
 */
unsigned char *parse_dns_authority(unsigned char *response, unsigned char *authority, DNSheader *dns_header);

/*
 * Function: parse_dns_additional
 * Description: parses additional section of DNS response
 * Parameters:
 *   - response:        response received from the server
 *   - additional:      pointer to additional section
 *   - dns_header:      DNS header received in parse_dns_header
 * Return:
 *   - nothing
 */
void parse_dns_additional(unsigned char *response, unsigned char *additional, DNSheader *dns_header);

/*
 * Function: read_domain_name
 * Description: used by parse_dns_answers, helps reading NAME and RDATA from response
 * Parameters:
 *   - reader:    current_ptr while working with response
 *   - response:  response
 *   - count:     stop variable
 * Return:
 *   - pointer to a read name
 *
 * source: https://mislove.org/teaching/cs4700/spring11/handouts/project1-primer.pdf
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * | 1  1|                 OFFSET                  |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
unsigned char *read_domain_name(unsigned char *reader, unsigned char *response, int *count);

#endif