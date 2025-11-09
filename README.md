```markdown
# Efficient Stochastic FP32 → INT4 Quantization on 32-bit RISC-V (f2i4.sr)

Compact implementation of a custom RISC-V instruction (`f2i4.sr`) that converts IEEE-754 FP32 → signed INT4 using hardware stochastic rounding (LFSR). Integrates into PicoRV32 via the PCPI interface and targets small FPGA flows (example: iCE40 HX8K).

# Summary
- Adds `f2i4.sr rd, rs1`: FP32 in `rs1` → signed INT4 in `rd` (lower 4 bits; sign preserved).
- Uses a small LFSR for stochastic rounding (unbiased rounding behavior over many samples).
- Implemented as a PCPI coprocessor (`pcpi_fp_conv`) that hooks into PicoRV32.
- Intended for area-constrained designs that need fast FP→low-precision conversion without a full FPU.

# Repo layout
```

/ (root)
├─ hw/                # Verilog RTL (pcpi_fp_conv, lfsr, picorv32 top, top)
├─ tb/                # Testbenches and simulation vectors
├─ sw/                # C/assembly tests and bench programs
├─ scripts/           # helper scripts: run_sim.sh, synth_ice40.sh, run_bench.sh
├─ docs/              # paper, design notes (e.g., RISCV.pdf)
└─ README.md

````

# Prerequisites
- yosys, nextpnr-ice40, icestorm (icepack, iceprog, icetime) — for iCE40 flow  
- iverilog, vvp, gtkwave — for simulation  
- riscv32 toolchain (riscv32-unknown-elf-gcc, objcopy) — for software tests  
- make, bash, python3

# Quickstart — Simulation
```bash
# from repo root
# compile + run testbench (adjust file names if your layout differs)
iverilog -g2012 -o sim.vvp tb/tb_pcpi_fp_conv.v hw/pcpi_fp_conv.v hw/lfsr32.v hw/picorv32_top.v
vvp sim.vvp          # produces sim.vcd
gtkwave sim.vcd      # inspect waveforms (optional)
````

# Quickstart — Synthesis (iCE40 HX8K example)

```bash
# synth -> place & route -> pack
TOP=top
yosys -p "read_verilog hw/*.v; synth_ice40 -top $TOP -json top.json"
nextpnr-ice40 --hx8k --package ct256 --json top.json --asc top.asc
icepack top.asc top.bin
# iceprog top.bin   # program board (if available)
```

Adjust `TOP`, package, pin constraints and clock constraints in `top.v` and nextpnr command for your board.

# Software / Usage

Assembly:

```asm
# FP32 value in x10 -> convert -> result in x11
f2i4.sr x11, x10
```

Build example (RISC-V toolchain):

```bash
riscv32-unknown-elf-gcc -march=rv32im -mabi=ilp32 -O2 -c sw/test_f2i4.c -o test_f2i4.o
riscv32-unknown-elf-ld -T sw/linker.ld test_f2i4.o -o test_f2i4.elf
riscv32-unknown-elf-objcopy -O binary test_f2i4.elf test_f2i4.bin
# load test_f2i4.bin into instruction memory for simulation or hardware
```

# Tests & Benchmarks

* `scripts/run_sim.sh` — runs simulation testbench and generates waveforms.
* `scripts/run_bench.sh` — compares cycles against software baseline and gathers rounding statistics.
  Include long-run statistical tests (N ≥ 1e5) to validate unbiased rounding.

# Limitations

* Area overhead: extra logic vs stock PicoRV32 (verify for your FPGA).
* Not a full FPU — only conversion to INT4. Use an FPU if more FP ops are required.
* LFSR is a cheap PRNG — not cryptographically secure; evaluate RNG strategy for your workload.
* Current implementation fixed to INT4; to support other widths modify scaling/saturation logic.

# License

MIT (add `LICENSE` file).

# Notes

* Verify corner cases: NaN, ±Inf, denormals, exact integers, saturation/clipping.
* Update `scripts/` paths if your repo layout differs.

```
```
