Custom PicoRV32: FP32→INT4 Stochastic Quantization Co-Processor

This project modifies the PicoRV32 RISC-V core to include a **custom PCPI co-processor** that performs **FP32 to INT4 quantization** using **stochastic rounding**.  
It’s a hardware-assisted quantization module designed to accelerate **TinyML and edge AI** workloads.


Usage

Requires a working RISC-V GCC toolchain and Icarus Verilog (or Verilator).

```bash
git clone https://github.com/<yourusername>/custompicorv32.git
cd custompicorv32
make test

