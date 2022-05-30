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
    

    // **** Opcodes ****
    // ADC
    static constexpr byte ADC_IM = 0x69;    // Immediate
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
                    byte operand = FetchInstruction(cycles, memory);
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
                } break;

                case LDA_IM:
                {
                    byte value = FetchInstruction(cycles, memory);
                    A = value;
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_ZP:
                {
                    byte address = FetchInstruction(cycles, memory);
                    A = ReadByte(cycles, address, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_ZX:
                {
                    byte address = FetchInstruction(cycles, memory);
                    address += X;
                    cycles--;
                    A = ReadByte(cycles, address, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_AB:
                {
                    byte address = FetchInstruction(cycles, memory);
                    byte address_2 = FetchInstruction(cycles, memory);
                    hword full_address = ((hword)address_2 << 8) | (hword)address;
                    A = ReadByte(cycles, full_address, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_AX:
                {
                    byte address = FetchInstruction(cycles, memory);
                    byte address_2 = FetchInstruction(cycles, memory);
                    hword full_address = ((hword)address_2 << 8) | (hword)address + X;
                    if ((full_address & 0xFF00) != (PC & 0xFF00))
                    {
                        cycles--;
                    }
                    A = ReadByte(cycles, full_address, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_AY:
                {
                    byte address = FetchInstruction(cycles, memory);
                    byte address_2 = FetchInstruction(cycles, memory);
                    hword full_address = ((hword)address_2 << 8) | (hword)address + Y;
                    if ((full_address & 0xFF00) != (PC & 0xFF00))
                    {
                        cycles--;
                    }
                    A = ReadByte(cycles, full_address, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_IX:
                {
                    byte address = FetchInstruction(cycles, memory);
                    byte address_2 = FetchInstruction(cycles, memory);
                    hword full_address = ((hword)address_2 << 8) | (hword)address + X;
                    address = ReadByte(cycles, full_address, memory);
                    address_2 = FetchInstruction(cycles, memory);
                    full_address = ((hword)address << 8) || (hword)address_2;
                    A = ReadByte(cycles, full_address, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                case LDA_IY:
                {
                    byte address = FetchInstruction(cycles, memory);
                    byte address_2 = FetchInstruction(cycles, memory);
                    hword full_address = ((hword)address_2 << 8) | (hword)address;
                    address = ReadByte(cycles, full_address, memory);
                    address_2 = FetchInstruction(cycles, memory);
                    full_address = ((hword)address_2 << 8) | (hword)address + Y;
                    
                    A = ReadByte(cycles, full_address, memory);
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                } break;

                // JMP

                case JMP_AB:
                {
                    byte address = FetchInstruction(cycles, memory);
                    byte address_2 = FetchInstruction(cycles, memory);
                    hword full_address = ((hword)address << 8) || (hword)address_2;
                    PC = full_address;
                } break;

                case JMP_IN:
                {
                    byte address = FetchInstruction(cycles, memory);
                    byte address_2 = FetchInstruction(cycles, memory);
                    hword full_address = ((hword)address << 8) || (hword)address_2;
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
                    hword full_address = ((hword)address << 8) || (hword)address_2;
                    PC = full_address;
                } break;

                default:
                {
                    return;
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