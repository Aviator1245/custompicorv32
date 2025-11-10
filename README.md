# Custom PicoRV32: FP32→INT4 Stochastic Quantization Co-Processor

This project modifies the **PicoRV32 RISC-V core** to include a **custom PCPI co-processor** that performs **FP32 to INT4 quantization** using **stochastic rounding**.  
The co-processor without the PCPI handshake latency gets it done in 2 cycles, while it takes up to 50–100 cycles on an optimized software randomized rounding.  
With the PCPI overlay, you can expect 10–15 cycles for the instruction set to complete.  
The project's goal is to accelerate **TinyML and edge AI** workloads.  


---

## 🚀 Features

- **Custom PCPI Instruction**
  - Extends the `custom-1` opcode space with a new FP32→INT4 instruction.
  - Converts 32-bit IEEE-754 floating-point numbers into signed 4-bit integers.
  - Performs stochastic rounding using an integrated LFSR RNG.

- **Special Case Handling**
  - Handles zeros, NaNs, infinities, and subnormal values.
  - Saturates INT4 results in the range [-7, 8].

---

## 🧩 Project Structure

```text
custompicorv32/
│
├── pcpi_fp_conv.v          # PCPI module for FP32→INT4 conversion
├── picorv32.v              # Modified PicoRV32 core supporting the new instruction
│
├── firmware/
│   ├── custom.c             # Benchmark firmware (software vs hardware comparison)
│   ├── instr.h             # Inline assembly for the custom instruction (.insn)
│   ├── print.c/.h          # UART print helpers
│   ├── stats.c/.h          # Benchmark utility functions
│   ├── sections.lds        # Linker script
│   ├── makehex.py          # Converts firmware binary to HEX format
│
├── testbench.v             # Verilog testbench for simulation
├── Makefile                # Build and simulation automation
└── README.md               # This file
```

### ⚙️ Usage Requirements

- **RISC-V GCC Toolchain**  
  The Makefile is configured for the `riscv64-unknown-elf` toolchain.  
  If you use a different prefix (e.g. `riscv32-unknown-elf`), modify the Makefile's RISCV_GNU_TOOLCHAIN_INSTALL_PREFIX and TOOLCHAIN_PREFIX accordingly.  
  You can install or build one from the [RISC-V GNU Toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain).

- **Simulator**  
  [Icarus Verilog](http://iverilog.icarus.com/) or [Verilator](https://www.veripool.org/verilator/)

- **Python 3**  
  Required for converting firmware binaries into HEX format.


```bash
git clone https://github.com/Aviator1245/custompicorv32.git
cd custompicorv32
make test
```

After building, the simulation runs a benchmark comparing software and hardware quantization.

Custom FP32 inputs (e.g., neural network weights or random values) can be tested by modifying the initialization of `input_bits_array[]` in `firmware/custom.c` and rebuilding using: `make test`


