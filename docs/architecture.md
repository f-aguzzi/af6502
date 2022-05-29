# 6502 Architecture

The 6502 is a 8-bit processor.

It has three registers and a 16-bit memory addressing system.

The first 256 bytes of memory are referred to as *page zero* and can be
addressed directly with some special high-speed instructions.

The second page of memory is dedicated to the system stack.

## Registers

- **PC** (*program counter*): contains the address of the current
  instruction. 16 bits of size.
- **SP** (*stack pointer*): contains the address of the top of the stack.
  16 bits of size.
- **A**, **X**, **Y**: three general-purpose registers. 8 bits of size.
- **Flags**: a series of flags for situations and errors. All 1-bit.
    - **C**: carry flag
    - **Z**: zero flag
    - **I**: interrupt
    - **D**: decimal mode
    - **B**: break
    - **V**: overflow
    - **N**: negative

### Default values

| Register  | Default value |
|-----------|:-------------:|
| PC        | 0xFFFC        |
| SP        | 0x00FF        |
| A, X, Y   | 0             |
| Flags     | 0             |
