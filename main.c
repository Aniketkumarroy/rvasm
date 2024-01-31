#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"hex.h"
#include"rv32Encoding.h"
#include"utilities.h"

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
    HexadecimalAdder(LINE_NO_HEX, "4", LINE_NO_HEX);
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
        ValueInterpreter(c, binary, 32, NULL);
        BinarytoHexadecimal(binary, hex);
        strcpy(LINE_NO_HEX, hex);
    }else if(strcmp(mnemonics, "#db") == 0){
        char binary[33];
        ValueInterpreter(c, binary, 32, NULL);
        WriteToFile(binary);
    }else exit_from_program_with_message("Wrong directive\n", 1);
}

void UpperImmediateType(char *code, char* mnemonics){
    char opcode[7], binary[33];
    if(strcmp(mnemonics, "lui") == 0) strcpy(opcode, "01101");
    else if(strcmp(mnemonics, "auipc") == 0) strcpy(opcode, "00101");
    else exit_from_program_with_message("Wrong Instruction\n", 1);
    U_Encoding(opcode, code, binary, NULL);
    WriteToFile(binary);
}

void ImmediateType(char *code, char *mnemonics){
    char opcode[7], binary[33];
    if(strcmp(mnemonics, "addi") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, NULL);
        binary[17] = '0'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "slti") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, NULL);
        binary[17] = '0'; binary[18] = '1'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "sltiu") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, NULL);
        binary[17] = '0'; binary[18] = '1'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "xori") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, NULL);
        binary[17] = '1'; binary[18] = '0'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "ori") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, NULL);
        binary[17] = '1'; binary[18] = '1'; binary[19] = '0';
    }
    else if(strcmp(mnemonics, "andi") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, NULL);
        binary[17] = '1'; binary[18] = '1'; binary[19] = '1';
    }
    else if(strcmp(mnemonics, "slli") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, NULL);
        binary[17] = '0'; binary[18] = '0'; binary[19] = '1';
        binary[0] = '0'; binary[1] = '0'; binary[2] = '0'; binary[3] = '0'; binary[4] = '0'; binary[5] = '0'; binary[6] = '0';
    }
    else if(strcmp(mnemonics, "srli") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, NULL);
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
        binary[0] = '0'; binary[1] = '0'; binary[2] = '0'; binary[3] = '0'; binary[4] = '0'; binary[5] = '0'; binary[6] = '0';
    }
    else if(strcmp(mnemonics, "srai") == 0){
        strcpy(opcode, "00100");
        I_Encoding(opcode, code, binary, NULL);
        binary[17] = '1'; binary[18] = '0'; binary[19] = '1';
        binary[0] = '0'; binary[1] = '1'; binary[2] = '0'; binary[3] = '0'; binary[4] = '0'; binary[5] = '0'; binary[6] = '0';
    }
    else exit_from_program_with_message("Wrong Instruction\n", 1);

    WriteToFile(binary);
}

void RegisterType(char *code, char *mnemonics){
    char *opcode = "01100", binary[33];
    R_Encoding(opcode, code, binary, NULL);
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
    
    I_Encoding(opcode, new_code, binary, NULL);

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

    S_Encoding(opcode, code, binary, NULL);

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
    J_Encoding(opcode, code, binary, NULL);
    WriteToFile(binary);
}

void JALR(char *code, char *mnemonics){
    char binary[33], *opcode = "11001";
    I_Encoding(opcode, code, binary, NULL);
    WriteToFile(binary);
}

void BranchType(char *code, char *mnemonics){
    char binary[33], *opcode = "11000";

    B_Encoding(opcode, code, binary, NULL);

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
            Preprocess(&line, COMMENT);
            Assembler(line);
            // printf("%s", line);
        }

        // Close the file
        fclose(INPUT_FILE);
        fclose(OUTPUT_FILE);
    }

    return 0;
}

