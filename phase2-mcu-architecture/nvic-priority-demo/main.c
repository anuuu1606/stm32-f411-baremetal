#define RCC_AHB1ENR (*(volatile unsigned int *)0x40023830)
#define GPIOA_MODER (*(volatile unsigned int *)0x40020000)
#define GPIOA_ODR (*(volatile unsigned int *)0x40020014)
#define NVIC_ISER0 (*(volatile unsigned int *)0xE000E100)
#define NVIC_IPR7 (*(volatile unsigned int *)0xE000E41C)
#define NVIC_ISPR0 (*(volatile unsigned int *)0xE000E200)

void led5_on(void) {
    GPIOA_ODR |= (1 << 5);
}

void led5_off(void) {
    GPIOA_ODR &= ~(1 << 5);
}

void led6_on(void) {
    GPIOA_ODR |= (1 << 6);
}

void led6_off(void) {
    GPIOA_ODR &= ~(1 << 6);
}

void delay(volatile int count) {
    while (count--) {}
}

void TIM2_IRQHandler(void) {
    led5_on();
    delay(100000000);
    NVIC_ISPR0 |= (1 << 29);
    led5_off();
}

void TIM3_IRQHandler(void) {
    led6_on();
    delay(1000000);
    led6_off();
}

int main() {
    RCC_AHB1ENR |= (1 << 0); // enable GPIOA clock
    GPIOA_MODER |= (1 << 10); // set bit 10 for pin A5
    GPIOA_MODER |= (1 << 12); // set bit 12 for pin A6
    
    NVIC_ISER0 |= (1 << 28); // enable interrupt 28 (TIM2)
    NVIC_ISER0 |= (1 << 29); // enable interrupt 29 (TIM3)
    NVIC_IPR7 |= (10 << 4); // TIM2 priority
    NVIC_IPR7 |= (2 << 12); // TIM3 priority

    NVIC_ISPR0 |= (1 << 28);

    while(1) {
        // idle state
    }
}
