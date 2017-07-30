/**
 * \file mmu.h
 * \author Stefan Naumann
 * \date 30. July 2017
 * \brief gives control over the MMU of the Raspberry Pi (ARMv6). May need some 
 *        testing and fiddling for the newer Pis.
 **/

#include <pios/mmu.h>

/**
 * \brief initialise the MMU
 **/
void pios_mmu_init ()
{
    __asm volatile (
        "mov r1,#0\n"
        // invalidate caches
        "mcr p15,0,r1,c7,c7,0\n"
        // invalidate TLB entries
        "mcr p15,0,r1,c8,c7,0\n" 
        // data synchronisation barrier
        "mcr p15,0,r1,c7,c10,4\n"
        
        // set all domains to 0b11
        "ldr r1, =0xffffffff\n"
        "mcr p15,0,r1,c3,c0,0\n"
        
        // set the translation table base address (remember to align 16 KiB!)
        "mcr p15,0,r0,c2,c0,0\n"
    );
}

/**
 * \brief enable the MMU (if the flags are set accordingly)
 **/
void pios_mmu_enable ( uint32_t flags )
{
    __asm volatile (
        "mrc p15,0,r0,c1,c0,0\n"
        "orr r0,r0,%0\n"
        "mcr p15,0,r0,c1,c0,0\n"
    : "=r" ( flags ));
}

/**
 * \brief disable the MMU and the data and instruction caches
 **/
void pios_mmu_disable ()
{
    __asm volatile (
        "mrc p15,0,r2,c1,c0,0\n"
        "bic r2,#0x1000\n"
        "bic r2,#0x0005\n"
        "mcr p15,0,r2,c1,c0,0\n"
    );
}

/**
 * \brief invalidate the TLB and do a data synchronisation barrier
 **/
void pios_mmu_invalidate_tlb ()
{
    __asm volatile (
        "mov r0,#0\n"
        "mcr p15,0,r0,c8,c7,0\n"
        "mcr p15,0,r0,c7,c10,4\n"
    );
}

/**
 * \brief set new domain definitions
 **/
void pios_mmu_domain ( uint32_t domain )
{
    __asm volatile (
        "mov r0, #0\n"
        "mcr p15, 0, r0, c3, c0, 0\n"
    );
}

/**
 * \brief returns the current domain-settings
 **/
uint32_t pios_mmu_getDomain ()
{
    uint32_t ret;
    __asm volatile (
       "mrc p15, 0, %0, c3, c0, 0\n"
    : "=r" (ret));
    return ret;
}

/**
 * \brief creates an translation table entry (for sections of size 1 MiB)
 * \param[in] virtual the virtual address (only top 12 bits used)
 * \param[in] physical the physical address (only top 12 bits used)
 * \param[in] flags the flags for the section
 **/
uint32_t pios_mmu_section ( uint32_t virtual, uint32_t physical, uint32_t flags )
{
    uint32_t offset = virtual >> 20;
    // plus and or are the same thing here, as MMUTABLEBASE is 14 bit aligned
    uint32_t* entry = (uint32_t*) (PIOS_MMU_TABLE_BASE | (offset<<2));
    
    // mask lower 20 bits of physical address then ORR flags and 0x02 for 1 MiB
    uint32_t physval = (physical & 0xfff00000) | (flags & 0x7ffa) | 0x02; 

    *entry = physval;
    return(0);
}

/**
 * \brief creates an translation table entry (for sections of size 1 MiB)
 * \param[in] virtual the virtual address (only top 12 bits used)
 * \param[in] physical the physical address (only top 12 bits used)
 * \param[in] flags the flags for the section
 **/
uint32_t pios_mmu_page ( uint32_t virtual, uint32_t physical, uint32_t flags, uint32_t secondbase )
{
    uint32_t offset = virtual >> 20;
    // plus and or are the same thing here, as MMUTABLEBASE is 14 bit aligned
    uint32_t* entry = (uint32_t*) (PIOS_MMU_TABLE_BASE| (offset<<2));
    // mask lower 20 bits of physical address then ORR flags and 0x01 for coarse translation
    uint32_t entryval = (secondbase & 0xfffffc00) | (flags & 0xf0) | 0x01; 

    // set first level descriptor
    *entry = entryval;
    
    // mask everything except bits 19:12
    offset = (virtual >> 12) & 0xff;
    // form the second level
    uint32_t* secondLevelEntry = (uint32_t*) ((secondbase & 0xfffffc00) | (offset << 2));
    
    // form the value of the second level descriptor
    // bytes 31:12 are the page base address, flags contain B,C, AP_x = 0b11 
    // for all and the 0x02 at the end to identify the entry as small page
    uint32_t physval = (physical & 0xfffff000) | 0xff0 | (flags & 0xc) | 0x02;
    
    // set the second level descriptor
    *secondLevelEntry = physval;
    return(0);
}
