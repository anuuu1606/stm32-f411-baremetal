// Q. Declare a global uint32_t without volatile. In main, spin in while(flag == 0){} 
// and have flag set to 1 only if a condition that never triggers at compile time.
// Compile with -O2 and check with objdump -d whether the loop gets optimized.

#include <stdio.h>
#include <stdint.h>

uint32_t flag = 0;   // not volatile

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