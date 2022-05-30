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
