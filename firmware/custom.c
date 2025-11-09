#include "firmware.h"
#include "instr.h"

// --- Helper Function ---
static inline uint32_t read_cycle(void) {
    uint32_t cycles;
    asm volatile ("rdcycle %0" : "=r"(cycles));
    return cycles;
}

#define NUM_CONVERSIONS 100
float weights[NUM_CONVERSIONS];
uint32_t input_bits_array[NUM_CONVERSIONS];

void lauda(void) {
    uint32_t start_cycles, end_cycles, cycle_count;

    float scale = 7.0f/23.12f ; 
    for (int i = 0; i < NUM_CONVERSIONS; i++) {
        union { float f; uint32_t u; } conv;
        conv.f = (12.5f + (i * 0.1f)) * scale;
        input_bits_array[i] = conv.u;
    }


    uint16_t rand_vals[NUM_CONVERSIONS]; 
    uint32_t lcg_state = 0xACE1BEEF;
    for (int i = 0; i < NUM_CONVERSIONS; i++) {
        lcg_state = (lcg_state * 1103515245 + 12345); 
        rand_vals[i] = (lcg_state >> 16) & 0xFFFF; 
    }

    // --- 2. TIMED SOFTWARE BENCHMARK (FAST VERSION) ---
    print_str("--- Running Optimized Software Benchmark ---\n");
    start_cycles = read_cycle();

    volatile int result_buffer; 

    for (int i = 0; i < NUM_CONVERSIONS; i++) {
        // 1. Reinterpret bits as float (this is just a move, no FP math)
        union { uint32_t u; float f; } conv;
        conv.u = input_bits_array[i];
        float input_val = conv.f;


        int floor_val = (int)input_val; // Software FP-to-INT conversion
        float frac = input_val - floor_val; // Software FP subtraction

        // 3. Convert fractional part to fixed-point (0.0-1.0 -> 0-65535)
        // This is still FP math and will be slow, but it's the best we can do in software.
        uint32_t frac_fixed = (uint32_t)(frac * 65536.0f);

        // 4. Stochastic rounding decision
        int round_up = (rand_vals[i] < frac_fixed);

        // 5. Clamp to INT4 range (-8 to 7)
        int int_result = floor_val + round_up;
        if (int_result > 7) int_result = 7;
        if (int_result < -8) int_result = -8;

        result_buffer = int_result; // Use result to prevent optimization
    }
    end_cycles = read_cycle();
    cycle_count = (end_cycles - start_cycles) / NUM_CONVERSIONS;
    print_str("Optimized Software Avg: ");
    print_dec(cycle_count);
    print_str(" cycles/conversion\n");



    // --- 3. TIMED HARDWARE BENCHMARK ---
    print_str("--- Running Hardware Benchmark ---\n");
    start_cycles = read_cycle();
    for (int i = 0; i < NUM_CONVERSIONS; i += 4) { // Unrolled loop, increment by 4
        // Load a block of 4 inputs
        uint32_t in0 = input_bits_array[i];
        uint32_t in1 = input_bits_array[i+1];
        uint32_t in2 = input_bits_array[i+2];
        uint32_t in3 = input_bits_array[i+3];

        // Process the block using the single, combined hardware instruction
        uint32_t res0 = hard_fpconv_stoch(in0);
        uint32_t res1 = hard_fpconv_stoch(in1);
        uint32_t res2 = hard_fpconv_stoch(in2);
        uint32_t res3 = hard_fpconv_stoch(in3);
    }
    end_cycles = read_cycle();
    cycle_count = (end_cycles - start_cycles) / NUM_CONVERSIONS;

    print_str("Hardware Average (Combined Instr): ");
    print_dec(cycle_count);
    print_str(" cycles/conversion\n");
}
