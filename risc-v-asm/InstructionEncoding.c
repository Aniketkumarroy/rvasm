#include<stdlib.h>
#include<string.h>
#include"hex.h"
#include"rv32Encoding.h"

void UpperImmediateType(char *code, char* mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int)){
    char opcode[7], binary[33];
    if(strcmp(mnemonics, "lui") == 0) strcpy(opcode, "01101");
    else if(strcmp(mnemonics, "auipc") == 0) strcpy(opcode, "00101");
    else if(Error != NULL) Error("Wrong Instruction\n", 1);
    U_Encoding(opcode, code, binary, Error);
    if(Writefunc != NULL)
    Writefunc(binary);
}

void ImmediateType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int)){
    char opcode[7], binary[33];
    if(strcmp(mnemonics, "addi") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, Error);
        binary[17] = '0'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "slti") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, Error);
        binary[17] = '0'; binary[18] = '1'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "sltiu") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, Error);
        binary[17] = '0'; binary[18] = '1'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "xori") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, Error);
        binary[17] = '1'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "ori") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, Error);
        binary[17] = '1'; binary[18] = '1'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "andi") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, Error);
        binary[17] = '1'; binary[18] = '1'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "slli") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, Error);
        binary[17] = '0'; binary[18] = '0'; binary[19] = '1';
        binary[0] = '0'; binary[1] = '0'; binary[2] = '0'; binary[3] = '0'; binary[4] = '0'; binary[5] = '0'; binary[6] = '0';
    }
    else if(strcmp(mnemonics, "srli") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, Error);
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
        binary[0] = '0'; binary[1] = '0'; binary[2] = '0'; binary[3] = '0'; binary[4] = '0'; binary[5] = '0'; binary[6] = '0';
    }
    else if(strcmp(mnemonics, "srai") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, Error);
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
        binary[0] = '0'; binary[1] = '1'; binary[2] = '0'; binary[3] = '0'; binary[4] = '0'; binary[5] = '0'; binary[6] = '0';
    }
    else {
        if(Error != NULL)
        Error("Wrong Instruction\n", 1);
    }

    if(Writefunc != NULL)
    Writefunc(binary);
}

void RegisterType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int)){
    char *opcode = "01100", binary[33];
    R_Encoding(opcode, code, binary, Error);
    if(strcmp(mnemonics, "add") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "sub") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '0';
        binary[1] = '1';
    }
    else if(strcmp(mnemonics, "slt") == 0){
        binary[17] = '0'; binary[18] = '1'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "sltu") == 0){
        binary[17] = '0'; binary[18] = '1'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "xor") == 0){
        binary[17] = '1'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "or") == 0){
        binary[17] = '1'; binary[18] = '1'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "and") == 0){
        binary[17] = '1'; binary[18] = '1'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "sll") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "srl") == 0){
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "sra") == 0){
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
        binary[1] = '1';
    }
    else if(strcmp(mnemonics, "mul") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '0';
        binary[6] = '1';
    }
    else if(strcmp(mnemonics, "mulh") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '1';
        binary[6] = '1';
    }
    else if(strcmp(mnemonics, "mulhsu") == 0){
        binary[17] = '0'; binary[18] = '1'; binary[19] = '0';
        binary[6] = '1';
    }
    else if(strcmp(mnemonics, "mulhu") == 0){
        binary[17] = '0'; binary[18] = '1'; binary[19] = '1';
        binary[6] = '1';
    }
    else if(strcmp(mnemonics, "div") == 0){
        binary[17] = '1'; binary[18] = '0'; binary[19] = '0';
        binary[6] = '1';
    }
    else if(strcmp(mnemonics, "divu") == 0){
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
        binary[6] = '1';
    }
    else if(strcmp(mnemonics, "rem") == 0){
        binary[17] = '1'; binary[18] = '1'; binary[19] = '0';
        binary[6] = '1';
    }
    else if(strcmp(mnemonics, "remu") == 0){
        binary[17] = '1'; binary[18] = '1'; binary[19] = '1';
        binary[6] = '1';
    }
    else {
        if(Error != NULL)
        Error("Wrong Instruction\n", 1);
    }

    if(Writefunc != NULL)
    Writefunc(binary);
}

void LoadType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int)){
    char binary[33], *opcode = "00000";
    char *found = strstr(code, "(");
    if(found != NULL)
    *found = ' ';
    found = strstr(code, ")");
    if(found != NULL)
    *found = '\0';

    char *L = strtok(code, " ");
    char *rd = strtok(NULL, " ");
    if(rd == NULL && Error != NULL) Error("Wrong format\ndestination register not provided", 1);
    char *offset = strtok(NULL, " ");
    if(offset == NULL && Error != NULL) Error("Wrong format\noffset not provided", 1);
    char *rs1 = strtok(NULL, " ");
    if(rs1 == NULL && Error != NULL) Error("Wrong format\nsource register not provided", 1);

    char new_code[32];
    new_code[0] = '\0';
    strcat(new_code, L);
    strcat(new_code, " ");
    strcat(new_code, rd);
    strcat(new_code, " ");
    strcat(new_code, rs1);
    strcat(new_code, " ");
    strcat(new_code, offset);
    
    I_Encoding(opcode, new_code, binary, Error);

    if(strcmp(mnemonics, "lb") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "lh") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "lw") == 0){
        binary[17] = '0'; binary[18] = '1'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "lbu") == 0){
        binary[17] = '1'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "lhu") == 0){
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
    }else {
        if(Error != NULL)
        Error("Wrong Instruction\n", 1);
    }

    if(Writefunc != NULL)
    Writefunc(binary);
}

void StoreType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int)){
    char binary[33], *opcode = "01000";
    char *found = strstr(code, "(");
    if(found != NULL)
    *found = ' ';
    found = strstr(code, ")");
    if(found != NULL)
    *found = '\0';

    S_Encoding(opcode, code, binary, Error);

    if(strcmp(mnemonics, "sb") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "sh") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "sw") == 0){
        binary[17] = '0'; binary[18] = '1'; binary[19] = '0';
    }
    else {
        if(Error != NULL)
        Error("Wrong Instruction\n", 1);
    }

    if(Writefunc != NULL)
    Writefunc(binary);
}

void JAL(char *code, char* mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int)){
    char binary[33], *opcode = "11011";
    J_Encoding(opcode, code, binary, Error);
    if(Writefunc != NULL)
    Writefunc(binary);
}

void JALR(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int)){
    char binary[33], *opcode = "11001";
    I_Encoding(opcode, code, binary, Error);
    if(Writefunc != NULL)
    Writefunc(binary);
}

void BranchType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int)){
    char binary[33], *opcode = "11000";

    B_Encoding(opcode, code, binary, Error);

    if(strcmp(mnemonics, "beq") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "bne") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "blt") == 0){
        binary[17] = '1'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "bge") == 0){
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "bltu") == 0){
        binary[17] = '1'; binary[18] = '1'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "bgeu") == 0){
        binary[17] = '1'; binary[18] = '1'; binary[19] = '1';
    }else {
        if(Error != NULL)
        Error("Wrong Instruction\n", 1);
    }

    if(Writefunc != NULL)
    Writefunc(binary);
}