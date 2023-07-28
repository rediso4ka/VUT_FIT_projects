#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<getopt.h>
#include<netinet/in.h>
#include<netdb.h>
#include<signal.h>

#define BUFSIZE 1024

int sock;

/**
 * Interrpution handler
 * @param sig
*/
void sigint_handler(int sig) {
    printf("\nCaught SIGINT signal, closing socket and exiting...\n");
    close(sock);
    exit(0);
}

/**
 * Help print
*/
void print_help() {
    printf("======HELP======\n");
    printf("-h -- IPv4 address of the server    (localhost)\n");
    printf("-m -- mode, either tcp or udp       (tcp)\n");
    printf("-p -- server port                   (2023)\n\n");
    printf("If something is not specified, program takes implicitly the values in brackets.\n");
    printf("======END======\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    ////////////////
    // PARSE ARGS //
    ////////////////

    signal(SIGINT, sigint_handler);
    
    char *host = NULL;
    int port = -1;
    char *mode = NULL;

    int option;
    while ((option = getopt(argc, argv, "h:p:m:x")) != -1) {
        switch (option) {
            case 'x':
                print_help();
                break;
            case 'h':
                if (host == NULL) {
                    host = optarg;
                } else {
                    fprintf(stderr, "./ipkcpc: MULTIPLE HOSTNAME OPTION SET\n");
                    return 1;
                }
                break;
            case 'p':
                if (port == -1) {
                    if (sscanf(optarg, "%d", &port) != 1) {
                        fprintf(stderr, "./ipkcpc: BAD PORT\n");
                        return 1;
                    }
                } else {
                    fprintf(stderr, "./ipkcpc: MULTIPLE PORT OPTION SET\n");
                    return 1;
                }
                break;
            case 'm':
                if (mode == NULL) {
                    mode = optarg;
                } else {
                    fprintf(stderr, "./ipkcpc: MULTIPLE MODE OPTION SET\n");
                    return 1;
                }
                break;
            case ':':
            case '?':
                fprintf(stderr, "./ipkcpc: Something went wrong, please try -x for help\n");
                return 1;
        }
    }

    if (host == NULL) {
        printf("./ipkcpc: MISSING MANDATORY OPTION -h, SET localhost\n");
        host = "localhost";
    }

    if (port == -1) {
        printf("./ipkcpc: MISSING MANDATORY OPTION -p, SET 2023\n");
        port = 2023;
    }

    if (mode == NULL) {
        printf("./ipkcpc: MISSING MANDATORY OPTION -m, SET tcp\n");
        mode = "tcp";
    }

    // printf("%s, %d, %s\n", host, port, mode);

    /////////////
    // NETWORK // (used templates from instructors' repo, check README)
    /////////////

    int bytestx, bytesrx;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFSIZE];

    /* GETTING SERVER ADDRESS USING DNS*/
    if ((server = gethostbyname(host)) == NULL) {
        fprintf(stderr,"ERROR: no such host as %s\n", host);
        exit(EXIT_FAILURE);
    }

    /* FINDING IP ADDRESS OF SERVER AND INITIALISATION OF STRUCTURE serv_addr */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    // printf("INFO: Server socket: %s : %d \n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));

    /////////
    // TCP //
    /////////
    if (!strcmp(mode, "tcp")) {
        /* TCP SOCKET CREATION*/
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
            fprintf(stderr, "./ipkcpc: TCP SOCKET CREATION ERRROR!\n");
            return 1;
        }

        /* CONNECTING TO SERVER */
        if (connect(sock, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
            fprintf(stderr, "./ipkcpc: TCP CONNECTION FAILED!\n");
            return 1;
        }

        /* COMMUNICATING UNTIL BYE RECEIVED*/
        while(strcmp(buffer, "BYE\n") != 0) {
            memset(buffer, 0, BUFSIZE);
            fgets(buffer, BUFSIZE, stdin);

            /* GOT A MESSAGE WITH NO \n (F.E. FROM INPUT FILE)*/
            if (strlen(buffer) > 0) {
                if (buffer[strlen(buffer) - 1] != '\n') {
                    buffer[strlen(buffer)] = '\n';
                }
            }

            /* SENDING MESSAGE TO SERVER */
            bytestx = send(sock, buffer, strlen(buffer), 0);
            if (bytestx < 0) {
                fprintf(stderr, "./ipkcpc: TCP SEND ERROR!\n");
                return 1;
            }

            memset(buffer, 0, BUFSIZE);

            /* RECEIVING THE ANSWER */
            bytesrx = recv(sock, buffer, BUFSIZE, 0);
            if (bytesrx < 0) {
                fprintf(stderr, "./ipkcpc: TCP RECEIVE ERROR!\n");
                return 1;
            }
            printf("%s", buffer);
        }
        close(sock);
    /////////
    // UDP //
    /////////
    } else if (!strcmp(mode, "udp")) {
        /* UDP SOCKET CREATION*/
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) {
            fprintf(stderr, "./ipkcpc: UDP SOCKET CREATION ERROR!\n");
            return 1;
        }
        
        socklen_t serverlen;

        /* COMMUNICATING UNTIL C-c INTERRUPTION OR ERROR*/
        while (1) {
            memset(buffer, 0, BUFSIZE);
            fgets(buffer, BUFSIZE, stdin);

            /* CREAETING A MESSAGE WITH OPCODE, LENGTH AND PAYLOAD*/
            uint8_t payload_length = strlen(buffer);
            char message[BUFSIZE + 2] = "";
            if (buffer[0] != '\0') {

                memcpy(message, "\0", 1);

                memcpy(message + 1, &payload_length, 1);

                memcpy(message + 2, buffer, payload_length);
            } else {
                memcpy(message, buffer, strlen(buffer+2) + 2);
                payload_length = strlen(buffer+2);
            }

            serverlen = sizeof(serv_addr);

            /* SENDING MESSAGE TO SERVER */
            bytestx = sendto(sock, message, 2 * 1 + payload_length, 0, (struct sockaddr *) &serv_addr, serverlen);
            if (bytestx < 0) {
                fprintf(stderr, "./ipkcpc: UDP SEND ERROR!\n");
                return 1;
            }
            memset(buffer, 0, BUFSIZE);

            /* RECEIVING THE ANSWER */
            bytesrx = recvfrom(sock, buffer, BUFSIZE, 0, (struct sockaddr *) &serv_addr, &serverlen);
            if (bytesrx < 0) {
                fprintf(stderr, "./ipkcpc: UDP RECEIVE ERROR!\n");
                return 1;
            }

            /* IF GOT AN ERROR FROM SERVER*/
            if (buffer[1] == 1) {
                fprintf(stderr, "ERR: EXITING / SERVER SENT ERROR STATUS!\n");
                return 1;
            }

            /* THE ANSWER ITSELF GOES AFTER 3 BYTES OF OPCODE, STATUS CODE AND PAYLOAD LENGTH*/
            printf("OK:%s\n", buffer+3);
        }
    }


    return 0;
}