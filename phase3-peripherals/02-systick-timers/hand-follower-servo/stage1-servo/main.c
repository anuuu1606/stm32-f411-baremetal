#include <stdint.h>

#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)
#define RCC_APB1ENR (*(volatile uint32_t *)0x40023840)
#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_AFRL (*(volatile uint32_t *)0x40020020)
#define TIM3_PSC (*(volatile uint32_t *)0x40000428)
#define TIM3_ARR (*(volatile uint32_t *)0x4000042C)
#define TIM3_CR1 (*(volatile uint32_t*)0x40000400)
#define TIM3_EGR (*(volatile uint32_t*)0x40000414)
#define TIM3_CCMR1 (*(volatile uint32_t*)0x40000418)
#define TIM3_CCR1 (*(volatile uint32_t*)0x40000434)
#define TIM3_CCER (*(volatile uint32_t*)0x40000420)


int main() {

    // enable clock for GPIOA
    RCC_AHB1ENR |= (1 << 0);

    // enable clock for TIM3
    RCC_APB1ENR |= (1 << 1);

    // clear GPIOA_MODER and put it into AF mode
    GPIOA_MODER &= ~(3u << 12);
    GPIOA_MODER |= (2u << 12);

    // clear GPIOA_AFRL and select AF2 (TIM3_CH1)
    GPIOA_AFRL &= ~(0xFu << 24);
    GPIOA_AFRL |= (2u << 24);
    
    TIM3_PSC = 15;           // PSC to 15 (15 + 1 = 16 to divide my HSI 16 MHz clock)
    TIM3_ARR = 19999;        // auto-reload value to 19999 to get 20ms wrap

    TIM3_CCMR1 &= ~(3u << 0); // channel 1 as output (CC1S = 00, bits 1:0)
    TIM3_CCMR1 |= (1u << 3); // OC1PE: CCR1 preload, new pulse width applies at the next cycle boundary
    
    // OC1M (bits 6:4): clear, then set 110 = PWM mode 1
    TIM3_CCMR1 &= ~(7u << 4);  
    TIM3_CCMR1 |= (6u << 4); // tells pwm to stay high then low after ccr1 value hits

    TIM3_CCR1 = 1500;  // pulse width 1500 us = 1.5 ms = servo centre

    TIM3_CCER |= (1u << 0); // CC1E: connect channel 1 output to the PA6 pin

    TIM3_CR1 |= (1u << 7);   // ARPE, buffer ARR
    TIM3_EGR  = (1u << 0);   // UG, load PSC and ARR into working registers
    TIM3_CR1 |= (1u << 0);   // CEN, start counting

    while (1) {}
}