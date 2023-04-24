// osasm.S
// Brandon Zupan

// Gets the current execution level
.globl get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret

// ARGS:
// x0: Address of previous RunPt
// x1: Address of next RunPt
.globl __SwitchTask
__SwitchTask:
    // Save registers onto the stack
    // push {x19-x29, x9, x30}     // x30 is PC

    // Save registers in TCB. They are at beginning of it
    mov x8, x0  // x8 has pointer to current TCB
    mov x9, sp
    stp x19, x20, [x8], #16        // STP stores pair of regs
    stp x21, x22, [x8], #16
    stp x23, x24, [x8], #16
    stp x25, x26, [x8], #16
    stp x27, x28, [x8], #16
    stp x29, x9, [x8], #16
    str x30, [x8]       // Link Register stored with PC label

    // Restore registers on TCB
    mov x8, x1
    ldp x19, x20, [x8], #16
    ldp x21, x22, [x8], #16
    ldp x23, x24, [x8], #16
    ldp x25, x26, [x8], #16
    ldp x27, x28, [x8], #16
    ldp x29, x9, [x8], #16
    ldr x30, [x8]       // Link Register stored with PC label
    mov sp, x9

    // msr    daifclr, #2  // enable interrupts

    ret     // Jump to new LR

// x0: RunPt
.globl __StartOS
__StartOS:
    // Run whatever is at RunPt
    mov x8, x0
    ldp x19, x20, [x8], #16
    ldp x21, x22, [x8], #16
    ldp x23, x24, [x8], #16
    ldp x25, x26, [x8], #16
    ldp x27, x28, [x8], #16
    ldp x29, x9, [x8], #16
    ldr x30, [x8]       // Link Register stored with PC label
    mov sp, x9

    eret     // Jump to new LR

// From https://github.com/s-matyukevich/raspberry-pi-os/blob/master/src/lesson04/src/entry.S#L146
.globl ret_from_fork
ret_from_fork:
	// bl	schedule_tail
    mov x0, x19
    bl Debug_ReturnFromFormMsg
	mov	x0, x20
	blr	x19 		//should never return
