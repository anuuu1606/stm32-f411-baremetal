# NVIC Priority Demo

Bare-metal demonstration of NVIC interrupt priority preemption on the STM32F411CEU6. No HAL, no CubeMX, direct register access only.

## What it proves

Two software-triggered timer interrupts (TIM2, TIM3) are assigned different NVIC priorities. TIM2 (low priority) starts a long-running handler; partway through, it triggers TIM3 (high priority). NVIC preempts TIM2's handler mid-execution to run TIM3's handler first, then resumes TIM2, thus proving priority-based interrupt preemption is actually happening, not just that interrupts fire.

## Hardware

- Black Pill (STM32F411CEU6)
- 2x LED + resistor, wired to PA5 (LED5) and PA6 (LED6), cathodes to GND
- ST-Link V2 (SWD)

## Build & flash
```make
make flash```

## Expected behavior

- LED5 (PA5) turns on and stays lit for a long delay
- Partway through, LED6 (PA6) briefly blinks **while LED5 is still lit**. This overlap is the proof of preemption
- LED6 turns off first (shorter delay), LED5 turns off later once its own delay finishes
- Cycle does not repeat (single-shot trigger from `main()`) — reset the board to see it again

## Registers used

- `RCC_AHB1ENR` — enable GPIOA clock
- `GPIOA_MODER`, `GPIOA_ODR` — pin config and output control
- `NVIC_ISER0` — enable TIM2 (IRQ 28) and TIM3 (IRQ 29)
- `NVIC_IPR7` — set TIM2 priority = 10, TIM3 priority = 2 (lower number = higher priority)
- `NVIC_ISPR0` — software-trigger (pend) each interrupt manually, no real timer counting used

## Notes

- Vector table (`startup.s`) manually extended to cover positions up to TIM3 (IRQ 29), since default minimal startup only covered core exceptions
- No scope/logic analyzer used — verified visually via LED overlap timing
