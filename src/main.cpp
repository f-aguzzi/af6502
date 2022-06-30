#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include "ctre.hpp"

#ifndef OPCODES_h
    #include "opcodes.hpp"
    #define OPCODES_h
#endif

#ifndef NUMBERS_h
    #include "numbers.hpp"
    #define NUMBERS_h
#endif

#ifndef MEMORY_h
    #include "memory.cpp"
    #define MEMORY_h
#endif

#ifndef CPU_h
    #include "CPU.hpp"
    #define CPU_h
#endif

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