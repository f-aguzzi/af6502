#include <array>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "ctre.hpp"

using byte = std::uint8_t;
using hword = std::uint16_t;
using word = std::uint32_t;

static constexpr hword MEM_SIZE = 1024 * 64 - 1;

class Memory
{
    std::array<byte, MEM_SIZE> Mem;

    public:
    void init()
    {
        Mem.fill(0);
    }

    byte operator[](hword address) const
    {
        return Mem[address];
    }

    // Write byte to memory, absolute
    void WriteByte(hword address, byte data)
    {
        printf("Indirizzo di scrittura: %d\n", (int)address);
        Mem[address] = data;
    }

    // Read byte from memory, absolute
    byte ReadByte(hword address)
    {
        return Mem[address];
    }

    // Write program to memory
    void WriteProgram(std::vector<byte> program, hword index)
    {
        int i = index;
        for (byte b: program)
        {
            Mem[i] = b;
            i++;
        }
    }
};

struct CPU
{
    hword PC;    // program counter
    hword SP;    // stack pointer

    byte A, X, Y;   // accumulator + 2 registers

    bool C; // carry flag
    bool Z; // zero flag
    bool I; // interrupt flag
    bool D; // decimal mode flag
    bool B; // break flag
    bool V; // overflow flag
    bool N; // negative flag

    Memory memory;
    word cycles;

    CPU(word n_cycles)
    {
        cycles = n_cycles;
        memory.init();
        reset();
    }

    void reset()
    {
        // Reset PC and SP to default values
        PC = 0xFFFC;
        SP = 0x0100;

        // Reset accumulator, registers and flags
        A = X = Y = 0;
        C = Z = I = D = B = V = N = false;
    }

    // Load instruction from memory
    byte FetchInstruction()
    {
        cycles--;
        byte temp = memory[PC];
        PC++;
        return temp;
    }

    // Byte fetch, absolute
    byte ReadByte(hword address)
    {
        cycles--;
        return memory[address];
    }

    // Byte write, absolute
    void WriteByte(hword address, byte data)
    {
        cycles--;
        memory.WriteByte(address, data);
    }

    // **** Addressing modes ****

    // Immediate addressing
    byte IM()
    {
        return FetchInstruction();
    }

    // Zeropage addressing
    byte ZP()
    {
        byte address = FetchInstruction();
        return ReadByte(address);
    }

    // Zeropage, X addressing
    byte ZX()
    {
        byte address = FetchInstruction() + X;
        cycles--;
        return ReadByte(address);
    }

    // Zeropage, Y addressing
    byte ZY()
    {
        byte address = FetchInstruction() + Y;
        cycles--;
        return ReadByte(address);
    }

    // Absolute addressing
    byte AB()
    {
        byte address = FetchInstruction();
        byte address_2 = FetchInstruction();
        hword full_address = (hword)address | (hword)(address_2 << 8);
        byte temp = ReadByte(full_address);
        return temp;
    }

    // Absolute, X addressing
    byte AX()
    {
        byte address = FetchInstruction();
        byte address_2 = FetchInstruction();
        hword full_address = (hword)address | (hword)(address_2 << 8) + X;
        if ((full_address & 0xFF00) != (PC & 0xFF00))
        {
            cycles--;
        }
        return ReadByte(full_address);
    }

    // Absolute, Y addressing
    byte AY()
    {
        byte address = FetchInstruction();
        byte address_2 = FetchInstruction();
        hword full_address = (hword)address | (hword)(address_2 << 8) + Y;
        if ((full_address & 0xFF00) != (PC & 0xFF00))
        {
            cycles--;
        }
        return ReadByte(full_address);
    }

    // (Indirect, X) addressing
    byte IX()
    {
        byte imm_address = FetchInstruction() + X;
        cycles--;
        byte address = ReadByte(imm_address);
        byte address_2 = ReadByte(++imm_address);
        hword full_address = (((hword)address_2 << 8) | (hword)address);
        return ReadByte(full_address);
    }

    // (Indirect), Y addressing
    byte IY()
    {
        byte imm_address = FetchInstruction();
        byte address = ReadByte(imm_address);
        byte address_2 = ReadByte(++imm_address);
        hword full_address = (((hword)address_2 << 8) | (hword)address) + Y;
        if ((full_address & 0xFF00) != (PC & 0xFF00))
        {
            cycles--;
        }
        return ReadByte(full_address);
    }

    // **** Write addressing ****

    // Zeropage address
    byte ZP_A()
    {
        return FetchInstruction();
    }

    // Zeropage, X address
    byte ZX_A()
    {
        cycles--;
        return FetchInstruction() + X;
    }

    // Zeropage, Y address
    byte ZY_A()
    {
        cycles--;
        return FetchInstruction() + Y;
    }

    // Absolute address
    hword AB_A()
    {
        byte address = FetchInstruction();
        byte address_2 = FetchInstruction();
        hword temp = (hword)address | (hword)(address_2 << 8);
        return temp;
    }

    // Absolute, X address
    hword AX_A()
    {
        byte address = FetchInstruction();
        byte address_2 = FetchInstruction();
        return (((hword)address_2 << 8) | (hword)address) + X;
    }

    // Absolute, Y address
    hword AY_A()
    {
        byte address = FetchInstruction();
        byte address_2 = FetchInstruction();
        return (((hword)address_2 << 8) | (hword)address) + Y;
    }

    // (Indirect, X) address
    hword IX_A()
    {
        byte imm_address = FetchInstruction() + X;
        cycles--;
        byte address = ReadByte(imm_address);
        byte address_2 = ReadByte(++imm_address);
        return (((hword)address_2 << 8) | (hword)address);
    }

    // (Indirect), Y address
    hword IY_A()
    {
        byte imm_address = FetchInstruction();
        byte address = ReadByte(imm_address);
        byte address_2 = ReadByte(++imm_address);
        cycles--;
        return ((((hword)address_2 << 8) | (hword)address)) + Y;
    }

    // **** Generic Operations ****

    // Generic ADC operation
    void ADC(byte operand)
    {
        hword temp = (hword)operand + (hword)A;
        if (temp > 255)
        {
            C = 1;
        }
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        V = (~((hword)A ^ (hword)operand) & ((hword)A ^ (hword)temp) & 0x0080);
        A = temp & 0x00FF;
    }
    
    // Generic AND operation
    void AND(byte operand)
    {
        A = A & operand;
        if ((A & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = A & 0x80;
    }

    // Generic ASL operation
    void ASL(byte operand)
    {
        hword temp = (hword)operand << 1;
        if (temp > 255)
        {
            C = 1;
        }
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        A = temp & 0x00FF;
    }

    // Generic branch operation
    void Branch(byte address, bool condition)
    {
        word page = PC % 256;
        if (condition)
        {
            PC += address;
        }
        if (PC % 256 > page)
        {
            cycles -= 2;
        }
    }

    // Generic BIT operation
    void BIT(byte operand)
    {
        cycles -= 2;
        N = operand & 0x2;
        V = operand & 0x3;
        Z = operand && A;
    }

    // Generic compare operation
    void Compare(byte &reg, byte operand)
    {
        hword temp = (hword)reg - (hword)operand;
        if (temp < 0)
        {
            Z = 0;
            C = 0;
            N = (temp >> 7) & 0x1;
        }
        if (temp == 0)
        {
            Z = 1;
            C = 1;
            N = 0;
        }
        if (temp > 0)
        {
            Z = 0;
            C = 1;
        }
    }

    // Generic CMP operation
    void CMP(byte operand)
    {
        Compare(A, operand);
    }

    // Generic CPX operation
    void CPX(byte operand)
    {
        Compare(X, operand);
    }

    // Generic CPY operation
    void CPY(byte operand)
    {
        Compare(Y, operand);
    }

    // Generic DEC operation
    void DEC(byte operand, hword address)
    {
        hword temp = (hword)operand--;
        cycles--;
        if (temp > 255)
        {
            C = 1;
        }
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        cycles--;
        WriteByte(address, temp & 0x00FF);
    }

    // Generic register decrement
    void Decrement(byte &reg)
    {
        reg--;
        if ((reg & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = reg & 0x80;
        cycles--;
    }

    // Generic EOR operation
    void EOR(byte operand)
    {
        byte temp = A ^ operand;
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        A = temp;
    }

    // Generic INC operation
    void INC(byte operand, hword address)
    {
        hword temp = (hword)operand + 1;
        cycles--;
        if (temp > 255)
        {
            C = 1;
        }
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        cycles--;
        WriteByte(address, temp & 0x00FF);
    }

    // Generic register increment
    void Increment(byte &reg)
    {
        reg++;
        if ((reg & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = reg & 0x80;
        cycles--;
    }

    // Generic LD(X-Y) operation
    void LD(byte &reg, byte operand)
    {
        reg = operand;
    }

    // Generic LSR operation
    byte LSR(byte operand)
    {
        hword temp = (hword)operand << 1;
        if (temp > 255)
        {
            C = 1;
        }
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        cycles--;
        return temp & 0xFF;
    }

    // Generic ORA operation
    void ORA(byte operand)
    {
        A = A | operand;
        if ((A & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = A & 0x80;
    }

    // Generic ROL operation
    byte ROL(byte operand)
    {
        hword temp = operand << 1;
        if (temp > 255)
        {
            C = 1;
        }
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        cycles--;
        return (byte)(temp | C);
    }

    // Generic ROR operation
    byte ROR(byte operand)
    {
        C = operand & 1;
        byte temp = operand >> 1;
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        cycles--;
        return (byte)(temp | (C << 7));
    }

    // Generic SBC operation
    void SBC(byte operand)
    {
        ADC(~operand + 1);
    }

    // Generic STA operation
    void STA(hword address)
    {
        WriteByte(address, A);
    }

    // Generic STX operation
    void STX(hword address)
    {
        WriteByte(address, X);
    }

    // Generic STY operation
    void STY(hword address)
    {
        WriteByte(address, Y);
    }

    // Generic DCP operation
    void DCP(hword address) //
    {
        byte operand = ReadByte(address);
        hword temp = (hword)operand - 1;
        if (temp > 255)
        {
            C = 1;
        }
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        V = (~((hword)A ^ (hword)operand) & ((hword)A ^ (hword)temp) & 0x0080);
        operand = temp;
        Compare(A, operand);
        WriteByte(address, operand);
        cycles-=2;
    }

    // Generic ISC operation
    void ISC(hword address) //
    {
        byte operand = ReadByte(address);
        hword temp = (hword)operand + 1;
        A = A - operand - !C;
        if (temp > 255)
        {
            C = 1;
        }
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        V = (~((hword)A ^ (hword)operand) & ((hword)A ^ (hword)temp) & 0x0080);
        operand = temp;
        Compare(A, operand);
        WriteByte(address, operand);
        cycles-=2;
    }

    // Generic RLA operation
    void RLA(hword address)
    {
        byte operand = ReadByte(address);
        hword temp = (hword)operand << 1;
        if (temp > 255)
        {
            C = 1;
        }
        temp = (temp & 0xFE) | C;
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        operand = (byte)temp;
        A = A & operand;
        WriteByte(address, operand);
        cycles-=2;
    }

    // Generic RRA operation
    void RRA(hword address)
    {
        byte operand = ReadByte(address);
        C = operand & 0x01;
        byte temp = operand >> 1;
        temp = (temp & 0x7F) & (C << 7);
        A = A + operand +  C;
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        V = (~((hword)A ^ (hword)operand) & ((hword)A ^ (hword)temp) & 0x0080);
        WriteByte(address, operand);
        cycles-=2;
    }

    // Generic SAX operation
    inline void SAX(hword address)
    {
        WriteByte(address, A & X);
    }

    // Generic SLO operation
    void SLO(hword address)
    {
        byte operand = ReadByte(address);
        hword temp = (hword)operand << 1;
        temp &= 0xFFFE;
        C = (temp >> 7) & 0x0001;
        if ((temp & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = temp & 0x80;
        cycles--;
        byte result = temp;
        WriteByte(address, result);
    }

    // Generic SRE operation
    void SRE(hword address)
    {
        byte operand = ReadByte(address);
        C = operand & 0x01;
        operand >>= 1;
        operand &= 0x7F;
        if ((operand & 0x00ff) == 0)
        {
            Z = 1;
        }
        N = operand & 0x80;
        cycles--;
        WriteByte(address, operand);
        A ^= operand;
    }

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

    void execute(hword init_addr)
    {
        PC = init_addr;
        while (cycles > 0)
        {
            byte instruction = FetchInstruction();

            switch (instruction)
            {
                case ADC_IM:
                {
                    byte operand = IM();
                    ADC(operand);
                } break;

                case ADC_ZP:
                {
                    byte operand = ZP();
                    ADC(operand);
                } break;

                case ADC_ZX:
                {
                    byte operand = ZX();
                    ADC(operand);
                } break;

                case ADC_AB:
                {
                    byte operand = AB();
                    ADC(operand);
                } break;

                case ADC_AX:
                {
                    byte operand = AX();
                    ADC(operand);
                } break;

                case ADC_AY:
                {
                    byte operand = AY();
                    ADC(operand);
                } break;

                case ADC_IX:
                { 
                    byte operand = IX();
                    ADC(operand);
                } break;

                case ADC_IY:
                {
                    byte operand = IY();
                    ADC(operand);
                } break;

                // AND

                case AND_IM:
                {
                    byte operand = IM();
                    AND(operand);
                } break;

                case AND_ZP:
                {
                    byte operand = ZP();
                    AND(operand);
                } break;

                case AND_ZX:
                {
                    byte operand = ZX();
                    AND(operand);
                } break;

                case AND_AB:
                {
                    byte operand = AB();
                    AND(operand);
                } break;

                case AND_AX:
                {
                    byte operand = AX();
                    AND(operand);
                } break;

                case AND_AY:
                {
                    byte operand = AY();
                    AND(operand);
                } break;

                case AND_IX:
                { 
                    byte operand = IX();
                    AND(operand);
                } break;

                case AND_IY:
                {
                    byte operand = IY();
                    AND(operand);
                } break;

                // ASL

                case ASL_AC:
                {
                    ASL(A);
                } break;

                case ASL_ZP:
                {
                    byte operand = ZP();
                    ASL(operand);
                } break;

                case ASL_ZX:
                {
                    byte operand = ZX();
                    ASL(operand);
                } break;

                case ASL_AB:
                {
                    byte operand = AB();
                    ASL(operand);
                } break;

                case ASL_AX:
                {
                    byte operand = AB();
                    ASL(operand);
                } break;

                // BRANCH   

                case BCC:
                {
                    byte address = IM();
                    bool condition = (C == 0);
                    Branch(address, condition);
                } break;

                case BCS:
                {
                    byte address = IM();
                    bool condition = (C == 1);
                    Branch(address, condition);
                } break;

                case BEQ:
                {
                    byte address = IM();
                    bool condition = (Z == 1);
                    Branch(address, condition);
                } break;

                case BMI:
                {
                    byte address = IM();
                    bool condition = (N == 1);
                    Branch(address, condition);
                } break;
                
                case BNE:
                {
                    byte address = IM();
                    bool condition = (Z == 0);
                    Branch(address, condition);
                } break;

                case BPL:
                {
                    byte address = IM();
                    bool condition = (N == 0);
                    Branch(address, condition);
                } break;

                case BVC:
                {
                    byte address = IM();
                    bool condition = (V == 0);
                    Branch(address, condition);
                } break;

                case BVS:
                {
                    byte address = IM();
                    bool condition = (V == 1);
                    Branch(address, condition);
                } break;

                // BIT

                case BIT_ZP:
                {
                    byte operand = ZP();
                    BIT(operand);
                } break;

                case BIT_AB:
                {
                    byte operand = AB();
                    BIT(operand);
                } break;

                // BRK

                /*
                case BRK:
                {
                    I = 1;
                    SP++;
                    cycles--;
                    WriteByte(SP++, (byte)((PC >> 8) & 0xFF));
                    cycles--;
                    WriteByte(SP++, (byte)(PC & 0xFF00));
                    cycles--;
                    byte flags =
                    ((C << 7) & 0x80)  | ((Z << 6) & 0x40) |
                    ((I << 5) & 0x20)  | ((D << 4) & 0x10) |
                    ((B << 3) & 0x08)  | ((V << 2) & 0x40) |
                    ((N << 1) & 0x02)  | ((0) & 0x0);
                    WriteByte(SP, flags);
                } break;
                */

                // Clear

                case CLC:
                {
                    cycles--;
                    C = 0;
                } break;

                case CLD:
                {
                    cycles--;
                    D = 0;
                } break;

                case CLI:
                {
                    cycles--;
                    I = 0;
                } break;

                case CLV:
                {
                    cycles--;
                    V = 0;
                } break;

                // CMP

                case CMP_IM:
                {
                    byte operand = IM();
                    CMP(operand);
                } break;

                case CMP_ZP:
                {
                    byte operand = ZP();
                    CMP(operand);
                } break;

                case CMP_ZX:
                {
                    byte operand = ZX();
                    CMP(operand);
                } break;

                case CMP_AB:
                {
                    byte operand = AB();
                    CMP(operand);
                } break;

                case CMP_AX:
                {
                    byte operand = AX();
                    CMP(operand);
                } break;

                case CMP_AY:
                {
                    byte operand = AY();
                    CMP(operand);
                } break;

                case CMP_IX:
                {
                    byte operand = IX();
                    CMP(operand);
                } break;

                case CMP_IY:
                {
                    byte operand = IY();
                    CMP(operand);
                } break;

                // CPX

                case CPX_IM:
                {
                    byte operand = IM();
                    CPX(operand);
                } break;

                case CPX_ZP:
                {
                    byte operand = ZP();
                    CPX(operand);
                } break;

                case CPX_AB:
                {
                    byte operand = AB();
                    CPX(operand);
                } break;

                // CPY  

                case CPY_IM:
                {
                    byte operand = IM();
                    CPY(operand);
                } break;

                case CPY_ZP:
                {
                    byte operand = ZP();
                    CPY(operand);
                } break;

                case CPY_AB:
                {
                    byte operand = AB();
                    CPY(operand);
                } break;

                // DEC

                case DEC_ZP:
                {
                    byte operand = ZP();
                    byte address = memory[PC];
                    DEC(operand, address);
                } break;

                case DEC_ZX:
                {
                    byte operand = ZX();
                    hword address = memory[PC] + X;
                    DEC(operand, address);
                } break;

                case DEC_AB:
                {
                    byte operand = AB();
                    byte address = memory[PC];
                    byte address_2 = memory[PC + 1];
                    hword full_address = (hword)address | (hword)(address_2 << 8);
                    DEC(operand, full_address);
                } break;

                case DEC_AX:
                {
                    byte operand = AB();
                    byte address = memory[PC];
                    byte address_2 = memory[PC + 1];
                    hword full_address = (hword)address | (hword)(address_2 << 8) + X;
                    DEC(operand, full_address);
                } break;

                // Decrement index

                case DEX:
                {
                    Decrement(X);
                } break;

                case DEY:
                {
                    Decrement(Y);
                } break;

                // EOR

                case EOR_IM:
                {
                    byte operand = IM();
                    EOR(operand);
                } break;

                case EOR_ZP:
                {
                    byte operand = ZP();
                    EOR(operand);
                } break;

                case EOR_ZX:
                {
                    byte operand = ZX();
                    EOR(operand);
                } break;

                case EOR_AB:
                {
                    byte operand = AB();
                    EOR(operand);
                } break;

                case EOR_AX:
                {
                    byte operand = AX();
                    EOR(operand);
                } break;

                case EOR_AY:
                {
                    byte operand = AY();
                    EOR(operand);
                } break;

                case EOR_IX:
                {
                    byte operand = IX();
                    EOR(operand);
                } break;

                case EOR_IY:
                {
                    byte operand = IY();
                    EOR(operand);
                } break;

                // INC

                case INC_ZP:
                {
                    byte operand = ZP();
                    byte address = memory[PC - 1];
                    INC(operand, address);
                } break;

                case INC_ZX:
                {
                    byte operand = ZX();
                    hword address = memory[PC - 1] + X;
                    INC(operand, address);
                } break;

                case INC_AB:
                {
                    byte operand = AB();
                    byte address = memory[PC - 1];
                    byte address_2 = memory[PC];
                    hword full_address = (hword)address | (hword)(address_2 << 8);
                    INC(operand, full_address);
                } break;

                case INC_AX:
                {
                    byte operand = AB();
                    byte address = memory[PC - 1];
                    byte address_2 = memory[PC];
                    hword full_address = (hword)address | (hword)(address_2 << 8) + X;
                    INC(operand, full_address);
                } break;

                // Increment index

                case INX:
                {
                    Increment(X);
                } break;

                case INY:
                {
                    Increment(Y);
                } break;

                // JMP

                case JMP_AB:
                {
                    byte address = FetchInstruction();
                    byte address_2 = FetchInstruction();
                    hword full_address = ((hword)address_2 << 8) | (hword)address;
                    PC = full_address;
                } break;

                case JMP_IN:
                {
                    byte address = FetchInstruction();
                    byte address_2 = FetchInstruction();
                    hword full_address = ((hword)address_2 << 8) | (hword)address;
                    byte jmp_byte = ReadByte(full_address);
                    full_address++;
                    byte jmp_byte_2 = ReadByte(full_address);
                    PC = ((hword)jmp_byte << 8) || (hword)jmp_byte_2;
                } break;

                // JSR

                case JSR:
                {
                    SP++;
                    cycles--;
                    byte PC_1 = (byte)((PC >> 8) & 0xFF00);
                    byte PC_2 = (byte)(PC & 0xFF);
                    WriteByte(SP, PC_1);
                    WriteByte(SP + 1, PC_2);
                    byte address = FetchInstruction();
                    byte address_2 = FetchInstruction();
                    hword full_address = ((hword)address_2 << 8) | (hword)address;
                    PC = full_address;
                } break;

                // LDA

                case LDA_IM:
                {
                    A = IM();
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_ZP:
                {
                    A = ZP();
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_ZX:
                {
                    A = ZX();
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_AB:
                {
                    A = AB();
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_AX:
                {
                    A = AX();
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_AY:
                {
                    A = AY();
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_IX:
                {
                    A = IX();
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_IY:
                {
                    A = IY();
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                // LDX

                case LDX_IM:
                {
                    byte operand = IM();
                    LD(X, operand);
                } break;

                case LDX_ZP:
                {
                    byte operand = ZP();
                    LD(X, operand);
                } break;

                case LDX_ZY:
                {
                    byte operand = ZY();
                    LD(X, operand);
                } break;

                case LDX_AB:
                {
                    byte operand = AB();
                    LD(X, operand);
                } break;

                case LDX_AY:
                {
                    byte operand = AY();
                    LD(X, operand);
                } break;

                // LDY

                case LDY_IM:
                {
                    byte operand = IM();
                    LD(Y, operand);
                } break;

                case LDY_ZP:
                {
                    byte operand = ZP();
                    LD(Y, operand);
                } break;

                case LDY_ZX:
                {
                    byte operand = ZX();
                    LD(Y, operand);
                } break;

                case LDY_AB:
                {
                    byte operand = AB();
                    LD(Y, operand);
                } break;

                case LDY_AX:
                {
                    byte operand = AX();
                    LD(Y, operand);
                } break;

                // LSR

                case LSR_AC:
                {
                    byte result = LSR(A);
                    A = result;
                } break;

                case LSR_ZP:
                {
                    byte operand = ZP();
                    byte result = LSR(operand);
                    byte address = PC - 1;
                    WriteByte(address, result);
                } break;

                case LSR_ZX:
                {
                    byte operand = ZX();
                    byte result = LSR(operand);
                    byte address = PC - 1;
                    WriteByte(address, result);
                } break;

                case LSR_AB:
                {
                    byte operand = AB();
                    byte result = LSR(operand);
                    byte address = PC - 2;
                    byte address_2 = PC - 1;
                    byte full_address = (hword)address | (hword)(address_2 << 8);
                    WriteByte(full_address, result);
                } break;

                case LSR_AX:
                {
                    byte operand = AX();
                    byte result = LSR(operand);
                    byte address = PC - 2;
                    byte address_2 = PC - 1;
                    byte full_address = ((hword)address | (hword)(address_2 << 8)) + X;
                    WriteByte(full_address, result);
                } break;

                // NOP
                
                case NOP:
                {
                    cycles--;
                } break;

                // ORA

                case ORA_IM:
                {
                    byte operand = IM();
                    ORA(operand);
                } break;

                case ORA_ZP:
                {
                    byte operand = ZP();
                    ORA(operand);
                } break;

                case ORA_ZX:
                {
                    byte operand = ZX();
                    ORA(operand);
                } break;

                case ORA_AB:
                {
                    byte operand = AB();
                    ORA(operand);
                } break;

                case ORA_AX:
                {
                    byte operand = AX();
                    ORA(operand);
                } break;

                case ORA_AY:
                {
                    byte operand = AY();
                    ORA(operand);
                } break;

                case ORA_IX:
                {
                    byte operand = IX();
                    ORA(operand);
                } break;

                case ORA_IY:
                {
                    byte operand = IY();
                    ORA(operand);
                } break;

                // PHA

                case PHA:
                {
                    SP++;
                    cycles--;
                    WriteByte(SP, A);
                }

                case PHP:
                {
                    SP++;
                    cycles--;
                    byte flags =
                    ((C << 7) & 0x80)  | ((Z << 6) & 0x40) |
                    ((I << 5) & 0x20)  | ((D << 4) & 0x10) |
                    ((B << 3) & 0x08)  | ((V << 2) & 0x40) |
                    ((N << 1) & 0x02)  | ((0) & 0x0);
                    WriteByte(SP, flags);
                } break;

                case PLA:
                {
                    byte temp = ReadByte(SP--);
                    cycles--;
                    A = temp;
                    if ((temp & 0x00ff) == 0)
                    {
                        Z = 1;
                    }
                    N = temp & 0x80;
                    cycles--;
                } break;

                case PLP:
                {
                    byte temp = ReadByte(SP--);
                    cycles--;
                    C = (temp >> 7) & 0x01;
                    Z = (temp >> 6) & 0x01;
                    I = (temp >> 5) & 0x01;
                    D = (temp >> 4) & 0x01;
                    B = (temp >> 3) & 0x01;
                    V = (temp >> 2) & 0x01;
                    N = (temp >> 1) & 0x01;
                    cycles--;
                } break;

                // ROL

                case ROL_AC:
                {
                    byte operand = A;
                    A = ROL(operand);
                } break;

                case ROL_ZP:
                {
                    byte operand = ZP();
                    byte temp = ROL(operand);
                    byte address = PC - 1;
                    WriteByte(address, temp);
                } break;

                case ROL_ZX:
                {
                    byte operand = ZX();
                    byte temp = ROL(operand);
                    byte address = PC - 1;
                    WriteByte(address, temp);
                } break;

                case ROL_AB:
                {
                    byte operand = AB();
                    byte temp = ROL(operand);
                    byte address = PC - 1;
                    WriteByte(address, temp);
                } break;

                case ROL_AX:
                {
                    byte operand = AX();
                    byte temp = ROL(operand);
                    byte address = PC - 1;
                    WriteByte(address, temp);
                } break;

                // ROR

                case ROR_AC:
                {
                    byte operand = A;
                    A = ROR(operand);
                } break;

                case ROR_ZP:
                {
                    byte operand = ZP();
                    byte temp = ROR(operand);
                    byte address = PC - 1;
                    WriteByte(address, temp);
                } break;

                case ROR_ZX:
                {
                    byte operand = ZX();
                    byte temp = ROR(operand);
                    byte address = PC - 1;
                    WriteByte(address, temp);
                } break;

                case ROR_AB:
                {
                    byte operand = AB();
                    byte temp = ROR(operand);
                    byte address = PC - 1;
                    WriteByte(address, temp);
                } break;

                case ROR_AX:
                {
                    byte operand = AX();
                    byte temp = ROR(operand);
                    byte address = PC - 1;
                    WriteByte(address, temp);
                } break;

                // RTI

                case RTI:
                {
                    byte flags = ReadByte(SP--);
                    C = (flags >> 7) & 0x01;
                    Z = (flags >> 6) & 0x01;
                    D = (flags >> 4) & 0x01;
                    V = (flags >> 2) & 0x01;
                    N = (flags >> 1) & 0x01;
                    PC = ReadByte(SP--);
                    cycles -= 3;
                } break;

                // RTI

                case RTS:
                {
                    PC = ReadByte(SP--);
                    PC++;
                    cycles -= 4;
                } break;

                // SEC

                case SEC:
                {
                    C = 1;
                    cycles--;
                } break;

                // SEC

                case SED:
                {
                    D = 1;
                    cycles--;
                } break;

                // SEI

                case SEI:
                {
                    I = 1;
                    cycles--;
                } break;

                // STA

                case STA_ZP:
                {
                    byte address = ZP_A();
                    STA(address);
                } break;

                case STA_ZX:
                {
                    byte address = ZX_A();
                    STA(address);
                } break;

                case STA_AB:
                {
                    hword address = AB_A();
                    STA(address);
                } break;

                case STA_AX:
                {
                    hword address = AX_A();
                    STA(address);
                } break;

                case STA_AY:
                {
                    hword address = AY_A();
                    STA(address);
                } break;

                case STA_IX:
                {
                    hword address = IX_A();
                    STA(address);
                } break;

                case STA_IY:
                {
                    hword address = IY_A();
                    STA(address);
                } break;

                // STX

                case STX_ZP:
                {
                    byte address = ZP();
                    STX(address);
                } break;

                case STX_ZY:
                {
                    byte address = ZY();
                    STX(address);
                } break;

                case STX_AB:
                {
                    byte address = AB();
                    STX(address);
                } break;

                // STY

                case STY_ZP:
                {
                    byte address = ZP();
                    STY(address);
                } break;

                case STY_ZX:
                {
                    byte address = ZX();
                    STY(address);
                } break;

                case STY_AB:
                {
                    byte address = AB();
                    STY(address);
                } break;

                // SBC

                case SBC_IM:
                {
                    byte operand = IM();
                    SBC(operand);
                } break;

                case SBC_ZP:
                {
                    byte operand = ZP();
                    SBC(operand);
                } break;

                case SBC_ZX:
                {
                    byte operand = ZX();
                    SBC(operand);
                } break;

                case SBC_AB:
                {
                    byte operand = AB();
                    SBC(operand);
                } break;

                case SBC_AX:
                {
                    byte operand = AX();
                    SBC(operand);
                } break;

                case SBC_AY:
                {
                    byte operand = AY();
                    SBC(operand);
                } break;

                case SBC_IX:
                {
                    byte operand = IX();
                    SBC(operand);
                } break;

                case SBC_IY:
                {
                    byte operand = IY();
                    SBC(operand);
                } break;

                // TAX

                case TAX:
                {
                    X = A;
                    if ((X & 0x00ff) == 0)
                    {
                        Z = 1;
                    }
                    N = X & 0x80;
                    cycles--;
                } break;

                // TAY

                case TAY:
                {
                    Y = A;
                    if ((Y & 0x00ff) == 0)
                    {
                        Z = 1;
                    }
                    N = Y & 0x80;
                    cycles--;
                } break;

                // TSX

                case TSX:
                {
                    X = SP;
                    if ((X & 0x00ff) == 0)
                    {
                        Z = 1;
                    }
                    N = X & 0x80;
                    cycles--;
                } break;

                // TXA

                case TXA:
                {
                    A = X;
                    if ((A & 0x00ff) == 0)
                    {
                        Z = 1;
                    }
                    N = A & 0x80;
                    cycles--;
                } break;

                // TXS

                case TXS:
                {
                    SP = X;
                    if ((SP & 0x00ff) == 0)
                    {
                        Z = 1;
                    }
                    N = SP & 0x80;
                    cycles--;
                } break;

                // TYA

                case TYA:
                {
                    A = Y;
                    if ((A & 0x00ff) == 0)
                    {
                        Z = 1;
                    }
                    N = A & 0x80;
                    cycles--;
                } break;

                // **** Illegal Opcodes ****

                // ALR

                case ALR:
                {
                    byte operand = IM();
                    operand &= A;
                    hword temp = (hword)operand << 1;
                    if (temp > 255)
                    {
                        C = 1;
                    }
                    if ((temp & 0x00ff) == 0)
                    {
                        Z = 1;
                    }
                    A = temp;
                    N = A & 0x80;
                } break;

                // ANC

                case ANC:
                case ANC2:
                {
                    byte operand = IM();
                    A &= operand;
                    C = (A & 0x80) >> 7; 
                    if (A == 0)
                    {
                        Z = 1;
                    }
                    N = A & 0x80;
                } break;

                // ANE

                case ANE:
                {
                    byte operand = IM();
                    A = (A | 0xFF) & X & operand;
                    if (A == 0)
                    {
                        Z = 1;
                    }
                    N = A & 0x80;
                } break;

                // ARR

                case ARR:
                {
                    byte operand = IM();
                    hword temp = ((hword)A & (hword)operand) + (hword)A;
                    V = (~((hword)A ^ (hword)operand) & ((hword)A ^ (hword)temp) & 0x0080);
                    if (A == 0)
                    {
                        Z = 1;
                    }
                    N = A & 0x80;
                    byte C_temp = (A & 0x80) >> 7;
                    temp &= 0xFF7F;
                    byte C_old = C << 7;
                    temp |= C_old;
                    C = C_temp;
                    A = temp;
                } break;

                // DCP

                case DCP_ZP:
                {
                    byte address = ZP_A();
                    DCP(address);
                } break;

                case DCP_ZX:
                {
                    byte address = ZX_A();
                    DCP(address);
                } break;

                case DCP_AB:
                {
                    byte address = AB_A();
                    DCP(address);
                } break;

                case DCP_AX:
                {
                    byte address = AX_A();
                    DCP(address);
                } break;

                case DCP_AY:
                {
                    byte address = AY_A();
                    DCP(address);
                } break;

                case DCP_IX:
                {
                    byte address = IX_A();
                    DCP(address);
                } break;

                case DCP_IY:
                {
                    byte address = IY_A();
                    DCP(address);
                } break;

                // ISC

                case ISC_ZP:
                {
                    byte address = ZP_A();
                    ISC(address);
                } break;

                case ISC_ZX:
                {
                    byte address = ZX_A();
                    ISC(address);
                } break;

                case ISC_AB:
                {
                    byte address = AB_A();
                    ISC(address);
                } break;

                case ISC_AX:
                {
                    byte address = AX_A();
                    ISC(address);
                } break;

                case ISC_AY:
                {
                    byte address = AY_A();
                    ISC(address);
                } break;

                case ISC_IX:
                {
                    byte address = IX_A();
                    ISC(address);
                } break;

                case ISC_IY:
                {
                    byte address = IY_A();
                    ISC(address);
                } break;

                // LAS

                case LAS:
                {
                    byte operand = AY();
                    A = operand & SP;
                    X = SP = A;
                    if (A == 0)
                    {
                        Z = 1;
                    }
                    N = A & 0x80;
                } break;

                // LAX

                case LAX_ZP:
                {
                    byte operand = ZP();
                    A = X = operand;
                } break;

                case LAX_ZY:
                {
                    byte operand = ZY();
                    A = X = operand;
                } break;

                case LAX_AB:
                {
                    byte operand = AB();
                    A = X = operand;
                } break;

                case LAX_AY:
                {
                    byte operand = AY();
                    A = X = operand;
                } break;

                case LAX_IX:
                {
                    byte operand = IX();
                    A = X = operand;
                } break;

                case LAX_IY:
                {
                    byte operand = IY();
                    A = X = operand;
                } break;

                // LXA

                case LXA:
                {
                    byte operand = IM();
                    X = A = (A | 0xFF) & operand;
                    if (A == 0)
                    {
                        Z = 1;
                    }
                    N = A & 0x80;
                } break;

                // RLA

                case RLA_ZP:
                {
                    hword address = ZP_A();
                    RLA(address);
                } break;

                case RLA_ZX:
                {
                    hword address = ZX_A();
                    RLA(address);
                } break;

                case RLA_AB:
                {
                    hword address = AB_A();
                    RLA(address);
                } break;

                case RLA_AX:
                {
                    hword address = AX_A();
                    RLA(address);
                } break;

                case RLA_AY:
                {
                    hword address = AY_A();
                    RLA(address);
                } break;

                case RLA_IX:
                {
                    hword address = IX_A();
                    RLA(address);
                } break;

                case RLA_IY:
                {
                    hword address = IY_A();
                    RLA(address);
                } break;

                // RRA

                case RRA_ZP:
                {
                    hword address = ZP_A();
                    RRA(address);
                } break;

                case RRA_ZX:
                {
                    hword address = ZX_A();
                    RRA(address);
                } break;

                case RRA_AB:
                {
                    hword address = AB_A();
                    RRA(address);
                } break;

                case RRA_AX:
                {
                    hword address = AX_A();
                    RRA(address);
                } break;

                case RRA_AY:
                {
                    hword address = AY_A();
                    RRA(address);
                } break;

                case RRA_IX:
                {
                    hword address = IX_A();
                    RRA(address);
                } break;

                case RRA_IY:
                {
                    hword address = IY_A();
                    RRA(address);
                } break;

                // SAX

                case SAX_ZP:
                {
                    hword address = ZP_A();
                    SAX(address);
                } break;

                case SAX_ZY:
                {
                    hword address = ZY_A();
                    SAX(address);
                } break;

                case SAX_AB:
                {
                    hword address = AB_A();
                    SAX(address);
                } break;

                case SAX_IX:
                {
                    hword address = IX_A();
                    SAX(address);
                } break;

                // SBX

                case SBX:
                {
                    byte operand = IM();
                    hword temp = (hword)(A & X) - operand;
                    C = (temp & 0x80) >> 7; 
                    if (temp == 0)
                    {
                        Z = 1;
                    }
                    N = temp & 0x80;
                    X = temp;
                } break;

                // SLO

                case SLO_ZP:
                {
                    hword address = ZP_A();
                    SLO(address);
                } break;

                case SLO_ZX:
                {
                    hword address = ZX_A();
                    SLO(address);
                } break;

                case SLO_AB:
                {
                    hword address = AB_A();
                    SLO(address);
                } break;

                case SLO_AX:
                {
                    hword address = AX_A();
                    SLO(address);
                } break;

                case SLO_AY:
                {
                    hword address = AY_A();
                    SLO(address);
                } break;

                case SLO_IX:
                {
                    hword address = IX_A();
                    SLO(address);
                } break;

                case SLO_IY:
                {
                    hword address = IY_A();
                    SLO(address);
                } break;

                // SRE

                case SRE_ZP:
                {
                    hword address = ZP_A();
                    SRE(address);
                } break;

                case SRE_ZX:
                {
                    hword address = ZX_A();
                    SRE(address);
                } break;

                case SRE_AB:
                {
                    hword address = AB_A();
                    SRE(address);
                } break;

                case SRE_AX:
                {
                    hword address = AX_A();
                    SRE(address);
                } break;

                case SRE_AY:
                {
                    hword address = AY_A();
                    SRE(address);
                } break;

                case SRE_IX:
                {
                    hword address = IX_A();
                    SRE(address);
                } break;

                case SRE_IY:
                {
                    hword address = IY_A();
                    SRE(address);
                } break;

                // TAS

                case TAS:
                {
                    hword address = AY_A();
                    SP = A & X;
                    byte result = SP & (((address >> 15) & 0x01) + 1);
                    cycles--;
                    WriteByte(address, result);
                } break;

                // USBC

                case USBC:
                {
                    byte operand = IM();
                    hword temp = (hword)A - (hword)operand - (hword)(!C);
                    if (temp > 255)
                    {
                        C = 1;
                    }
                    if ((temp & 0x00ff) == 0)
                    {
                        Z = 1;
                    }
                    N = temp & 0x80;
                    V = (~((hword)A ^ (hword)operand) & ((hword)A ^ (hword)temp) & 0x0080);
                    A = temp & 0x00FF;
                } break;

                // Implied, 2-cycle NOPs

                case 0x1A:
                case 0x3A:
                case 0x5A:
                case 0x7A:
                case 0xDA:
                case 0xFA:
                {
                    cycles--;
                } break;

                // Immediate, 2-cycle NOPs

                case 0x80:
                case 0x82:
                case 0x89:
                case 0xC2:
                case 0xE2:
                {
                    IM();
                } break;

                // Zeropage, 3-cycle NOPs

                case 0x04:
                case 0x44:
                case 0x64:
                {
                    ZP();
                } break;

                // Zeropage, X, 3-cycle NOPs

                case 0x14:
                case 0x34:
                case 0x54:
                case 0x74:
                case 0xD4:
                case 0xF4:
                {
                    ZX();
                } break;

                // Absolute, 4-cycle NOP
                
                case 0x0C:
                {
                    AB();
                } break;

                // Absolute, X, 4+ cycle NOPs

                case 0x1C:	
                case 0x3C:	
                case 0x5C:	
                case 0x7C:	
                case 0xDC:	
                case 0xFC:
                {
                    AX();
                } break;

                // JAM

                case 0x02:
                case 0x12:
                case 0x22:
                case 0x32:
                case 0x42:
                case 0x52:
                case 0x62:
                case 0x72:
                case 0x92:
                case 0xB2:
                case 0xD2:
                case 0xF2:
                {
                    while (true)
                    {

                    }
                } break;


                // Default

                default:
                {
                    // PC++;
                    cycles = 0;
                    break;
                }
            }
        }
    }
};

// Load 6502 binary from file
std::vector<byte> load_program(std::string path)
{
    std::ifstream fin;
    fin.open(path, std::ios::binary);
    if (fin.fail())
    {
        std::printf("Error loading file");
        exit(1);
    }
    auto file_size = std::filesystem::file_size(path);
    return std::vector<byte>((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
}

// Dump memory to file
void dump_exec(std::string path, Memory m)
{
    std::ofstream fout;
    fout.open(path, std::ios::binary);
    if (fout.fail())
    {
        std::printf("Error writing dump file");
        exit(1);
    }

    for (int i = 0; i < MEM_SIZE; i++)
    {
        fout << m[i];
    }
}

// Store the CLI arguments in here
struct ProgramFlags
{
    word cycles = MEM_SIZE;
    hword start_point = 0x100;
    std::string path = "";
    bool dumpStatus = false;
    bool dumpMem = false;
    std::string dumpPath;
};

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    ProgramFlags pf;

    // Regex patterns for CLI argument matching 
    static constexpr auto cycles_rxp = ctll::fixed_string{ "(-cycles=)(\\d*)" };
    static constexpr auto path_rxp = ctll::fixed_string{ "(-input=)(.*)" };
    static constexpr auto status_rxp = ctll::fixed_string{ "-showstatus" };
    static constexpr auto dump_rxp = ctll::fixed_string{ "(-dump=)(.*)" };
    static constexpr auto start_rxp = ctll::fixed_string{ "(-start=)(\\d*)" };

    // Match CLI arguments
    for (std::string s: args)
    {
        // Number of cycles
        if (auto m = ctre::match<cycles_rxp>(s))
        {
            pf.cycles = m.get<2>();
        }
        // 6502 binary path
        else if (auto m = ctre::match<path_rxp>(s))
        {
            pf.path = m.get<2>().to_string();
        }
        // Show processor status at end of execution
        else if (ctre::match<status_rxp>(s))
        {
            pf.dumpStatus = true;
        }
        // Dump to file
        else if (auto m = ctre::match<dump_rxp>(s))
        {
            pf.dumpMem = true;
            pf.dumpPath = m.get<2>().to_string();
        }
        // PC start point
        else if (auto m = ctre::match<start_rxp>(s))
        {
            pf.start_point = m.get<2>();
        }
    }

    // Quit if no file specified
    if (pf.path == "")
    {
        printf("No input specified. Quitting...");
        exit(1);
    }

    // Create CPU and load program
    CPU cpu(pf.cycles);
    std::vector<byte> loaded_program = load_program(pf.path);
    cpu.memory.WriteProgram(loaded_program, pf.start_point);

    // Execute
    cpu.execute(pf.start_point);

    // Output registers at end of execution (if required)
    if (pf.dumpStatus)
    {
        // Main registers
        std::printf("%s", "---- Registers: ---- \n");
        std::printf("Accumulator (A): %d \n", (int)cpu.A);
        std::printf("Index 1 (X): %d \n", (int)cpu.X);
        std::printf("Index 2 (Y): %d \n", (int)cpu.Y);
        std::printf("Program Counter (PC): %d \n", (int)cpu.PC);
        std::printf("Stack Pointer (SP): %d \n", (int)cpu.SP);

        // Status flags
        std::printf("\n%s", "---- Flags: ---- \n");
        std::printf("Negative flag (N): %d \n", (int)cpu.N);
        std::printf("Zero flag (Z): %d \n", (int)cpu.Z);
        std::printf("Break flag (B): %d \n", (int)cpu.B);
        std::printf("Carry flag (C): %d \n", (int)cpu.C);
        std::printf("Interrupt flag (I): %d \n", (int)cpu.I);
        std::printf("Decimal flag (D): %d \n", (int)cpu.D);
        std::printf("Overflow flag (V): %d \n", (int)cpu.V);
    }


    // Write memdump file (if required)
    if (pf.dumpMem)
    {
        dump_exec(pf.dumpPath, cpu.memory);
    }

    return 0;
}