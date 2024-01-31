# rvasm

rvasm is a assembler for the RV32M Instruction set for RISC-V processors
the supported instructions are given [here](/instruction_set_encoding.s)

## building the library

create a build folder, navigate into it and use cmake command to generate Makefile
```
mkdir build
```
```
cd build
```
```
cmake ..
```
now run the generated Makefile to generate the assembler
```
make
```
your assembler binary ```rvasm``` is ready in build folder

now you can use it by
```
rvasm YOUR_CODE
```

it will generate a file with name ```YOUR_CODE.out```
for specifying the output file name use the use the ```-o``` flag
```
rvasm YOUR_CODE -o output
```
the output will be in binary format in 0s and 1s
to get a hex output or hex file use the ```-hex``` flag
```
rvasm YOUR_CODE -o output -hex
```