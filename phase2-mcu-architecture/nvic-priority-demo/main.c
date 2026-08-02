#define RCC_AHB1ENR (*(volatile unsigned int *)0x40023830)
#define GPIOA_MODER (*(volatile unsigned int *)0x40020000)
#define GPIOA_ODR (*(volatile unsigned int *)0x40020014)

// turn led on pin A5 ON
void led5_on(void) {
    GPIOA_ODR |= (1 << 5);
}

// turn led on pin A5 OFF
void led5_off(void) {
    GPIOA_ODR &= ~(1 << 5);
}

// turn led on pin A6 ON
void led6_on(void) {
    GPIOA_ODR |= (1 << 6);
}

// turn led on pin A6 OFF
void led6_off(void) {
    GPIOA_ODR &= ~(1 << 6);
}

void delay(volatile int count) {
    while (count--) {}
}

int main() {
    RCC_AHB1ENR |= (1 << 0); // enable GPIOA clock
    GPIOA_MODER |= (1 << 10); // set bit 10 for pin A5
    GPIOA_MODER |= (1 << 12); // set bit 12 for pin A6

    while(1) {
        led5_on();
        delay(1000000);
        led5_off();
        led6_on();
        delay(1000000);
        led6_off();
        delay(1000000);
    }
}