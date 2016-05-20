/**
 * \file main.s
 * \brief contains start-code
 * \note needs cleanup
 **/

.section .init
.globl _start

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
    
    /*mov r5, #10
    mov r6, #65
    $characterloop:
        mov r0, r6
        mov r1, r5
        mov r2, #8
        bl drawCharacter
        
        add r5, #10
        add r6, #1
        cmp r6, #90
        bls $characterloop;*/
 /*
    mov r1, #0x63
    push {r1}
    mov r1, #42
    push {r1, r1}
    ldr r1, =osname
    push {r1}
    
    ldr r0,=mystring
    bl printf   */
    
    ldr r0, =targetstring
    ldr r1, =osname
    bl strcpy
    
    mov r1, #10
    mov r2, #10
    bl printString
    /*
    ldr r0, =osname
    bl strlen
    
    mov r2, r0
    ldr r0, =targetstring
    mov r1, #0x31
    
    bl memset
    mov r1, #10
    mov r2, #30
    bl printString
    
    ldr r0, =osname
    bl strlen
    
    mov r2, r0
    ldr r0, =targetstring
    ldr r1, =osname
    bl strncpy
    
    mov r1, #10
    mov r2, #50
    bl printString
    
    
    ldr r0, =osname
    mov r1, #10
    mov r2, #70
    bl printString
    
    ldr r0, =targetstring
    ldr r1, =osname
    bl strcat
    
    mov r1, #10
    mov r2, #90
    bl printString

    ldr r0, =osname
    mov r1, #10
    mov r2, #150
    bl printString

    ldr r0, =targetstring
    ldr r1, =mystring
    mov r2, #14
    bl memcpy
    
    mov r1, #10
    mov r2, #110
    bl printString
    
    ldr r0, =targetstring
    ldr r1, =osname
    bl strcmp
    
    bl itoa
    
    mov r1, #10
    mov r2, #130
    bl printString

    ldr r0, =mystring
    mov r1, #10
    mov r2, #200
    bl printString
    
    */
    mov r0, #1000
    mov r1, #100
    bl copyImageAlpha
    
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
    
.section .data
mystring:
    .ascii "Hello world from %s.\n Booting slowly and making the text very long,\r so hopefully I get a bloody linebreak,\n if the code works alright. \n\rAlso: some numbers: %d %x and a c-Character: %c. \nBooting: 100 %%\0"
    .zero 1
osname:
    .ascii "pilotOS\0"
    .zero 1


targetstring:
    .zero 20

