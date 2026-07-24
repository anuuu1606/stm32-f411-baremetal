// Q. Given reg = 0xF0, check whether bit 3 and bit 7 are set (print 1/0 for each) without modifying reg.

#include <stdio.h>
#include <stdint.h>

void check_set(uint32_t reg, int num_bit) {
    if ((reg & (1 << num_bit)) == 0) {
            printf("0\n");
        } else {
            printf("1\n");
        }
}

int main() {
    uint32_t reg = 0xF0;

    check_set(reg, 3);
    check_set(reg, 7);

    return 0;
}