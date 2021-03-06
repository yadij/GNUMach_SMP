/* 
 * Copyright (c) 1994 The University of Utah and
 * the Computer Systems Laboratory at the University of Utah (CSL).
 * All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software is hereby
 * granted provided that (1) source code retains these copyright, permission,
 * and disclaimer notices, and (2) redistributions including binaries
 * reproduce the notices in supporting documentation, and (3) all advertising
 * materials mentioning features or use of this software display the following
 * acknowledgement: ``This product includes software developed by the
 * Computer Systems Laboratory at the University of Utah.''
 *
 * THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS
 * IS" CONDITION.  THE UNIVERSITY OF UTAH AND CSL DISCLAIM ANY LIABILITY OF
 * ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * CSL requests users of this software to return to csl-dist@cs.utah.edu any
 * improvements that they make and grant CSL redistribution rights.
 *
 *      Author: Bryan Ford, University of Utah CSL
 */
#ifndef _IMPS_APIC_
#define _IMPS_APIC_

#ifndef __ASSEMBLER__

#include <stdint.h>

typedef struct ApicReg
{
	unsigned r;	/* the actual register */
	unsigned p[3];	/* pad to the next 128-bit boundary */
} ApicReg;

typedef struct ApicIoUnit
{
	ApicReg select;
	ApicReg window;
} ApicIoUnit;


struct ioapic {
    uint8_t apic_id;
    uint32_t addr;
    uint32_t base;
};

extern int nioapic;
extern struct ioapic ioapics[16];

typedef struct ApicLocalUnit
{
    /* 0x000 */
    ApicReg reserved0;
    /* 0x010 */
    ApicReg reserved1;
    /* 0x020 */
    ApicReg apic_id;
    /* 0x030 */
    ApicReg version;
    /* 0x040 */
    ApicReg reserved4;
    /* 0x050 */
    ApicReg reserved5;
    /* 0x060 */
    ApicReg reserved6;
    /* 0x070 */
    ApicReg reserved7;
    /* 0x080 */
    ApicReg task_pri;
    /* 0x090 */
    ApicReg arbitration_pri;
    /* 0x0a0 */
    ApicReg processor_pri;
    /* 0x0b0 */
    ApicReg eoi;
    /* 0x0c0 */
    ApicReg remote;
    /* 0x0d0 */
    ApicReg logical_dest;
    /* 0x0e0 */
    ApicReg dest_format;
    /* 0x0f0 */
    ApicReg spurious_vector;
    /* 0x100 */
    ApicReg isr[8];
    /* 0x180 */
    ApicReg tmr[8];
    /* 0x200 */
    ApicReg irr[8];
    /* 0x280 */
    ApicReg error_status;
    /* 0x290 */
    ApicReg reserved28[6];
    /* 0x2f0 */
    ApicReg lvt_cmci;
    /* 0x300 */
    ApicReg icr_low;
    /* 0x310 */
    ApicReg icr_high;
    /* 0x320 */
    ApicReg lvt_timer;
    /* 0x330 */
    ApicReg lvt_thermal;
    /* 0x340 */
    ApicReg lvt_performance_monitor;
    /* 0x350 */
    ApicReg lvt_lint0;
    /* 0x360 */
    ApicReg lvt_lint1;
    /* 0x370 */
    ApicReg lvt_error;
    /* 0x380 */
    ApicReg init_count;
    /* 0x390 */
    ApicReg cur_count;
    /* 0x3a0 */
    ApicReg reserved3a;
    /* 0x3b0 */
    ApicReg reserved3b;
    /* 0x3c0 */
    ApicReg reserved3c;
    /* 0x3d0 */
    ApicReg reserved3d;
    /* 0x3e0 */
    ApicReg divider_config;
    /* 0x3f0 */
    ApicReg reserved3f;
} ApicLocalUnit;


extern volatile ApicLocalUnit* lapic;




#endif

#define APIC_IO_UNIT_ID			0x00
#define APIC_IO_VERSION			0x01
#define APIC_IO_REDIR_LOW(int_pin)	(0x10+(int_pin)*2)
#define APIC_IO_REDIR_HIGH(int_pin)	(0x11+(int_pin)*2)

/* Address at which the local unit is mapped in kernel virtual memory.
 *   Must be constant.  
 * TODO: Get real address from ACPI tables
*/

#define APIC_LOCAL_VA	lapic

#define apic_local_unit (*((volatile ApicLocalUnit*)APIC_LOCAL_VA))


/* Set or clear a bit in a 255-bit APIC mask register.
   These registers are spread through eight 32-bit registers.  */
#define APIC_SET_MASK_BIT(reg, bit) \
	((reg)[(bit) >> 5].r |= 1 << ((bit) & 0x1f))
#define APIC_CLEAR_MASK_BIT(reg, bit) \
	((reg)[(bit) >> 5].r &= ~(1 << ((bit) & 0x1f)))

#endif /*_IMPS_APIC_*/
