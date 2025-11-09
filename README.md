🧮 Custom PicoRV32 with FP32→INT4 Stochastic Quantization Co-Processor

This repository contains a **modified PicoRV32 RISC-V core** extended with a **PCPI (Pico Co-Processor Interface)** module for **hardware-accelerated stochastic FP32 → INT4 quantization**.  
It introduces a **custom instruction** that performs floating-point to low-bit integer conversion with **on-chip stochastic rounding** using an integrated **LFSR-based RNG** — designed for **edge AI and TinyML** workloads.



🚀 Features

- **Custom PCPI Instruction**
  - Extends the `custom-1` opcode space with a new FP32→INT4 instruction.
  - Converts 32-bit IEEE-754 floating-point numbers into signed 4-bit integers.
  - Performs stochastic rounding using an integrated LFSR RNG.

- **Hardware Stochastic Rounding**
  - Ensures unbiased quantization and reduces rounding error.
  - Built-in RNG eliminates software overhead.

- **Special Case Handling**
  - Handles zeros, NaNs, infinities, and subnormal values gracefully.
  - Saturates INT4 results in the range [-7, 7].

- **Performance Boost**
  - Hardware version runs **3.5× faster** than software quantization.
  - Software: ~54 cycles/conversion  
    Hardware: ~15 cycles/conversion

---

🧩 Project Structure

