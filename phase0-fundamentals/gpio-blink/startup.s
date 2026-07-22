.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler

.section .isr_vector, "a"
.word 0x20020000        /* 0: initial stack pointer */
.word Reset_Handler      /* 1: Reset */
.word Default_Handler    /* 2: NMI */
.word Default_Handler    /* 3: HardFault */
.word Default_Handler    /* 4: MemManage */
.word Default_Handler    /* 5: BusFault */
.word Default_Handler    /* 6: UsageFault */

.section .text
Reset_Handler:
    bl main
loop:
    b loop

Default_Handler:
    b Default_Handler