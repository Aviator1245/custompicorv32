.section .text
.global lauda

// Macro for the custom LFSR instruction
.macro lfsr rd, rs1
    .insn r 0x0b, 0, 0x01, \rd, \rs1, x0
.endm

lauda:
    # Save registers that we will modify (make space for s1)
    addi sp, sp, -20
    sw   ra, 16(sp)
    sw   s0, 12(sp)
    sw   s1, 8(sp)      # Save s1
    sw   t0, 4(sp)
    sw   t1, 0(sp)

    # Test 1: Generate and print a sequence of LFSR values
    lfsr s0, x0
    mv   a0, s0
    jal  ra, print_dec
    li   a0, '\n'
    jal  ra, print_chr

    # Use s1 for the loop counter
    li   s1, 12
loop_lfsr_gen:
    lfsr s0, x0
    mv   a0, s0
    jal  ra, print_dec    # s1 will now survive this call
    li   a0, '\n'
    jal  ra, print_chr
    addi s1, s1, -1
    bnez s1, loop_lfsr_gen

    # ... the rest of your code ...

    # Restore registers
    lw   ra, 16(sp)
    lw   s0, 12(sp)
    lw   s1, 8(sp)      # Restore s1
    lw   t0, 4(sp)
    lw   t1, 0(sp)
    addi sp, sp, 20
    ret
