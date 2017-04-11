.section ".init"

// Make Start global.
.globl Start
.global PUT32
.global GET32
.global dummy
.global BOOTUP
.globl blinkloop

/**
 * from the original raspbootin
 **/
start:
	// Setup the stack.
	mov	sp, #0x8000
    
    /* enabling the ACT-Pin */
    mov r0, #16
    mov r1, #1
    bl gpio_pinmode
    
    // enable FPU in coprocessor enable register - this gives everybody access to both locations of coprocessor.
    ldr r0, =(0xF << 20)
    mcr p15, 0, r0, c1, c0, 2
    
    // enable FPU in FP exception register
    MOV r3, #0x40000000
    #VMSR FPEXC, r3    # assembler bug -> the following data describes the instruction
    .long 0xeee83a10
    
    ldr r0, =1920
    ldr r1, =1080
    mov r2, #24
    bl framebufferInit
    // returnvalue is frameinfo-address
	
	bl main
	
halt:
	wfe
	b halt

blinkloop:
    mov r0, #16
    mov r1, #1
    bl gpio_write
    
    mov r0, #1
    bl wait
    
    mov r0, #16
    mov r1, #0
    bl gpio_write
    
    mov r0, #1
    bl wait
    b blinkloop   

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
