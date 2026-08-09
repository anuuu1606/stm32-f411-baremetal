#define RCC_AHB1ENR (*(volatile unsigned int *)0x40023830)
#define GPIOA_MODER (*(volatile unsigned int *)0x40020000)
#define GPIOB_MODER (*(volatile unsigned int *)0x40020400)
#define GPIOB_OTYPER (*(volatile unsigned int *)0x40020404)
#define GPIOA_PUPDR (*(volatile unsigned int *)0x4002000C)
#define GPIOA_IDR (*(volatile unsigned int *)0x40020010)
#define GPIOB_ODR (*(volatile unsigned int *)0x40020414)

#include <stdbool.h>

// function to check whether button has been properly pressed and not just electrical noise
bool ButtonPressed (int num) {
    static int count[2] = {0, 0}; // one slot per button & initializer only runs once
    static bool already_fired[2] = {false, false};

    int pin_val = (GPIOA_IDR >> num) & 1;

    if (pin_val == 0 ) { // as the buttons are active low and 0 gives 'pressed' reading
        count[num]++;
        if (count[num] == 100 && !already_fired[num]) { // 100 may require tuning
            count[num] = 0;
            already_fired[num] = true;
            return true;
        } else {
            return false;
        }
    } else {
        count[num] = 0;
        already_fired[num] = false;  // released -> allow next press to fire
    }
    return false;
}

int main() {
    
    // enable clocks for GPIOA and GPIOB in RCC_AHB1ENR
    RCC_AHB1ENR |= ((1 << 0) | (1 << 1));

    // set input mode for buttons at PA0 (Mode Button) and PA1 (Speed Button)
    GPIOA_MODER &= ~((1 << 0) | (1 << 1)); // PA0
    GPIOA_MODER &= ~((1 << 2) | (1 << 3)); // PA1

    // set general purpose output mode for LEDS at PB0, PB1 and PB2
    GPIOB_MODER |= (1 << 0); // PB0
    GPIOB_MODER &= ~(1 << 1);

    GPIOB_MODER |= (1 << 2); // PB1
    GPIOB_MODER &= ~(1 << 3);

    GPIOB_MODER |= (1 << 4); // PB2
    GPIOB_MODER &= ~(1 << 5);

    // open drain configuration for PB1 (LED 2)
    GPIOB_OTYPER |= (1 << 1); 

    // pull up configuration for both buttons at PA0 and PA1
    GPIOA_PUPDR |= (1 << 0); // PA0
    GPIOA_PUPDR &= ~(1 << 1); 

    GPIOA_PUPDR |= (1 << 2); // PA1
    GPIOA_PUPDR &= ~(1 << 3); 
    
    int mode_state = 0;
    int speed_state = 0;

    while (1) {

        // check button pressed state -> advance mode state
        if (ButtonPressed(0) == true) {
            mode_state = (mode_state + 1) % 4;
        }

        switch (mode_state) {
            
            case 0:
                // ALL OFF MODE

                GPIOB_ODR &= ~(1 << 0); // drive PB0 LOW
                GPIOB_ODR &= ~(1 << 1); // drive PB1 LOW
                GPIOB_ODR &= ~(1 << 2); // drive PB2 LOW
                break;

            case 1:
                // ALL ON MODE

                GPIOB_ODR |= (1 << 0); // drive PB0 HIGH
                GPIOB_ODR |= (1 << 1); // drive PB1 HIGH
                GPIOB_ODR |= (1 << 2); // drive PB2 HIGH
                break;

            case 2: {
                // ALTERNATING MODE
                // LEDs 1 and 2 blink opposite to each other (1 on while 2 off, then swap)
                // a delay function will block the ButtonPressed function so can not be used

                static int counter = 0;
                static int led_state = 0;
                counter++;
                if (counter >= 1000000) { // counter may need tuning
                    counter = 0;
                    led_state = !led_state;
                    if (led_state == 0) {
                        GPIOB_ODR &= ~(1 << 0); // drive PB0 LOW (LED3 OFF)
                        GPIOB_ODR &= ~(1 << 1); // drive PB1 LOW (LED2 OFF)
                        GPIOB_ODR |= (1 << 2); // drive PB2 HIGH (LED1 ON)
                    } else {
                        GPIOB_ODR &= ~(1 << 0); // drive PB0 LOW (LED3 OFF)
                        GPIOB_ODR &= ~(1 << 2); // drive PB2 LOW (LED1 OFF)
                        GPIOB_ODR |= (1 << 1); // drive PB1 HIGH (LED2 ON)
                    }
                }
                }
                break;
            
            case 3: {
                // CHASE MODE

                GPIOB_ODR &= ~(1 << 2); // drive PB2 LOW (LED1 OFF)
                GPIOB_ODR &= ~(1 << 1); // drive PB1 LOW (LED2 OFF)

                // check speed mode: slow, medium, fast
                if (ButtonPressed(1) == true) {
                    speed_state = (speed_state + 1) % 3;
                }

                // set speed of blinking led
                int threshold;
                if (speed_state == 0) threshold = 1000000;
                else if (speed_state == 1) threshold = 150000;
                else threshold = 100000;

                // blink LED based on speed selected
                static int counter3 = 0;
                counter3++;
                if (counter3 >= threshold) {
                    counter3 = 0;
                    GPIOB_ODR ^= (1 << 0); // toggle PB0 (LED3)
                }
                }
                break;
        }
    }
}