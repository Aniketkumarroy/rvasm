#ifndef HEX
#define HEX
#include<stdint.h>

void hexDigitAdder(char a, char b, char c, char *s, char *carry);
void HexadecimalAdder(char *A, char *B, char *S);
int isHexadecimal(char *c);
int isdecimal(char *c);
void hexDigitToBinary(char hexDigit, char *binary, void (*error)(char*, int));
char BinaryTohexDigit(char *binary);
void HexadecimalToBinary(char *hex, char binaryString[], unsigned short size);
void decimalToBinary(int decimalNumber, char binaryString[], unsigned short size, void (*error)(char*, int));
void BinarytoHexadecimal(char *binary, char *hex);
int hexDigitToDecimal(char c);
uint32_t hexaDecimalToDecimal(char hex[9]);
int32_t hexaDecimalToSignedDecimal(char hex[9], int size);
int32_t BinaryToSignedDecimal(char *binary);
uint32_t BinaryToDecimal(char *binary);
void ValueInterpreter(char *value, char binaryString[], unsigned short size, void (*error)(char*, int));
#endif