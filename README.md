# rvasm

rvasm is a assembler for the RV32M Instruction set for RISC-V processors
the supported instructions are given [here](/instruction_set_encoding.s)

for using it first compile the c code
```
gcc rvasm.c -o rvasm
```
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