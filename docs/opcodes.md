# Opcodes

## Load accumulator (LDA)

| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| LDA (immediate)   | 0xA9      |
| LDA (zeropage)    | 0xA5      |
| LDA (zeropage, X) | 0xB5      |
| LDA (absolute)    | 0xAD      |
| LDA (absolute, X) | 0xBD      |
| LDA (absolute, Y) | 0xB9      |

## Jump (JMP)

| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| JMP (absolute)    | 0x4C      |
| JMP (indirect)    | 0x6C      |

## Jump and save return (JSR)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| JSR (absolute)    | 0x20      |

## Addition with carry (ADC)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| ADC (immediate)   | 0x69      |
| ADC (zeropage)    | 0x65      |
| ADC (zeropage, X) | 0x75      |
| ADC (absolute)    | 0x6D      |
| ADC (absolute, X) | 0x7D      |
| ADC (absolute, Y) | 0x79      |
| ADC (indirect, X) | 0x61      |
| ADC (indirect), Y | 0x71      |

## Logical AND (AND)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| AND (immediate)   | 0x29      |
| AND (zeropage)    | 0x25      |
| AND (zeropage, X) | 0x35      |
| AND (absolute)    | 0x2D      |
| AND (absolute, X) | 0x3D      |
| AND (absolute, Y) | 0x39      |
| AND (indirect, X) | 0x21      |
| AND (indirect), Y | 0x31      |

## Arithmetical left shift (ASL)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| ASL (accumulator) | 0x0A      |
| ASL (zeropage)    | 0x06      |
| ASL (zeropage, X) | 0x16      |
| ASL (absolute)    | 0x0E      |
| ASL (absolute, X) | 0x1E      |

## Branches
All use immediate addressing.
| Mnemonic                          | Opcode    |
|:---------------------------------:|:---------:|
| BCC (branch on carry clear)       | 0x90      |
| BCS (branch on carry set)         | 0xB0      |
| BEQ (branch on result 0)          | 0xF0      |       
| BMI (branch on result minus)      | 0x30      |
| BNE (branch on result not 0)      | 0xD0      |
| BPL (branch on result plus        | 0x10      |
| BVC (branch on overflow clear)    | 0x50      |
| BVS (branch on overflow set)      | 0x70      |

## Test bits in memory with accumulator (BIT)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| BIT (zeropage)    | 0x24      |
| BIT (absolute)    | 0x2C      |

## Software interrupt - Force Break (BRK)
Uses implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| BRK       | 0x00      |

