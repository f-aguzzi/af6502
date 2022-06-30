#ifndef NUMBERS_h
    #include "numbers.hpp"
    #define NUMBERS_h
#endif

#ifndef MEMORY_h
    #include "memory.hpp"
    #define MEMORY_h
#endif

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

    Memory memory;  // Memory object
    word cycles;    // Number of execution cycles available

    // Constructors
    CPU(word);
    CPU(word, Memory);

    void reset();
 
    byte FetchInstruction();    // Load instruction from memory
    byte ReadByte(hword address);   // Byte fetch, absolute
    void WriteByte(hword address, byte data);   // Byte write, absolute

    // **** Addressing modes ****

    byte IM();  // Immediate addressing
    byte ZP();  // Zeropage addressing   
    byte ZX();  // Zeropage, X addressing   
    byte ZY();  // Zeropage, Y addressing 
    byte AB();  // Absolute addressing
    byte AX();  // Absolute, X addressing
    byte AY();  // Absolute, Y addressing
    byte IX();  // (Indirect, X) addressing
    byte IY();  // (Indirect), Y addressing

    // **** Write addressing ****

    byte ZP_A();    // Zeropage address
    byte ZX_A();    // Zeropage, X address
    byte ZY_A();    // Zeropage, Y address
    hword AB_A();   // Absolute address
    hword AX_A();   // Absolute, X address
    hword AY_A();   // Absolute, Y address
    hword IX_A();   // (Indirect, X) address
    hword IY_A();   

    // **** Generic Operations ****

    void ADC(byte); // Generic ADC operation
    void AND(byte); // Generic AND operation
    void ASL(byte); // Generic ASL operation
    void Branch(byte, bool);  // Generic branch operation
    void BIT(byte ); // Generic BIT operation
    void Compare(byte&, byte);  // Generic compare operation  
    void CMP(byte); // Generic CMP operation
    void CPX(byte); // Generic CPX operation
    void CPY(byte); // Generic CPY operation
    void DEC(byte, hword);  // Generic DEC operation
    void Decrement(byte&);  // Generic register decrement
    void EOR(byte); // Generic EOR operation
    void INC(byte, hword);  // Generic INC operation
    void Increment(byte&);  // Generic register increment
    void LD(byte&, byte);   // Generic LD(X-Y) operation
    byte LSR(byte); // Generic LSR operation
    void ORA(byte); // Generic ORA operation
    byte ROL(byte); // Generic ROL operation
    byte ROR(byte); // Generic ROR operation
    void SBC(byte); // Generic SBC operation
    void STA(hword);    // Generic STA operation
    void STX(hword);    // Generic STX operation
    void STY(hword);    // Generic STY operation
    void DCP(hword);    // Generic DCP operation
    void ISC(hword);    // Generic ISC operation
    void RLA(hword);    // Generic RLA operation
    void RRA(hword);    // Generic RRA operation
    inline void SAX(hword); // Generic SAX operation 
    void SLO(hword);    // Generic SLO operation
    void SRE(hword);    // Generic SRE operation
    void execute(hword);  // Opcode decoding & execution
};