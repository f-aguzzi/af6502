# af6502
6502 8-bit processor emulator

This emulator is still a work in progress.

All opcodes have been implemented, and they were tested individually
through unit tests, but no global testing (i.e., running a piece of
assembly through the emulator) has been performed yet.

## Usage

The emulator shall be operated from the command line.

The command line flags must have space between each other, but their
arguments are attached. For example, to load a file and activate the
register status output:

```
af6502 -input=./program.bin -showstatus
```

### Command line arguments:

- `-cycles=` *(followed by a number, without quotes)*: sets the number of
  clock cycles to execute
- `-input=` *(followed by the path, without quotes)*: selects the input
  binary file
- `-showstatus`: displays the status of the registers in the terminal,
  at the end of the execution
- `-dump=` *(followed by the path, without quotes)*: selects the path for
  the memory dump file
- `-start=` *(followed by a number, without quotes)*: sets the inital value
  for the Program Counter register, that is, the starting memory location
  of the executable. When loading a binary, it will be transcribed to
  memory starting from this value

Invalid flags are ignored.
