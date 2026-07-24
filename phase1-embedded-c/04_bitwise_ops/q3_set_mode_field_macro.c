// Q. Simulate a 2-bit "mode" field at bit position 4 (bits 4-5) of a register. 
// Write a macro or function set_mode(reg, value) that clears just those 2 bits and sets them to value, leaving all other bits untouched. 
// Test starting from a non-zero reg.

#include <stdio.h>
#include <stdint.h>

void print_binary(uint32_t value, int num_bits) {
    for (int i = num_bits - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

uint32_t clear_set_mode(uint32_t reg, uint32_t value) {
    uint32_t mask = (1 << 4) | (1 << 5);
    reg &= ~(mask);
    uint32_t val_mask = (value << 4);
    reg |= val_mask;
    return reg;
}

int main(){
    uint32_t reg = 0b10010100;
    int value = 3;
    print_binary(clear_set_mode(reg, value), 8);

    return 0;
}
