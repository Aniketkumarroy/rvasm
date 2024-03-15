#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include"hex.h"
#include"utilities.h"

const char* HELP = "rvasm is a assembler for RISC-V 32-bit instruction set architecture. \n"
                   "It supports assembling RISC-V assembly code into machine code.\n\n"
                   "Usage:\n"
                   "./rvasm [source file] | [-o output file] | [-c comment string] | [-hex hexadecimal format] | [-e little endian] | [-f format]\n\n"
                   "Options:\n"
                   "-o, output file name, optionsl flag, default is <input file name>.out\n"
                   "-c, comment string, string that will be interpreted as a comment, optionsl flag, default is ';'\n"
                   "-hex, hexadecimal, to generate outpurt in hexadecimal number, optionsl flag, by default it generates output in binary numbers\n"
                   "-e, little endian, it produces output in Little Endian memory model, optionsl flag, by default big Endian is used\n"
                   "-f, format, it specifies how many bytes separated by spaces to be printed in each line of output file, optionsl flag, by default whole instruction is printed in a line without any spaces\n"
                   "-h, -help, help flag\n";

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
void Preprocess(char **line, char *comment){
    RemoveSpaces(line);             // Removing leading and trailing spaces
    RemoveComments(*line, comment); // Removing comments
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
void RegisterCode(char* r, char *binary, void (*error)(char*, int)){
    if(!isdecimal(r+1) && error != NULL) error("Invalid Register\n", 1);
    if((atoi(r+1) > 31 || atoi(r+1) < 0) && error != NULL){
        error("Register does not exist in RV32M\n", 1);
    }
    ValueInterpreter(r+1, binary, 5, error);
}
int isFull(struct Queue* q){
    if ((q->r+1)%q->size == q->f)
    return 1;
    return 0;
}
int isEmpty(struct Queue* q){
    if ((q->f)%q->size == q->r)
    return 1;
    return 0;
}

void enqueue(struct Queue* q, char* val){
    if (!isFull(q)){
        q->r = (q->r + 1)%q->size;
        strcpy(q->arr[q->r], val);
        q->no_elements++;
    }
}
char* dequeue(struct Queue* q){
    // don't worry if we are returning a pointer, this pointer is referring to a 
    // initialized location in heap, so it is not a dangling pointer
    if (!isEmpty(q)){
        q->f = (q->f + 1)%q->size;
        char* val = q->arr[q->f];
        q->no_elements--;
        return val;
    }
    return NULL;
}
void InitializeQueue(struct Queue *q, int size, int str_size, void (*error)(char*, int)){
    q->size = size;
    q->str_size = str_size; // 8(1 for each 8 binary bits) + 1(null character)
    q->r = size - 1;
    q->f = size - 1;
    q->no_elements = 0;

    q->arr = (char**)malloc(q->size*sizeof(char**));
    if(q->arr == NULL) error("cannot allocate memory for file output buffer", 0);
    
    for(int i = 0; i< q->size; i++) {
        q->arr[i] = (char*)malloc(q->str_size*sizeof(char*));
        if(q->arr[i] == NULL) error("cannot allocate memory for file output buffer", 0);
    }
}
void DeleteQueue(struct Queue *q){
    for(int i = 0; i< q->size; i++)
    free(q->arr[i]);

    free(q->arr);
}