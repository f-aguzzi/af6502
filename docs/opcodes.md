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

## Rotate One Bit Left (ROL)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| ROL (accumulator) | 0x2A      |
| ROL (zeropage)    | 0x26      |
| ROL (zeropage, X) | 0x36      |
| ROL (absolute)    | 0x2E      |
| ROL (absolute, X) | 0x3E      |

## Rotate One Bit Right (ROR)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| ROR (accumulator) | 0x6A      |
| ROR (zeropage)    | 0x66      |
| ROR (zeropage, X) | 0x76      |
| ROR (absolute)    | 0x6E      |
| ROR (absolute, X) | 0x7E      |

## Return from Interrupt (RTI)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| RTI       | 0x40      |

## Return from Subroutine (RTS)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| RTS       | 0x60      |

## Subtraction with carry (SBC)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| SBC (immediate)   | 0xE9      |
| SBC (zeropage)    | 0xE5      |
| SBC (zeropage, X) | 0xF5      |
| SBC (absolute)    | 0xED      |
| SBC (absolute, X) | 0xFD      |
| SBC (absolute, Y) | 0xF9      |
| SBC (indirect, X) | 0xE1      |
| SBC (indirect), Y | 0xF1      |

## Set Carry Flag (SEC)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| SEC       | 0x38      |

## Set Decimal Flag (SED)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| SED       | 0xF8      |

## Set Interrupt Disable Status (SEI)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| SEI       | 0x78      |

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

## Store X to Memory (STX)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| STX (zeropage)    | 0x86      |
| STX (zeropage, Y) | 0x96      |
| STX (absolute)    | 0x8E      |

## Store Y to Memory (STY)
| Mnemonic          | Opcode    |
|:-----------------:|:---------:|
| STY (zeropage)    | 0x84      |
| STY (zeropage, X) | 0x94      |
| STY (absolute)    | 0x8C      |

## Transfer Accumulator to X (TAX)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| TAX       | 0xAA      |

## Transfer Accumulator to Y (TAY)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| TAY       | 0xA8      |

## Transfer Stack Pointer to X (TSX)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| TSX       | 0xBA      |

## Transfer X to Accumulator (TXA)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| TXA       | 0x8A      |

## Transfer X to Stack Pointer (TXS)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| TXS       | 0x9A      |

## Transfer Y to Accumulator (TYA)
Implied addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| TYA       | 0x98      |



# Illegal opcodes


## AND + LSR (ALR/ASR)
Immediate addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| ALR       | 0x48      |

## AND + set C as ASL (ANC)
Immediate addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| ANC       | 0x0B      |

## AND + set C as ROL (ANC2)
Immediate addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| ANC2      | 0x2B      |

## * AND X + AND oper (ANE/XAA)
Immediate addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| ANE       | 0x8B      |

## AND + ROR (ARR)
Immediate addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| ARR       | 0x6B      |

## DEC + CMP (DCP/DCM)
| Mnemonic              | Opcode    |
|:---------------------:|:---------:|
| DCP (zeropage)	    | 0xC7      |
| DCP (zeropage,X)      | 0xD7      |
| DCP (absolute)        | 0xCF      |
| DCP (absolute ,X)     | 0xDF      |
| DCP (absolute,Y)      | 0xDB      |
| DCP ((indirect,X))    | 0xC3      |
| DCP ((indirect),Y)    | 0xD3      |

## INC + SBC (ISC)
| Mnemonic              | Opcode    |
|:---------------------:|:---------:|
| ISC (zeropage)	    | 0xE7      |
| ISC (zeropage,X)      | 0xF7      |
| ISC (absolute)        | 0xEF      |
| ISC (absolute ,X)     | 0xFF      |
| ISC (absolute,Y)      | 0xFB      |
| ISC ((indirect,X))    | 0xE3      |
| ISC ((indirect),Y)    | 0xF3      |

## LDA/TSX (LAS/LAR)
Absolute addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| LAS       | 0xBB      |

## LDA + LDX (LAX)
Absolute addressing.
| Mnemonic              | Opcode    |
|:---------------------:|:---------:|
| LAX (zeropage)	    | 0xA7      |
| LAX (zeropage,Y)      | 0xB7      |
| LAX (absolute)        | 0xAF      |
| LAX (absolute,Y)      | 0xBF      |
| LAX ((indirect,X))    | 0xA3      |
| LAX ((indirect),Y)    | 0xB3      |

## LAX immediate (LXA)
Immediate addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| LXA       | 0xAB      |

## ROL + AND (RLA)
| Mnemonic              | Opcode    |
|:---------------------:|:---------:|
| RLA (zeropage)	    | 0x27      |
| RLA (zeropage,X)      | 0x37      |
| RLA (absolute)        | 0x2F      |
| RLA (absolute,X)      | 0x3F      |
| RLA (absolute,Y)      | 0x3B      |
| RLA ((indirect,X))    | 0x23      |
| RLA ((indirect),Y)    | 0x33      |

## ROR + ADC (RRA)
| Mnemonic              | Opcode    |
|:---------------------:|:---------:|
| RLA (zeropage)	    | 0x67      |
| RLA (zeropage,X)      | 0x77      |
| RLA (absolute)        | 0x6F      |
| RLA (absolute,X)      | 0x7F      |
| RLA (absolute,Y)      | 0x7B      |
| RLA ((indirect,X))    | 0x63      |
| RLA ((indirect),Y)    | 0x73      |

## A & X to memory (SAX)
| Mnemonic              | Opcode    |
|:---------------------:|:---------:|
| SAX (zeropage)	    | 0x87      |
| SAX (zeropage,Y)	    | 0x97      |
| SAX (absolute)	    | 0x8F      |
| SAX ((indirect,X)	    | 0x83      |

## CMP + DEX (SBX)
Immediate addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| SBX       | 0xCB      |

## Store X AND (high-byte of addr. + 1) at address (SHA/AXH/AXA)
| Mnemonic              | Opcode    |
|:---------------------:|:---------:|
| RLA (absolute,Y)      | 0x7F      |
| RLA ((indirect,Y))    | 0x63      |

## (SHX/A11/SXA/XAS)
(Absolute, Y) addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| SBX       | 0x9E      |

## Store Y AND (high-byte of addr. + 1) at address (SHY/A11/SYA/SAY)
(Absolute, X) addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| SBX       | 0x9C      |

## ASL + ORA (SLO/ASO)
| Mnemonic              | Opcode    |
|:---------------------:|:---------:|
| SLO (zeropage)	    |   0x07    |	 
| SLO (zeropage, X)	    |   0x17    |  
| SLO (absolute)	    |   0x0F	|
| SLO (absolute, X)	    |   0x1F	|
| SLO (absolute, Y)     |   0x1B	|
| SLO ((indirect, X)    |   0x03	|
| SLO ((indirect), Y    |   0x13	|

## LSR + EOR (SRE/LSE)
| Mnemonic              | Opcode    |
|:---------------------:|:---------:|
| SRE (zeropage)	    |   0x47    |	 
| SRE (zeropage, X)	    |   0x57    |  
| SRE (absolute)	    |   0x4F	|
| SRE (absolute, X)	    |   0x5F	|
| SRE (absolute, Y)     |   0x5B	|
| SRE ((indirect, X)    |   0x43	|
| SRE ((indirect), Y    |   0x53	|

## Put A AND X in SP and stores A AND X AND (high-byte of addr. + 1) at address (TAS/XAS/SHS)
(Absolute, Y) addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| TAS       | 0x9B      |

## SBC + NOP (USBC/SBC)
Immediate addressing.
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| USBC      | 0xEB      |

## Null opcodes (NOP)

### Implied, 2-cycle NOPs
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| NOP       | 0x1A      |
| NOP       | 0x3A      |
| NOP       | 0x5A      |
| NOP       | 0x7A      |
| NOP       | 0xDA      |
| NOP       | 0xFA      |

### Immediate, 2-cycle NOPs
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| NOP       | 0x80      |
| NOP       | 0x82      |
| NOP       | 0x89      |
| NOP       | 0xC2      |
| NOP       | 0xE2      |

### Zeropage, 3-cycle NOPs
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| NOP       | 0x04      |
| NOP       | 0x44      |
| NOP       | 0x64      |

### Zeropage, X, 3-cycle NOPs
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| NOP       | 0x14      |
| NOP       | 0x34      |
| NOP       | 0x54      |
| NOP       | 0x74      |
| NOP       | 0xD4      |
| NOP       | 0xF4      |

### Absolute, 4-cycle NOP
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| NOP       | 0x0C      |

### Absolute, X, 4+ cycle NOPs
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| NOP       | 0x1C      |
| NOP       | 0x3C      |
| NOP       | 0x5C      |
| NOP       | 0x7C      |
| NOP       | 0xDC      |
| NOP       | 0xFC      |

## Block CPU (JAM)
| Mnemonic  | Opcode    |
|:---------:|:---------:|
| JAM       | 0x02      |
| JAM       | 0x12      |
| JAM       | 0x22      |
| JAM       | 0x32      |
| JAM       | 0x42      |
| JAM       | 0x52      |
| JAM       | 0x62      |
| JAM       | 0x72      |
| JAM       | 0x92      |
| JAM       | 0xB2      |
| JAM       | 0xD2      |
| JAM       | 0xF2      |