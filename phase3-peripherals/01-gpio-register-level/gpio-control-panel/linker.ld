MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 128K
}

ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text)
        *(.rodata)
    } > FLASH

    _sidata = LOADADDR(.data);   /* where .data's initial values live in FLASH */

    .data :
    {
        _sdata = .;               /* start of .data in RAM */
        *(.data)
        _edata = .;                /* end of .data in RAM */
    } > RAM AT> FLASH

    .bss :
    {
        _sbss = .;                 /* start of .bss in RAM */
        *(.bss)
        _ebss = .;                  /* end of .bss in RAM */
    } > RAM
}