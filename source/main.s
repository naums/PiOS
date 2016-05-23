/**
 * \file main.s
 * \brief contains start-code
 * \note needs cleanup
 **/

.section .init
.globl _start
.globl blinkloop

_start:
    mov sp, #0x8000
    
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
    mov r2, #32
    bl framebufferInit
    // returnvalue is frameinfo-address
    
    cmp r0, #0
    bls blinkloop
    
    // setting r3 to the framebuffer ptr
    ldr r1, [r0, #0x0c] // width
    ldr r2, [r0, #0x10] // height 
    ldr r0, [r0, #0x20] // framebuffer addr
    
    bl setFrameBuffer
    
    ldr r0, =0xffcccccc
    bl setForeColor
    
    bl fillColor
    
    ldr r0, =0xff000000
    bl setForeColor
    
    ldr r0, =mystring
    mov r1, #10
    mov r2, #500
    bl printString
    
    ldr r0, =_p1
    bl createProcess
    
    ldr r0, =_p2
    bl createProcess
    
    bl startProcess
    
    bl cmain

    b blinkloop;
    // blinks the LED when an error has occured while initiating the framebuffer
    
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
    
_p1:
    mov r5, #10
process1:
    
    mov r0, #0x61
    mov r1, r5
    mov r2, #100
    
    bl drawCharacter
    
    add r5, #10
    
    mov r0, #1
    bl wait
    
    bl yield
    b process1
    
_p2:
    mov r5, #10
process2:
    
    mov r0, #0x62
    mov r1, r5
    mov r2, #200
    
    bl drawCharacter
    
    add r5, #10
    
    mov r0, #1
    bl wait
    
    bl yield
    b process2
    
  
.section .data
mystring:
    .ascii "Hello world\0"
    .zero 1
osname:
    .ascii "pilotOS\0"
    .zero 1


targetstring:
    .zero 20

