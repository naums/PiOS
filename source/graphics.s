.globl framebufferAddr
.globl framebufferInit

/**
 * \brief initiates the framebuffer
 * \param r0 framebuffer width
 * \param r1 framebuffer height
 * \param r2 color depth
 * \return pointer to the framebufferInfo-Data or 0 on error
 **/
framebufferInit:
    // check the inputs
    cmp r0, #4096
    cmpls r1, #4096
    cmpls r2, #32
    movhi r0, #0
    movhi pc, lr

    push {lr}
    // write the data into the framebuffer-info-structure
    ldr r3, =framebufferInfo
    // framebuffer size
    str r0, [r3, #0x00]
    str r1, [r3, #0x04]
    // virtual framebuffer size
    str r0, [r3, #0x08]
    str r1, [r3, #0x0c]
    // color depth
    str r2, [r3, #0x14]
    
    // write the address of the framebuffer-info-structure to the video-controllers mailbox
    ldr r2, =0x40000000
    add r0, r3, r2
    mov r1, #1
    bl postmanWrite
    
    // read the video-controllers mailbox
    mov r0, #1
    bl postmanRead
    
    // answer != 0 -> failure, return with 0
    cmp r0, #0
    movhi r0, #0
    pophi {pc}
    
    // answer == 0 -> success, return the framebuffer-infos address
    ldr r0,=framebufferInfo
    pop {pc}

.section .data
.align 4
.globl framebufferInfo
framebufferInfo:
    .int 720  // 0x00 framebuffer width
    .int 480  // 0x04 framebuffer height
    .int 720  // 0x08 virtual framebuffer width
    .int 480  // 0x0c virtual framebuffer height
    .int 0    // 0x10 GPU pitch
    .int 16   // 0x14 Color depth
    .int 0    // 0x18 X
    .int 0    // 0x1c Y
    .int 0    // 0x20 GPU Framebuffer ptr
    .int 0    // 0x24 GPU Framebuffer Size

