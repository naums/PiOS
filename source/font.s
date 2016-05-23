.globl drawCharacter

.globl font
.globl printString
.globl itoa
.globl hex32

/**
 * \brief prints a number in hex-format
 * \param r0 the intergernumber
 * \return r0 the ptr to the memory block (will be overwritten when called twice!)
 * \note shared memory with itoa!
 **/
hex32:
    ldr r2, =itoadata
    
    push {lr}

    teq r0, #0
    beq printNull

    push {r5, r7}
    
    mov r3, #32
    mov r7, #0xf0000000
    hexloop:
        // r4 => masked 4 Bits of r0
        and r4, r0, r7
        
        sub r3, #4
        
        lsr r4, r3
        cmp r4, #9
        movls r5, #0x30
        movhi r5, #0x57 // start 0x61 ('a') but r4 is >= 10!
        
        add r4, r5
        strb r4, [r2]
        add r2, #1
        
        lsr r7, #4       
        
        teq r3, #0
        bne hexloop

    pop {r5, r7}
    b nullbytehex

printNullHex:
    mov r1, #0x30
    strb r1, [r2]

nullbytehex:
    mov r1, #0
    strb r1, [r2, #1]    
    
    ldr r0, =itoadata
    pop {pc}    
    

/**
 * \brief converts an integer number (unsigned) into a string
 * \param r0 the integernumber
 * \return r0 the ptr to the memory block (will be overwritten when called twice!)
 **/
itoa:   
    ldr r2, =afterdata
    sub r2, #2
    push {lr}
    teq r0, #0
    beq printNull;
    
    push {r6, r7}
    mov r6, #10        
    mov r7, r2
    
    $itoa_loop:
        mov r1, r6
        bl udiv       
    
        add r1, #0x30
        strb r1, [r7]
        sub r7, #1

        cmp r0, #0
        bhi $itoa_loop;
    
    mov r2, r7
    pop {r6, r7}
    
    b itoaexit

printNull:
    mov r1, #0x30
    strb r1, [r2]
    mov r0, r2
    pop {pc}
itoaexit:  
    add r2, #1
    mov r0, r2
    pop {pc}
    
    // the following code is used by itoa and hex32!


/**
 * \brief prints a string to the screen
 * \param r0 ptr to the first character (first 8 bit) of the string
 * \param r1 x-position of the first character
 * \param r2 y-position of the first character
 * \note if a character does not fit on the screen anymore, it will be wrapped back to x-position
 * \return r0 -> x, r1 -> y
 **/
printString:
    teq r0, #0
    moveq pc, lr

    push {lr}
    push {r5, r6, r7, r8, r9, r10}
    width .req r7
    height .req r8
    x .req r5
    y .req r6
    fb .req r3
    
    ldr fb, =framebufferInfo
    ldr width, [fb, #0x08]
    ldr height, [fb, #0x0c]
    ldr fb, [fb, #0x20]
    
    mov x, r1
    mov y, r2
    
    offset .req r9
    mul offset, y, width
    lsl offset, #2
    add offset, x, lsl #2
    add fb, offset
    .unreq offset
    
    address .req r10
    mov address, r0
    char .req r0
    $putchar:
        ldrb char, [address]
        teq char, #0
        beq out // read the nullbyte -> exit
            add address, #1
        
        teq char, #'\n'
        addeq r2, #16
        moveq r1, x
        beq $putchar
        
        teq char, #'\r'
        addeq r2, #16
        moveq r1, x
        beq $putchar
        
        push {r1, r2}   // save both r1 and r2
        bl drawCharacter
        pop {r1, r2}
        
        add r1, #16  // add 16 px to the x value
        // if ((newX+8) > width) then 
        cmp r1, width
        addhi r2, #16     // add y + 16
        movhi r1, x       // reset x value
        subls r1, #8    
        // if ((newY + 16) > height) then
        add r2, #16
        cmp r2, height
        bhi out
        subls r2, #16
        
        b $putchar; 
out:        
    mov r0, x
    mov r1, y

    .unreq width
    .unreq height
    .unreq address
    .unreq char
    .unreq x
    .unreq y

    pop {r5, r6, r7, r8, r9, r10}
    pop {pc}    


/**
 * \brief draws a single character to the screen.
 * \param r0 the character to draw (lowest 8 Bit)
 * \param r1 the x-position of the character on the framebuffer
 * \param r2 the y-position
 **/
drawCharacter:
    // if (r0 < 32 || r0 > 127) return 0;
    cmp r0, #127
    movhi r0, #0
    movhi pc, lr
    cmp r0, #31
    movls r0, #0
    movls pc, lr
    
    ldr r3, =font
    
    // ptr to the first 32 Bit to load from the bitmap-file
    begin .req r4
    add begin, r3, r0, lsl #4
    
    push {lr}
    push {r5,r6,r7,r8,r9, r10, r11, r12, r14}
    mov r0, r1 
    mov r1, r2
        
    x .req r12
    y .req r14
    
    fb .req r3
    width .req r2
    height .req r11
    ldr fb, =framebufferInfo
    ldr width, [fb, #0x08]
    ldr height, [fb, #0x0c]
    ldr fb, [fb, #0x20]
    
    // offset = width*y*4 + x*4
    lsl r1, #2
    mul r10, width, r1
    add r10, r0, lsl #2

    mov x, r0
    mov y, r1

    add fb, fb, r10
    mov r10, width
    .unreq width
    width .req r10
    
    color .req r2   
    ldr color, =foreColor
    ldr color, [color]
   
    /**
     * r0          -> counting register counting pixels (and therefor linebreak)
     * r2 (color)  -> the color-value to be written
     * r3 (fb)     -> framebuffer ptr (the current pixel)
     * r4 (begin)  -> ptr to the current 32 bit to be read from bitmap-fontfile
     * r5          -> bitmask
     * r6          -> 32 Bits of the bitmap-fontfile
     * r8          -> counting the lines per character (16)
     * r10 (width) -> width of the framebuffer
     **/
    add fb, #36
    mov r8, #16
    $drawChar:
        ldrb r6, [begin]
        ldr r5, =0x100
        mov r0, #8
        
        $drawByte:
            and r9, r6, r5
            teq r9, #0
            
            strne color, [fb]
            sub fb, #4
            lsr r5, #1
            teq r5, #0
            bne $drawByte;
        
        add fb, width, lsl #2
        add fb, #36
        add begin, #1
        sub r8, #1
        teq r8, #0
        bne $drawChar;

    .unreq begin
    .unreq width
    .unreq height
    .unreq y
    .unreq x
    .unreq color
    .unreq fb
    
    mov r0, #1
    pop {r5,r6,r7,r8,r9, r10, r11, r12, r14}
    pop {pc}


.section .data
.align 4
    
itoadata:
    .ascii "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    .zero 1
afterdata:
    .ascii "\0"

font:
    .incbin "font1.bin"
