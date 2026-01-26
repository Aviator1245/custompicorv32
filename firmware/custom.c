#include "firmware.h"
#include "instr.h"

// Example list of weights
float weights[] = { 12.5f, 11.589f, 10.5f, 23.13f };

// Scaled weights (scale = 7.0 / 23.13 ≈ 0.302637)
// [12.5, 11.589, 10.5, 23.13] -> [3.78296, 3.50699, 3.17769, 7.0]
// hence the expected output will be randomly rounding first element 3.78 which is ~77% up and ~23%down 

int len = sizeof(weights) / sizeof(weights[0]);

void custom(void)
{
    int iterations = 5000;  // number of stochastic rounding trials per weight

    // Find max value for scaling
    float res = 0;
    for (int i = 0; i < len; i++) {
        if (weights[i] > res)
            res = weights[i];
    }
    float scale = 7.0f / res;  // scale to max 7

    union { float f; uint32_t u; } conv;

    for (int j = 0; j < len; j++) {
        float scaled_val = scale * weights[j];
        int floor_val = (int)scaled_val;
        int ceil_val  = floor_val + 1;
        if (ceil_val > 7) ceil_val = 7;

        int count_down = 0;
        int count_up   = 0;

        conv.f = scaled_val;
        uint32_t input_bits = conv.u;

        for (int k = 0; k < iterations; k++) {
            uint32_t result = hard_fpconv_stoch(input_bits);
            int int_result = (int)result;

            if (int_result == floor_val)
                count_down++;
            else if (int_result == ceil_val)
                count_up++;
        }

        // Print final percentages for this element
        print_str("Element ");
        print_dec(j);
        print_str(": ");
        print_dec((count_up * 100) / iterations);
        print_str("% round up, ");
        print_dec((count_down * 100) / iterations);
        print_str("% round down\n");
    }
}


