// Q. Given two "before" and "after" register values, write a function that returns a value showing exactly which bits changed, 
// then print which bit positions differ.

#include <stdio.h>
#include <stdint.h>

void print_binary(uint32_t value, int num_bits) {
    for (int i = num_bits - 1; i >= 0; i--) {
        printf("%d ", (value >> i) & 1);
    }
    printf("\n");
}

void check_change(uint32_t before, uint32_t after) {
    for(int i=0; i < 8; i++) {
        uint32_t mask = (1 << i);
        uint32_t before_reg = before & mask;
        uint32_t after_reg = after & mask;
        if (((before_reg ^ after_reg)) == mask) {
            printf("Bit %d has been changed.\n", i);
        }
    }
}

int main() {
    uint32_t reg1 = 0b10010011;
    uint32_t reg2 = 0b00010110;

    printf("Position: 7 6 5 4 3 2 1 0\n");
    printf("Before:   ");
    print_binary(reg1, 8);
    printf("After:    ");
    print_binary(reg2, 8);

    check_change(reg1, reg2);

    return 0;
}