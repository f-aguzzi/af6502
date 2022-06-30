#ifndef MEMORY_h
    #include "memory.hpp"
    #define MEMORY_h
#endif

void Memory::init()
{
    Mem.fill(0);
}

byte Memory::operator[](hword address) const
{
    return Mem[address];
}

void Memory::WriteByte(hword address, byte data)
{
    printf("Indirizzo di scrittura: %d\n", (int)address);
    Mem[address] = data;
}

byte Memory::ReadByte(hword address)
{
    return Mem[address];
}

void Memory::WriteProgram(std::vector<byte> program, hword index)
{
    int i = index;
    for (byte b: program)
    {
        Mem[i] = b;
        i++;
    }
}