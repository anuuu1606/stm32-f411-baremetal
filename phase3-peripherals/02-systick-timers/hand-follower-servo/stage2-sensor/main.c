#include <stdint.h>

#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)
#define RCC_APB1ENR (*(volatile uint32_t *)0x40023840)
#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_AFRL (*(volatile uint32_t *)0x40020020)
#define GPIOA_PUPDR (*(volatile uint32_t *)0x4002040C)
#define GPIOB_MODER (*(volatile uint32_t *)0x40020400)
#define GPIOB_ODR (*(volatile uint32_t *)0x40020414)
#define GPIOB_BSRR (*(volatile uint32_t *)0x40020418)
#define TIM2_CR1 (*(volatile uint32_t *)0x40000000)
#define TIM2_SR (*(volatile uint32_t *)0x40000010)
#define TIM2_EGR (*(volatile uint32_t *)0x40000014)
#define TIM2_CCMR1 (*(volatile uint32_t *)0x40000018)
#define TIM2_CCER (*(volatile uint32_t *)0x40000020)
#define TIM2_CNT (*(volatile uint32_t *)0x40000024)
#define TIM2_PSC (*(volatile uint32_t *)0x40000028)
#define TIM2_ARR (*(volatile uint32_t *)0x4000002C)
#define TIM2_CCR1 (*(volatile uint32_t *)0x40000034)
#define TIM2_CCR2 (*(volatile uint32_t *)0x40000038)

// busy-wait for n microseconds using the free-running TIM2 counter
static void delay_us(uint32_t n) {
    uint32_t start = TIM2_CNT;
    while ((TIM2_CNT - start) < n) {}
}

// 10 us high pulse on TRIG (PB5)
static void trigger(void) {
    GPIOB_BSRR = (1u << 5);    // PB5 high
    delay_us(10);
    GPIOB_BSRR = (1u << 21);   // PB5 low
}

int main(){

    RCC_AHB1ENR |= ((1 << 0) | (1 << 1)); // enable clock for GPIOA & GPIOB
    RCC_APB1ENR |= (1 << 0); // enable clock for TIM2

    // clear and set PB5 to output mode
    GPIOB_MODER &= ~(3u << 10);
    GPIOB_MODER |= (1u << 10); 

    // clear and set PB0 to output mode
    GPIOB_MODER &= ~(3u << 0);
    GPIOB_MODER |= (1u << 0); 
    // clear and set PB1 to output mode
    GPIOB_MODER &= ~(3u << 2);
    GPIOB_MODER |= (1u << 2); 
    // clear and set PB2 to output mode
    GPIOB_MODER &= ~(3u << 4);
    GPIOB_MODER |= (1u << 4); 

    // clear PA5 and put it into AF mode
    GPIOA_MODER &= ~(3u << 10);
    GPIOA_MODER |= (2u << 10);

    // clear PA5 GPIOA_AFRL and select AF1 (TIM2_CH1)
    GPIOA_AFRL &= ~(0xFu << 20);
    GPIOA_AFRL |= (1u << 20);

    GPIOA_PUPDR &= ~(2u << 10);

    TIM2_PSC = 15; // timer clock is 16 MHz HSI, so PSC = 15 gives 1 MHz (1 us per count)
    TIM2_ARR = 0xFFFFFFFF; // max 32-bit reload, so the counter effectively never wraps (about 71 minutes)

    // channel 1 input = TI1 (PA5), direct: CC1S (bits 1:0) = 01
    TIM2_CCMR1 &= ~(3u << 0);
    TIM2_CCMR1 |=  (1u << 0);

    // channel 2 input = TI1 (PA5), indirect: CC2S (bits 9:8) = 10
    TIM2_CCMR1 &= ~(3u << 8);
    TIM2_CCMR1 |=  (2u << 8);

    // polarity: clear CC1P, CC1NP, CC2P, CC2NP first
    TIM2_CCER &= ~((1u << 1) | (1u << 3) | (1u << 5) | (1u << 7));

    // channel 1 stays rising edge (CC1P = 0, CC1NP = 0)
    // channel 2 falling edge: CC2P = 1, CC2NP = 0
    TIM2_CCER |= (1u << 5);

    // enable both captures: CC1E (bit 0) and CC2E (bit 4)
    TIM2_CCER |= (1u << 0) | (1u << 4);

    // UG: force update event to load PSC and ARR into working registers
    TIM2_EGR = (1u << 0);

    // CEN: start counting
    TIM2_CR1 |= (1u << 0);
    
    while (1) {
        trigger();               // ask the sensor to measure
        delay_us(60000);         // wait 60 ms, longer than any echo

        // reading the CCR registers also clears the capture flags
        echo_start = TIM2_CCR1;
        echo_end   = TIM2_CCR2;
        width      = echo_end - echo_start;
    }

}