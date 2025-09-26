#ifndef INSTR_H
#define INSTR_H

#include <stdint.h>

// --- Your new combined instruction ---
// This version uses the standard GNU assembler '.insn' directive
// to correctly assemble the custom instruction from its component parts.
static inline uint32_t hard_fpconv_stoch(uint32_t input) {
    uint32_t result;
    asm volatile (
        // Format: .insn r opcode, funct3, funct7, rd, rs1, rs2
        // We define an R-type instruction with our custom fields.
        // rs2 is unused, so we pass the 'zero' register.
        ".insn r 0x0F, 0x01, 0x01, %0, %1, zero"
        
        : "=r"(result)  // Output operand %0 (rd)
        : "r"(input)    // Input operand %1 (rs1)
    );
    return result;
}

#endif // INSTR_H
