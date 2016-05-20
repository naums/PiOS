
.globl bresenham
.globl setpixel
.globl setFrameBuffer
.globl setForeColor

.globl foreColor
.globl frameBuffer

.globl fillColor
.globl copyImage
//.globl copyImageAlpha

/**
 * \brief prints an image to the framebuffer
 * \param r0 x
 * \param r1 y
 * \note really really slow
 **/
/*copyImageAlpha:
    push {lr}
    push {r5, r6, r7, r8, r9, r10, r11, r12}
    
    ldr r2, =framebufferInfo
    width .req r3
    height .req r4
    fb .req r11
    ldr width, [r2,#0x08]
    ldr height, [r2,#0x0c]
    ldr fb, [r2, #0x20]

    ldr r5, =oslogosize
    ldr r6, [r5]
    ldr r7, [r5, #4]
    lsl r5, r6, #2
    
    add r0, r6
    cmp r0, width
    bhi copyImageOutAlpha
    
    sub r0, r6
    
    add r1, r7
    cmp r1, height
    bhi copyImageOutAlpha
    sub r1, r7
    
    // calculate the starting address of framebuffer
    lsl r1, #2
    mul r10, width, r1
    add r10, r0, lsl #2

    add fb, fb, r10
    mov r10, fb
    ldr r12, =oslogo
    mov r5, r6
    
    $copyImageLoopAlpha:
        teq r7, #0
        beq copyImageOutAlpha
        sub r7, #1
        
        push {width}
        copyPixelAlpha:
            alpha .req r8
            ldrb r8, [r12,#3]
            ldr r9, =1000
            mul r8, r9
            mov r0, r8
            mov r1, #255
            bl udiv
            
            mov r8, r0
            
            mov r2, #3
            copySubPixel:
                push {r2}
                ldrb r4, [r12]
                ldrb r3, [fb]
                
                push {r8}
                sub r8, r9, r8
                mul r3, r8                
                pop {r8}
                
                mul r4, r8
               
                mov r0, r4
                mov r1, r9
                push {r3}
                bl udiv
                pop {r3}
                
                mov r4, r0
                
                mov r0, r3
                mov r1, r9
                push {r4}
                bl udiv
                pop {r4}
                
                add r3, r4, r0
                strb r3, [fb] 
            
                add fb, #1
                add r12, #1
                pop {r2}
                sub r2, #1
                teq r2, #0
                bne copySubPixel
            
            add r12, #1
            add fb, #1
            sub r6, #1
            teq r6, #0
            bne copyPixelAlpha
        
        pop {width}
        add fb, r10, width, lsl #2
        mov r10, fb
        mov r6, r5
        //add r12, r12, r6, lsl #2
    
        b $copyImageLoopAlpha
    
    .unreq fb
    .unreq width
    .unreq height

copyImageOutAlpha:
    pop {r5, r6, r7, r8, r9, r10, r11, r12}
    pop {pc}
*/
/**
 * \brief prints an image to the framebuffer
 * \param r0 x
 * \param r1 y
 **/
copyImage:
    push {lr}
    push {r5, r6, r7, r8, r9, r10, r11, r12}
    
    ldr r2, =framebufferInfo
    width .req r3
    height .req r4
    fb .req r11
    ldr width, [r2,#0x08]
    ldr height, [r2,#0x0c]
    ldr fb, [r2, #0x20]

    ldr r5, =oslogosize
    ldr r6, [r5]
    ldr r7, [r5, #4]
    lsl r5, r6, #2
    
    add r0, r6
    cmp r0, width
    bhi copyImageOut
    
    sub r0, r6
    
    add r1, r7
    cmp r1, height
    bhi copyImageOut
    sub r1, r7
    
    // calculate the starting address of framebuffer
    lsl r1, #2
    mul r10, width, r1
    add r10, r0, lsl #2

    add fb, fb, r10
    ldr r12, =oslogo
    
    $copyImageLoop:
        teq r7, #0
        beq copyImageOut
        sub r7, #1
        
        mov r1, r12
        mov r0, fb
        mov r2, r5
        push {r3, r4}
        bl memcpy
        pop {r3, r4}
        
        add fb, fb, width, lsl #2
        add r12, r12, r6, lsl #2
    
        b $copyImageLoop
    
    .unreq fb
    .unreq width
    .unreq height
    

copyImageOut:
    pop {r5, r6, r7, r8, r9, r10, r11, r12}
    pop {pc}

/**
 * \brief sets the color of a pixel (x,y)
 * \param r0 x coordinate (will not be changed)
 * \param r1 y coordinate (will not be changed)
 **/
setpixel:
    ldr r2, =framebufferInfo
    width .req r3
    height .req r4
    ldr width, [r2,#0x08]
    ldr height, [r2,#0x0c]
    
    // x > width, y < height
    /*cmp r0, width
    cmplt r1, height
    movge pc, lr
    cmp r0, #0
    cmpge r1, #0
    movlt pc, lr*/
    
    // framebuffer still 0 => return
    ldr r2, =frameBuffer
    ldr r2, [r2]
    /*cmp r2, #0
    moveq pc, lr
    */
    // mla => mul + add 
    mul r4, r1, width
    add r2, r4, r0
    lsl r2, #2
    ldr r3, =foreColor
    ldr r3, [r3]
    str r3, [r2]
    .unreq width
    .unreq height
    mov pc, lr
        
/**
 * \brief sets the framebuffer-address
 * \param r0 framebuffer address
 **/
setFrameBuffer:
    ldr r1, =frameBuffer
    str r0, [r1]
    mov pc, lr

/**
 * \brief sets the foreground-color
 * \param r0, the color
 **/
setForeColor:
    ldr r1, =foreColor
    str r0, [r1]
    mov pc, lr

/**
 * \brief fill the framebuffer with one flat color (32Bit)
 **/
fillColor:
    color .req r0
    ldr color, =foreColor
    ldr color, [color]
    
    width .req r2
    height .req r3    
    ldr r1, =framebufferInfo
    ldr width, [r1, #8]
    ldr height, [r1, #12]
    
    push {lr}
    push {r5}
    
    // framebuffer
    fb .req r5
    ldr fb, =frameBuffer
    ldr fb, [fb]
    
    x .req r1
    y .req r4
    mov x, #0
    mov y, #0
    
    mov y, height
    drawImage:
        mov x, width
        drawLine:
            str color, [fb]
            add fb, #4
            sub x, #1
            teq x, #0
            bne drawLine
        
        sub y, #1
        teq y, #0
        bne drawImage
        
    pop {r5}
    pop {pc}
    .unreq width
    .unreq height
    .unreq fb
    .unreq x
    .unreq y

/**
 * \brief uses the bresenham algorithm for drawing a line from (x0,y0) to (x1,y1)
 * \param r0 x0
 * \param r1 y0
 * \param r2 x1
 * \param r3 y1
 * \note untested!
 **/
bresenham:
    push {lr}
    push {r14}
    push {r4, r5, r6, r7, r8, r9, r10, r11, r12}
    ldr r4, =framebufferInfo
    width .req r5
    height .req r6
    ldr width, [r4,#0x08]
    ldr height, [r4,#0x0c]
    
    // check if x0,x1 > width or < 0
    cmp r0, #0
    cmplt r2, #0
    blt bresenham_abort
    cmp r0, width
    cmpgt r2, width 
    bgt bresenham_abort
    
    // check if y0,y1 > width or < 0
    cmp r1, #0
    cmplt r3, #0
    blt bresenham_abort
    cmp r1, height
    cmpgt r3, height
    bgt bresenham_abort
    
    .unreq width
    .unreq height

    deltax .req r7
    deltay .req r8
    error .req r9
    stepx .req r10
    stepy .req r11

    // if x1 > x0
    cmp r2, r0
        subgt deltax, r2, r0
        movgt stepx, #1
    // otherwise
        suble deltax, r0, r2
        movle stepx, #-1
    
    // if y1 > y0
    cmp r3, r1
        subgt deltay, r3, r1
        movgt stepy, #1
    // otherwise
        suble deltay, r1, r3
        movle stepy, #-1
    
    sub error, deltax, deltay
    
    /**
     * until x0=x1+stepx || y0=y1+stepy
            setPixel (x0, y0)
            if error << 1 >= -deltay then
                x0 + = stepx
                error -= deltay
            endif
            if error << 1 <= deltax then 
                y0 += stepy
                error += deltax
            endif
     **/
    maxX .req r12
    maxY .req r14
    add maxX, r2, stepx
    add maxY, r3, stepy
bresenham_inner:
    cmp r0, maxX
    cmple r1, maxY
    bgt bresenham_abort
    
    bl setpixel
    
    cmp deltay, error, lsl #1
    addle error, deltay
    addle r0, stepx
    
    cmp deltax, error, lsl #1
    addge error, deltax
    addge r1, stepy
    
    b bresenham_inner
    
    .unreq maxX
    .unreq maxY
    .unreq stepx
    .unreq stepy
    .unreq deltax
    .unreq deltay
    .unreq error
    
bresenham_abort:
    pop {r4, r5, r6, r7, r8, r9, r10, r11, r12}
    pop {r14}
    pop {pc}
    
    
.section .data
.align 4
foreColor:
    .int 0xffffffff
frameBuffer:
    .int 0
