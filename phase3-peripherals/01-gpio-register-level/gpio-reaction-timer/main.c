#define RCC_AHB1ENR (*(volatile unsigned int *)0x40023830)
#define GPIOA_MODER (*(volatile unsigned int *)0x40020000)
#define GPIOB_MODER (*(volatile unsigned int *)0x40020400)
#define GPIOA_PUPDR (*(volatile unsigned int *)0x4002000C)
#define GPIOA_IDR (*(volatile unsigned int *)0x40020010)
#define GPIOB_ODR (*(volatile unsigned int *)0x40020414)

int main() {
    // enable clocks for GPIOA and GPIOB in RCC_AHB1ENR
    RCC_AHB1ENR |= ((1 << 0) | (1 << 1));

    // set input mode for buttons at PA0, PA1, PA2
    GPIOA_MODER &= ~((1 << 0) | (1 << 1)); // PA0
    GPIOA_MODER &= ~((1 << 2) | (1 << 3)); // PA1
    GPIOA_MODER &= ~((1 << 4) | (1 << 5)); // PA2

    // set general purpose output mode for LEDS at PB0, PB1 and PB2
    GPIOB_MODER |= (1 << 0); // PB0
    GPIOB_MODER &= ~(1 << 1);

    GPIOB_MODER |= (1 << 2); // PB1
    GPIOB_MODER &= ~(1 << 3);

    GPIOB_MODER |= (1 << 4); // PB2
    GPIOB_MODER &= ~(1 << 5);

    // pull up configuration for buttons
    GPIOA_PUPDR |= (1 << 0); // PA0
    GPIOA_PUPDR &= ~(1 << 1); 

    GPIOA_PUPDR |= (1 << 2); // PA1
    GPIOA_PUPDR &= ~(1 << 3); 

    GPIOA_PUPDR |= (1 << 4); // PA2
    GPIOA_PUPDR &= ~(1 << 5);
    
}