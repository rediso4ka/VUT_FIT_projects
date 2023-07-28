//
// xshevc01
//
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pcap/pcap.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>
#include <netinet/igmp.h>
#include <netinet/icmp6.h>
#include <net/ethernet.h>
#include <netinet/ip6.h>
#include <signal.h>
#include <time.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <ctype.h>


pcap_t* handle;
int linkhdrlen;
int packets;

/**
 * Auxiliary structure for passing flags
*/
typedef struct {
    bool tcp_f;
    bool udp_f;
    bool icmp4_f;
    bool icmp6_f;
    bool arp_f;
    bool ndp_f;
    bool igmp_f;
    bool mld_f;
    int port;
} filter_params;

/**
 * Creates a packet capture endpoint to receive packets
 * described by a packet capture filter
 * @param interface
 * @param filter
*/
pcap_t* create_pcap_handle(char* interface, char* filter) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    pcap_if_t* interfaces = NULL;
    struct bpf_program bpf;
    bpf_u_int32 netmask;
    bpf_u_int32 srcip;


    // If no network interface is specified, print all of them
    if (!*interface) {
        if (pcap_findalldevs(&interfaces, errbuf)) {
            fprintf(stderr, "pcap_findalldevs(): %s\n", errbuf);
            return NULL;
        }
        while (interfaces != NULL) {
            printf("%s\n", interfaces->name);
            interfaces = interfaces->next;
        }
        exit(0);
    }

    // Get network device source IP address and netmask
    if (pcap_lookupnet(interface, &srcip, &netmask, errbuf) == PCAP_ERROR) {
        fprintf(stderr, "pcap_lookupnet(): %s\n", errbuf);
        return NULL;
    }

    // Open the device for live capture
    handle = pcap_open_live(interface, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "pcap_open_live(): %s\n", errbuf);
        return NULL;
    }

    // Convert the packet filter expression into a packet filter binary
    if (pcap_compile(handle, &bpf, filter, 1, netmask) == PCAP_ERROR) {
        fprintf(stderr, "pcap_compile(): %s\n", pcap_geterr(handle));
        return NULL;
    }

    // Bind the packet filter to the libpcap handle
    if (pcap_setfilter(handle, &bpf) == PCAP_ERROR) {
        fprintf(stderr, "pcap_setfilter(): %s\n", pcap_geterr(handle));
        return NULL;
    }

    return handle;
}

/**
 * Gets the link header type and size that will
 * be used during the packet capture and parsing
 * @param handle
*/
void get_link_header_len(pcap_t* handle) {
    int linktype;

    // Determine the datalink layer type
    if ((linktype = pcap_datalink(handle)) == PCAP_ERROR) {
        fprintf(stderr, "pcap_datalink(): %s\n", pcap_geterr(handle));
        return;
    }

    // Set the datalink layer header size
    switch (linktype) {
        case DLT_NULL:
            linkhdrlen = 4;
            break;
        case DLT_EN10MB:
            linkhdrlen = 14;
            break;
        case DLT_SLIP:
        case DLT_PPP:
            linkhdrlen = 24;
            break;
        
        default:
            printf("Unsupported datalink (%d)\n", linktype);
            linkhdrlen = 0;
    }
}

/**
 * Auxiliary function for printing MAC addresses
 * @param name
 * @param mac
*/
void print_mac(const char* name, const uint8_t* mac) {
    printf("%s MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           name, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/**
 * Auxiliary function for printing packet data
 * @param packet_data
 * @param packet_len
*/
void print_packet_data(char *packet_data, int packet_len) {
    const int bytes_per_row = 16;
    const unsigned char * byte = (unsigned char *)packet_data;
    int byte_offset = 0;

    while (byte_offset < packet_len) {
        // Print byte offset in hexadecimal format
        printf("0x%04x: ", byte_offset);

        // Print each byte in hexadecimal and ASCII format
        for (int i = 0; i < bytes_per_row; i++) {
            if (byte_offset + i < packet_len) {
                printf("%02x ", byte[i]);
            } else {
                printf("   ");
            }
        }
        printf(" ");

        for (int i = 0; i < bytes_per_row; i++) {
            if (byte_offset + i < packet_len) {
                if (isprint(byte[i])) {
                    printf("%c", byte[i]);
                } else {
                    printf(".");
                }
            } else {
                printf(" ");
            }
        }
        printf("\n");

        byte += bytes_per_row;
        byte_offset += bytes_per_row;
    }
}

/**
 * Auxiliary function for printing timestamp
 * @param packethdr
*/
void print_timestamp(const struct pcap_pkthdr *packethdr) {
    char timestamp_string[150];
    strftime(timestamp_string, sizeof(timestamp_string),
         "%Y-%m-%dT%H:%M:%S.%d:%z", localtime(&packethdr->ts.tv_sec));
    snprintf(timestamp_string + 19, 22, ".%06ld", (long)packethdr->ts.tv_usec);
    struct tm *time = localtime(&packethdr->ts.tv_sec);
    char timezone_buffer[11];

    strftime(timezone_buffer, 10, "%z", time);

    printf("timestamp: %s+%c%c:%c%c\n", timestamp_string, timezone_buffer[1], timezone_buffer[2], timezone_buffer[3], timezone_buffer[4]);
}

/**
 * Call back function that parses and displays
 * the contents of each captured packet
 * @param user
 * @param packethdr
 * @param packetptr
*/
void packet_handler(u_char *user, const struct pcap_pkthdr *packethdr, const u_char *packetptr) {
    struct ip* iphdr;
    struct icmp* icmphdr;
    struct tcphdr* tcphdr;
    struct udphdr* udphdr;
    struct igmp* igmp_packet;
    char srcip[256];
    char dstip[256];
    char *packet_data;
    int packet_data_len;

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");

    struct ether_header *ethhdr = (struct ether_header *) packetptr;

    print_timestamp(packethdr);

    print_mac("src", ethhdr->ether_shost);
    print_mac("dst", ethhdr->ether_dhost);

    u_int16_t ether_type;
    u_int32_t frame_length;

    ether_type = ntohs(ethhdr->ether_type);

    if (ether_type <= ETHERMTU) {
        // Length field indicates the length of the data (not including Ethernet header)
        frame_length = ntohs(ether_type);
    } else {
        // Length field is actually the Ethernet type field, which is 2 bytes long
        frame_length = ntohs(ethhdr->ether_type) + sizeof(struct ether_header);
    }
    printf("frame length: %d bytes\n", frame_length);
    // Skip the datalink layer header
    packetptr += linkhdrlen;
    if (ntohs(ethhdr->ether_type) == ETHERTYPE_IP){
        iphdr = (struct ip*)packetptr;
        strcpy(srcip, inet_ntoa(iphdr->ip_src));
        strcpy(dstip, inet_ntoa(iphdr->ip_dst));

        // print the source and destination IP addresses
        printf("src IP: %s\n", srcip);
        printf("dst IP: %s\n", dstip);
        
        // Advance to the transport layer header
        packetptr += 4*iphdr->ip_hl;
        switch (iphdr->ip_p) {
            case IPPROTO_TCP:
                tcphdr = (struct tcphdr*)packetptr;
                printf("type: TCP\n");
                printf("src port: %d\n", ntohs(tcphdr->th_sport));
                printf("dst port: %d\n\n", ntohs(tcphdr->th_dport));
                packet_data = (char*)(packetptr + sizeof(struct tcphdr));
                packet_data_len = ntohs(iphdr->ip_len) - (iphdr->ip_hl * 4) - (tcphdr->th_off * 4);
                print_packet_data(packet_data, packet_data_len);
                packets += 1;
                break;

            case IPPROTO_UDP:
                udphdr = (struct udphdr*)packetptr;
                printf("type: UDP\n");
                printf("src port: %d\n", ntohs(udphdr->uh_sport));
                printf("dst port: %d\n\n", ntohs(udphdr->uh_dport));
                packet_data = (char*)(packetptr + sizeof(struct udphdr));
                packet_data_len = ntohs(udphdr->uh_ulen) - sizeof(struct udphdr);
                print_packet_data(packet_data, packet_data_len);
                packets += 1;
                break;
    
            case IPPROTO_ICMP:
                icmphdr = (struct icmp*)packetptr;
                printf("type: ICMP\n\n");
                packet_data = (char*)icmphdr->icmp_data;
                packet_data_len = ntohs(iphdr->ip_len) - sizeof(struct icmp);
                print_packet_data(packet_data, packet_data_len);
                packets += 1;
                break;
            case IPPROTO_IGMP:
                igmp_packet = (struct igmp *)packetptr;
                printf("type: IGMP\n");
                packet_data = (char *)((u_char *)igmp_packet + sizeof(struct igmp));
                packet_data_len = ntohs(iphdr->ip_len) - (iphdr->ip_hl * 4) - sizeof(struct igmp);
                print_packet_data(packet_data, packet_data_len);
                break;
        }
    } else if (ntohs(ethhdr->ether_type) == ETHERTYPE_ARP) {
        struct ether_arp *arp_header;
        char src_ip[INET_ADDRSTRLEN];
        char dest_ip[INET_ADDRSTRLEN];

        arp_header = (struct ether_arp *) packetptr;

        // convert the source and destination IP addresses to strings
        inet_ntop(AF_INET, &arp_header->arp_spa, src_ip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &arp_header->arp_tpa, dest_ip, INET_ADDRSTRLEN);

        // print the source and destination IP addresses
        printf("src IP: %s\n", src_ip);
        printf("dst IP: %s\n", dest_ip);
        printf("type: ARP\n\n");
        print_packet_data((char *) arp_header + sizeof(struct ether_arp), sizeof(arp_header));

    } else if (ntohs(ethhdr->ether_type) == ETHERTYPE_IPV6) {
        struct ip6_hdr *ipv6_hdr = (struct ip6_hdr*) packetptr;

            if (ipv6_hdr->ip6_nxt == IPPROTO_ICMPV6)
            {
                struct icmp6_hdr *icmp6_hdr = (struct icmp6_hdr*) (packetptr + sizeof(struct ip6_hdr));
                if (icmp6_hdr->icmp6_type >= 130 && icmp6_hdr->icmp6_type <= 132) {
                    printf("type: MLD\n");
                } else if (icmp6_hdr->icmp6_type >= 133 && icmp6_hdr->icmp6_type <= 137) {
                    printf("type: NDP\n");
                } else {
                    printf("type: ICMPv6\n");
                }
            }
            char srcip[INET6_ADDRSTRLEN];
            char dstip[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &(ipv6_hdr->ip6_src), srcip, INET6_ADDRSTRLEN);
            inet_ntop(AF_INET6, &(ipv6_hdr->ip6_dst), dstip, INET6_ADDRSTRLEN);
            // print the source and destination IP addresses
            printf("Source IP: %s\n", srcip);
            printf("Destination IP: %s\n\n", dstip);
            char *payload = ((char *)ipv6_hdr) + sizeof(struct ip6_hdr);
            int payload_len = ntohs(ipv6_hdr->ip6_plen);
            print_packet_data(payload, payload_len);
    }
}

/**
 * Handler function for SIGINT, SIGTERM and SIGQUIT
 * signals. Also called when the program terminates
 * normally after a specified number of packets is captured
 * @param signo
*/
void stop_capture(int signo) {
    pcap_close(handle);
    exit(0);
}

/**
 * Creates a filter depending on the flags
 * received after parsing arguments
 * @param params
 * @param filter
*/
void filter_builder(filter_params *params, char *filter) {
    char tmp[100];

    if (params->tcp_f) {
        if (params->port == -1) {
            sprintf(tmp, "tcp");
        } else {
            sprintf(tmp, "(tcp port %d)", params->port);
        }
        strcat(filter, tmp);
    }

    if (params->udp_f) {
        if (strlen(filter) > 0) strcat(filter, " or ");
        if (params->port == -1) {
            sprintf(tmp, "udp");
        } else {
            sprintf(tmp, "(udp port %d)", params->port);
        }
        strcat(filter, tmp);
    }

    if (params->icmp4_f) {
        if (strlen(filter) > 0) strcat(filter, " or ");
        strcat(filter, "icmp");
    }

    if (params->icmp6_f) {
        if (strlen(filter) > 0) strcat(filter, " or ");
        strcat(filter, "icmp6");
    }

    if (params->arp_f) {
        if (strlen(filter) > 0) strcat(filter, " or ");
        strcat(filter, "arp");
    }

    if (params->ndp_f) {
        if (strlen(filter) > 0) strcat(filter, " or ");
        strcat(filter, "icmp6[0] == 135 or icmp6[0] == 136");
    }

    if (params->igmp_f) {
        if (strlen(filter) > 0) strcat(filter, " or ");
        strcat(filter, "igmp");
    }

    if (params->mld_f) {
        if (strlen(filter) > 0) strcat(filter, " or ");
        strcat(filter, "icmp6[0] == 130 or icmp6[0] == 131");
    }
}

/**
 * MAIN BODY
*/
int main(int argc, char *argv[]) {
    // parse args
    int opt;
    int port = -1, num_packets = 1;
    char interface[256];
    bool tcp_f = false, udp_f = false, icmp4_f = false, icmp6_f = false;
    bool arp_f = false, ndp_f = false, igmp_f = false, mld_f = false;

    char filter[1024];
    *filter = 0;
    *interface = 0;

    static struct option long_options[] = {
        {"interface", optional_argument, NULL, 'i'},
        {"tcp", no_argument, NULL, 't'},
        {"udp", no_argument, NULL, 'u'},
        {"arp", no_argument, NULL, 'a'},
        {"icmp4", no_argument, NULL, '4'},
        {"icmp6", no_argument, NULL, '6'},
        {"ndp", no_argument, NULL, 'd'},
        {"igmp", no_argument, NULL, 'g'},
        {"mld", no_argument, NULL, 'm'},
        {"port", required_argument, NULL, 'p'},
        {"num", required_argument, NULL, 'n'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "i::tup:n:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'i':
                if (optarg != NULL) {
                    strcpy(interface, optarg);
                }
                break;
            case 't':
                tcp_f = true;
                break;
            case 'u':
                udp_f = true;
                break;
            case 'a':
                arp_f = true;
                break;
            case '4':
                icmp4_f = true;
                break;
            case '6':
                icmp6_f = true;
                break;
            case 'd':
                ndp_f = true;
                break;
            case 'g':
                igmp_f = true;
                break;
            case 'm':
                mld_f = true;
                break;
            case 'p':
                if (sscanf(optarg, "%d", &port) != 1) {
                        fprintf(stderr, "./ipk-sniffer: BAD PORT\n");
                        return 1;
                }
                if (port <= 0) {
                        fprintf(stderr, "./ipk-sniffer: BAD PORT\n");
                        return 1;
                }
                break;
            case 'n':
                if (sscanf(optarg, "%d", &num_packets) != 1) {
                        fprintf(stderr, "./ipk-sniffer: BAD NUM\n");
                        return 1;
                }
                if (num_packets <= 0) {
                        fprintf(stderr, "./ipk-sniffer: BAD NUM\n");
                        return 1;
                }
                break;
            case 'h':
                printf("Usage: ./ipk-sniffer [-i interface | --interface interface] {-p port [--tcp|-t] [--udp|-u]} [--arp] [--ndp] [--icmp4] [--icmp6] [--igmp] [--mld] {-n num}");
                printf("\n");
                printf("Options:\n");
                printf("  -i, --interface   interface to capture packets from (optional)\n");
                printf("  -t, --tcp         capture TCP packets\n");
                printf("  -u, --udp         capture UDP packets\n");
                printf("      --arp         capture ARP packets\n");
                printf("      --icmp4       capture ICMPv4 packets\n");
                printf("      --icmp6       capture ICMPv6 packets\n");
                printf("      --ndp         capture NDP packets\n");
                printf("      --igmp        capture IGMP packets\n");
                printf("      --mld         capture MLD packets\n");
                printf("  -p, --port        port number to capture packets on\n");
                printf("  -n, --num         number of packets to capture\n");
                printf("  -h, --help        display this help message and exit\n");
                exit(EXIT_SUCCESS);
            case '?':
                printf("Unknown option: %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }

    // Get the interface, if any is given
    for (int i = optind; i < argc; i++) {
        strcpy(interface, argv[i]);
    }

    filter_params params = {
        .tcp_f = tcp_f,
        .udp_f = udp_f,
        .icmp4_f = icmp4_f,
        .icmp6_f = icmp6_f,
        .arp_f = arp_f,
        .ndp_f = ndp_f,
        .igmp_f = igmp_f,
        .mld_f = mld_f,
        .port = port
    };

    // Create a filter expression
    filter_builder(&params, filter);

    signal(SIGINT, stop_capture);
    signal(SIGTERM, stop_capture);
    signal(SIGQUIT, stop_capture);

    // Create packet capture handle
    handle = create_pcap_handle(interface, filter);
    if (handle == NULL) {
        return -1;
    }

    // Get the type of link layer
    get_link_header_len(handle);
    if (linkhdrlen == 0) {
        return -1;
    }

    // Start the packet capture with a set count or continually if the count is 0
    if (pcap_loop(handle, num_packets, packet_handler, (u_char*)NULL) == PCAP_ERROR) {
        fprintf(stderr, "pcap_loop failed: %s\n", pcap_geterr(handle));
        return -1;
    }

    stop_capture(0);
}

