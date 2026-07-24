// Q. Write a function that takes a uint32_t register value and a field position + width, and extracts just that field's value using mask/shift (no bitfields at all).

#include <stdio.h>
#include <stdint.h>

// no format specifier in standard c to print binary digits easily, so we can make a function instead
void print_binary(uint32_t value, int num_bits) {
    for (int i = num_bits - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

void extract_field(uint32_t reg, int start_bit, int width) {
    uint32_t mask;
    reg = (reg >> start_bit);
    mask = (1 << width) - 1;
    reg &= mask;

    print_binary(reg, 8);
}

int main() {
    uint32_t reg = 0b10110100;   
    extract_field(reg, 2, 3);

    return 0;
}