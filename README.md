```markdown
# Efficient Stochastic FP32 → INT4 Quantization for PicoRV32 (`f2i4.sr`)

Single-file summary: a compact PCPI coprocessor (`pcpi_fp_conv`) that implements a custom RISC-V instruction `f2i4.sr rd, rs1` to convert IEEE-754 FP32 → signed INT4 using hardware stochastic rounding (LFSR). Integrates into PicoRV32; example FPGA flow shown for iCE40 HX8K.

## Features
- Instruction: `f2i4.sr rd, rs1` — FP32 (rs1) → signed INT4 in rd (stored in low 4 bits, sign preserved).
- Hardware stochastic rounding using a small LFSR for unbiased rounding over many samples.
- Lightweight RTL (Verilog) that plugs into PicoRV32 via PCPI interface.
- Example simulation and iCE40 synthesis flow provided.

## Repo layout (expected)
```

/
├─ hw/                # Verilog RTL: pcpi_fp_conv.v, lfsr32.v, picorv32_top.v, top.v
├─ tb/                # Testbenches: tb_pcpi_fp_conv.v, sim vectors
├─ sw/                # C / assembly test programs: test_f2i4.c, test_f2i4.S, linker.ld
├─ scripts/           # run_sim.sh, synth_ice40.sh, run_bench.sh
├─ docs/              # design notes / paper (optional)
└─ README.md          # this file

````

## Prerequisites
Install these on Linux:
- yosys, nextpnr-ice40, icestorm (icepack, iceprog, icetime) — for iCE40 flow  
- iverilog, vvp, gtkwave — for simulation  
- riscv32 toolchain (`riscv32-unknown-elf-gcc`, `riscv32-unknown-elf-objcopy`) — for software tests  
- make, bash, python3

## Quickstart — Simulation
Run the behavioral testbench (adjust filenames if your repo differs):
```bash
# from repo root
iverilog -g2012 -o sim.vvp \
  tb/tb_pcpi_fp_conv.v \
  hw/pcpi_fp_conv.v \
  hw/lfsr32.v \
  hw/picorv32_top.v

vvp sim.vvp                    # runs simulation, produces sim.vcd
gtkwave sim.vcd                # inspect waveforms (optional)
````

What to check in sim:

* Correct conversion for corner cases: NaN, ±Inf, denormals, exact integers.
* Statistical rounding: run many random vectors and verify up/down counts are ~balanced.

## Quickstart — Synthesis (iCE40 HX8K example)

Minimal synthesis/place & route commands:

```bash
# set TOP to your top-level module name (top.v in repo example)
TOP=top

# synthesize with yosys -> produce JSON for nextpnr
yosys -p "read_verilog hw/*.v; synth_ice40 -top $TOP -json top.json"

# place & route for HX8K, package must match your board
nextpnr-ice40 --hx8k --package ct256 --json top.json --asc top.asc

# pack to bitfile
icepack top.asc top.bin

# program (uncomment if board connected)
# iceprog top.bin
```

Adjust pin constraints, clocks, and `TOP` to match your board and `top.v`.

## Software / Usage

Assembly example:

```asm
# assume FP32 value in x10, result will be in x11
f2i4.sr x11, x10
```

Build example (RISC-V toolchain):

```bash
riscv32-unknown-elf-gcc -march=rv32im -mabi=ilp32 -O2 -c sw/test_f2i4.c -o test_f2i4.o
riscv32-unknown-elf-ld -T sw/linker.ld test_f2i4.o -o test_f2i4.elf
riscv32-unknown-elf-objcopy -O binary test_f2i4.elf test_f2i4.bin
# load test_f2i4.bin into your simulation/hardware instruction memory
```

## Tests & Benchmarks

* `scripts/run_sim.sh` — run simulation and generate waveform / CSV outputs.
* `scripts/run_bench.sh` — compare hardware path cycles vs software baseline; generate rounding statistics.
  Recommendations:
* Run bench with N ≥ 1e5 random inputs to validate statistical unbiasedness.
* Include edge-case vector sets (NaN, Inf, denormals, max/min floats, repeated exact integers).

## Limitations

* Area overhead: adds logic compared to stock PicoRV32 — measure on your FPGA/device.
* Not a general FPU: only FP32→INT4 conversion. If you need FP arithmetic, add an FPU.
* LFSR is a simple PRNG — not cryptographically secure; for sensitive workloads use a better RNG.
* Implementation fixed to INT4 — to support other widths (INT2/INT8/etc.) modify scaling & saturation logic.

## Final checklist (do these before pushing to GitHub)

* [ ] Run full sim testbench and include sim.vcd or summary CSV in CI artifacts.
* [ ] Add `sw/test_f2i4` that verifies instruction behavior on simulator/emulator.
* [ ] Confirm top-level pin constraints and clock are correct for your board.
* [ ] Add a brief comment block in `pcpi_fp_conv.v` with instruction encoding and opcode used.
* [ ] Run the long statistical test (N ≥ 1e5) and commit the result CSV/plot.

**Be blunt:** if you push this without tests and data, you haven't finished — you only packaged potential. Run the bench, fix issues, then claim results.

```
```
