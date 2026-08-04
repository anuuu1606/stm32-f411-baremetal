# Phase 2 — MCU Architecture

Building working understanding of the ARM Cortex-M4 core and STM32F411's supporting hardware (NVIC, RCC/clock tree) at the register level, with no HAL and no CubeMX-generated code. The goal is to manually do what CubeMX normally does invisibly, so the abstraction stops being a black box.

Two hands-on build projects, plus concepts absorbed along the way.

## Projects

### `nvic-priority-demo/`

Two software-triggered interrupts (via `NVIC_SetPendingIRQ()`, no real timer events needed) at different NVIC priorities, proving priority-based preemption visually: a high-priority ISR interrupts a low-priority ISR mid-execution, shown as one LED flashing while another is still lit.

**Concepts covered:**
- NVIC priority registers and priority numbering (lower number = higher priority on Cortex-M)
- Enabling interrupts vs. setting their priority (two separate steps)
- Software-pending an interrupt without a real peripheral event
- Interrupt preemption, observed rather than just read about

### `clock-configuration-from-scratch/`

Full manual clock bring-up: HSE → PLL → SYSCLK/AHB/APB, taking the chip from its default 16 MHz internal oscillator to 84 MHz off the external crystal, with a SysTick heartbeat LED as runtime proof the math and register writes were correct.

**Concepts covered:**
- Oscillator startup is not instant: enable-then-poll-ready pattern (HSE, then PLL)
- Flash wait states must be set *before* raising clock speed, not after
- PLL M/N/P math and the legal VCO input/output ranges that constrain valid values
- Bus prescalers — not every peripheral bus tolerates full core speed
- SW vs. SWS as separate write/read-back fields for clock source switching
- `SystemCoreClock` as a variable your code depends on, not just documentation

## Study topics in this phase (conceptual)

### Cortex-M4 core
- ARM Cortex-M4 Technical Reference Manual: core registers (R0–R15, xPSR, MSP/PSP), the 3-stage pipeline at a conceptual level, Thumb-2 instruction set basics
- Exception model: what exception numbers exist, vector table layout, exception entry/exit stacking behavior
- NVIC: priority grouping (group vs. sub-priority), priority registers, tail-chaining/late-arrival

### Memory map
- RM0383 (memory map): Flash region, SRAM region, peripheral bus regions and their base addresses
- AHB1/AHB2/APB1/APB2 — what's on which bus, why it matters (bus speed limits on APB1 vs. APB2)

### Reset & clock system
- RM0383 (RCC): HSI/HSE/PLL sources, PLL multiplier/divider math, SYSCLK/HCLK/PCLK1/PCLK2 prescaler tree
- Reset sources (POR, NRST pin, watchdog, software reset) and what triggers each
- Flash wait-states vs. clock frequency table (needed for the clock-config project's practical work)
