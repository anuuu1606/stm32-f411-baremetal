# Clock Configuration From Scratch

Manual, register-level configuration of the STM32F411CEU6 clock tree (HSE → PLL → SYSCLK/AHB/APB), with SysTick used as a visual confirmation of correct clock output via an LED heartbeat using onboard PC13 LED.

No HAL, no CubeMX-generated code. Every register is set directly from RM0383.

## What this program does

1. Enables the HSE oscillator and waits for `HSERDY`, blinking a failure pattern on PC13 if it doesn't lock within the timeout.
2. Configures Flash wait states (2 WS) to support running at 84 MHz.
3. Configures the main PLL (M = 25, N = 168, P = 2, source = HSE) to produce SYSCLK = 84 MHz.
4. Enables the PLL and waits for `PLLRDY`, blinking a failure pattern on timeout.
5. Configures bus prescalers:
   - APB1 = /2 (must stay ≤ 50 MHz)
   - APB2 = /1
   - AHB = /1
6. Switches SYSCLK source to the PLL and waits for `SWS` to confirm the switch, blinking a failure pattern on timeout.
7. Configures SysTick to interrupt every 0.1 s and toggles PC13 every 10 ticks (1 s heartbeat) as a runtime confirmation that the clock tree is actually running at the expected speed.

## Hardware

- Black Pill (STM32F411CEU6)
- ST-Link V2 clone (SWD)
- Onboard LED: PC13

## Build & Flash

```
make
make flash
```

## Major bug found & fixed: uninitialized `.bss` / `.data`

**Symptom:** `if (tick_count == 10)` in `SysTick_Handler` never fired. The LED never toggled correctly. Switching the condition to `if (tick_count >= 10)` appeared to "fix" it, but actually caused the LED to toggle on *every* SysTick interrupt instead of every 10th.

**Root cause:** `Reset_Handler` in `startup.s` jumped straight to `bl main` with no initialization of RAM:

```
Reset_Handler:
    bl main
loop:
    b loop
```

This skipped two things every startup file is responsible for before `main()` runs:
- Copying `.data` (globals with a non-zero initial value, e.g. `SystemCoreClock = 16000000`) from their stored values in Flash into RAM.
- Zeroing `.bss` (globals initialized to 0, e.g. `volatile int tick_count = 0;`) in RAM.

A C initializer like `= 0` is only a build-time instruction to the compiler about where to place a variable and what it *should* contain, it is not itself a runtime write. Only the startup code's copy/zero loops actually perform that write. Without them, `tick_count` retained whatever garbage was already sitting in SRAM (confirmed via GDB: ~251,720,120 immediately after `monitor reset halt`, since a core reset does not clear SRAM contents). From that already-huge starting value, `tick_count` incremented every tick but never landed on exactly 10, while `>= 10` was trivially true on every tick, masking the bug instead of fixing it.

**Fix:** added explicit `.data` copy and `.bss` zero loops to `Reset_Handler`, using the `_sidata`/`_sdata`/`_edata`/`_sbss`/`_ebss` symbols already exported by `linker.ld`:

```
Reset_Handler:
    /* Copy .data from FLASH to RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata
copy_data_loop:
    cmp r1, r2
    bge copy_data_done
    ldr r3, [r0], #4
    str r3, [r1], #4
    b copy_data_loop
copy_data_done:

    /* Zero .bss in RAM */
    ldr r0, =_sbss
    ldr r1, =_ebss
    movs r2, #0
zero_bss_loop:
    cmp r0, r1
    bge zero_bss_done
    str r2, [r0], #4
    b zero_bss_loop
zero_bss_done:

    bl main
loop:
    b loop
```

**Debugging method:** used `gdb-multiarch` with a breakpoint at the `if (tick_count == 10)` line and `display tick_count`, stepping through `continue` repeatedly to watch the raw value across interrupts, then cross-checked `print &tick_count` / `info symbol &tick_count` to confirm it lived in `.bss`, before inspecting `startup.s` directly.

This is uniquely a bug experienced on writing baremetal code from the ground up myself. In HAL, Cube-MX generated code, this is always included by default and thus, is never a visible issue to the user.


