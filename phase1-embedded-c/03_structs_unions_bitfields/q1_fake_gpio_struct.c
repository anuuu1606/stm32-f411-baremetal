// Q. Define a struct FakeGpio with members MODER, OTYPER, IDR, ODR (all volatile uint32_t). Declare an instance, write to each field, and print them.

#include <stdio.h>
#include <stdint.h>

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
} FakeGpio;

int main() {
    FakeGpio G1;
    G1.MODER = 0x00000001;
    G1.OTYPER = 0x00000000;
    G1.IDR = 0x00000001;
    G1.ODR = 0x00000001;

    printf("MODER = %u\n", G1.MODER);
    printf("OTYPER = %u\n", G1.OTYPER);
    printf("IDR = %u\n", G1.IDR);
    printf("ODR = %u\n", G1.ODR);

    return 0;
}