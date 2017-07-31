/**
 * \file mmu.h
 * \author Stefan Naumann
 * \date 30. July 2017
 * \brief gives control over the MMU of the Raspberry Pi (ARMv6). May need some 
 *        testing and fiddling for the newer Pis.
 **/

#ifndef PIOS_MMU
#define PIOS_MMU

#include <stdint.h>
#include "pios_port_config.h"

/// the base address of our MMU first level table
#define PIOS_MMU_TABLE_BASE 0x4000

/// enable force AP functionality in the MMU that generates Access Bit Faults
#define PIOS_CONTROL_FA (1<<29)
/// enable the TEX remap functionality of the MMU
#define PIOS_CONTROL_TR (1<<28)
/// decides how the E flag in the CPSR register is set, when an exception occurrs
#define PIOS_CONTROL_EE (1<<25)
/// enable VIC interface to determine interrupt vectors
#define PIOS_CONTROL_VE (1<<24)
/// disable extended page tables to be configures for the hw page translation
#define PIOS_CONTROL_XP (1<<23)
/// enable unaligned data access
#define PIOS_CONTROL_U  (1<<22)
/// enable low latency features for fast interrupts
#define PIOS_CONTROL_FI (1<<21)
/// disables setting the T-flag when PC load occurs
#define PIOS_CONTROL_L4 (1<<15)
/// enable the Round Robin Replacement in Cache
#define PIOS_CONTROL_RR (1<<14)
/// high exception valectors
#define PIOS_CONTROL_V  (1<<13)
/// enable L1 intruction cache
#define PIOS_CONTROL_I  (1<<12)
/// enable branch prediction
#define PIOS_CONTROL_Z  (1<<11)
/// enable Big endian / if 0 -> little endian
#define PIOS_CONTROL_B   (1<<7)
/// enable the L1 data cache
#define PIOS_CONTROL_C   (1<<2)
/// enable strict alignment of data (for finding alignment faults, takes priority over U)
#define PIOS_CONTROL_A   (1<<1)
/// enable the MMU
#define PIOS_CONTROL_M   (1<<0)

#define PIOS_MMU_DOMAIN_NOACCESS (0x0 << 5)
#define PIOS_MMU_DOMAIN_CLIENT   (0x1 << 5)
#define PIOS_MMU_DOMAIN_MANAGER  (0x3 << 5)
/// flag for marking a section bufferable (write-cache)
#define PIOS_MMU_BUFFER (1 << 2)
/// flag for marking a section cacheable (read-cache)
#define PIOS_MMU_CACHE (1 << 3)

void pios_mmu_scrap_table ();

/**
 * \brief initialise the MMU, but not enable the MMU yet
 **/
void pios_mmu_init ();
/**
 * \brief enable the MMU (if the flags are set accordingly)
 **/
void pios_mmu_enable ( uint32_t flags );
/**
 * \brief disable the MMU and the data and instruction caches
 **/
void pios_mmu_disable ();
/**
 * \brief invalidate the TLB and do a data synchronisation barrier
 **/
void pios_mmu_invalidate_tlb ();
/**
 * \brief set new domain definitions
 **/
void pios_mmu_domain ( uint32_t domain );
/**
 * \brief returns the current domain-settings
 **/
uint32_t pios_mmu_getDomain ();

/**
 * \brief return the value written in the first level descriptor table
 * \param[in] virtual the virtual address for the entry
 * \return the entry of the first level address conversion table for the specified virtual address
 **/
uint32_t pios_mmu_getSection ( uint32_t virtual );

/**
 * \brief creates an translation table entry (for sections of size 1 MiB)
 * \param[in] virtual the virtual address (only top 12 bits used)
 * \param[in] physical the physical address (only top 12 bits used)
 * \param[in] flags the flags for the section
 **/
uint32_t pios_mmu_section ( uint32_t virtual, uint32_t physical, uint32_t flags );
/**
 * \brief creates an translation table entry (for sections of size 1 MiB)
 * \param[in] virtual the virtual address (only top 12 bits used)
 * \param[in] physical the physical address (only top 12 bits used)
 * \param[in] flags the flags for the section
 * \param[in] secondbase the base address of the secondary table 
 **/
uint32_t pios_mmu_page ( uint32_t virtual, uint32_t physical, uint32_t flags, uint32_t secondbase );

#endif
