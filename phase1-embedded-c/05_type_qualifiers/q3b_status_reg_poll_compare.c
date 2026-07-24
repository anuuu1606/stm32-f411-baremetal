// Q. Write a "fake status register" as volatile uint32_t *status = (volatile uint32_t *)someAddress. 
// Poll it in a loop (while ((*status & 0x1) == 0);). Compile with and without volatile, compare assembly for the loop body.
// PART B - without volatile

#include <stdio.h>
#include <stdint.h>

uint8_t fake_memory[4];

int main(int argc, char *argv[]) {
    if (argc > 100) {
        fake_memory[0] = 0x1;
    }

    uint32_t *status = (uint32_t *)fake_memory;
    while ((*status & 0x1) == 0);

    printf("done waiting\n");
    return 0;
}