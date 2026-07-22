# FIRST PROGRAM ON STM32-F411: GPIO LED BLINK

Bare-metal LED blink on board GPIO C13 LED of BlackPill STM32F411CEU6, written at register level. No HAL, no CubeMX-generated code.

## What this does

Toggles PC13 on and off in infinite loop, using direct register writes to RCC & GPIOC. Built at register level with no vendor tooling.

## Hardware
- Board: Black PIll STM32F411CEU6
- Debugger: ST-Link V2
- LED: Onboard LED PC13 (Active-Low)

## Build & Flash
```bash
make # compiles and links, produces blink.elf and blink.bin
make flash # flashes via OpenOCD + ST-Link over SWD
```

## Files
- `linker.ld` — defines FLASH/RAM memory regions and section placement
- `startup.s` — vector table and reset handler
- `main.c` — GPIO clock enable, pin config, and toggle loop
- `Makefile` — build and flash automation

## Debugging notes

First flash attempt resulted in HardFault before main loop code even ran due to:

1. **Incomplete vector table** - only stack pointer and reset handler were defined. Cortex M-4 requires vector table entries for all faults regardless of whether they are used or not. Due to not being initially defined, any fault jumped to garbage memory.
2. **Missing `.thumb_func` directive** - Cortex M-4 functions only in Thumb mode which accordingly requires handler addresses to have their low bit set. Without `.thumb_func`, addresses in vector table were not correctly marked, causing a fault even with completed table.

Diagnosed using `gdb-multiarch` attached to OpenOCD's GDB server `localhost:3333`. By halting execution and inspecting program counter (`info register pc`), it showed that it was stuck outside of expected Flash range of `0x08000000`, confirming fault.

Confirmed working on hardware after applying both fixes.
