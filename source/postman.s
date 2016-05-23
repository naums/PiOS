/**
 * \file postman.s
 * \author Stefan Naumann
 * \brief implements the postman-usage to send messages to the vchiq. 
 **/

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
      
