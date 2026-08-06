#include <stdbool.h>
#include <stdint.h>

#define RCC_CR (*(volatile unsigned int *)0x40023800)
#define RCC_AHB1ENR (*(volatile unsigned int *)0x40023830)
#define GPIOC_MODER (*(volatile unsigned int *)0x40020800)
#define GPIOC_ODR (*(volatile unsigned int *)0x40020814)
#define FLASH_ACR (*(volatile unsigned int *)0x40023C00)
#define RCC_PLLCFGR (*(volatile unsigned int *)0x40023804)
#define RCC_CFGR (*(volatile unsigned int *)0x40023808)
#define STK_CTRL (*(volatile unsigned int *)0xE000E010)
#define STK_LOAD (*(volatile unsigned int *)0xE000E014)
#define STK_VAL (*(volatile unsigned int *)0xE000E018)

volatile int tick_count = 0;
uint32_t SystemCoreClock = 16000000;

void SysTick_Handler(void) {
    tick_count++;
    if (tick_count == 10) {
        GPIOC_ODR ^= (1 << 13);   // toggle PC13
        tick_count = 0;
        }
    }


void delay(volatile int count) {
    while (count--) {}
}


int main() {
    RCC_CR |= (1 << 16); // set HSEON to enable HSE oscillator
    RCC_AHB1ENR |= (1 << 2); // enable GPIOC clock (bit 2 = GPIOC)
    GPIOC_MODER |= (1 << 26); // set PC13 mode bits to 01 (output)
    GPIOC_MODER &= ~(1 << 27); // ensure bit 27 is 0

    // check if HSERDY is set to confirm it is actually ready and oscillating
    // if faulty and did not start then FAILURE block executes
    bool HSE_STATE = false;
    int hse_failure = 0;
    for (int HSE_COUNTER = 0; HSE_COUNTER < 1000; HSE_COUNTER++) {
        if (RCC_CR & (1 << 17)) {
            HSE_STATE = true;
            break;
        } 
    } 
    if (HSE_STATE == false) {
        hse_failure = 1;
        while(1) {
            // blink twice to indicate failure of HSE oscillator
            if (hse_failure == 1) {
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                hse_failure = 0;
            }

        }
    }

    // telling flash memory control to insert 2 extra WS (3 CPU Cycles) for reading instructions
    // because clock will be running at 84 MHz which may cause corrupt data execution
    FLASH_ACR &= ~(0xF);      // clear LATENCY field (bits 0-3) first
    FLASH_ACR |= (2 << 0); // setting LATENCY field to 0010 (2 WS)

    // writing to RCC_PLLCFGR for PLL configuration
    RCC_PLLCFGR &= ~(0x3F << 0);
    RCC_PLLCFGR |= (25 << 0); // setting M as 25 (bits 5:0)

    RCC_PLLCFGR &= ~(0x1FF << 6);
    RCC_PLLCFGR |= (168 << 6); // setting N as 168 (bits 14:6)

    RCC_PLLCFGR &= ~(0x03 << 16);
    RCC_PLLCFGR |= (0 << 16); // setting P as 2 (bits 17:16)

    RCC_PLLCFGR |= (1 << 22); // setting PLLSRC for HSE

    // setting PLLON to turn it on
    RCC_CR |= (1 << 24);

    // check if PLLRDY is set to confirm it is actually ready and locked
    // if faulty and did not start then FAILURE block executes
    bool PLL_STATE = false;
    int PLL_failure = 0;
    for (int PLL_COUNTER = 0; PLL_COUNTER < 1000; PLL_COUNTER++) {
        if (RCC_CR & (1 << 25)) {
            PLL_STATE = true;
            break;
        } 
    } 
    if (PLL_STATE == false) {
        PLL_failure = 1;
        while(1) {
            // blink twice to indicate failure of PLL oscillator
            if (PLL_failure == 1) {
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                PLL_failure = 0;
            }

        }
    }

    // APB1 (PPRE1) has max speed of 50 MHz so it needs minimum prescalar of 2
    RCC_CFGR &= ~(0x07 << 10); // bits 12:10
    RCC_CFGR |= (4 << 10);

    RCC_CFGR |= (0 << 13); // APB2 prescaler = /1 (explicit)
    RCC_CFGR |= (0 << 4);  // AHB prescaler = /1 (explicit)

    // setting 10 in SW (bits 1:0) for PLL selected as system clock
    RCC_CFGR &= ~(0x3 << 0);
    RCC_CFGR |= (2 << 0);

    // check if SWS is set to 0b2 to confirm it is actually ready 
    // if faulty and did not start then FAILURE block executes
    bool SWS_STATE = false;
    int SWS_failure = 0;
    for (int SWS_COUNTER = 0; SWS_COUNTER < 1000; SWS_COUNTER++) {
        if (((RCC_CFGR >> 2) & 0x3) == 2) { 
            // important: in previous checks, only single bit was being checked
            // here 2 bits are being checked if they are 10
            // thus bits 3:2 shifted down to 1:0, masks off everything else and checks if == 2
            SWS_STATE = true;
            break;
        } 
    } 
    if (SWS_STATE == false) {
        SWS_failure = 1;
        while(1) {
            // blink twice to indicate failure of SWS oscillator
            if (SWS_failure == 1) {
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                GPIOC_ODR ^= (1 << 13);   // toggle PC13
                delay(2000000);
                SWS_failure = 0;
            }

        }
    }

    // updating CPU's current clock frequency in Hz
    SystemCoreClock = 84000000;

    // using SysTick for confirmation of program easily
    // reload_value = (desired_time_seconds × SystemCoreClock) − 1
    // reload_value = (1 * 84,000,000) − 1 = 83,999,999

    // 83,999,999 does not fit in STK_LOAD so 0.1 s used instead of 1 s
    // reload_value = (0.1 * 84,000,000) - 1 = 8,399,999
    STK_LOAD = 8399999;

    // starting value for SysTick
    STK_VAL = 0;

    // enable SysTick, enable interrupt-driven, use core clock (not divided)
    STK_CTRL |= (1 << 0) | (1 << 1) | (1 << 2);

    while (1) {
    // idle state
    }
}
