// Q. Define a bitfield struct for a made-up register (e.g. 3 fields: mode:2, speed:2, reserved:28), set values through it, and print the field values
// then print sizeof the struct.

#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint32_t MODE: 2;
    uint32_t SPEED: 2;
    uint32_t RESERVED: 28;
} fake_reg;

int main(){
    fake_reg reg1;
    reg1.MODE = 3;
    reg1.SPEED = 1;
    reg1.RESERVED = 0x112233;

    printf("%02x\n", reg1.MODE);
    printf("%02x\n", reg1.SPEED);
    printf("%06x\n", reg1.RESERVED);

    printf("%zu\n", sizeof(fake_reg));

}
