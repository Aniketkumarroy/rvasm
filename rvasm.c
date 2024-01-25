#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>

FILE *INPUT_FILE = NULL;
FILE *OUTPUT_FILE = NULL;
unsigned int LINE_NO = 0;
char LINE_NO_HEX[9] = "00000000";
char *COMMENT = ";";
int HEX_PRINT = 0;

unsigned int assembly_file_line_no = -1;

void exit_from_program_with_message(char *msg, unsigned short int e){
    printf("%s", msg);
    if(e) printf("ERROR at line no. %d", assembly_file_line_no);
    if(INPUT_FILE != NULL) fclose(INPUT_FILE);
    if(OUTPUT_FILE != NULL) fclose(OUTPUT_FILE);
    exit(0);
}
void exit_from_program(unsigned short int e){
    if(e) printf("ERROR at line no. %d", assembly_file_line_no);
    if(INPUT_FILE != NULL) fclose(INPUT_FILE);
    if(OUTPUT_FILE != NULL) fclose(OUTPUT_FILE);
    exit(0);
}
int find(int n_array, const char **c_array, char *c){
    for(int i = 0; i< n_array; i++){
        if (strcmp(c_array[i], c) == 0)
        return i;
    }
    return -1;
}
void RemoveSpaces(char **line){
    // removing leading spaces
    while(isspace(**line))
    (*line)++;
    // removing trailing spaces
    char *end = *line + strlen(*line) - 1;
    if(isspace(*end)){
        while(isspace(*end))
        end--;
        *(end+1) = '\0';
    }
}
void RemoveComments(char *line, char *comment){
    char *found = strstr(line, comment); // found points to the address of the comment symbol(s)
    if(found != NULL)
    *found = '\0';
}
void Preprocess(char **line){
    RemoveSpaces(line);             // Removing leading and trailing spaces
    RemoveComments(*line, COMMENT); // Removing comments
    char *c = *line;
    int i = 0;
    while(*c != '\0'){
        if(*c == ',')
        *c = ' '; // removing ',' with ' '

        // ensuring that there should be only one space separating two mnemonics
        (*line)[i] = tolower(*c);
        i++;
        if(*c == ' '){
            while(*(c+1) == ' ')
            c++;
        }
        c++;
    }
    (*line)[i] = '\0';
}
int hexDigitAdder(char a, char b, char c, char *s, char *carry){
    int a_int, b_int, c_int, s_int, carry_int;
    if('0' <= a && a <= '9')
    a_int = a - '0';
    else if('a' <= a && a <= 'f')
    a_int = a - 'a' + 10;
    else if('A' <= a && a <= 'F')
    a_int = a - 'A' + 10;
    else a_int = 0;

    if('0' <= b && b <= '9')
    b_int = b - '0';
    else if('a' <= b && b <= 'f')
    b_int = b - 'a' + 10;
    else if('A' <= b && b <= 'F')
    b_int = b - 'A' + 10;
    else b_int = 0;

    c_int = (c == '1') ? 1 : 0;

    s_int = (a_int + b_int + c_int)%16;
    carry_int = (a_int + b_int + c_int)/16;

    *carry = (carry_int) ? '1' : '0';
    switch (s_int)
    {
    case  0: *s = '0'; break;
    case  1: *s = '1'; break;
    case  2: *s = '2'; break;
    case  3: *s = '3'; break;
    case  4: *s = '4'; break;
    case  5: *s = '5'; break;
    case  6: *s = '6'; break;
    case  7: *s = '7'; break;
    case  8: *s = '8'; break;
    case  9: *s = '9'; break;
    case 10: *s = 'a'; break;
    case 11: *s = 'b'; break;
    case 12: *s = 'c'; break;
    case 13: *s = 'd'; break;
    case 14: *s = 'e'; break;
    case 15: *s = 'f'; break;
    
    default: *s = '0'; break;
    }
}

void HexadecimalAdder(char *A, char *B, char *S){
    int sA = strlen(A), sB = strlen(B);
    char sum, carry;
    int s = (sA > sB) ? sA : sB;
    char tmpA, tmpB, tmpC = '0';

    S[s] = '\0';
    s--;
    while(s >= 0){
        if(sA > 0)
        tmpA = *(A + (--sA));
        else
        tmpA = '0';

        if(sB > 0)
        tmpB = *(B + (--sB));
        else
        tmpB = '0';

        hexDigitAdder(tmpA, tmpB, tmpC, &sum, &carry);
        tmpC = carry;
        S[s] = sum;
        s--;
    }
}
int isHexadecimal(char *c){
    if(*c == '\0')
    return 0;

    while(*c != '\0'){
        if(!isxdigit(*c))
        return 0;
        c++;
    }
    return 1;
}
int isdecimal(char *c){
    if(*c == '\0')
    return 0;

    if(*c == '+' || *c == '-')
    c++;

    while(*c != '\0'){
        if(!isdigit(*c))
        return 0;
        c++;
    }
    return 1;
}
void hexDigitToBinary(char hexDigit, char *binary) {
    switch (hexDigit) {
        case '0': strcpy(binary, "0000"); break;
        case '1': strcpy(binary, "0001"); break;
        case '2': strcpy(binary, "0010"); break;
        case '3': strcpy(binary, "0011"); break;
        case '4': strcpy(binary, "0100"); break;
        case '5': strcpy(binary, "0101"); break;
        case '6': strcpy(binary, "0110"); break;
        case '7': strcpy(binary, "0111"); break;
        case '8': strcpy(binary, "1000"); break;
        case '9': strcpy(binary, "1001"); break;
        case 'a': case 'A': strcpy(binary, "1010"); break;
        case 'b': case 'B': strcpy(binary, "1011"); break;
        case 'c': case 'C': strcpy(binary, "1100"); break;
        case 'd': case 'D': strcpy(binary, "1101"); break;
        case 'e': case 'E': strcpy(binary, "1110"); break;
        case 'f': case 'F': strcpy(binary, "1111"); break;
        default: exit_from_program(1);
    }
}
char BinaryTohexDigit(char *binary) {
    if(strcmp(binary, "0000") == 0)      return '0';
    else if(strcmp(binary, "0001") == 0) return '1';
    else if(strcmp(binary, "0010") == 0) return '2';
    else if(strcmp(binary, "0011") == 0) return '3';
    else if(strcmp(binary, "0100") == 0) return '4';
    else if(strcmp(binary, "0101") == 0) return '5';
    else if(strcmp(binary, "0110") == 0) return '6';
    else if(strcmp(binary, "0111") == 0) return '7';
    else if(strcmp(binary, "1000") == 0) return '8';
    else if(strcmp(binary, "1001") == 0) return '9';
    else if(strcmp(binary, "1010") == 0) return 'a';
    else if(strcmp(binary, "1011") == 0) return 'b';
    else if(strcmp(binary, "1100") == 0) return 'c';
    else if(strcmp(binary, "1101") == 0) return 'd';
    else if(strcmp(binary, "1110") == 0) return 'e';
    else if(strcmp(binary, "1111") == 0) return 'f';
    else
    return 'x';
}
void HexadecimalToBinary(char *hex, char binaryString[], unsigned short size){
    char binary[5];
    binaryString[0] = '\0';
    int hex_size = size/4 + (size%4 != 0);
    int s = strlen(hex);
    if(s < hex_size){
        // this is to just extend the hex number to 32 bits if it is smaller than that
        while(s++ < hex_size)
        strcat(binaryString, "0000");
    }else if(s > hex_size) hex += s - hex_size; // since the maximum space is of 32 bits, so if user give a hex number having value greater than 32 bits, we will consider the lower 32 bits
    while(*hex != '\0'){
        hexDigitToBinary(*hex, binary);
        strcat(binaryString, binary);
        hex++;
    }
}
void decimalToBinary(int decimalNumber, char binaryString[], unsigned short size) {
    if (binaryString == NULL) exit_from_program(1);

    // Use bitwise operations to convert decimal to binary
    for (int i = size-1; i >= 0; i--) {
        binaryString[size - 1 - i] = ((decimalNumber >> i) & 1) ? '1' : '0';
    }

    // Null-terminate the string
    binaryString[size] = '\0';
}
void BinarytoHexadecimal(char *binary, char *hex){
    char tmp1[5], tmp2[2];
    int s = strlen(binary);
    int binary_size = (s/4 + s%4)*4; // this formula ensures that the minimum size of the binary string is to be a multiple of 4
    char binary_string[binary_size + 1]; // allocating memory for our string locally
    char *binary_string_ptr = binary_string;
    binary_string_ptr[0] = '\0';
    while(s++ < binary_size)
    strcat(binary_string_ptr, "0");

    strcat(binary_string_ptr, binary);
    hex[0] = '\0';
    tmp2[1] = '\0';
    while(*binary_string_ptr != '\0'){
        tmp1[0] = binary_string_ptr[0];
        tmp1[1] = binary_string_ptr[1];
        tmp1[2] = binary_string_ptr[2];
        tmp1[3] = binary_string_ptr[3];
        tmp1[4] = '\0';
        tmp2[0] = BinaryTohexDigit(tmp1);
        strcat(hex, tmp2);
        binary_string_ptr += 4;
    }
}
void ValueInterpreter(char *value, char binaryString[], unsigned short size){
        if(*value == '\0') exit_from_program(1);

    if(strlen(value) > 2 && value[0] == '0' && value[1] == 'x'){
        if(isHexadecimal(value + 2)){
            HexadecimalToBinary(value+2, binaryString, size);
        }else exit_from_program(1);
    }else{
        if (isdecimal(value)){
            decimalToBinary(atoi(value), binaryString, size);
        }else exit_from_program(1);
    }
}
void RegisterCode(char* r, char *binary){
    if(!isdecimal(r+1)) exit_from_program(1);
    if(atoi(r+1) > 31 || atoi(r+1) < 0){
        printf("Invalid Register\n");
        exit_from_program(1);
    }
    ValueInterpreter(r+1, binary, 5);
}
void WriteToFile(char *code){
    int line_size = 45; // 10(decimal address) + 1(colon)+ 1(space) 8(instruction byte) + 1(null character)
    char buffer[line_size];
    // format will be:
    // LINE_NO: CODE
    sprintf(buffer, "%s: ", LINE_NO_HEX); // LINE_NO:
    if(HEX_PRINT){
        char hex_code[9];
        BinarytoHexadecimal(code, hex_code);
        strcat(buffer, hex_code);
    }
    else strcat(buffer, code);                 // LINE_NO: CODE
    fprintf(OUTPUT_FILE, "%s\n", buffer);
    HexadecimalAdder(LINE_NO_HEX, "1", LINE_NO_HEX);
}
void U_Encoding(char *opcode, char *code, char *binary){
    char *ins_1_0 = "11";
    char *ins_6_2 = opcode; // opcode
    char ins_11_7[6]; // rd
    char ins_31_12[21];
    char *c = strtok(code, " ");
    c = strtok(NULL, " ");

    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_11_7);

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    ValueInterpreter(c, binary, 32);
    strcpy(ins_31_12, binary + 12);

    binary[0] = '\0';
    strcat(binary, ins_31_12);
    strcat(binary, ins_11_7);
    strcat(binary, ins_6_2);
    strcat(binary, ins_1_0);
}
void I_Encoding(char *opcode, char *code, char *binary){
    char *ins_1_0 = "11";
    char *ins_6_2 = opcode; // opcode
    char ins_11_7[6]; // rd
    char ins_14_12[4]; // funct3
    char ins_19_15[6]; // rs1
    char ins_31_20[13]; // imm
    char *c = strtok(code, " ");

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_11_7);

    ins_14_12[0] = '0';
    ins_14_12[1] = '0';
    ins_14_12[2] = '0';
    ins_14_12[3] = '\0';

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_19_15);

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    ValueInterpreter(c, binary, 32);
    strcpy(ins_31_20, binary + 20);

    binary[0] = '\0';
    strcat(binary, ins_31_20);
    strcat(binary, ins_19_15);
    strcat(binary, ins_14_12);
    strcat(binary, ins_11_7);
    strcat(binary, ins_6_2);
    strcat(binary, ins_1_0);
}
void R_Encoding(char *opcode, char *code, char *binary){
    char *ins_1_0 = "11";
    char *ins_6_2 = opcode; // opcode
    char ins_11_7[6]; // rd
    char ins_14_12[4]; // funct3
    char ins_19_15[6]; // rs1
    char ins_24_20[6]; // rs2
    char ins_31_25[8];
    char *c = strtok(code, " ");

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_11_7);

    ins_14_12[0] = '0';
    ins_14_12[1] = '0';
    ins_14_12[2] = '0';
    ins_14_12[3] = '\0';

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_19_15);

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_24_20);

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
void S_Encoding(char *opcode, char *code, char *binary){
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
    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_24_20);


    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    ValueInterpreter(c, binary, 32); // offset

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_19_15);

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
void J_Encoding(char *opcode, char *code, char *binary){
    char *ins_1_0 = "11";
    char *ins_6_2 = opcode; // opcode
    char ins_11_7[6]; // rd
    char ins_19_12[9];
    char ins_31_20[13];
    char *c = strtok(code, " ");
    c = strtok(NULL, " ");

    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_11_7);

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    ValueInterpreter(c, binary, 32);

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
void B_Encoding(char *opcode, char *code, char *binary){
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
    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_19_15);

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    RegisterCode(c, ins_24_20);

    c = strtok(NULL, " ");
    if(c==NULL) exit_from_program(1);
    ValueInterpreter(c, binary, 32); // offset

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
typedef struct OpcodeLookup
{
    const char *opcode;
    void (*func)(char *code, char *mnemonics);
}LookupTable;

void DirectiveType(char *code, char *mnemonics){
    char *c = strtok(code, " ");
    c = strtok(NULL, " ");

    if(strcmp(mnemonics, "#org") == 0){
        if(*c == '-' || *c == '+')
        ++c;
        char binary[33], hex[9];
        ValueInterpreter(c, binary, 32);
        BinarytoHexadecimal(binary, hex);
        strcpy(LINE_NO_HEX, hex);
    }else if(strcmp(mnemonics, "#db") == 0){
        char binary[33], hex[9];
        ValueInterpreter(c, binary, 32);
        WriteToFile(binary);
    }else exit_from_program_with_message("Wrong directive\n", 1);
}

void UpperImmediateType(char *code, char* mnemonics){
    char opcode[7], binary[33];
    if(strcmp(mnemonics, "lui") == 0) strcpy(opcode, "01101");
    else if(strcmp(mnemonics, "auipc") == 0) strcpy(opcode, "00101");
    else exit_from_program_with_message("Wrong Instruction\n", 1);
    U_Encoding(opcode, code, binary);
    WriteToFile(binary);
}

void ImmediateType(char *code, char *mnemonics){
    char opcode[7], binary[33];
    if(strcmp(mnemonics, "addi") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary);
        binary[17] = '0'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "slti") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary);
        binary[17] = '0'; binary[18] = '1'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "sltiu") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary);
        binary[17] = '0'; binary[18] = '1'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "xori") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary);
        binary[17] = '1'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "ori") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary);
        binary[17] = '1'; binary[18] = '1'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "andi") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary);
        binary[17] = '1'; binary[18] = '1'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "slli") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary);
        binary[17] = '0'; binary[18] = '0'; binary[19] = '1';
        binary[0] = '0'; binary[1] = '0'; binary[2] = '0'; binary[3] = '0'; binary[4] = '0'; binary[5] = '0'; binary[6] = '0';
    }
    else if(strcmp(mnemonics, "srli") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary);
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
        binary[0] = '0'; binary[1] = '0'; binary[2] = '0'; binary[3] = '0'; binary[4] = '0'; binary[5] = '0'; binary[6] = '0';
    }
    else if(strcmp(mnemonics, "srai") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary);
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
        binary[0] = '0'; binary[1] = '1'; binary[2] = '0'; binary[3] = '0'; binary[4] = '0'; binary[5] = '0'; binary[6] = '0';
    }
    else exit_from_program_with_message("Wrong Instruction\n", 1);

    WriteToFile(binary);
}

void RegisterType(char *code, char *mnemonics){
    char *opcode = "01100", binary[33];
    R_Encoding(opcode, code, binary);
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
    else exit_from_program_with_message("Wrong Instruction\n", 1);

    WriteToFile(binary);
}

void LoadType(char *code, char *mnemonics){
    char binary[33], *opcode = "00000";
    char *found = strstr(code, "(");
    if(found != NULL)
    *found = ' ';
    found = strstr(code, ")");
    if(found != NULL)
    *found = '\0';

    char *L = strtok(code, " ");
    char *rd = strtok(NULL, " ");
    if(rd == NULL) exit_from_program_with_message("Wrong format\n", 1);
    char *offset = strtok(NULL, " ");
    if(rd == NULL) exit_from_program_with_message("Wrong format\n", 1);
    char *rs1 = strtok(NULL, " ");
    if(rd == NULL) exit_from_program_with_message("Wrong format\n", 1);

    char new_code[32];
    new_code[0] = '\0';
    strcat(new_code, L);
    strcat(new_code, " ");
    strcat(new_code, rd);
    strcat(new_code, " ");
    strcat(new_code, rs1);
    strcat(new_code, " ");
    strcat(new_code, offset);
    
    I_Encoding(opcode, new_code, binary);

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
    }else exit_from_program_with_message("Wrong Instruction\n", 1);

    WriteToFile(binary);
}

void StoreType(char *code, char *mnemonics){
    char binary[33], *opcode = "01000";
    char *found = strstr(code, "(");
    if(found != NULL)
    *found = ' ';
    found = strstr(code, ")");
    if(found != NULL)
    *found = '\0';

    S_Encoding(opcode, code, binary);

    if(strcmp(mnemonics, "sb") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "sh") == 0){
        binary[17] = '0'; binary[18] = '0'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "sw") == 0){
        binary[17] = '0'; binary[18] = '1'; binary[19] = '0';
    }
    else exit_from_program_with_message("Wrong Instruction\n", 1);

    WriteToFile(binary);
}

void JAL(char *code, char* mnemonics){
    char binary[33], *opcode = "11011";
    J_Encoding(opcode, code, binary);
    WriteToFile(binary);
}

void JALR(char *code, char *mnemonics){
    char binary[33], *opcode = "11001";
    I_Encoding(opcode, code, binary);
    WriteToFile(binary);
}

void BranchType(char *code, char *mnemonics){
    char binary[33], *opcode = "11000";

    B_Encoding(opcode, code, binary);

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
    }else exit_from_program_with_message("Wrong Instruction\n", 1);

    WriteToFile(binary);
}

LookupTable opcode_lookup[] = {
    {"#org"  , DirectiveType},
    {"#db"   , DirectiveType},
    {"lui"   , UpperImmediateType},
    {"auipc" , UpperImmediateType},
    {"addi"  , ImmediateType},
    {"slti"  , ImmediateType},
    {"sltiu" , ImmediateType},
    {"xori"  , ImmediateType},
    {"ori"   , ImmediateType},
    {"andi"  , ImmediateType},
    {"slli"  , ImmediateType},
    {"srli"  , ImmediateType},
    {"srai"  , ImmediateType},
    {"add"   , RegisterType},
    {"sub"   , RegisterType},
    {"slt"   , RegisterType},
    {"sltu"  , RegisterType},
    {"xor"   , RegisterType},
    {"or"    , RegisterType},
    {"and"   , RegisterType},
    {"sll"   , RegisterType},
    {"srl"   , RegisterType},
    {"sra"   , RegisterType},
    {"mul"   , RegisterType},
    {"mulh"  , RegisterType},
    {"mulhsu", RegisterType},
    {"mulhu" , RegisterType},
    {"div"   , RegisterType},
    {"divu"  , RegisterType},
    {"rem"   , RegisterType},
    {"remu"  , RegisterType},
    {"lb"    , LoadType},
    {"lh"    , LoadType},
    {"lw"    , LoadType},
    {"lbu"   , LoadType},
    {"lhu"   , LoadType},
    {"sb"    , StoreType},
    {"sh"    , StoreType},
    {"sw"    , StoreType},
    {"jal"   , JAL},
    {"jalr"  , JALR},
    {"beq"   , BranchType},
    {"bne"   , BranchType},
    {"blt"   , BranchType},
    {"bge"   , BranchType},
    {"bltu"  , BranchType},
    {"bgeu"  , BranchType},
};

void Assembler(char *instruction){
    if(*instruction == '\0') return;
    char line[strlen(instruction) + 1];
    strcpy(line, instruction);
    char *mnemonics = strtok(line, " ");

    int n = sizeof(opcode_lookup)/sizeof(opcode_lookup[0]);
    int i;
    for(i = 0; i<n ; i++){
        if (strcmp(opcode_lookup[i].opcode, mnemonics) == 0){
            opcode_lookup[i].func(instruction, mnemonics);
            break;
        }
    }

    if(i == n) exit_from_program(1);
}

int main(int argc, char const *argv[])
{

    if (argc < 2){
        // no input file is provided
        printf("ERROR: input file not provided");
    }else{
        
        char buffer[32];
        // Open the file in read mode
        INPUT_FILE = fopen(argv[1], "r");
        // Check if the file was successfully opened
        if (INPUT_FILE == NULL) {
            perror("Error opening the file");
            exit_from_program(0);
        }

        // argument for getting the comment symbol
        int i = find(argc, argv, "-c");
        if(i != -1)
        COMMENT = (char*)argv[i+1];
        
        // argument for getting the comment symbol
        i = find(argc, argv, "-hex");
        if(i != -1)
        HEX_PRINT = 1;


        // argument for getting the name of output file
        i = find(argc, argv, "-o");
        if(i != -1){
            // if output file is provided use it
            if(i+1 < argc){
                char *OUT = (char*)argv[i+1];
                OUTPUT_FILE = fopen(OUT, "w");
            }
            else{
                printf("output file name not provided");
                exit_from_program(0);
            }
        }else{
            // if output file is not provided, use the name of input file
            char OUT[strlen(argv[1]) + 4];
            strcpy(OUT, argv[1]);
            char *token = strtok(OUT, ".");
            strcpy(OUT, token);
            strcat(OUT, ".out");
            OUTPUT_FILE = fopen(OUT, "w");
        }

        // reading every instruction
        while (fgets(buffer, sizeof(buffer), INPUT_FILE) != NULL) {
            assembly_file_line_no++;
            char *line = buffer;
            Preprocess(&line);
            Assembler(line);
            // printf("%s", line);
        }

        // Close the file
        fclose(INPUT_FILE);
        fclose(OUTPUT_FILE);
    }

    return 0;
}

