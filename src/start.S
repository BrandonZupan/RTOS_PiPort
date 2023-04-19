// start.S
// https://github.com/s-matyukevich/raspberry-pi-os/blob/master/src/lesson01/src/boot.S

#include "mm.h"
#include "sysregs.h"

.section ".text.boot"

.globl _start
_start:
    mrs    x0, mpidr_el1        
    and    x0, x0,#0xFF        // Check processor id
    cbz    x0, master        // Hang for all non-primary CPU
    b    proc_hang

proc_hang: 
    wfe
    b proc_hang


/**
 * Set Exception Level
 * ARM.v8 Supports 4 exception level. We want the OS to run at EL1 
 * and the user code to run at EL0. 
 * By default the processor runs at EL3 so we must change to EL1. 
 */
master:
    // Disable MMU
	ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	msr	sctlr_el1, x0		

    // Set execution state to AArch64
	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

    // EL2 is in secure mode
	ldr	x0, =SCR_VALUE
	msr	scr_el3, x0

    // Set Saved PSR so all interrupts are masked and EL1 uses own SP
	ldr	x0, =SPSR_VALUE
	msr	spsr_el3, x0

    // Set address we are going to return from
	adr	x0, el1_entry		
	msr	elr_el3, x0

	eret				

el1_entry:
	adr	x0, bss_begin
	adr	x1, bss_end
	sub	x1, x1, x0
	bl 	memzero

	mov	sp, #LOW_MEMORY
	bl	kernel_main
	b 	proc_hang		// should never come here

// Gets the current execution level
.globl get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret