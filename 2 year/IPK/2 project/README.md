# README documentation

## Content structuring
- Theory
- Interesting source code sections
- Testing
- Extra functionality
- Used sources

## Theory

*Packet sniffer* is a tool used to capture and analyze network traffic. Sniffers work by capturing packets of data that are transmitted over a network, and then analyzing the data to extract information about the network traffic. To get more detailed information about sniffers and their features visit [1] nad [2]. Network protocols such as TCP, UDP, ICMPv4, ICMPv6, ARP, NDP, IGMP, and MLD, can be analyzed using a network sniffer.
- *TCP*: the Transmission Control Protocol is a connection-oriented protocol used to provide reliable data transmission over a network.
- *UDP*: the User Datagram Protocol is a connectionless protocol used to provide fast, unreliable data transmission over a network.
- *ICMPv4*: the Internet Control Message Protocol version 4 is used to send error messages and operational information about network conditions.
- *ICMPv6*: the Internet Control Message Protocol version 6 is used in IPv6 networks to send error messages and operational information about network conditions. 
- *ARP*: the Address Resolution Protocol is used to map a network address (such as an IP address) to a physical address (such as a MAC address).
- *NDP*: the Neighbor Discovery Protocol is used in IPv6 networks to discover neighboring nodes and their addresses.
- *IGMP*: the Internet Group Management Protocol is used to manage multicast groups on a network.
- *MLD*: the Multicast Listener Discovery protocol is used in IPv6 networks to manage multicast groups.

Using pcap library in C, the process of capturing and analyzing network packets using a sniffer is as follows (more details on [3] and [4]):

1) Open a pcap session: A session is opened using the pcap_open_live() function, specifying the network interface to capture packets on, the maximum packet size to capture, and whether to put the interface into promiscuous mode.

2) Compile a filter: A packet filter is compiled using the pcap_compile() function, specifying the filter expression in the form of a string. This filter can be used to capture only the packets that match certain criteria, such as packets with a specific port.

3) Set the filter: The compiled filter is set using the pcap_setfilter() function, which associates the filter with the pcap session.

4) Capture packets: Packets are captured using the pcap_loop() function, which repeatedly reads packets from the network interface and passes them to a user-supplied callback function for processing.

5) Analyze packets: Extract information about the packet headers and payloads from captured packets.

6) Print packet data: The extracted information can be printed to the console or to a log file using the printf() function.

7) Close the pcap session: When the sniffing operation is complete, the pcap session can be closed using the pcap_close() function.

## Interesting source code sections
In my opinion, the most interesting parts of the code are parsing arguments and extracting data from packages. Whereas the rest of the code is a standard procedure for any sniffer, but here I had to make an extra effort.

The abundance of different input parameters, as well as the fact that some of them start with the same letter, complicated the task of parsing arguments. Whereas in the first project I managed to bypass the system using `-x` instead of `--help`, then here it would look out of place. I had to study the function `getopt_long` and its features more deeply, so that in the end the parsing of arguments meets the condition of the project.

As I have already described in the theory section, each protocol performs a different function and they all differ primarily in different header lengths. To work with each protocol, I had to use different data structures, count the length of the payload and the places in the packet from where it starts in different ways.

## Testing
With the consent of my gracious colleague xassat00, I used his tests, which are implemented in Python using scapy library. Out of respect for his work and in order not to deprive him of his credit, I do not attach tests to the solution. I can only say that these tests create and send one packet for each type of protocol. I am also notified that he does not vouch for their correctness.

Testing took place on WSL Ubuntu 22.04.2 LTS and on Linux 2.6 (NixOS, given by IPK instructors), the program and Python script were run on two parallel terminals. Time zones on NixOS and WSL are different, I will leave the one that NixOS shows, which is `+00:00`. Due to the fact that the sniffer also accepts those packets that were not sent by the script, it does not matter how fast I run the script after starting the program. Anyway, the result of the program will be different, running the script (made on each test cases, described below) only guarantees that at least 1 package of the specified type(s) will be received. In each of the following test cases, running a python script looks like this:
```sh
sudo python3 ./test.py
./ipkcpc -m tcp
WARNING: Mac address to reach destination not found. Using broadcast.
.WARNING: Mac address to reach destination not found. Using broadcast.
..WARNING: more Mac address to reach destination not found. Using broadcast.
.....
Sent 8 packets.
```
The expected and actual output of all the following tests is the same, so I only specify the input, output and what and why was tested, where it is not clear from the context. I have divided all the tests into several groups depending on their purpose.
### Testing of compilation
0)
    ```sh
    input:
    make
    output:
    gcc -Werror -Wall -pedantic -Wunused-result -o ipk-sniffer ipk-sniffer.c -lpcap 
    ```
### Testing of print help
Options `-h` and `--help` print help message, and not depending on other flags exit the program successfully.

1)
    ```sh
    input:
    ./ipk-sniffer -h
    output:
    Usage: ./ipk-sniffer [-i interface | --interface interface] {-p port [--tcp|-t] [--udp|-u]} [--arp] [--ndp] [--icmp4] [--icmp6] [--igmp] [--mld] {-n num}
    Options:
    -i, --interface   interface to capture packets from (optional)
    -t, --tcp         capture TCP packets
    -u, --udp         capture UDP packets
        --arp         capture ARP packets
        --icmp4       capture ICMPv4 packets
        --icmp6       capture ICMPv6 packets
        --ndp         capture NDP packets
        --igmp        capture IGMP packets
        --mld         capture MLD packets
    -p, --port        port number to capture packets on
    -n, --num         number of packets to capture
    -h, --help        display this help message and exit
    ```
### Testing of list of active interfaces
According to the task, if an interface is not specified, a list of active interfaces is printed, regardless of what additional parameters are specified. All inputs below gave the same output, so I wrote it only once:

2)
    ```sh
    input:
    sudo ./ipk-sniffer
    ```
3)
    ```sh
    input:
    sudo ./ipk-sniffer -i
    ```
4)
    ```sh
    input:
    sudo ./ipk-sniffer --interface
    ```
5)
    ```sh
    input:
    sudo ./ipk-sniffer --interface -n 7
    ```
6)
    ```sh
    input:
    sudo ./ipk-sniffer --interface -u -t --arp -p 23
    ```
7)
    ```sh
    input:
    sudo ./ipk-sniffer -u -t --igmp -p 12 -n 3 -i
    ```
    ```sh
    output:
    eth0
    any
    lo
    dummy0
    tunl0
    sit0
    bluetooth-monitor
    nflog
    nfqueue
    dbus-system
    dbus-session
    bond0
    ```
### Testing of wrong parameters
Checking the behavior of the program when the user sets incorrect parameters, for example, incorrect flags or incorrect values of the port, number of packets or interface.

8)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth1
    output:
    pcap_lookupnet(): SIOCGIFADDR: eth1: No such device
    ```
9)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 -p 0
    output:
    ./ipk-sniffer: BAD PORT
    ```
10)
    ```sh
    input:
    sudo ./ipk-sniffer -p -9
    output:
    ./ipk-sniffer: BAD PORT
    ```
11)
    ```sh
    input:
    sudo ./ipk-sniffer -p
    output:
    ./ipk-sniffer: option requires an argument -- 'p'
    Unknown option: p
    ```
12)
    ```sh
    input:
    sudo ./ipk-sniffer -n 0
    output:
    ./ipk-sniffer: BAD NUM
    ```
13)
    ```sh
    input:
    sudo ./ipk-sniffer -n -10
    output:
    ./ipk-sniffer: BAD NUM
    ```
14)
    ```sh
    input:
    sudo ./ipk-sniffer -n
    output:
    ./ipk-sniffer: option requires an argument -- 'n'
    Unknown option: n
    ```
15)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 -a
    output:
    ./ipk-sniffer: invalid option -- 'a'
    Unknown option: a
    ```
16)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 -m
    output:
    ./ipk-sniffer: invalid option -- 'm'
    Unknown option: m
    ```
17)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 --mld --udp -t --arp --icmpv6 -n 9
    output:
    ./ipk-sniffer: unrecognized option '--icmpv6'
    Unknown option:
    ```
### Testing of 1 packet of each protocol
Checking if the program handles each given type of protocol (with long and short flags).

18)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 -t
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-16T18:47:32.748332+00:00
    src MAC: 00:15:5d:ab:25:97
    dst MAC: 00:15:5d:ac:e6:51
    frame length: 2062 bytes
    src IP: 172.30.110.144
    dst IP: 172.18.91.60
    type: TCP
    src port: 29910
    dst port: 8080

    0x0000: 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21           Hello, world!
    ```
19)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 --tcp
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-16T18:53:12.687381+00:00
    src MAC: 00:15:5d:ab:25:97
    dst MAC: 00:15:5d:ac:e6:51
    frame length: 2062 bytes
    src IP: 172.30.110.144
    dst IP: 172.18.91.60
    type: TCP
    src port: 42494
    dst port: 8080

    0x0000: 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21           Hello, world!
    ```
20)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 -u
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-16T18:39:28.794915+00:00
    src MAC: 00:15:5d:ab:25:97
    dst MAC: 00:15:5d:ac:e6:51
    frame length: 2062 bytes
    src IP: 172.30.110.144
    dst IP: 172.18.91.60
    type: UDP
    src port: 64728
    dst port: 8080

    0x0000: 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21           Hello, world!
    ```

(here I waited for random packet)

21)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 --udp
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-16T18:42:48.565763+00:00
    src MAC: 00:15:5d:ac:e6:51
    dst MAC: 01:00:5e:7f:ff:fa
    frame length: 2062 bytes
    src IP: 172.30.96.1
    dst IP: 239.255.255.250
    type: UDP
    src port: 61821
    dst port: 1900

    0x0000: 4d 2d 53 45 41 52 43 48 20 2a 20 48 54 54 50 2f  M-SEARCH * HTTP/
    0x0010: 31 2e 31 0d 0a 48 4f 53 54 3a 20 32 33 39 2e 32  1.1..HOST: 239.2
    0x0020: 35 35 2e 32 35 35 2e 32 35 30 3a 31 39 30 30 0d  55.255.250:1900.
    0x0030: 0a 4d 41 4e 3a 20 22 73 73 64 70 3a 64 69 73 63  .MAN: "ssdp:disc
    0x0040: 6f 76 65 72 22 0d 0a 4d 58 3a 20 31 0d 0a 53 54  over"..MX: 1..ST
    0x0050: 3a 20 75 72 6e 3a 64 69 61 6c 2d 6d 75 6c 74 69  : urn:dial-multi
    0x0060: 73 63 72 65 65 6e 2d 6f 72 67 3a 73 65 72 76 69  screen-org:servi
    0x0070: 63 65 3a 64 69 61 6c 3a 31 0d 0a 55 53 45 52 2d  ce:dial:1..USER-
    0x0080: 41 47 45 4e 54 3a 20 43 68 72 6f 6d 69 75 6d 2f  AGENT: Chromium/
    0x0090: 31 31 31 2e 30 2e 35 35 36 33 2e 36 35 20 57 69  111.0.5563.65 Wi
    0x00a0: 6e 64 6f 77 73 0d 0a 0d 0a                       ndows....
    ```
22)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 --arp
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-16T18:54:44.211927+00:00
    src MAC: 00:15:5d:ab:25:97
    dst MAC: ff:ff:ff:ff:ff:ff
    frame length: 2068 bytes
    src IP: 172.30.110.144
    dst IP: 172.30.96.1
    type: ARP

    0x0000: 00 00 00 00 54 28 3c 64                          ....T(<d
    ```
23)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 --icmp4
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-16T18:56:11.361616+00:00
    src MAC: 00:15:5d:ab:25:97
    dst MAC: 00:15:5d:ac:e6:51
    frame length: 2062 bytes
    src IP: 172.30.110.144
    dst IP: 172.18.91.60
    type: ICMP

    0x0000: 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21           Hello, world!
    ```
24)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 --icmp6
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-17T13:56:44.168120+00:00
    src MAC: 00:15:5d:ab:27:b5
    dst MAC: 33:33:ff:81:68:99
    frame length: 34539 bytes
    type: NDP
    Source IP: fe80::215:5dff:feab:27b5
    Destination IP: ff02::1:ff81:6899

    0x0000: 87 00 8d 87 00 00 00 00 fe 80 00 00 00 00 00 00  ................
    0x0010: 90 ce 18 27 68 81 68 99 01 01 00 15 5d ab 27 b5  ...'h.h.....].'.
    ```
25)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 --ndp
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-17T13:55:58.159538+00:00
    src MAC: 00:15:5d:ab:27:b5
    dst MAC: 33:33:ff:81:68:99
    frame length: 34539 bytes
    type: NDP
    Source IP: fe80::215:5dff:feab:27b5
    Destination IP: ff02::1:ff81:6899

    0x0000: 87 00 8d 87 00 00 00 00 fe 80 00 00 00 00 00 00  ................
    0x0010: 90 ce 18 27 68 81 68 99 01 01 00 15 5d ab 27 b5  ...'h.h.....].'.
    ```
26)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 --igmp
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-16T19:01:18.041005+00:00
    src MAC: 00:15:5d:ab:25:97
    dst MAC: 01:00:5e:00:00:01
    frame length: 2062 bytes
    src IP: 172.30.110.144
    dst IP: 224.0.0.1
    type: IGMP
    ```
27)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 --mld
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-17T13:55:09.675339+00:00
    src MAC: 00:15:5d:ab:27:b5
    dst MAC: 33:33:00:00:00:16
    frame length: 34539 bytes
    type: MLD
    Source IP: fe80::215:5dff:feab:27b5
    Destination IP: ff02::16

    0x0000: 82 00 d3 87 27 10 00 00 ff 02 00 00 00 00 00 00  ....'...........
    0x0010: 00 00 00 00 00 01 00 02                          ........
    ```

### Testing of complicated requests
The remaining tests, including ports, the number of packets and several types of protocols at the same time.

Sniffing a single packet of any type:

28)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-17T13:54:15.282652+00:00
    src MAC: 00:15:5d:ac:e6:51
    dst MAC: 01:00:5e:7f:ff:fa
    frame length: 2062 bytes
    src IP: 172.30.96.1
    dst IP: 239.255.255.250
    type: UDP
    src port: 59942
    dst port: 1900

    0x0000: 4d 2d 53 45 41 52 43 48 20 2a 20 48 54 54 50 2f  M-SEARCH * HTTP/
    0x0010: 31 2e 31 0d 0a 48 4f 53 54 3a 20 32 33 39 2e 32  1.1..HOST: 239.2
    0x0020: 35 35 2e 32 35 35 2e 32 35 30 3a 31 39 30 30 0d  55.255.250:1900.
    0x0030: 0a 4d 41 4e 3a 20 22 73 73 64 70 3a 64 69 73 63  .MAN: "ssdp:disc
    0x0040: 6f 76 65 72 22 0d 0a 4d 58 3a 20 31 0d 0a 53 54  over"..MX: 1..ST
    0x0050: 3a 20 75 72 6e 3a 64 69 61 6c 2d 6d 75 6c 74 69  : urn:dial-multi
    0x0060: 73 63 72 65 65 6e 2d 6f 72 67 3a 73 65 72 76 69  screen-org:servi
    0x0070: 63 65 3a 64 69 61 6c 3a 31 0d 0a 55 53 45 52 2d  ce:dial:1..USER-
    0x0080: 41 47 45 4e 54 3a 20 47 6f 6f 67 6c 65 20 43 68  AGENT: Google Ch
    0x0090: 72 6f 6d 65 2f 31 31 32 2e 30 2e 35 36 31 35 2e  rome/112.0.5615.
    0x00a0: 38 36 20 57 69 6e 64 6f 77 73 0d 0a 0d 0a        86 Windows....
    ```

The same, but on different interface:

29)
    ```sh
    input:
    sudo ./ipk-sniffer -i lo
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-17T13:53:16.188149+00:00
    src MAC: 00:00:00:00:00:00
    dst MAC: 00:00:00:00:00:00
    frame length: 2062 bytes
    src IP: 127.0.0.1
    dst IP: 127.0.0.1
    type: TCP
    src port: 59088
    dst port: 39283

    0x0000: 01 01 08 0a 1d 73 37 78 1d 73 2a 85 82 8d 98 ce  .....s7x.s*.....
    0x0010: 7b 2d 91                                         {-.
    ```

Sniffing many packets of any type, luckily received a package with unknown type:

30)
    ```sh
    input:
    sudo ./ipk-sniffer --interface eth0 -n 3
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-17T13:51:51.073009+00:00
    src MAC: 00:15:5d:ac:e6:51
    dst MAC: 01:00:5e:00:00:fb
    frame length: 2062 bytes
    src IP: 172.30.96.1
    dst IP: 224.0.0.251
    type: UDP
    src port: 5353
    dst port: 5353

    0x0000: 00 00 00 00 00 01 00 00 00 00 00 00 0e 5f 6d 69  ............._mi
    0x0010: 63 72 6f 73 6f 66 74 5f 6d 63 63 04 5f 74 63 70  crosoft_mcc._tcp
    0x0020: 05 6c 6f 63 61 6c 00 00 0c 80 01                 .local.....
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-17T13:51:51.075903+00:00
    src MAC: 00:15:5d:ac:e6:51
    dst MAC: 33:33:00:00:00:fb
    frame length: 34539 bytes
    Source IP: fe80::c0f:ede9:d35b:81e8
    Destination IP: ff02::fb

    0x0000: 14 e9 14 e9 00 33 de 7c 00 00 00 00 00 01 00 00  .....3.|........
    0x0010: 00 00 00 00 0e 5f 6d 69 63 72 6f 73 6f 66 74 5f  ....._microsoft_
    0x0020: 6d 63 63 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00  mcc._tcp.local..
    0x0030: 0c 80 01                                         ...
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-17T13:51:52.072308+00:00
    src MAC: 00:15:5d:ac:e6:51
    dst MAC: 01:00:5e:00:00:fb
    frame length: 2062 bytes
    src IP: 172.30.96.1
    dst IP: 224.0.0.251
    type: UDP
    src port: 5353
    dst port: 5353

    0x0000: 00 00 00 00 00 01 00 00 00 00 00 00 0e 5f 6d 69  ............._mi
    0x0010: 63 72 6f 73 6f 66 74 5f 6d 63 63 04 5f 74 63 70  crosoft_mcc._tcp
    0x0020: 05 6c 6f 63 61 6c 00 00 0c 00 01                 .local.....
    ```

Based on the previous test, we conclude that UDP packets often come to port 5353. Now let's try to manually set this port and UDP. It is worth mentioning that in my implementation, the port specified without the TCP or UDP flag is ignored.

31)
    ```sh
    input:
    sudo ./ipk-sniffer --interface eth0 -p 5353 -u
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-16T19:15:37.756426+00:00
    src MAC: 00:15:5d:ac:e6:51
    dst MAC: 01:00:5e:00:00:fb
    frame length: 2062 bytes
    src IP: 172.30.96.1
    dst IP: 224.0.0.251
    type: UDP
    src port: 5353
    dst port: 5353

    0x0000: 00 00 00 00 00 01 00 00 00 00 00 00 10 5f 73 70  ............._sp
    0x0010: 6f 74 69 66 79 2d 63 6f 6e 6e 65 63 74 04 5f 74  otify-connect._t
    0x0020: 63 70 05 6c 6f 63 61 6c 00 00 0c 00 01           cp.local.....
    ```

Test case for all protocol flags, given in the assignment (sent by command `ping 8.8.8.8`):

32)
    ```sh
    input:
    sudo ./ipk-sniffer -i eth0 -p 22 --tcp --udp --icmp4 --icmp6 --arp --ndp --igmp --mld
    output:
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    timestamp: 2023-04-17T13:49:44.039823+00:00
    src MAC: 00:15:5d:ab:27:b5
    dst MAC: 00:15:5d:ac:e6:51
    frame length: 2062 bytes
    src IP: 172.30.106.211
    dst IP: 8.8.8.8
    type: ICMP

    0x0000: 58 32 3d 64 00 00 00 00 71 9a 00 00 00 00 00 00  X2=d....q.......
    0x0010: 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f  ................
    0x0020: 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f   !"#$%&'()*+,-./
    0x0030: 30 31 32 33 34 35 36 37                          01234567
    ```

Based on the tests, it can be concluded that at least partially my sniffer performs its main function. I managed to catch the packages of Google Chrome, Spotify, Microsoft and others.

## Extra functionality
In addition to the project task, for each given protocol I also output its type in the format `type:`*`type`*. For TCP, UDP, ICMP and IGMP it wasn't a difficult task because their type is easily determined based on `ip->ip_p`. It is a field in the IP header of a network packet, which indicates the transport protocol used by the packet.

One of the fields in an Ethernet frame is the Ethernet Type field, which specifies the type of protocol being carried in the payload of the frame. The value `0x0806` or `ETHERTYPE_ARP` is used to indicate that the payload of the Ethernet frame is an ARP packet.

As for MLD, NDP and ICMPv6, the task is more complicated. When the value of the `ip6_hdr->ip6_nxt` field is set to `0x3A` or `IPPROTO_ICMPV6`, it indicates that the payload of the IPv6 packet contains an ICMPv6 message.

The `icmp6_hdr->icmp6_type` field is a 1-byte field in the ICMPv6 header that specifies the type of ICMPv6 message being carried in the payload of the IPv6 packet. This field is used to differentiate between different types of ICMPv6 messages, such as NDP messages or MLD messages. If the value of the `icmp6_hdr->icmp6_type` field is set between 133 and 137 (decimal), it indicates that the packet contains an NDP message. Similarly, if the value is set between 130 and 132 (decimal), it indicates that the packet contains an MLD message.
## Used sources

[1] What is a Packet Sniffer? https://www.kaspersky.com/resource-center/definitions/what-is-a-packet-sniffer   
[2] PROGRAMMING WITH PCAP https://www.tcpdump.org/pcap.html  
[3] Programming with Libpcap - Sniffing the Network From Our Own Application http://recursos.aldabaknocking.com/libpcapHakin9LuisMartinGarcia.pdf  
[4] Develop a Packet Sniffer with Libpcap https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/