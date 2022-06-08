# Opcodes

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

## Clear instructions
All use implied addressing.
| Mnemonic                              | Opcode    |
|:-------------------------------------:|:---------:|
| CLC (clear carry flag)                | 0x18      |
| CLD (clear decimal mode)              | 0xD8      |
| CLI (clear interrupt disable bit)     | 0x58      |
| CLV (clear overflow flag)             | 0xB8      |

## Compare memory with accumulator (CMP)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| CMP (immediate)   | 0xC9      |
| CMP (zeropage)    | 0xC5      |
| CMP (zeropage, X) | 0xD5      |
| CMP (absolute)    | 0xCD      |
| CMP (absolute, X) | 0xDD      |
| CMP (absolute, Y) | 0xD9      |
| CMP (indirect, X) | 0xC1      |
| CMP (indirect), Y | 0xD1      |

## Compare Memory and Index X (CPX)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| CPX (immediate)   | 0xE0      |
| CPX (zeropage)    | 0xE4      |
| CPX (absolute)    | 0xEC      |

## Compare Memory and Index Y (CPY)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| CPY (immediate)   | 0xC0      |
| CPY (zeropage)    | 0xC4      |
| CPY (absolute)    | 0xCC      |

## Decrement Memory by One (DEC)
 Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| DEC (zeropage)    | 0xC6      |
| DEC (zeropage, X) | 0xD6      |
| DEC (absolute)    | 0xCE      |
| DEC (absolute, X) | 0xDE      |

## Decrement Index by One
Both use implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| DEX       | 0xCA      |
| DEY       | 0x88      |

## Exclusive OR (EOR)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| EOR (immediate)   | 0x49      |
| EOR (zeropage)    | 0x45      |
| EOR (zeropage, X) | 0x55      |
| EOR (absolute)    | 0x4D      |
| EOR (absolute, X) | 0x5D      |
| EOR (absolute, Y) | 0x59      |
| EOR (indirect, X) | 0x41      |
| EOR (indirect), Y | 0x51      |

## Increment Memory by One (INC)
 Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| INC (zeropage)    | 0xE6      |
| INC (zeropage, X) | 0xF6      |
| INC (absolute)    | 0xEE      |
| INC (absolute, X) | 0xFE      |

## Increment Index
Both use implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| INX       | 0xE8      |
| INY       | 0xC8      |

## Jump (JMP)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| JMP (absolute)    | 0x4C      |
| JMP (indirect)    | 0x6C      |

## Jump and save return (JSR)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| JSR (absolute)    | 0x20      |

## Load accumulator (LDA)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| LDA (immediate)   | 0xA9      |
| LDA (zeropage)    | 0xA5      |
| LDA (zeropage, X) | 0xB5      |
| LDA (absolute)    | 0xAD      |
| LDA (absolute, X) | 0xBD      |
| LDA (absolute, Y) | 0xB9      |

## Load X (LDX)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| LDX (immediate)   | 0xA2      |
| LDX (zeropage)    | 0xA6      |
| LDX (zeropage, Y) | 0xB6      |
| LDX (absolute)    | 0xAE      |
| LDX (absolute, Y) | 0xBE      |

## Load Y (LDY)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| LDY (immediate)   | 0xA0      |
| LDY (zeropage)    | 0xA4      |
| LDY (zeropage, X) | 0xB4      |
| LDY (absolute)    | 0xAC      |
| LDY (absolute, X) | 0xBC      |

## Shift One Bit Right (LSR)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| LDY (accumulator) | 0x4A      |
| LDY (zeropage)    | 0x46      |
| LDY (zeropage, X) | 0x56      |
| LDY (absolute)    | 0x4E      |
| LDY (absolute, X) | 0x5E      |

## No operation (NOP)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| NOP       | 0xEA      |

## Logical OR (ORA)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| ORA (immediate)   | 0x09      |
| ORA (zeropage)    | 0x05      |
| ORA (zeropage, X) | 0x15      |
| ORA (absolute)    | 0x0D      |
| ORA (absolute, X) | 0x1D      |
| ORA (absolute, Y) | 0x19      |
| ORA (indirect, X) | 0x01      |
| ORA (indirect), Y | 0x11      |

## Push Accumulator on Stack (PHA)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| PHA       | 0x48      |

## Push Processor Status on Stack (PHP)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| PHP       | 0x08      |

## Pull Accumulator from Stack (PLA)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| PLA       | 0x68      |

## Pull Processor Status from Stack (PLP)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| PLP       | 0x28      |

## Store Accumulator to Memory (STA)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| STA (zeropage)    | 0x85      |
| STA (zeropage, X) | 0x95      |
| STA (absolute)    | 0x8D      |
| STA (absolute, X) | 0x9D      |
| STA (absolute, Y) | 0x99      |
| STA (indirect, X) | 0x81      |
| STA (indirect), Y | 0x91      | 