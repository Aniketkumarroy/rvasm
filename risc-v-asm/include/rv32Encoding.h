#ifndef ENCODING
#define ENCODING
void U_Encoding(char *opcode, char *code, char *binary, void (*error)(char*, int));
void I_Encoding(char *opcode, char *code, char *binary, void (*error)(char*, int));
void R_Encoding(char *opcode, char *code, char *binary, void (*error)(char*, int));
void S_Encoding(char *opcode, char *code, char *binary, void (*error)(char*, int));
void J_Encoding(char *opcode, char *code, char *binary, void (*error)(char*, int));
void B_Encoding(char *opcode, char *code, char *binary, void (*error)(char*, int));
#endif