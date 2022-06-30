#include <gtest/gtest.h>

#ifndef MEMORY_h
    #include "../memory.hpp"
    #define MEMORY_h
#endif

#ifndef CPU_h
    #include "../CPU.hpp"
    #define CPU_h
#endif

TEST(AF6502Tests, DemonstrateGTestMacro)
{
    EXPECT_EQ(true, true);
}

TEST(AF6502Tests, ResetTest)
{
    CPU cpu(0);

    cpu.A = 152;
    cpu.X = 11;
    cpu.Y = 144;

    cpu.PC = 3465;
    cpu.SP = 2324;

    cpu.C = true;
    cpu.Z = true;
    cpu.I = true;
    cpu.D = true;
    cpu.B = true;
    cpu.V = true;
    cpu.N = true;

    cpu.reset();

    EXPECT_EQ(cpu.A, 0);
    EXPECT_EQ(cpu.X, 0);
    EXPECT_EQ(cpu.Y, 0);

    EXPECT_EQ(cpu.PC, 0xFFC);
    EXPECT_EQ(cpu.SP, 0x0100);

    EXPECT_EQ(cpu.C, false);
    EXPECT_EQ(cpu.Z, false);
    EXPECT_EQ(cpu.I, false);
    EXPECT_EQ(cpu.D, false);
    EXPECT_EQ(cpu.B, false);
    EXPECT_EQ(cpu.V, false);
    EXPECT_EQ(cpu.N, false);
}

// CPU instructions tested: FetchInstruction, WriteByte, ReadByte
// Memory instructions tested: WriteByte, ReadByte, []
TEST(AF6502Tests, MemoryInstructionTest)
{
    // Create CPU
    CPU cpu(20);

    // Hardcode an instruction in memory and read it
    cpu.memory.WriteByte(0xFFC, 0x01);
    byte instr = cpu.FetchInstruction();

    // Check if everything behaves as expected
    EXPECT_EQ(cpu.cycles, 19);
    EXPECT_EQ(instr, 0x01);
    EXPECT_EQ(cpu.PC, 0xFFD);

    // Hardcode a number in memory and read it
    cpu.memory.WriteByte(0xFFD, 0xA9);
    byte num1 = cpu.ReadByte(0xFFD);

    // Check if everything behaves as expected
    EXPECT_EQ(cpu.cycles, 18);
    EXPECT_EQ(num1, 0xA9);
    EXPECT_EQ(cpu.PC, 0xFFD);

    // Write a number to memory via CPU instruction and read it
    cpu.WriteByte(0xFFE, 0xD4);
    byte num2 = cpu.memory.ReadByte(0xFFE);

    // Check if everything behaves as expected
    EXPECT_EQ(cpu.cycles, 17);
    EXPECT_EQ(num2, 0xD4);
    EXPECT_EQ(cpu.PC, 0xFFD);
}

// Test all the addressing modes
TEST(AF6502Tests, AddressingModesTest)
{
    // Create CPU
    CPU cpu(40);

    // Immediate addressing
    cpu.memory.WriteByte(0xFFC, 0x6A);
    EXPECT_EQ(cpu.IM(), 0x6A);
    EXPECT_EQ(cpu.PC, 0xFFD);
    EXPECT_EQ(cpu.cycles, 39);  // 1 cycle consumed

    // Zeropage addressing
    cpu.memory.WriteByte(0xFFD, 0xA1);
    cpu.memory.WriteByte(0xA1, 0xE4);
    EXPECT_EQ(cpu.ZP(), 0xE4);
    EXPECT_EQ(cpu.PC, 0xFFE);
    EXPECT_EQ(cpu.cycles, 37);  // 2 cycles consumed
    
    // Zeropage, X addressing
    cpu.X = 14;
    cpu.memory.WriteByte(0xFFE, 0xA1);
    cpu.memory.WriteByte(0xA1 + cpu.X, 0xEE);
    EXPECT_EQ(cpu.ZX(), 0xEE);
    EXPECT_EQ(cpu.PC, 0xFFF);
    EXPECT_EQ(cpu.cycles, 34);  // 3 cycles consumed

    // Zeropage, Y addressing
    cpu.Y = 33;
    cpu.memory.WriteByte(0xFFF, 0xA1);
    cpu.memory.WriteByte(0xA1 + cpu.Y, 0x81);
    EXPECT_EQ(cpu.ZY(), 0x81);
    EXPECT_EQ(cpu.PC, 0x1000);
    EXPECT_EQ(cpu.cycles, 31);  // 3 cycles consumed

    // Absolute addressing
    cpu.memory.WriteByte(0x1000, 0xEA);
    cpu.memory.WriteByte(0x1001, 0xAE);
    cpu.memory.WriteByte(0xAEEA, 0x27);
    EXPECT_EQ(cpu.AB(), 0x27);
    EXPECT_EQ(cpu.PC, 0x1002);
    EXPECT_EQ(cpu.cycles, 28);  // 3 cycles consumed

    // Absolute, X addressing (without page jump)
    cpu.X = 10;
    cpu.memory.WriteByte(0x1002, 0x08);
    cpu.memory.WriteByte(0x1003, 0x10); // no 8-bit carry, no page jump
    cpu.memory.WriteByte(0x1008 + cpu.X, 0x22);
    EXPECT_EQ(cpu.AX(), 0x22);
    EXPECT_EQ(cpu.PC, 0x1004);
    EXPECT_EQ(cpu.cycles, 25);  // 3 cycles consumed

    // Absolute, X addressing (with page jump)
    cpu.X = 192;
    cpu.memory.WriteByte(0x1004, 0xB7); // carry of 0xB7+ 192 induces page jump
    cpu.memory.WriteByte(0x1005, 0xC9);
    cpu.memory.WriteByte(0xC9B7 + cpu.X, 0xAA);
    EXPECT_EQ(cpu.AX(), 0xAA);
    EXPECT_EQ(cpu.PC, 0x1006);
    EXPECT_EQ(cpu.cycles, 21);  // 4 cycles consumed

    // Absolute, Y addressing (without page jump)
    cpu.Y = 1;
    cpu.memory.WriteByte(0x1006, 0xD5); // no 8-bit carry, no page jump
    cpu.memory.WriteByte(0x1007, 0xBB);
    cpu.memory.WriteByte(0xBBD5 + cpu.Y, 0x62);
    EXPECT_EQ(cpu.AY(), 0x62);
    EXPECT_EQ(cpu.PC, 0x1008);
    EXPECT_EQ(cpu.cycles, 18);   // 3 cycles consumed

    // Absolute, Y addressing (with page jump)
    cpu.Y = 33;
    cpu.memory.WriteByte(0x1008, 0xE9); // carry of 0xE9 + 33 induces page jump
    cpu.memory.WriteByte(0x1009, 0xEC);
    cpu.memory.WriteByte(0xECE9 + cpu.Y, 0x33);
    EXPECT_EQ(cpu.AY(), 0x33);
    EXPECT_EQ(cpu.PC, 0x100A);
    EXPECT_EQ(cpu.cycles, 14);   // 4 cycles consumed

    // (Indirect, X) addressing
    cpu.X = 82;
    cpu.memory.WriteByte(0x100A, 0xB4); // Deliberately cause zeropage overflow
    cpu.memory.WriteByte(0x06, 0xBB);   // Carry ignored: (0xB4 + 82) % 256 = 6
    cpu.memory.WriteByte(0x07, 0xAA);
    cpu.memory.WriteByte(0xAABB, 0x03);
    EXPECT_EQ(cpu.IX(), 0x03);
    EXPECT_EQ(cpu.cycles, 9);  // 5 cycles consumed

    // (Indirect), Y addressing (without page jump)
    cpu.Y = 12;
    cpu.memory.WriteByte(0x100B, 0xEE); // no 8-bit carry, no page jump
    cpu.memory.WriteByte(0xEE, 0xAA);
    cpu.memory.WriteByte(0xEE + 1, 0xB4);
    cpu.memory.WriteByte(0xB4AA + cpu.Y, 0x02);
    EXPECT_EQ(cpu.IY(), 0x02);
    EXPECT_EQ(cpu.cycles, 5);  // 4 cycles consumed

    // (Indirect), Y addressing (with page jump)
    cpu.Y = 199;
    cpu.memory.WriteByte(0x100C, 0xBC);
    cpu.memory.WriteByte(0xBC, 0xE0);
    cpu.memory.WriteByte(0xBD, 0xAE);
    cpu.memory.WriteByte(0xAEE0 + cpu.Y, 0x05);
    EXPECT_EQ(cpu.IY(), 0x05);
    EXPECT_EQ(cpu.cycles, 0);  // 5 cycles consumed
}