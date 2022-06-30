#ifndef CPU_h
    #include "CPU.hpp"
    #define CPU_h
#endif

#ifndef OPCODES_h
    #include "opcodes.hpp"
    #define OPCODES_h
#endif



CPU::CPU(word n_cycles)
{
    cycles = n_cycles;
    memory.init();
    reset();
}

void CPU::reset()
{
    // Reset PC and SP to default values
    PC = 0xFFFC;
    SP = 0x0100;

    // Reset accumulator, registers and flags
    A = X = Y = 0;
    C = Z = I = D = B = V = N = false;
}

byte CPU::FetchInstruction()
{
    cycles--;
    byte temp = memory[PC];
    PC++;
    return temp;
}

byte CPU::ReadByte(hword address)
{
    cycles--;
    return memory[address];
}

void CPU::WriteByte(hword address, byte data)
{
    cycles--;
    memory.WriteByte(address, data);
}

byte CPU::IM()
{
    return FetchInstruction();
}

byte CPU::ZP()
{
    byte address = FetchInstruction();
    return ReadByte(address);
}

byte CPU::ZX()
{
    byte address = FetchInstruction() + X;
    cycles--;
    return ReadByte(address);
}

byte CPU::ZY()
{
    byte address = FetchInstruction() + Y;
    cycles--;
    return ReadByte(address);
}

byte CPU::AB()
{
    byte address = FetchInstruction();
    byte address_2 = FetchInstruction();
    hword full_address = (hword)address | (hword)(address_2 << 8);
    byte temp = ReadByte(full_address);
    return temp;
}

byte CPU::AX()
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

byte CPU::AY()
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

byte CPU::IX()
{
    byte imm_address = FetchInstruction() + X;
    cycles--;
    byte address = ReadByte(imm_address);
    byte address_2 = ReadByte(++imm_address);
    hword full_address = (((hword)address_2 << 8) | (hword)address);
    return ReadByte(full_address);
}

byte CPU::IY()
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

byte CPU::ZP_A()
{
    return FetchInstruction();
}

byte CPU::ZX_A()
{
    cycles--;
    return FetchInstruction() + X;
}

byte CPU::ZY_A()
{
    cycles--;
    return FetchInstruction() + Y;
}

hword CPU::AB_A()
{
    byte address = FetchInstruction();
    byte address_2 = FetchInstruction();
    hword temp = (hword)address | (hword)(address_2 << 8);
    return temp;
}

hword CPU::AX_A()
{
    byte address = FetchInstruction();
    byte address_2 = FetchInstruction();
    return (((hword)address_2 << 8) | (hword)address) + X;
}

hword CPU::AY_A()
{
    byte address = FetchInstruction();
    byte address_2 = FetchInstruction();
    return (((hword)address_2 << 8) | (hword)address) + Y;
}

hword CPU::IX_A()
{
    byte imm_address = FetchInstruction() + X;
    cycles--;
    byte address = ReadByte(imm_address);
    byte address_2 = ReadByte(++imm_address);
    return (((hword)address_2 << 8) | (hword)address);
}

hword CPU::IY_A()
{
    byte imm_address = FetchInstruction();
    byte address = ReadByte(imm_address);
    byte address_2 = ReadByte(++imm_address);
    cycles--;
    return ((((hword)address_2 << 8) | (hword)address)) + Y;
}

void CPU::ADC(byte operand)
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

void CPU::AND(byte operand)
{
    A = A & operand;
    if ((A & 0x00ff) == 0)
    {
        Z = 1;
    }
    N = A & 0x80;
}

void CPU::ASL(byte operand)
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

void CPU::Branch(byte address, bool condition)
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

void CPU::BIT(byte operand)
{
    cycles -= 2;
    N = operand & 0x2;
    V = operand & 0x3;
    Z = operand && A;
}

void CPU::Compare(byte &reg, byte operand)
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

void CPU::CMP(byte operand)
{
    Compare(A, operand);
}

void CPU::CPX(byte operand)
{
    Compare(X, operand);
}

void CPU::CPY(byte operand)
{
    Compare(Y, operand);
}

void CPU::DEC(byte operand, hword address)
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

void CPU::Decrement(byte &reg)
{
    reg--;
    if ((reg & 0x00ff) == 0)
    {
        Z = 1;
    }
    N = reg & 0x80;
    cycles--;
}

void CPU::EOR(byte operand)
{
    byte temp = A ^ operand;
    if ((temp & 0x00ff) == 0)
    {
        Z = 1;
    }
    N = temp & 0x80;
    A = temp;
}

void CPU::INC(byte operand, hword address)
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

void CPU::Increment(byte &reg)
{
    reg++;
    if ((reg & 0x00ff) == 0)
    {
        Z = 1;
    }
    N = reg & 0x80;
    cycles--;
}

void CPU::LD(byte &reg, byte operand)
{
    reg = operand;
}

byte CPU::LSR(byte operand)
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

void CPU::ORA(byte operand)
{
    A = A | operand;
    if ((A & 0x00ff) == 0)
    {
        Z = 1;
    }
    N = A & 0x80;
}

byte CPU::ROL(byte operand)
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

byte CPU::ROR(byte operand)
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

void CPU::SBC(byte operand)
{
    ADC(~operand + 1);
}

void CPU::STA(hword address)
{
    WriteByte(address, A);
}

void CPU::STX(hword address)
{
    WriteByte(address, X);
}

void CPU::STY(hword address)
{
    WriteByte(address, Y);
}

void CPU::DCP(hword address) //
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

void CPU::ISC(hword address) //
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

void CPU::RLA(hword address)
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

void CPU::RRA(hword address)
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

inline void CPU::SAX(hword address)
{
    WriteByte(address, A & X);
}

void CPU::SLO(hword address)
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

void CPU::SRE(hword address)
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

void CPU::execute(hword init_addr)
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