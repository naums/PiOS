#include <stdint.h>

struct pios_reg_t
{
    uint32_t r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12;
    uint32_t lr,pc,sp,cpsr;
};

void pios_dump_registers ()
{   
    struct pios_reg_t r;
    pios_retrieve_registers( &r );
    printf ("Register Dump:\n\
     r0: 0x%08x, \t  r1: 0x%08x, \t  r2: 0x%08x, \n\
     r3: 0x%08x, \t  r4: 0x%08x, \t  r5: 0x%08x, \n\
     r6: 0x%08x, \t  r7: 0x%08x, \t  r8: 0x%08x, \n\
     r9: 0x%08x, \t r10: 0x%08x, \t r11: 0x%08x, \n\
    r12: 0x%08x\n\
     lr: 0x%08x, \t sp: 0x%08x, \t pc: 0x%08x, \n\
   cpsr: 0x%08x", r.r0, r.r1, r.r2, r.r3, r.r4, r.r5, r.r6, r.r7, r.r8, r.r9,
    r.r10, r.r11, r.r12, r.lr, r.sp, r.pc, r.cpsr );
}
