#!/bin/bash
#
# File: dns_test.sh
# Author: xshevc01
#

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'


echo ""
echo -e "${NC}TESTS FROM THE ASSIGNMENT:"

# TEST 1
result=$(./dns -r -s kazi.fit.vutbr.cz www.fit.vut.cz)

if [[ $result == *"147.229.9.26"* ]]; then
    echo -e "${GREEN}Test 1: passed!       (www.fit.vut.cz)"
else
    echo -e "${RED}--- Test 1 FAILED: Received IP does not match 147.229.9.26.        (www.fit.vut.cz)"
fi

# TEST 2
result=$(./dns -r -s kazi.fit.vutbr.cz www.github.com)

if [[ $result == *"CNAME"* && $result == *"140.82.121.3"* || $result == *"140.82.121.4"* ]]; then
    echo -e "${GREEN}Test 2: passed!       (www.github.com)"
else
    echo -e "${RED}--- Test 2 FAILED: Received IP does not match 140.82.121.3/4 or no occurance of CNAME.        (www.github.com)"
fi



echo ""
echo -e "${NC}TESTS FOR A:"

# TEST 1
result=$(./dns -r -s kazi.fit.vutbr.cz roundcube.fit.vutbr.cz)

if [[ $result == *"CNAME"* && $result == *"147.229.9.226"* ]]; then
    echo -e "${GREEN}Test 1: passed!       (roundcube.fit.vutbr.cz)"
else
    echo -e "${RED}--- Test 1 FAILED: Received IP does not match 147.229.9.226 or no occurance of CNAME.     (roundcube.fit.vutbr.cz)"
fi

# TEST 2
result=$(./dns -r -s kazi.fit.vutbr.cz vut.cz)

if [[ $result == *"147.229.2.90"* ]]; then
    echo -e "${GREEN}Test 2: passed!       (vut.cz)"
else
    echo -e "${RED}--- Test 2 FAILED: Received IP does not match 147.229.2.90.       (vut.cz)"
fi

# TEST 3
result=$(./dns -r -s kazi.fit.vutbr.cz slovnik.seznam.cz)

if [[ $result == *"77.75.76.131"* && $result == *"77.75.78.131"* ]]; then
    echo -e "${GREEN}Test 3: passed!       (slovnik.seznam.cz)"
else
    echo -e "${RED}--- Test 3 FAILED: Received IP does not match 77.75.76.131 or 77.75.78.131.       (slovnik.seznam.cz)"
fi

# TEST 4
result=$(./dns -r -s kazi.fit.vutbr.cz fluentu.com)

if [[ $result == *"65.9.95.117"* && $result == *"65.9.95.96"* && $result == *"65.9.95.55"* && $result == *"65.9.95.121"* ]]; then
    echo -e "${GREEN}Test 4: passed!       (fluentu.com)"
else
    echo -e "${RED}--- Test 4 FAILED: Received IP does not match 65.9.95.121/117/96/55.       (fluentu.com)"
fi



echo ""
echo -e "${NC}TESTS FOR AAAA (-6):"

# TEST 1
result=$(./dns  -6 -r -s kazi.fit.vutbr.cz www.fit.vut.cz)

if [[ $result == *"2001:67c:1220:809::93e5:91a"* && $result == *"AAAA"* ]]; then
    echo -e "${GREEN}Test 1: passed!       (www.fit.vut.cz)"
else
    echo -e "${RED}--- Test 1 FAILED: Received address name does not match 2001:67c:1220:809::93e5:91a or no occurance of AAAA.     (www.fit.vut.cz)"
fi

# TEST 2
result=$(./dns -r -6 -s kazi.fit.vutbr.cz roundcube.fit.vutbr.cz)

if [[ $result == *"2001:67c:1220:809::93e5:9e2"* && $result == *"AAAA"* && $result == *"CNAME"* ]]; then
    echo -e "${GREEN}Test 2: passed!       (roundcube.fit.vutbr.cz)"
else
    echo -e "${RED}--- Test 2 FAILED: Received address name does not match 2001:67c:1220:809::93e5:9e2 or no occurance of AAAA / CNAME.     (roundcube.fit.vutbr.cz)"
fi

# TEST 3
result=$(./dns -r -6 -s kazi.fit.vutbr.cz slovnik.seznam.cz)

if [[ $result == *"2a02:598:2::131"* && $result == *"2a02:598:a::78:131"* && $result == *"AAAA"* ]]; then
    echo -e "${GREEN}Test 3: passed!       (slovnik.seznam.cz)"
else
    echo -e "${RED}--- Test 3 FAILED: Received address name does not match 2a02:598:2::131 / 2a02:598:a::78:131 or no occurance of AAAA.     (slovnik.seznam.cz)"
fi

# TEST 4
result=$(./dns -r -6 -s kazi.fit.vutbr.cz dns.google)

if [[ $result == *"2001:4860:4860::8888"* && $result == *"2001:4860:4860::8844"* && $result == *"AAAA"* ]]; then
    echo -e "${GREEN}Test 4: passed!       (dns.google)"
else
    echo -e "${RED}--- Test 4 FAILED: Received address name does not match 2001:4860:4860::8888 / 2001:4860:4860::8844 or no occurance of AAAA.     (dns.google)"
fi


echo ""
echo -e "${NC}TESTS FOR PTR (-x):"

# TEST 1
result=$(./dns -r -x -s kazi.fit.vutbr.cz 147.229.9.26)

if [[ $result == *"PTR"* && $result == *"www.fit.vut.cz"* ]]; then
    echo -e "${GREEN}Test 1: passed!       (147.229.9.26)"
else
    echo -e "${RED}--- Test 1 FAILED: Received domain name does not match www.fit.vut.cz or no occurance of PTR.     (147.229.9.26)"
fi

# TEST 2
result=$(./dns -r -x -s kazi.fit.vutbr.cz 140.82.121.3)

if [[ $result == *"PTR"* && $result == *"github.com"* ]]; then
    echo -e "${GREEN}Test 2: passed!       (140.82.121.3)"
else
    echo -e "${RED}--- Test 2 FAILED: Received domain name does not match github.com or no occurance of PTR.     (140.82.121.3)"
fi

# TEST 3
result=$(./dns -r -s kazi.fit.vutbr.cz 8.8.8.8 -x)

if [[ $result == *"PTR"* && $result == *"dns.google"* ]]; then
    echo -e "${GREEN}Test 3: passed!       (8.8.8.8)"
else
    echo -e "${RED}--- Test 3 FAILED: Received domain name does not match dns.google or no occurance of PTR.     (8.8.8.8)"
fi

# TEST 4
result=$(./dns -r -s kazi.fit.vutbr.cz 147.229.9.20 -x)

if [[ $result == *"PTR"* && $result == *"app.fit.vutbr.cz"* ]]; then
    echo -e "${GREEN}Test 4: passed!       (147.229.9.20)"
else
    echo -e "${RED}--- Test 4 FAILED: Received domain name does not match app.fit.vutbr.cz or no occurance of PTR.       (147.229.9.20)"
fi


echo ""
echo -e "${NC}TESTS FOR DIFFERENT SERVERS (-s):"
# TEST 1
result=$(./dns -r -s dns.google www.fit.vut.cz)

if [[ $result == *"147.229.9.26"* ]]; then
    echo -e "${GREEN}Test 1: passed!       (dns.google)"
else
    echo -e "${RED}--- Test 1 FAILED: Received IP does not match 147.229.9.26.     (dns.google)"
fi

# TEST 2
result=$(./dns -r -s resolver1.opendns.com www.fit.vut.cz)

if [[ $result == *"147.229.9.26"* ]]; then
    echo -e "${GREEN}Test 2: passed!       (resolver1.opendns.com)"
else
    echo -e "${RED}--- Test 2 FAILED: Received IP does not match 147.229.9.26.     (resolver1.opendns.com)"
fi

# TEST 3
result=$(./dns -r -s 1dot1dot1dot1.cloudflare-dns.com www.fit.vut.cz)

if [[ $result == *"147.229.9.26"* ]]; then
    echo -e "${GREEN}Test 3: passed!       (1dot1dot1dot1.cloudflare-dns.com)"
else
    echo -e "${RED}--- Test 3 FAILED: Received IP does not match 147.229.9.26.     (1dot1dot1dot1.cloudflare-dns.com)"
fi


echo ""
echo -e "${NC}TESTS FOR SOA (bonus):"
# TEST 1
result=$(./dns  -6 -r -s kazi.fit.vutbr.cz fluentu.com)

if [[ $result == *"SOA"* && $result == *"ns-376.awsdns-47.com. awsdns-hostmaster.amazon.com"* ]]; then
    echo -e "${GREEN}Test 1: passed!       (fluentu.com)"
else
    echo -e "${RED}--- Test 1 FAILED: Received domain name does not match ns-376.awsdns-47.com. awsdns-hostmaster.amazon.com or no occurance of SOA.     (fluentu.com)"
fi

# TEST 2
result=$(./dns  -6 -r -s kazi.fit.vutbr.cz vut.cz)

if [[ $result == *"SOA"* && $result == *"rhino.cis.vutbr.cz. hostmaster.vutbr.cz"* ]]; then
    echo -e "${GREEN}Test 2: passed!       (vut.cz)"
else
    echo -e "${RED}--- Test 2 FAILED: Received domain name does not match rhino.cis.vutbr.cz. hostmaster.vutbr.cz or no occurance of SOA.     (vut.cz)"
fi


echo ""
echo -e "${NC}OTHER TESTS:"

# TEST 1
result=$(./dns -r -x -6 -s kazi.fit.vutbr.cz www.fit.vutbr.cz 2>&1)

if [[ $result == *"Error: this version of program does not support flags -6 and -x at the same time!"* ]]; then
    echo -e "${GREEN}Test 1: passed!       (-6 and -x)"
else
    echo -e "${RED}--- Test 1 FAILED: -6 and -x are prohibited, but somehow the program ran.     (-6 and -x)"
fi

# TEST 2
result=$(./dns -r -p a -6 -s kazi.fit.vutbr.cz www.fit.vutbr.cz 2>&1)

if [[ $result == *"invalid port number"* ]]; then
    echo -e "${GREEN}Test 2: passed!       (-p a)"
else
    echo -e "${RED}--- Test 2 FAILED: port number is parsed incorrectly.     (-p a)"
fi

# TEST 3
result=$(./dns -r -s -x kazi.fit.vutbr.cz www.fit.vutbr.cz 2>&1)

if [[ $result == *"Error: could not receive IP address of domain!"* ]]; then
    echo -e "${GREEN}Test 3: passed!       (-s -x kazi...)"
else
    echo -e "${RED}--- Test 3 FAILED: server name must be set right after -s.     (-s -x kazi...)"
fi

# TEST 4
result=$(./dns -r  www.fit.vutbr.cz 2>&1)

if [[ $result == *"Error: server or address not given!"* ]]; then
    echo -e "${GREEN}Test 4: passed!       (no -s)"
else
    echo -e "${RED}--- Test 4 FAILED: you must set up server name.     (no -s)"
fi

# TEST 5
result=$(./dns -r -x -6 -s kazi.fit.vutbr.cz www.fit.vutbr.cz -h)

if [[ $result == *"Description"* ]]; then
    echo -e "${GREEN}Test 5: passed!       (-h)${NC}"
else
    echo -e "${RED}--- Test 5 FAILED: no help message.     (-h)${NC}"
fi