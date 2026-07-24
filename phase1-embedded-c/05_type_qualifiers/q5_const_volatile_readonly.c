// Q. Write a function void print_status(const volatile uint32_t *reg) that reads and prints
// a fake status register through a const volatile pointer. In the function, try to write to
// *reg (e.g. *reg = 1;) and observe the compiler error. Explain in a comment why const volatile
// together makes sense for a real hardware status register.

#include <stdio.h>
#include <stdint.h>

void print_status(const volatile uint32_t *reg) {
    printf("Status: %u\n", *reg);   // reading is fine

    // uncommenting the next line will result in compiler error:
    // *reg = 1;   // ERROR: assignment of read-only location
}

int main() {
    uint8_t fake_memory[4] = {5, 0, 0, 0};

    print_status((const volatile uint32_t *)fake_memory);

    return 0;
}

// Why const volatile together makes sense for a real hardware status register:
// - volatile is needed because the register's value can change at any time due to
//   hardware activity, so the compiler must never assume it stays the same and must 
//   always re-read it from memory.
// - const is needed because, even though the register's value changes on its own 
//   (from hardware), that doesn't mean your program should ever try to write to it. 
//   A status register is typically read-only by design.