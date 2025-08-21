// custom_instr.h
#ifndef CUSTOM_INSTR_H
#define CUSTOM_INSTR_H

#include <stdint.h>

static inline uint32_t hard_lfsr(uint32_t rs1) {
    uint32_t rd;
    __asm__ volatile (".insn r 0x0b, 0, 0x01, %0, %1, x0"
                      : "=r"(rd) : "r"(rs1));
    return rd;
}

static inline uint32_t hard_fpconv(uint32_t rs1, uint32_t rs2) {
    uint32_t rd;
    __asm__ volatile (".insn r 0x0F, 1, 1, %0, %1, %2"
                      : "=r"(rd) : "r"(rs1), "r"(rs2));
    return rd;
}

#endif

