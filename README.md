This is a modified version of PicoRV32 that includes a custom FP32→INT4 stochastic quantization co-processor.
The co-processor implements a lightweight stochastic rounding mechanism using a 32-bit LFSR and integrates through the PCPI (Pico Co-Processor Interface).

It lets the core perform quantized arithmetic efficiently without a floating-point unit.

The custom instruction f2i4.sr performs:

FP32 → INT4 conversion

Stochastic rounding using a 32-bit LFSR

Saturation to range [-8, +7]

The design is implemented as a small FSM with 3 states (IDLE, CALC, DONE).
