/*
 * C implementation that calls the custom maddsq instruction via inline assembly.
 */

#include "firmware.h"
#include <stdint.h>

void lauda(void) {
    uint32_t x5 = 5;
    uint32_t x6 = 10;
    uint32_t x7;

    // Use inline assembly to call the custom instruction.
    // "maddsq rd, rs1, rs2" is mapped to the instruction format.
    // 'r' specifies the r-type instruction format.
    // "0x0B, 0x1, 0x21" are the opcode, funct3, and funct7 fields.
    // "%[x7]" is the output register (rd), mapped to the C variable x7.
    // "%[x5]" is the first input register (rs1), mapped to C variable x5.
    // "%[x6]" is the second input register (rs2), mapped to C variable x6.
    
    // The "memory" clobber tells the compiler that this instruction
    // might read from or write to memory, preventing unwanted reordering.
    __asm__ volatile (
        ".insn r 0x0b, 0x1, 0x21, %[x7], %[x5], %[x6]"
        : [x7] "=r" (x7)        // Output: C variable x7 into register specified by compiler
        : [x5] "r" (x5),        // Input: C variable x5 into a general-purpose register
          [x6] "r" (x6)         // Input: C variable x6 into a general-purpose register
        : "memory"
    );

    // Print a clear message to the console
    print_chr('C');
    print_chr('u');
    print_chr('s');
    print_chr('t');
    print_chr('o');
    print_chr('m');
    print_chr(' ');
    print_chr('I');
    print_chr('n');
    print_chr('s');
    print_chr('t');
    print_chr('r');
    print_chr('u');
    print_chr('c');
    print_chr('t');
    print_chr('i');
    print_chr('o');
    print_chr('n');
    print_chr(' ');
    print_chr('R');
    print_chr('e');
    print_chr('s');
    print_chr('u');
    print_chr('l');
    print_chr('t');
    print_chr(':');
    print_chr(' ');

    // Print the value stored in the C variable x7
    print_dec(x7);

    // Print a newline
    print_chr('\n');
}

