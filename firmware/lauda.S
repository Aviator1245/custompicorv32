    .section .text
    .global lauda

    # ---- Macros ----
    .macro lfsr rd, rs1
        .insn r 0x0b, 0, 0x01, \rd, \rs1, x0
    .endm

    .macro fp_conv rd, rs1, rs2
        .insn r 0x0F, 1, 1, \rd, \rs1, \rs2
    .endm

    # ---- Entry point ----
    lauda:
        addi sp, sp, -36
        sw   ra, 32(sp)
        sw   s0, 28(sp)
        sw   s1, 24(sp)
        sw   s2, 20(sp)
        sw   s3, 16(sp)
        sw   s4, 12(sp)
        sw   t0, 8(sp)
        sw   t1, 4(sp)
        sw   t2, 0(sp)

        # Load FP32 value (example: 4.86f = 0x409B851F)
        li   s0, 0x40dbd70a

        # Loop counter = 10000
        li   s1, 10000

        # Counters: s2 = count_round_down, s3 = count_round_up
        li   s2, 0
        li   s3, 0

        # Expected floor/ceil
        li   s4, 6        
        li   t2, 7       

    loop_test:
        # Generate new LFSR random number
        lfsr t0, x0

    
        fp_conv t1, s0, t0   

        # If t1 == floor → count_round_down++
        beq  t1, s4, inc_down

        # If t1 == ceil → count_round_up++
        beq  t1, t2, inc_up

        j    loop_continue    # ignore other values (shouldn’t happen)

    inc_down:
        addi s2, s2, 1
        j    loop_continue

    inc_up:
        addi s3, s3, 1

    loop_continue:
        addi s1, s1, -1
        bnez s1, loop_test

        # ---- Print results ----
        la   a0, msg_down
        jal  ra, print_str
        mv   a0, s2
        jal  ra, print_dec

        la   a0, msg_val
        jal  ra, print_str
        mv   a0, s4          # floor value
        jal  ra, print_dec
        li   a0, '\n'
        jal  ra, print_chr

        la   a0, msg_up
        jal  ra, print_str
        mv   a0, s3
        jal  ra, print_dec

        la   a0, msg_val
        jal  ra, print_str
        mv   a0, t2          # ceil value
        jal  ra, print_dec
        li   a0, '\n'
        jal  ra, print_chr

        # ---- Restore and return ----
        lw   ra, 32(sp)
        lw   s0, 28(sp)
        lw   s1, 24(sp)
        lw   s2, 20(sp)
        lw   s3, 16(sp)
        lw   s4, 12(sp)
        lw   t0, 8(sp)
        lw   t1, 4(sp)
        lw   t2, 0(sp)
        addi sp, sp, 36
        ret

    .section .data
    msg_down: .asciz "CountRoundedDown = "
    msg_up:   .asciz "CountRoundedUp   = "
    msg_val:  .asciz " (value = "

