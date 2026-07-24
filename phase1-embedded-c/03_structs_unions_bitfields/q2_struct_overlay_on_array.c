// Q. Overlay that same struct on a raw byte array using a pointer cast (like a real peripheral base address), 
// then modify a field through the pointer and confirm it shows up correctly in the byte array.

#include <stdio.h>
#include <stdint.h>

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
} FakeGpio;

// using memory_dump function from pointers_and_cast q3 to print all 4 bytes of specified field
void memory_dump(void *addr, size_t size) {
   uint8_t *ptr = (uint8_t *)addr;
   for (size_t i=0; i < size; i++) {
       printf("%u",*(ptr+i));
   }
}

uint8_t fake_memory[16];

int main() {
    FakeGpio *fake_reg = (FakeGpio *)fake_memory;
    fake_reg->MODER = 0x00000001;

    // will print 1000 as most desktops are little-endian and store least significant bit first
    memory_dump(fake_memory, 4);
    printf("\n");

    return 0;
}
