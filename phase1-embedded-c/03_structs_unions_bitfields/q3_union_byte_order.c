// Q. Write a union that lets you access the same 4 bytes either as one uint32_t or as four individual uint8_t bytes.
//  Set the 32-bit value and print the four bytes to see the byte order (endianness) on your machine.

#include <stdio.h>
#include <stdint.h>

typedef union {
    uint32_t full;
    uint8_t bytes4[4];
} byte_union;

int main() {
    byte_union inst;
    inst.full = 0x11223344;

    for(int i = 0; i < 4; i++){
        printf("%02x ", inst.bytes4[i]);
    }

    // output is 44 33 22 11
    // meaning there is little endianness with lsb being stored first

    return 0;

}