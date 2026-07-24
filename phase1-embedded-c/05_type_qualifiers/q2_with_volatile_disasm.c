// Q. Repeat Q1 but declare the variable volatile. Compile with -O2 again, diff the disassembly against Q1's.

#include <stdio.h>
#include <stdint.h>

uint32_t volatile flag = 0;   // volatile

int main(int argc, char *argv[]) {
    if (argc > 100) {
        flag = 1;
    }

    while (flag == 0) {
        // do nothing
    }

    printf("done waiting\n");
    return 0;
}
