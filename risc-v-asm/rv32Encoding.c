#include<string.h>
#include"utilities.h"
#include"hex.h"

#include"rv32Encoding.h"

void U_Encoding(char *opcode, char *code, char *binary, void (*error)(char*)){
    char *ins_1_0 = "11";
    char *ins_6_2 = opcode; // opcode
    char ins_11_7[6]; // rd
    char ins_31_12[21];
    char *c = strtok(code, " ");
    c = strtok(NULL, " ");

    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_11_7, error);

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    ValueInterpreter(c, binary, 32, error);
    strcpy(ins_31_12, binary + 12);

    binary[0] = '\0';
    strcat(binary, ins_31_12);
    strcat(binary, ins_11_7);
    strcat(binary, ins_6_2);
    strcat(binary, ins_1_0);
}
void I_Encoding(char *opcode, char *code, char *binary, void (*error)(char*)){
    char *ins_1_0 = "11";
    char *ins_6_2 = opcode; // opcode
    char ins_11_7[6]; // rd
    char ins_14_12[4]; // funct3
    char ins_19_15[6]; // rs1
    char ins_31_20[13]; // imm
    char *c = strtok(code, " ");

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_11_7, error);

    ins_14_12[0] = '0';
    ins_14_12[1] = '0';
    ins_14_12[2] = '0';
    ins_14_12[3] = '\0';

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_19_15, error);

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    ValueInterpreter(c, binary, 32, error);
    strcpy(ins_31_20, binary + 20);

    binary[0] = '\0';
    strcat(binary, ins_31_20);
    strcat(binary, ins_19_15);
    strcat(binary, ins_14_12);
    strcat(binary, ins_11_7);
    strcat(binary, ins_6_2);
    strcat(binary, ins_1_0);
}
void R_Encoding(char *opcode, char *code, char *binary, void (*error)(char*)){
    char *ins_1_0 = "11";
    char *ins_6_2 = opcode; // opcode
    char ins_11_7[6]; // rd
    char ins_14_12[4]; // funct3
    char ins_19_15[6]; // rs1
    char ins_24_20[6]; // rs2
    char ins_31_25[8];
    char *c = strtok(code, " ");

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_11_7, error);

    ins_14_12[0] = '0';
    ins_14_12[1] = '0';
    ins_14_12[2] = '0';
    ins_14_12[3] = '\0';

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_19_15, error);

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_24_20, error);

    strcpy(ins_31_25, "0000000");

    binary[0] = '\0';
    strcat(binary, ins_31_25);
    strcat(binary, ins_24_20);
    strcat(binary, ins_19_15);
    strcat(binary, ins_14_12);
    strcat(binary, ins_11_7);
    strcat(binary, ins_6_2);
    strcat(binary, ins_1_0);
}
void S_Encoding(char *opcode, char *code, char *binary, void (*error)(char*)){
    char *ins_1_0 = "11";
    char *ins_6_2 = opcode; // opcode
    char ins_11_7[6]; // offset[4:0]
    char ins_14_12[4]; // funct3
    char ins_19_15[6]; // rs1
    char ins_24_20[6]; // rs2
    char ins_31_25[8]; // offset[11:5]
    char *c = strtok(code, " ");

    ins_14_12[0] = '0';
    ins_14_12[1] = '0';
    ins_14_12[2] = '0';
    ins_14_12[3] = '\0';

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_24_20, error);


    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    ValueInterpreter(c, binary, 32, error); // offset

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_19_15, error);

    strcpy(ins_11_7, binary + 27);
    binary[27] = '\0';
    strcpy(ins_31_25, binary + 20);

    binary[0] = '\0';
    strcat(binary, ins_31_25);
    strcat(binary, ins_24_20);
    strcat(binary, ins_19_15);
    strcat(binary, ins_14_12);
    strcat(binary, ins_11_7);
    strcat(binary, ins_6_2);
    strcat(binary, ins_1_0);
}
void J_Encoding(char *opcode, char *code, char *binary, void (*error)(char*)){
    char *ins_1_0 = "11";
    char *ins_6_2 = opcode; // opcode
    char ins_11_7[6]; // rd
    char ins_19_12[9];
    char ins_31_20[13];
    char *c = strtok(code, " ");
    c = strtok(NULL, " ");

    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_11_7, error);

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    ValueInterpreter(c, binary, 32, error);

    binary[31] = '\0';
    ins_31_20[0] = binary[11];
    ins_31_20[1] = '\0';
    strcat(ins_31_20, binary + 21);
    ins_31_20[11] = binary[20];
    ins_31_20[12] = '\0';

    binary[20] = '\0';
    strcpy(ins_19_12, binary + 12);

    binary[0] = '\0';
    strcat(binary, ins_31_20);
    strcat(binary, ins_19_12);
    strcat(binary, ins_11_7);
    strcat(binary, ins_6_2);
    strcat(binary, ins_1_0);
}
void B_Encoding(char *opcode, char *code, char *binary, void (*error)(char*)){
    char *ins_1_0 = "11";
    char *ins_6_2 = opcode; // opcode
    char ins_11_7[6]; // offset[4:1|11]
    char ins_14_12[4]; // funct3
    char ins_19_15[6]; // rs1
    char ins_24_20[6]; // rs2
    char ins_31_25[8]; // offset[12|10:5]
    char *c = strtok(code, " ");

    ins_14_12[0] = '0';
    ins_14_12[1] = '0';
    ins_14_12[2] = '0';
    ins_14_12[3] = '\0';

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_19_15, error);

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    RegisterCode(c, ins_24_20, error);

    c = strtok(NULL, " ");
    if(c==NULL && error != NULL) error("Wrong instruction\n");
    ValueInterpreter(c, binary, 32, error); // offset

    binary[31] = '\0';
    strcpy(ins_11_7, binary + 27);
    ins_11_7[4] = binary[20];
    ins_11_7[5] = '\0';

    ins_31_25[0] = binary[19];
    ins_31_25[1] = '\0';
    binary[27] = '\0';
    strcat(ins_31_25, binary + 21);

    binary[0] = '\0';
    strcat(binary, ins_31_25);
    strcat(binary, ins_24_20);
    strcat(binary, ins_19_15);
    strcat(binary, ins_14_12);
    strcat(binary, ins_11_7);
    strcat(binary, ins_6_2);
    strcat(binary, ins_1_0);
}