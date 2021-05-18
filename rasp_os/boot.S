.section ".text.boot"

.global _start

_start:
    // mpidr_el1 stores the info of CPU
    // mrs move register system
    // we have to shut three of four core
    mrs x1, mpidr_el1
    and x1, x1, #3
    // jmp to 2 if zero
    cbz x1, 2f
    // infinity loop, wfe represent wait for event
1:  wfe
    b   1b
2:  // main core part now
    // set stack pointer
    ldr x1, =_start
    mov sp, x1

    // clean the bss section
    ldr x1, =__bss_start
    ldr w2, =__bss_size
3:  cbz w2, 4f
    // xzr word zero register, represent 0
    str xzr, [x1], #8
    sub w2, w2, #1
    cbnz w2, 3b
4:  bl main // bl will store the return address in x30
    b 1b    // halt the master core    
