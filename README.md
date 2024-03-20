![RISC-V Logo](https://riscv.org/wp-content/uploads/2020/06/riscv-color.svg)
# rvasm

rvasm is a assembler for the RV32M Instruction set for RISC-V processors.
the supported instructions are given [here](/instruction_set_encoding.s)

## building the assembler
git clone the repository and navigate inside it
```bash
git clone https://github.com/Aniketkumarroy/rvasm.git
```
```bash
cd rvasm
```
create a build folder, navigate into it and use cmake command to generate Makefile
```bash
mkdir build
```
```bash
cd build
```
```bash
cmake ..
```
now run the generated Makefile to generate the assembler
```bash
make
```
your assembler binary ```rvasm``` is ready in build folder

now you can use it by
```bash
./rvasm YOUR_CODE
```

it will generate a file with name ```YOUR_CODE.out```
for specifying the output file name use the ```-o``` flag
```bash
./rvasm YOUR_CODE -o output
```
the output will be in binary format in 0s and 1s
to get a hex output or hex file use the ```-hex``` flag
```bash
./rvasm YOUR_CODE -o output -hex
```
## options
to know about its various options use
```bash
./rvasm -h
```
or
```bash
./rvasm -help
```
```
rvasm is a assembler for RISC-V 32-bit instruction set architecture. 
It supports assembling RISC-V assembly code into machine code.

Usage:
./rvasm [source file] | [-o output file] | [-c comment string] | [-hex hexadecimal format] | [-e little endian] | [-f format]

Options:
-o, output file name, optional flag, default is <input file name>.out
-c, comment string, string that will be interpreted as a comment, optional flag, default is ';'
-hex, hexadecimal, to generate output in hexadecimal format, optional flag, by default it generates output in binary format
-e, little endian, it produces output in Little Endian memory model, optional flag, by default big Endian is used
-f, format, it specifies how many bytes separated by spaces to be printed in each line of output file, optional flag, by default whole instruction is printed in a line without any spaces
-h, -help, help flag
```
## how to contribute
if you want to add more instruction support, start first implementing functional behaviour of the instruction in [InstructionEncoding.c](risc-v-asm/InstructionEncoding.c). add your function signature in [InstructionEncoding.h](risc-v-asm/include/InstructionEncoding.h). take care of the function signature format, its return type and parameter list should match the other function signatures in the file. you may also need to make the binary encoding for your instruction type, for that you need to define the encoding function in [rv32Encoding.h](risc-v-asm/include/rv32Encoding.h) and implement it in [rv32Encoding.c](risc-v-asm/rv32Encoding.c).

once all this is done just add your function and its corresponding mnemonic in [main.c](main.c#opcode_lookup) in ``opcode_lookup``. remember to write the mnemonic in all smaller case.