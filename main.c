#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<hex.h>
#include<rv32Encoding.h>
#include<InstructionEncoding.h>
#include<utilities.h>

FILE *INPUT_FILE = NULL;
FILE *OUTPUT_FILE = NULL;
unsigned int LINE_NO = 0;
char LINE_NO_HEX[9] = "00000000";
char *COMMENT = ";";
int HEX_PRINT = 0;

unsigned int assembly_file_line_no = 0;

void exit_from_program_with_message(char *msg, int e){
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

void DirectiveType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int)){
    char *c = strtok(code, " ");
    c = strtok(NULL, " ");

    if(strcmp(mnemonics, "#org") == 0){
        if(*c == '-' || *c == '+')
        ++c;
        char binary[33], hex[9];
        ValueInterpreter(c, binary, 32, Error);
        BinarytoHexadecimal(binary, hex);
        strcpy(LINE_NO_HEX, hex);
    }else if(strcmp(mnemonics, "#db") == 0){
        char binary[33];
        ValueInterpreter(c, binary, 32, Error);
        WriteToFile(binary);
    }else {
        if(Error != NULL)
        exit_from_program_with_message("Wrong directive\n", 1);
    }
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
            opcode_lookup[i].func(instruction, mnemonics, WriteToFile, exit_from_program_with_message);
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
        if(i != -1) {
            if(i + 1 < argc)
            COMMENT = (char*)argv[i+1];
            else {
                printf("comment token not provided");
                exit_from_program(0);
            }
        }
        
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

