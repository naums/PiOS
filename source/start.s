.section ".init"

// Make Start global.
.globl Start
.global PUT32
.global GET32
.global dummy
.global BOOTUP

/**
 * from the original raspbootin
 **/
start:
	// Setup the stack.
	mov	sp, #0x8000
	bl main
	
halt:
	wfe
	b halt


PUT32:
    str r1, [r0]
    mov pc, lr

GET32:
    ldr r0, [r0]
    mov pc, lr

dummy:
    mov pc, lr
    
BOOTUP:
    mov pc, r0
    b halt
