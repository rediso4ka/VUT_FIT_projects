
# IZP 1: shortly

Checking password strength. Order of argumets: level of security, parameter, [statistics] (or advanced solution with using -l, -p).

## Usage:
``` terminal
    gcc -std=c99 -Wall -Wextra -Werror pwcheck.c -o pwcheck
    ./pwcheck 1 1 <hesla.txt
    ./pwcheck 4 2 <hesla.txt
    ./pwcheck 2 4 --stats <hesla.txt
```