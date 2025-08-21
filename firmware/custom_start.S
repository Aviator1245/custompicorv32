    .section .text
    .globl _start
    .globl lauda
    .globl stats

_start:
    # Set stack pointer
    lui sp, %hi(stack_top)
    addi sp, sp, %lo(stack_top)

    # Call lauda function
    jal ra, lauda

#ifdef ENABLE_STATS
	/* call stats C code */
	jal ra,stats
#endif

	/* print "DONE\n" */
	lui a0,0x10000000>>12
	addi a1,zero,'D'
	addi a2,zero,'O'
	addi a3,zero,'N'
	addi a4,zero,'E'
	addi a5,zero,'\n'
	sw a1,0(a0)
	sw a2,0(a0)
	sw a3,0(a0)
	sw a4,0(a0)
	sw a5,0(a0)

	li a0, 0x20000000
	li a1, 123456789
	sw a1,0(a0)

	/* trap */
	ebreak


    # Reserve stack memory
    .section .bss
    .balign 16
stack:
    .space 4096      # 4 KB stack
stack_top:

