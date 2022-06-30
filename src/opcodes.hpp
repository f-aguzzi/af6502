#ifndef NUMBERS_h
    #include "numbers.hpp"
    #define NUMBERS_h
#endif

// **** Opcodes ****
// ADC
static constexpr byte ADC_IM = 0x69;    // Immediate
static constexpr byte ADC_ZP = 0x65;    // Zeropage
static constexpr byte ADC_ZX = 0x75;    // Zeropage, X
static constexpr byte ADC_AB = 0x6D;    // Absolute
static constexpr byte ADC_AX = 0x7D;    // Absolute, X
static constexpr byte ADC_AY = 0x79;    // Absolute, Y
static constexpr byte ADC_IX = 0x61;    // (Indirect, X)
static constexpr byte ADC_IY = 0x71;    // (Indirect), Y
// AND
static constexpr byte AND_IM = 0x29;    // Immediate
static constexpr byte AND_ZP = 0x25;    // Zeropage
static constexpr byte AND_ZX = 0x35;    // Zeropage, X
static constexpr byte AND_AB = 0x2D;    // Absolute
static constexpr byte AND_AX = 0x3D;    // Absolute, X
static constexpr byte AND_AY = 0x39;    // Absolute, Y
static constexpr byte AND_IX = 0x21;    // (Indirect, X)
static constexpr byte AND_IY = 0x31;    // (Indirect), Y
// ASL
static constexpr byte ASL_AC = 0x0A;    // Accumulator
static constexpr byte ASL_ZP = 0x06;    // Zeropage
static constexpr byte ASL_ZX = 0x16;    // Zeropage, X
static constexpr byte ASL_AB = 0x0E;    // Absolute
static constexpr byte ASL_AX = 0x1E;    // Absolute, X
// Branches
static constexpr byte BCC = 0x90;   // Relative
static constexpr byte BCS = 0xB0;   // Relative
static constexpr byte BEQ = 0xF0;   // Relative
static constexpr byte BMI = 0x30;   // Relative
static constexpr byte BNE = 0xD0;   // Relative
static constexpr byte BPL = 0x10;   // Relative
static constexpr byte BVC = 0x50;   // Relative
static constexpr byte BVS = 0x70;   // Relative
// BIT
static constexpr byte BIT_ZP = 0x24;   // zeropage
static constexpr byte BIT_AB = 0x2C;   // absolute
// BRK
static constexpr byte BRK = 0x00;   // Implied
// Clears
static constexpr byte CLC = 0x18;   // Implied
static constexpr byte CLD = 0xD8;   // Implied
static constexpr byte CLI = 0x58;   // Implied
static constexpr byte CLV = 0xB8;   // Implied
// CMP
static constexpr byte CMP_IM = 0xC9;    // Immediate
static constexpr byte CMP_ZP = 0xC5;    // Zeropage
static constexpr byte CMP_ZX = 0xD5;    // Zeropage, X
static constexpr byte CMP_AB = 0xCD;    // Absolute
static constexpr byte CMP_AX = 0xDD;    // Absolute, X
static constexpr byte CMP_AY = 0xD9;    // Absolute, Y
static constexpr byte CMP_IX = 0xC1;    // (Indirect, X)
static constexpr byte CMP_IY = 0xD1;    // (Indirect), Y
// CPX
static constexpr byte CPX_IM = 0xE0;    // Immediate
static constexpr byte CPX_ZP = 0xE4;    // Zeropage
static constexpr byte CPX_AB = 0xEC;    // Absolute
// CPY
static constexpr byte CPY_IM = 0xC0;    // Immediate
static constexpr byte CPY_ZP = 0xC4;    // Zeropage
static constexpr byte CPY_AB = 0xCC;    // Absolute
// DEC
static constexpr byte DEC_ZP = 0xC6;    // Zeropage
static constexpr byte DEC_ZX = 0xD6;    // Zeropage, X
static constexpr byte DEC_AB = 0xCE;    // Absolute
static constexpr byte DEC_AX = 0xDE;    // Absolute, X
// Decrement index
static constexpr byte DEX = 0xCA;   // Implied
static constexpr byte DEY = 0x88;   // Implied
// EOR
static constexpr byte EOR_IM = 0x49;    // Immediate
static constexpr byte EOR_ZP = 0x45;    // Zeropage
static constexpr byte EOR_ZX = 0x55;    // Zeropage, X
static constexpr byte EOR_AB = 0x4D;    // Absolute
static constexpr byte EOR_AX = 0x5D;    // Absolute, X
static constexpr byte EOR_AY = 0x59;    // Absolute, Y
static constexpr byte EOR_IX = 0x41;    // (Indirect, X)
static constexpr byte EOR_IY = 0x51;    // (Indirect), Y
// INC
static constexpr byte INC_ZP = 0xE6;    // Zeropage
static constexpr byte INC_ZX = 0xF6;    // Zeropage, X
static constexpr byte INC_AB = 0xEE;    // Absolute
static constexpr byte INC_AX = 0xFE;    // Absolute, X
// Increment index
static constexpr byte INX = 0xE8;   // Implied
static constexpr byte INY = 0xC8;   // Implied
// JMP
static constexpr byte JMP_AB = 0x4C;    // Absolute jump
static constexpr byte JMP_IN = 0x6C;    // Indirect jump
// JSR
static constexpr byte JSR = 0x20;   // Jump, save original location
// LDA
static constexpr byte LDA_IM = 0xA9;    // Immediate
static constexpr byte LDA_ZP = 0xA5;    // Zeropage
static constexpr byte LDA_ZX = 0xB5;    // Zeropage, X
static constexpr byte LDA_AB = 0xAD;    // Absolute
static constexpr byte LDA_AX = 0xBD;    // Absolute, X
static constexpr byte LDA_AY = 0xB9;    // Absolute, Y
static constexpr byte LDA_IX = 0xA1;    // (Indirect, X)
static constexpr byte LDA_IY = 0xB1;    // (Indirect), Y
// LDX
static constexpr byte LDX_IM = 0xA2;    // Immediate
static constexpr byte LDX_ZP = 0xA6;    // Zeropage
static constexpr byte LDX_ZY = 0xB6;    // Zeropage, Y
static constexpr byte LDX_AB = 0xAE;    // Absolute
static constexpr byte LDX_AY = 0xBE;    // Absolute, Y
// LDY
static constexpr byte LDY_IM = 0xA0;    // Immediate
static constexpr byte LDY_ZP = 0xA4;    // Zeropage
static constexpr byte LDY_ZX = 0xB4;    // Zeropage, X
static constexpr byte LDY_AB = 0xAC;    // Absolute
static constexpr byte LDY_AX = 0xBC;    // Absolute, X
// LSR
static constexpr byte LSR_AC = 0x4A;    // Accumulator
static constexpr byte LSR_ZP = 0x46;    // Zeropage
static constexpr byte LSR_ZX = 0x56;    // Zeropage, X
static constexpr byte LSR_AB = 0x4E;    // Absolute
static constexpr byte LSR_AX = 0x5E;    // Absolute, X
// NOP
static constexpr byte NOP = 0xEA;   // Implied
// ORA
static constexpr byte ORA_IM = 0x09;    // Immediate
static constexpr byte ORA_ZP = 0x05;    // Zeropage
static constexpr byte ORA_ZX = 0x15;    // Zeropage, X
static constexpr byte ORA_AB = 0x0D;    // Absolute
static constexpr byte ORA_AX = 0x1D;    // Absolute, X
static constexpr byte ORA_AY = 0x19;    // Absolute, Y
static constexpr byte ORA_IX = 0x01;    // (Indirect, X)
static constexpr byte ORA_IY = 0x11;    // (Indirect), Y
// PHA
static constexpr byte PHA = 0x48;   // Implied
// PHP
static constexpr byte PHP = 0x08;   // Implied
// PLA
static constexpr byte PLA = 0x68;   // Implied
// PLP
static constexpr byte PLP = 0x28;   // Implied
// ROL
static constexpr byte ROL_AC = 0x2A;    // Accumulator
static constexpr byte ROL_ZP = 0x26;    // Zeropage
static constexpr byte ROL_ZX = 0x36;    // Zeropage, X
static constexpr byte ROL_AB = 0x2E;    // Absolute
static constexpr byte ROL_AX = 0x3E;    // Absolute, X
// ROR
static constexpr byte ROR_AC = 0x6A;    // Accumulator
static constexpr byte ROR_ZP = 0x66;    // Zeropage
static constexpr byte ROR_ZX = 0x76;    // Zeropage, X
static constexpr byte ROR_AB = 0x6E;    // Absolute
static constexpr byte ROR_AX = 0x7E;    // Absolute, X
// RTI
static constexpr byte RTI = 0x40;   // Implied
// RTS
static constexpr byte RTS = 0x60;   // Implied
// SBC
static constexpr byte SBC_IM = 0xE9;    // Immediate
static constexpr byte SBC_ZP = 0xE5;    // Zeropage
static constexpr byte SBC_ZX = 0xF5;    // Zeropage, X
static constexpr byte SBC_AB = 0xED;    // Absolute
static constexpr byte SBC_AX = 0xFD;    // Absolute, X
static constexpr byte SBC_AY = 0xF9;    // Absolute, Y
static constexpr byte SBC_IX = 0xE1;    // (Indirect, X)
static constexpr byte SBC_IY = 0xF1;    // (Indirect), Y
// SEC
static constexpr byte SEC = 0x38;   // Implied
// SED
static constexpr byte SED = 0xF8;   // Implied
// SEI
static constexpr byte SEI = 0x78;   // Implied
// STA
static constexpr byte STA_ZP = 0x85;    // Zeropage
static constexpr byte STA_ZX = 0x95;    // Zeropage, X
static constexpr byte STA_AB = 0x8D;    // Absolute
static constexpr byte STA_AX = 0x9D;    // Absolute, X
static constexpr byte STA_AY = 0x99;    // Absolute, Y
static constexpr byte STA_IX = 0x81;    // (Indirect, X)
static constexpr byte STA_IY = 0x91;    // (Indirect), Y
// STX
static constexpr byte STX_ZP = 0x86;    // Zeropage
static constexpr byte STX_ZY = 0x96;    // Zeropage, Y
static constexpr byte STX_AB = 0x8E;    // Absolute
// STY
static constexpr byte STY_ZP = 0x84;    // Zeropage
static constexpr byte STY_ZX = 0x94;    // Zeropage, X
static constexpr byte STY_AB = 0x8C;    // Absolute
// TAX
static constexpr byte TAX = 0xAA;   // Implied
// TAY
static constexpr byte TAY = 0xA8;   // Implied
// TSX
static constexpr byte TSX = 0xBA;   // Implied
// TXA
static constexpr byte TXA = 0x8A;   // Implied
// TXS
static constexpr byte TXS = 0x9A;   // Implied
// TYA
static constexpr byte TYA = 0x98;   // Implied
// **** Illegal Opcodes ****
// ALR
static constexpr byte ALR = 0x4B;   // Immediate
// ANC
static constexpr byte ANC = 0x0B;   // Immediate
static constexpr byte ANC2 = 0x2B;   // Immediate
// ANE
static constexpr byte ANE = 0x8B;   // Immediate
// ARR
static constexpr byte ARR = 0x6B;   // Immediate
// DCP
static constexpr byte DCP_ZP = 0xC7;   // Zeropage
static constexpr byte DCP_ZX = 0xD7;   // Zeropage, X
static constexpr byte DCP_AB = 0xCF;   // Absolute
static constexpr byte DCP_AX = 0xDF;   // Absolute, X
static constexpr byte DCP_AY = 0xDB;   // Absolute, Y
static constexpr byte DCP_IX = 0xC3;   // (Indirect, X)
static constexpr byte DCP_IY = 0xD3;   // (Indirect), Y
// ISC
static constexpr byte ISC_ZP = 0xE7;   // Zeropage
static constexpr byte ISC_ZX = 0xF7;   // Zeropage, X
static constexpr byte ISC_AB = 0xEF;   // Absolute
static constexpr byte ISC_AX = 0xFF;   // Absolute, X
static constexpr byte ISC_AY = 0xFB;   // Absolute, Y
static constexpr byte ISC_IX = 0xE3;   // (Indirect, X)
static constexpr byte ISC_IY = 0xF3;   // (Indirect), Y
// LAS
static constexpr byte LAS = 0xBB;   // Absolute, Y
// LAX
static constexpr byte LAX_ZP = 0xA7;   // Zeropage
static constexpr byte LAX_ZY = 0xB7;   // Zeropage, Y
static constexpr byte LAX_AB = 0xAF;   // Absolute
static constexpr byte LAX_AY = 0xBF;   // Absolute, Y
static constexpr byte LAX_IX = 0xA3;   // (Indirect, X)
static constexpr byte LAX_IY = 0xB3;   // (Indirect), Y
// LXA
static constexpr byte LXA = 0xAB;   // Immediate
// RLA
static constexpr byte RLA_ZP = 0x27;   // Zeropage
static constexpr byte RLA_ZX = 0x37;   // Zeropage, X
static constexpr byte RLA_AB = 0x2F;   // Absolute
static constexpr byte RLA_AX = 0x3F;   // Absolute, X
static constexpr byte RLA_AY = 0x3B;   // Absolute, Y
static constexpr byte RLA_IX = 0x23;   // (Indirect, X)
static constexpr byte RLA_IY = 0x33;   // (Indirect), Y
// RRA
static constexpr byte RRA_ZP = 0x67;   // Zeropage
static constexpr byte RRA_ZX = 0x77;   // Zeropage, X
static constexpr byte RRA_AB = 0x6F;   // Absolute
static constexpr byte RRA_AX = 0x7F;   // Absolute, X
static constexpr byte RRA_AY = 0x7B;   // Absolute, Y
static constexpr byte RRA_IX = 0x63;   // (Indirect, X)
static constexpr byte RRA_IY = 0x73;   // (Indirect), Y
// SAX
static constexpr byte SAX_ZP = 0x87;   // Zeropage
static constexpr byte SAX_ZY = 0x97;   // Zeropage, Y
static constexpr byte SAX_AB = 0x8F;   // Absolute
static constexpr byte SAX_IX = 0x83;   // (Indirect, X)
// SBX
static constexpr byte SBX = 0xCB;   // Immediate
// SHA
static constexpr byte SHA_AY = 0x9F;   // Absolute, Y
static constexpr byte SHA_IY = 0x93;   // (Indirect), Y
// SHX
static constexpr byte SHX = 0x9E;   // Absolute, Y
// SHY
static constexpr byte SHY = 0x9C;   // Absolute, X
// SLO
static constexpr byte SLO_ZP = 0x07;   // Zeropage
static constexpr byte SLO_ZX = 0x17;   // Zeropage, X
static constexpr byte SLO_AB = 0x0F;   // Absolute
static constexpr byte SLO_AX = 0x1F;   // Absolute, X
static constexpr byte SLO_AY = 0x1B;   // Absolute, Y
static constexpr byte SLO_IX = 0x03;   // (Indirect, X)
static constexpr byte SLO_IY = 0x13;   // (Indirect), Y
// SRE
static constexpr byte SRE_ZP = 0x47;   // Zeropage
static constexpr byte SRE_ZX = 0x57;   // Zeropage, X
static constexpr byte SRE_AB = 0x4F;   // Absolute
static constexpr byte SRE_AX = 0x5F;   // Absolute, X
static constexpr byte SRE_AY = 0x5B;   // Absolute, Y
static constexpr byte SRE_IX = 0x43;   // (Indirect, X)
static constexpr byte SRE_IY = 0x53;   // (Indirect), Y
// TAS
static constexpr byte TAS = 0x9B;   // Absolute, Y
// USBC
static constexpr byte USBC = 0xEB;   // Immediate
// NOPs: not listed here