# README

## Author
Aleksandr Shevchenko (xshevc01)

## Date
22.10.2023

## Description
The DNS (Domain Name System) Resolver program is a command-line utility designed to perform DNS queries for both IPv4 and IPv6 addresses, supporting a range of query types, including standard queries (A and AAAA records) and reverse queries (PTR records). The program can be used to resolve domain names into IP addresses and retrieve additional information about authoritative name servers and more. It provides users with the flexibility to choose query types, control recursion, and specify DNS server information.

### Extensions
In order for the DNS Authority section to write out at least basic information, as an extension, I
implemented handling `SOA` responses. For some other types, such as NS, DS, DNSKEY and MX my program just prints out the type, not parsing the rest. It lets the user at least to see which response type was received.

## Instructions on use
To unpack the project archive, use `tar -xvf xshevc01.tar`. Then, use `make` to compile the project and `make test` to run tests.

**Usage:**

`./dns [-r] [-x] [-6] -s server [-p port] address`

**Options:**

`-r`            Enable recursion (Recursion Desired = 1), otherwise recursion is disabled.

`-x`            Perform a reverse DNS query instead of a direct query.

`-6`            Perform an AAAA record query instead of the default A record query.

`-s server`     Specify the IP address or domain name of the DNS server to send the query to.

`-p port`       Specify the port number for the DNS query (default is 53).

`address`       The address or hostname to query.

## Files

 Files | Description
--- | --- | 
dns.c | Main body of DNS program
error.c/h | Printing errors and warnings
network.c/h | Creating a socket and sending / receiving packets
parser.c/h | Argument parser, including constants from the task
query.c/h | Creating and parsing packets
dns_test.sh | Tests
Makefile | Makefile
manual.pdf | Documentation
README.md | Readme