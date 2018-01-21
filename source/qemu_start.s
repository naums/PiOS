.section ".init"

.globl _start

_start:
    ldr sp, =stack_top
    bl main

hang:
    b hang
