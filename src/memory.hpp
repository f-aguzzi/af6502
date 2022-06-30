#include <array>
#include <vector>

#ifndef NUMBERS_h
    #include "numbers.hpp"
    #define NUMBERS_h
#endif

class Memory
{
    private:
    std::array<byte, MEM_SIZE> Mem;

    public:
    void init();
    byte operator[](hword address) const;
    void WriteByte(hword address, byte data);   // Write byte to memory, absolute
    byte ReadByte(hword address);   // Read byte from memory, absolute
    void WriteProgram(std::vector<byte> program, hword index);  // Write program to memory
};