#define RCC_AHB1ENR   (*(volatile unsigned int *)0x40023830)
#define GPIOC_MODER   (*(volatile unsigned int *)0x40020800)
#define GPIOC_ODR     (*(volatile unsigned int *)0x40020814)

void delay(volatile int count) {
    while (count--) {}
}

int main(void) {
    RCC_AHB1ENR |= (1 << 2);      // enable GPIOC clock (bit 2 = GPIOC)
    GPIOC_MODER |= (1 << 26);     // set PC13 mode bits to 01 (output)
    GPIOC_MODER &= ~(1 << 27);    // ensure bit 27 is 0

    while (1) {
        GPIOC_ODR ^= (1 << 13);   // toggle PC13
        delay(1000000);
    }
}