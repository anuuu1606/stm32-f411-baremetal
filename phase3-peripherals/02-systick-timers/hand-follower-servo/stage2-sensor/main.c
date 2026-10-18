#include <stdint.h>

#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)
#define RCC_APB1ENR (*(volatile uint32_t *)0x40023840)
#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_PUPDR (*(volatile uint32_t *)0x4002000C)
#define GPIOA_AFRL (*(volatile uint32_t *)0x40020020)
#define GPIOB_MODER (*(volatile uint32_t *)0x40020400)
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

#define ECHO_TIMEOUT_US  30000u   // longer than any real echo

// values to watch in GDB
volatile uint32_t width;          // echo width in us, 0 = no echo
volatile uint32_t distance_cm;    // distance in cm

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

// trigger, wait for the falling-edge capture, return echo width in us
// returns 0 if no echo arrived within the timeout
static uint32_t measure(void) {

    // clear CC1IF (bit 1) and CC2IF (bit 2): writing 0 clears, writing 1 does nothing
    TIM2_SR = ~((1u << 1) | (1u << 2));

    trigger();

    uint32_t start = TIM2_CNT;

    // wait until CC2IF is set (falling edge captured), or give up
    while (!(TIM2_SR & (1u << 2))) {
        if ((TIM2_CNT - start) > ECHO_TIMEOUT_US) {
            return 0;          // no echo
        }
    }

    // both timestamps are fresh
    return TIM2_CCR2 - TIM2_CCR1;
}

// turn on the LEDs in 'on_mask' (bits 0 to 2 = PB0 to PB2), turn the other two off
// one BSRR write: low half sets pins, high half clears pins, so no flicker
static void show_leds(uint32_t on_mask) {
    GPIOB_BSRR = on_mask | ((7u & ~on_mask) << 16);
}

int main(){

    RCC_AHB1ENR |= ((1u << 0) | (1u << 1)); // enable clock for GPIOA & GPIOB
    RCC_APB1ENR |= (1u << 0);               // enable clock for TIM2

    // PB5 (TRIG) to output mode
    GPIOB_MODER &= ~(3u << 10);
    GPIOB_MODER |=  (1u << 10);

    // PB0, PB1, PB2 (LEDs) to output mode
    GPIOB_MODER &= ~(3u << 0);
    GPIOB_MODER |=  (1u << 0);
    GPIOB_MODER &= ~(3u << 2);
    GPIOB_MODER |=  (1u << 2);
    GPIOB_MODER &= ~(3u << 4);
    GPIOB_MODER |=  (1u << 4);

    // PA5 (ECHO) to AF mode
    GPIOA_MODER &= ~(3u << 10);
    GPIOA_MODER |=  (2u << 10);

    // PA5 AFRL: select AF1 (TIM2_CH1)
    GPIOA_AFRL &= ~(0xFu << 20);
    GPIOA_AFRL |=  (1u << 20);

    // PA5: no pull-up / pull-down (bits 11:10 = 00)
    GPIOA_PUPDR &= ~(3u << 10);

    TIM2_PSC = 15;          // 16 MHz HSI / 16 = 1 MHz (1 us per count)
    TIM2_ARR = 0xFFFFFFFF;  // effectively never wraps (about 71 minutes)

    // channel 1 input = TI1 (PA5), direct: CC1S (bits 1:0) = 01
    TIM2_CCMR1 &= ~(3u << 0);
    TIM2_CCMR1 |=  (1u << 0);

    // channel 2 input = TI1 (PA5), indirect: CC2S (bits 9:8) = 10
    TIM2_CCMR1 &= ~(3u << 8);
    TIM2_CCMR1 |=  (2u << 8);

    // polarity: clear CC1P, CC1NP, CC2P, CC2NP first
    TIM2_CCER &= ~((1u << 1) | (1u << 3) | (1u << 5) | (1u << 7));

    // channel 1 stays rising edge, channel 2 falling edge (CC2P = 1)
    TIM2_CCER |= (1u << 5);

    // enable both captures: CC1E (bit 0) and CC2E (bit 4)
    TIM2_CCER |= (1u << 0) | (1u << 4);

    // UG: load PSC and ARR into working registers
    TIM2_EGR = (1u << 0);

    // CEN: start counting
    TIM2_CR1 |= (1u << 0);

    while (1) {
        width = measure();

        if (width == 0) {
            distance_cm = 0;
            show_leds(7u);                 // no echo: all three LEDs on
        } else {
            distance_cm = width / 58;      // 58 us of echo per cm

            if (distance_cm < 10) {
                show_leds(1u << 0);        // under 10 cm: PB0
            } else if (distance_cm < 20) {
                show_leds(1u << 1);        // 10 to 19 cm: PB1
            } else {
                show_leds(1u << 2);        // 20 cm and over: PB2
            }
        }

        delay_us(60000);                   // at least 60 ms between pings
    }
}