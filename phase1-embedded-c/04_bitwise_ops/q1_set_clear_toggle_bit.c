// Q. Given a uint32_t reg = 0, set bit 5, then clear bit 5, then toggle bit 5 twice
// print the value after each step in binary or hex.

#include <stdio.h>
#include <stdint.h>

void print_binary(uint32_t value, int num_bits) {
    for (int i = num_bits - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

int main() {
    uint32_t reg = 0;

    // set bit 5
    reg |= (1 << 5);
    print_binary(reg, 8);

    // clear bit 5
    reg &= ~(1 << 5);
    print_binary(reg, 8);

    // toggle bit 5
    reg ^= (1 << 5);
    print_binary(reg, 8);

    // toggle bit 5 again 
    reg ^= (1 << 5);
    print_binary(reg, 8);

    return 0;
}