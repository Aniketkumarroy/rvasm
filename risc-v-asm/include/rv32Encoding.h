#ifndef ENCODING
#define ENCODING
void U_Encoding(char *opcode, char *code, char *binary, void (*error)(char*));
void I_Encoding(char *opcode, char *code, char *binary, void (*error)(char*));
void R_Encoding(char *opcode, char *code, char *binary, void (*error)(char*));
void S_Encoding(char *opcode, char *code, char *binary, void (*error)(char*));
void J_Encoding(char *opcode, char *code, char *binary, void (*error)(char*));
void B_Encoding(char *opcode, char *code, char *binary, void (*error)(char*));
#endif