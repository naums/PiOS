/**
 * \file tags.s
 * \author Stefan Naumann
 * \brief implements finding and reading the tags, like commandline-parameters, etc
 * \note needs testing
 **/

.globl discoverTags

/**
 * \brief returns the base address of the tags
 **/
tagsAddr:
    ldr r0, =0x100
    mov pc, lr

/**
 * \brief discovers the tags, given in the RAM b the firmware
 **/
discoverTags:
    push {lr}
    push {r5}
    bl tagsAddr
    ldr r4, =tag_core
    mov r5, r4

    $discoverloop:
        ldr r1, [r0]
        ldrh r2, [r0, #4]
        teq r1, #0
        beq discoverexit
        teq r2, #0
        beq discoverexit 
        
        sub r2, #1
        lsl r2, #2
        add r4, r2
        str r0, [r4]
        
        lsl r1, #4
        add r0, r1
        mov r4, r5
    
        b $discoverloop
    
discoverexit:  
    pop {r5}
    pop {pc}
    
.section .data
tag_core:      .int 0
tag_mem:       .int 0
tag_videotext: .int 0
tag_ramdisk:   .int 0
tag_initrd2:   .int 0
tag_serial:    .int 0
tag_revision:  .int 0
tag_videolfb:  .int 0
tag_cmdline:   .int 0
