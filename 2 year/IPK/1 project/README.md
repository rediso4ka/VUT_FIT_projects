# README documentation

## Content structuring
- Theory
- Interesting source code sections
- Testing
- Extra functionality
- Used sources

## Theory
The task was to write a program to establish a TCP or UDP connection with an AaaS provider. As for TCP, the stages of connection establishing are:
- Getting server address using DNS
- Finding IP address of server
- Socket creation
- Connecting to server
- Sending message to server
- Receiving the answer
- Communicating until "BYE" or C-c signal received
- Closing the socket

For UDP:
- Getting server address using DNS
- Finding IP address of server
- Socket creation
- Sending message to server
- Receiving the answer
- Communicating until C-c signal received

It means that the main difference of TCP from UDP is that UDP does not connect to the server, but right away sends data.
## Interesting source code sections
The parts of the code related to establishing a connection to the server, in my opinion, are not particularly interesting, because I use code templates, given by IPK instructors. But the most interesting and difficult part for me was parsing arguments, complicated by the fact that `-h` and `--help` start with the same letter.
Since I'm using the function `getopt`, appearance of the argument `--help` complicated the task. After trying many other functions that work with enums and complicated data structures, I decided to simplify the task and make a help print using `-x`. It doesn't complicate the user's work with program at all, because if used incorrectly, a message is displayed asking the user to write `-x`, but in the future, for perfectionism, it's worth changing the way of reading arguments.
## Testing
#### TCP
##### (all tests below gave the same result, not depending on the way of program execution):

```sh
./ipkcpc
./ipkcpc -m tcp
./ipkcpc -m tcp -p 2023
./ipkcpc -m tcp -p 2023 -h localhost
./ipkcpc -h 127.0.0.1 -p 2023 -m tcp
```

input, output:

1)
    ```sh
    HELLO                                   HELLO
    SOLVE (+ 1 2)                           RESULT 3
    BYE                                     BYE
    ```
2)
    ```sh
    HELO                                    BYE
    ```
3)
    ```sh
    HELLO                                   HELLO
    BYE                                     BYE
    ```
4)
    ```sh
    HELLO                                   HELLO
    '\n'                                    BYE
    ```
5)
    ```sh
    HELLO                                   HELLO
    SOLVE (+ 1 (* 10 10))                   RESULT 101
    SOLVE (/ (* (+ 4 4) (- 8 2)) 48)        RESULT 1
    bye                                     BYE
    ```
6)
    ```sh
    HELLO                                   HELLO
    ^C                                      Caught SIGINT signal, closing socket and exiting...
    ```
7)
    ```sh
    '\n'                                    BYE
    ```
8)
    ```sh
    ~server is running on TCP~              ./ipkcpc: TCP CONNECTION FAILED!
    ```
9)
    ```sh
    HELLO                                   HELLO
    SOLVE (- 40 50)                         RESULT -10
    SOLVE (+ -100 -100)                     RESULT -200
    nicejob                                 BYE
    ```
10)
    ```sh
    HELLO                                   HELLO
    SOLVE (+ a b)                           BYE
    ```
#### UDP
##### (all tests below gave the same result, not depending on the way of program execution):

```sh
./ipkcpc -m udp
./ipkcpc -m udp -p 2023
./ipkcpc -m udp -p 2023 -h localhost
./ipkcpc -h 127.0.0.1 -m udp -p 2023
```

input, output:

1)
    ```sh
    (+ 2 1)                                 OK:3
    '\n'                                    ERR: EXITING / SERVER SENT ERROR STATUS!
    ```
2)
    ```sh
    '\n'                                    ERR: EXITING / SERVER SENT ERROR STATUS!
    ```
3)
    ```sh
    (+ 1 (* 10 10))                         OK:101
    (/ (* (+ 4 4) (- 8 2)) 48)              OK:1
    BYE                                     ERR: EXITING / SERVER SENT ERROR STATUS!
    ```
4)
    ```sh
    ^C                                      Caught SIGINT signal, closing socket and exiting...
    ```
5)
    ```sh
    (+ 2 2)                                 OK:4
    ^C                                      Caught SIGINT signal, closing socket and exiting...
    ```
6)
    ```sh
    (- 40 50)                               OK:-10
    (+ -100 -100)                           OK:-200
    nicejob                                 ERR: EXITING / SERVER SENT ERROR STATUS!
    ```
7)
    ```sh
    (+ a b)                                 ERR: EXITING / SERVER SENT ERROR STATUS!
    ```
## Extra functionality
In addition to the task of the project, I made the input of arguments by the user optional. It means, that in case of absence of a port, mode or host, the program itself sets them implicitly to 2023, "tcp" and "localhost" respectively. Especially it helped during the testing of the program, so that I didn't have to constantly enter arguments.
Also during testing I noticed, that in case of TCP connection, if data from a file sent to a standard input does't end with new line, the server does not respond to the last request. Therefore, in the case when data is received without new line character, I manually add it for a correct processing of requests.
## Used sources
- ##### [getopt]
- ##### [tcp]
- ##### [udp]

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [getopt]: <https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/>
   [tcp]: <https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoTcp/client.c>
   [udp]: <https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoUdp/client.c>
