.globl framebufferAddr
.globl framebufferInit
.globl postmanWrite
.globl postmanRead

/**
 * \brief returns the base address of the postman
 **/
postmanAddress:
    ldr r0, =0x2000b880
    mov pc, lr

/**
 * \brief checks the status-register of the postman
 * \param r0 the base-address of the postman
 * \param r1 the value to be checked with (AND-Operation!)
 * \note will spin-lock until the status-register checks positively (status & r1 == 0)
 **/
postmanCheckStatus:
    ldr r3, [r0, #0x18]
    tst r3, r1
    bne postmanCheckStatus
    mov pc, lr
    
/**
 * \brief writes a message to a mailbox
 * \param r0 value to be written to the mailbox (lower 4 Bits MUST be 0!)
 * \param r1 mailbox to be written to (only lower 4 Bits set!, therefor 0-15)
 **/
postmanWrite:
    tst r0, #0x0000000f
    movne pc, lr
    cmp r1, #15
    movhi pc, lr
    
    push {lr}
    push {r5, r6}
    mov r5, r0
    bl postmanAddress
    
    // loading status register of the postman
    mov r6, r1
    ldr r1, =0x80000000
    bl postmanCheckStatus
    
    // store the value into the postbox
    orr r5, r5, r6
    str r5, [r0, #0x20]
    pop {r5, r6}
    pop {pc}
    
/**
 * \brief reads from a mailbox
 * \param r0 the mailbox you want to read from (0-15, only lower 4 Bits set)
 * \return the value read in the mailbox
 **/
postmanRead:
    cmp r0, #15
    movhi pc, lr
    
    push {lr}
    push {r5}
    mov r5, r0
    bl postmanAddress
    
postmanReadloop:
    // check postman-status register (30th bit, one behind the top bit)
    ldr r1, =0x40000000
    bl postmanCheckStatus
    
    // check if the correct mailbox is read
    mov r1, #0x0000000f
    ldr r2, [r0]
    and r3, r2, r1
    cmp r5, r3
    bne postmanReadloop
    
    // move the value into the return-register
    ldr r1, =0xfffffff0
    and r0, r2, r1
    
    pop {r5}
    pop {pc}      
      


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

