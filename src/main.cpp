using byte = unsigned char;
using hword = unsigned short;
using word = unsigned int;

struct Memory
{
    static constexpr word MEM_SIZE = 1024 * 64;
    byte Mem[MEM_SIZE];

    void init()
    {
        for (word i = 0; i < MEM_SIZE; i++)
        {
            Mem[i] = 0;
        }
    }

    byte operator[](word address) const
    {
        return Mem[address];
    }

    // Write byte to memory, absolute
    void WriteByte(word cycles, hword address, byte data)
    {
        cycles--;
        Mem[address] = data;
    }
};

struct CPU
{
    hword PC;    // program counter
    hword SP;    // stack pointer

    byte A, X, Y;   // accumulator + 2 registers

    byte C : 1; // carry flag
    byte Z : 1; // zero flag
    byte I : 1; // interrupt flag
    byte D : 1; // decimal mode flag
    byte B : 1; // break flag
    byte V : 1; // overflow flag
    byte N : 1; // negative flag

    void reset()
    {
        // Reset PC and SP to default values
        PC = 0xFFFC;
        SP = 0x00FF;

        // Reset accumulator, registers and flags
        A = X = Y = 0;
        C = Z = I = D = B = V = N = 0;
    }

    // Load instruction from memory
    byte FetchInstruction(word cycles, Memory &memory)
    {
        cycles--;
        byte temp = memory[PC];
        PC++;
        return temp;
    }

    // Byte fetch, zeropage
    byte ReadByte(word cycles, byte address, Memory &memory)
    {
        cycles--;
        return memory[address];
    }

    // Byte fetch, absolute
    byte ReadByte(word cycles, hword address, Memory &memory)
    {
        cycles--;
        return memory[address];
    }

    // **** Addressing modes ****

    // Immediate addressing
    byte IM(word cycles, Memory memory)
    {
        return FetchInstruction(cycles, memory);
    }

    // Zeropage addressing
    byte ZP(word cycles, Memory memory)
    {
        byte address = FetchInstruction(cycles, memory);
        return ReadByte(cycles, address, memory);
    }

    // Zeropage, X addressing
    byte ZX(word cycles, Memory memory)
    {
        byte address = FetchInstruction(cycles, memory) + X;
        cycles--;
        return ReadByte(cycles, address, memory);
    }

    // Absolute addressing
    byte AB(word cycles, Memory memory)
    {
        byte address = FetchInstruction(cycles, memory);
        byte address_2 = FetchInstruction(cycles, memory);
        hword full_address = (hword)address | (hword)(address_2 << 8);
        return ReadByte(cycles, full_address, memory);
    }

    // Absolute, X addressing
    byte AX(word cycles, Memory memory)
    {
        byte address = FetchInstruction(cycles, memory);
        byte address_2 = FetchInstruction(cycles, memory);
        hword full_address = (hword)address | (hword)(address_2 << 8) + X;
        if ((full_address & 0xFF00) != (PC & 0xFF00))
        {
            cycles--;
        }
        return ReadByte(cycles, full_address, memory);
    }

    // Absolute, Y addressing
    byte AY(word cycles, Memory memory)
    {
        byte address = FetchInstruction(cycles, memory);
        byte address_2 = FetchInstruction(cycles, memory);
        hword full_address = (hword)address | (hword)(address_2 << 8) + Y;
        if ((full_address & 0xFF00) != (PC & 0xFF00))
        {
            cycles--;
        }
        return ReadByte(cycles, full_address, memory);
    }

    // (Indirect, X) addressing
    byte IX(word cycles, Memory memory)
    {
        byte imm_address = FetchInstruction(cycles, memory) + X;
        cycles--;
        byte address = ReadByte(cycles, imm_address, memory);
        byte address_2 = ReadByte(cycles, ++imm_address, memory);
        hword full_address = (((hword)address_2 << 8) | (hword)address);
        return ReadByte(cycles, full_address, memory);
    }

    // (Indirect), Y addressing
    byte IY(word cycles, Memory memory)
    {
        byte imm_address = FetchInstruction(cycles, memory);
        byte address = ReadByte(cycles, imm_address, memory);
        byte address_2 = ReadByte(cycles, ++imm_address, memory);
        hword full_address = (((hword)address_2 << 8) | (hword)address) + Y;
        if ((full_address & 0xFF00) != (PC & 0xFF00))
        {
            cycles--;
        }
        return ReadByte(cycles, full_address, memory);
    }

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
    // LDA
    static constexpr byte LDA_IM = 0xA9;    // Immediate
    static constexpr byte LDA_ZP = 0xA5;    // Zeropage
    static constexpr byte LDA_ZX = 0xB5;    // Zeropage, X
    static constexpr byte LDA_AB = 0xAD;    // Absolute
    static constexpr byte LDA_AX = 0xBD;    // Absolute, X
    static constexpr byte LDA_AY = 0xB9;    // Absolute, Y
    static constexpr byte LDA_IX = 0xA1;    // (Indirect, X)
    static constexpr byte LDA_IY = 0xB1;    // (Indirect), Y
    // JMP
    static constexpr byte JMP_AB = 0x4C;    // Absolute jump
    static constexpr byte JMP_IN = 0x6C;    // Indirect jump
    // JSR
    static constexpr byte JSR = 0x20;   // Jump, save original location

    void execute(word &cycles, Memory &memory)
    {
        while (cycles > 0)
        {
            byte instruction = FetchInstruction(cycles, memory);

            switch (instruction)
            {
                case ADC_IM:
                {
                    byte operand = IM(cycles, memory);
                    ADC(operand);
                } break;

                case ADC_ZP:
                {
                    byte operand = ZP(cycles, memory);
                    ADC(operand);
                } break;

                case ADC_ZX:
                {
                    byte operand = ZX(cycles, memory);
                    ADC(operand);
                } break;

                case ADC_AB:
                {
                    byte operand = AB(cycles, memory);
                    ADC(operand);
                } break;

                case ADC_AX:
                {
                    byte operand = AX(cycles, memory);
                    ADC(operand);
                } break;

                case ADC_AY:
                {
                    byte operand = AY(cycles, memory);
                    ADC(operand);
                } break;

                case ADC_IX:
                { 
                    byte operand = IX(cycles, memory);
                    ADC(operand);
                } break;

                case ADC_IY:
                {
                    byte operand = IY(cycles, memory);
                    ADC(operand);
                } break;

                // AND

                case AND_IM:
                {
                    byte operand = IM(cycles, memory);
                    AND(operand);
                } break;

                case AND_ZP:
                {
                    byte operand = ZP(cycles, memory);
                    AND(operand);
                } break;

                case AND_ZX:
                {
                    byte operand = ZX(cycles, memory);
                    AND(operand);
                } break;

                case AND_AB:
                {
                    byte operand = AB(cycles, memory);
                    AND(operand);
                } break;

                case AND_AX:
                {
                    byte operand = AX(cycles, memory);
                    AND(operand);
                } break;

                case AND_AY:
                {
                    byte operand = AY(cycles, memory);
                    AND(operand);
                } break;

                case AND_IX:
                { 
                    byte operand = IX(cycles, memory);
                    AND(operand);
                } break;

                case AND_IY:
                {
                    byte operand = IY(cycles, memory);
                    AND(operand);
                } break;

                // ASL

                case ASL_AC:
                {
                    ASL(A);
                } break;

                case ASL_ZP:
                {
                    byte operand = ZP(cycles, memory);
                    ASL(operand);
                } break;

                case ASL_ZX:
                {
                    byte operand = ZX(cycles, memory);
                    ASL(operand);
                } break;

                case ASL_AB:
                {
                    byte operand = AB(cycles, memory);
                    ASL(operand);
                } break;

                case ASL_AX:
                {
                    byte operand = AB(cycles, memory);
                    ASL(operand);
                } break;

                // LDA

                case LDA_IM:
                {
                    A = IM(cycles, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_ZP:
                {
                    A = ZP(cycles, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_ZX:
                {
                    A = ZX(cycles, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_AB:
                {
                    A = AB(cycles, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_AX:
                {
                    A = AX(cycles, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_AY:
                {
                    A = AY(cycles, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_IX:
                {
                    A = IX(cycles, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_IY:
                {
                    A = IY(cycles, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                // JMP

                case JMP_AB:
                {
                    byte address = FetchInstruction(cycles, memory);
                    byte address_2 = FetchInstruction(cycles, memory);
                    hword full_address = ((hword)address_2 << 8) | (hword)address;
                    PC = full_address;
                } break;

                case JMP_IN:
                {
                    byte address = FetchInstruction(cycles, memory);
                    byte address_2 = FetchInstruction(cycles, memory);
                    hword full_address = ((hword)address_2 << 8) | (hword)address;
                    byte jmp_byte = ReadByte(cycles, full_address, memory);
                    full_address++;
                    byte jmp_byte_2 = ReadByte(cycles, full_address, memory);
                    PC = ((hword)jmp_byte << 8) || (hword)jmp_byte_2;
                } break;

                // JSR

                case JSR:
                {
                    SP++;
                    cycles--;
                    byte PC_1 = (byte)((PC >> 8) & 0xFF00);
                    byte PC_2 = (byte)(PC & 0xFF);
                    memory.WriteByte(cycles, SP, PC_1);
                    memory.WriteByte(cycles, SP + 1, PC_2);
                    byte address = FetchInstruction(cycles, memory);
                    byte address_2 = FetchInstruction(cycles, memory);
                    hword full_address = ((hword)address_2 << 8) | (hword)address;
                    PC = full_address;
                } break;

                default:
                {
                    PC++;
                }
            }
        }
    }
};


int main()
{
    CPU cpu;
    Memory memory;

    return 0;
}