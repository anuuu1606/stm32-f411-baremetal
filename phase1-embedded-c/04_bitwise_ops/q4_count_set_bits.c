// Q. Write a function that counts how many bits are set in a uint32_t 

#include <stdio.h>
#include <stdint.h>

int count_bits(uint32_t reg) {
    int count = 0;
    for (int i=0; i < 32; i++) {
        if ((reg & (1 << i)) != 0){
            count++;
        }
    }
    return count;
}

int main() {
    uint32_t reg = 0x11223344;
    printf("Number of set bits: %d\n", count_bits(reg));

    return 0;
}