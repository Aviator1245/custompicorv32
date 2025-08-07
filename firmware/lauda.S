/*
 * This is the minimized version of lauda.S
 */

.section .text
.global lauda

# Custom instruction macro
.macro maddsq rd, rs1, rs2
    .insn r 0x0B, 0x1, 0x21, \rd, \rs1, \rs2
.endm

lauda:
    # Set up input values
    li x5, 10
    li x6, 5

    # Call your custom instruction
    maddsq x7, x5, x6

    # Print the "Result of custom instruction: " string
    li a0, 'R'; jal ra, print_chr
    li a0, 'e'; jal ra, print_chr
    li a0, 's'; jal ra, print_chr
    li a0, 'u'; jal ra, print_chr
    li a0, 'l'; jal ra, print_chr
    li a0, 't'; jal ra, print_chr
    li a0, ' '; jal ra, print_chr
    li a0, 'o'; jal ra, print_chr
    li a0, 'f'; jal ra, print_chr
    li a0, ' '; jal ra, print_chr
    li a0, 'c'; jal ra, print_chr
    li a0, 'u'; jal ra, print_chr
    li a0, 's'; jal ra, print_chr
    li a0, 't'; jal ra, print_chr
    li a0, 'o'; jal ra, print_chr
    li a0, 'm'; jal ra, print_chr
    li a0, ' '; jal ra, print_chr
    li a0, 'i'; jal ra, print_chr
    li a0, 'n'; jal ra, print_chr
    li a0, 's'; jal ra, print_chr
    li a0, 't'; jal ra, print_chr
    li a0, 'r'; jal ra, print_chr
    li a0, 'u'; jal ra, print_chr
    li a0, 'c'; jal ra, print_chr
    li a0, 't'; jal ra, print_chr
    li a0, 'i'; jal ra, print_chr
    li a0, 'o'; jal ra, print_chr
    li a0, 'n'; jal ra, print_chr
    li a0, ':'; jal ra, print_chr
    li a0, ' '; jal ra, print_chr
    
    # Print the result of the custom instruction
    mv a0, x7
    jal ra, print_dec

    li a0, '\n'
    jal ra, print_chr

    # Write the success code to the testbench
    li a0, 0x20000000
    li a1, 123456789
    sw a1, 0(a0)

    # Terminate the simulation
    ebreak

