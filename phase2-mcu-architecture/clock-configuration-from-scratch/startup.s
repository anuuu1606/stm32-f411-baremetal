.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler

.section .isr_vector, "a"
.word 0x20020000
.word Reset_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word SysTick_Handler

.section .text

.thumb_func
Reset_Handler:
    /* Copy .data section from FLASH to RAM */
    ldr r0, =_sidata      /* r0 = source address (in FLASH) */
    ldr r1, =_sdata        /* r1 = destination address (start of .data in RAM) */
    ldr r2, =_edata         /* r2 = destination end address (end of .data in RAM) */

copy_data_loop:
    cmp r1, r2               /* have we reached the end? */
    bge copy_data_done        /* if r1 >= r2, we're done copying */
    ldr r3, [r0], #4            /* load word from FLASH, post-increment r0 by 4 */
    str r3, [r1], #4             /* store word to RAM, post-increment r1 by 4 */
    b copy_data_loop
copy_data_done:

    /* Zero out .bss section in RAM */
    ldr r0, =_sbss         /* r0 = start of .bss */
    ldr r1, =_ebss          /* r1 = end of .bss */
    movs r2, #0               /* r2 = 0, the value we'll write */

zero_bss_loop:
    cmp r0, r1
    bge zero_bss_done
    str r2, [r0], #4
    b zero_bss_loop
zero_bss_done:

    bl main
loop:
    b loop
.thumb_func
Default_Handler:
    b Default_Handler
