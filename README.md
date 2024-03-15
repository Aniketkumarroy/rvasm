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
rvasm YOUR_CODE
```

it will generate a file with name ```YOUR_CODE.out```
for specifying the output file name use the use the ```-o``` flag
```bash
rvasm YOUR_CODE -o output
```
the output will be in binary format in 0s and 1s
to get a hex output or hex file use the ```-hex``` flag
```bash
rvasm YOUR_CODE -o output -hex
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
-o, output file name, optionsl flag, default is <input file name>.out
-c, comment string, string that will be interpreted as a comment, optionsl flag, default is ';'
-hex, hexadecimal, to generate outpurt in hexadecimal number, optionsl flag, by default it generates output in binary numbers
-e, little endian, it produces output in Little Endian memory model, optionsl flag, by default big Endian is used
-f, format, it specifies how many bytes separated by spaces to be printed in each line of output file, optionsl flag, by default whole instruction is printed in a line without any spaces
-h, -help, help flag
```