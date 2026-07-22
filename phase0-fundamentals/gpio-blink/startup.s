.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler

.section .isr_vector, "a"
.word 0x20020000
.word Reset_Handler

.section .text
Reset_Handler:
    bl main
loop:
    b loop