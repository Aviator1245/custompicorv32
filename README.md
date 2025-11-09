# 🧮 Custom PicoRV32: FP32→INT4 Stochastic Quantization Co-Processor

This project modifies the **PicoRV32 RISC-V core** to include a **custom PCPI co-processor** that performs **FP32 to INT4 quantization** using **stochastic rounding**.  
It’s a hardware-assisted quantization module designed to accelerate **TinyML and edge AI** workloads.

---

## 🚀 Features

- **Custom PCPI Instruction**
  - Extends the `custom-1` opcode space with a new FP32→INT4 instruction.
  - Converts 32-bit IEEE-754 floating-point numbers into signed 4-bit integers.
  - Performs stochastic rounding using an integrated LFSR RNG.

- **Special Case Handling**
  - Handles zeros, NaNs, infinities, and subnormal values.
  - Saturates INT4 results in the range [-7, 7].


---

## 🧩 Project Structure

custompicorv32/
│
├── pcpi_fp_conv.v # PCPI module for FP32→INT4 conversion
├── picorv32.v # Modified PicoRV32 core supporting the new instruction
│
├── firmware/
│ ├── lauda.c # Benchmark firmware (software vs hardware comparison)
│ ├── instr.h # Inline assembly for the custom instruction (.insn)
│ ├── print.c/.h # UART print helpers
│ ├── stats.c/.h # Benchmark utility functions
│ ├── sections.lds # Linker script
│ ├── makehex.py # Converts firmware binary to HEX format
│
├── testbench.v # Verilog testbench for simulation
├── Makefile # Build and simulation automation
└── README.md # This file


---

## ⚙️ Usage

Requires a working **RISC-V GCC toolchain** and **Icarus Verilog** (or Verilator).

```bash
git clone https://github.com/Aviator1245/custompicorv32.git
cd custompicorv32
make test

After building, the simulation runs a benchmark comparing software and hardware quantization.

