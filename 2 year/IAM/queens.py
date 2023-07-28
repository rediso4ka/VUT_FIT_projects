# CHESS SOLVER
# xshevc01

import sys


def read_n():
    try:
        n = int(sys.argv[1])
    except:
        print("Bad argument input!")
        exit(1)
    if n <= 0:
        print("Bad argument input!")
        exit(1)
    return n


def place_queen(n, desk, x, y):
    global answer
    global count
    for i in range(n):
        if i != x:
            print_clause(-desk[x][y], -desk[i][y])
            count += 1
        if i != y:
            print_clause(-desk[x][y], -desk[x][i])
            count += 1
        if x + i < n and y + i < n and i != 0:
            print_clause(-desk[x][y], -desk[x+i][y+i])
            count += 1
        if x - i >= 0 and y + i < n and i != 0:
            print_clause(-desk[x][y], -desk[x-i][y+i])
            count += 1
        if x + i < n and y - i >= 0 and i != 0:
            print_clause(-desk[x][y], -desk[x+i][y-i])
            count += 1
        if x - i >= 0 and y - i >= 0 and i != 0:
            print_clause(-desk[x][y], -desk[x-i][y-i])
            count += 1
    answer += "\n"


def field_print(desk):
    for row in desk:
        for col in row:
            print(col, end="\t\t")
        print()


def print_start(n):
    print("p cnf " + str(n ** 2) + " " + str(count))
    print()


def print_clause(x, y):
    global answer
    answer += str(x) + " " + str(y) + " " + str(0) + "\n"


def all_cells(desk, n):
    for i in range(n):
        for j in range(n):
            place_queen(n, desk, i, j)


def define_n_queens(desk, n):
    global count
    global answer
    for i in range(n):
        for j in range(n):
            answer += str(desk[i][j]) + " "
        answer += "0\n"
    answer += "\n"
    count += n


count = 0
N = read_n()
field = [[j + 1 + N * i for j in range(N)] for i in range(N)]
answer = ""
define_n_queens(field, N)
all_cells(field, N)
print_start(N)
print(answer)
