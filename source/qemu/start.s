.section ".init"

.globl start

start:
    ldr sp, =stack_top
    bl main

hang:
    b hang
