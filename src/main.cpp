using byte = unsigned char;
using word = unsigned short;

struct CPU
{
    word PC;    // program counter
    word SP;    // stack pointer

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
};

int main()
{
    CPU cpu;
    return 0;
}