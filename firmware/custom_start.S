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
    # Call stats C function if enabled
    jal ra, stats
#endif

    # Execution will return here (halt if desired)
    ebreak

    # Reserve stack memory
    .section .bss
    .balign 16
stack:
    .space 4096      # 4 KB stack
stack_top:

