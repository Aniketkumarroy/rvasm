#include<string.h>
#include<ctype.h>
#include<stdint.h>
#include<math.h>
#include<stdlib.h>

#include"hex.h"

void hexDigitAdder(char a, char b, char c, char *s, char *carry){
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
void hexDigitToBinary(char hexDigit, char *binary, void (*error)(char*)) {
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
        default: if (error != NULL) error("Wrong hex digit provided\n");
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
        hexDigitToBinary(*hex, binary, NULL);
        strcat(binaryString, binary);
        hex++;
    }
}
void decimalToBinary(int decimalNumber, char binaryString[], unsigned short size, void (*error)(char*)) {
    if (binaryString == NULL && error != NULL) error("NO binary no. is provided\n");

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
int hexDigitToDecimal(char c){

    c = (char)tolower(c);
    switch (c)
    {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'a': return 10;
    case 'b': return 11;
    case 'c': return 12;
    case 'd': return 13;
    case 'e': return 14;
    case 'f': return 15;
    default: return 0;
    }
}
uint32_t hexaDecimalToDecimal(char hex[9]){
    int s = strlen(hex);
    uint32_t value = 0;
    for(int i = 0; i< s; i++)
    value += hexDigitToDecimal(hex[s - i - 1]) * pow(16, i);

    return value;
}
int32_t hexaDecimalToSignedDecimal(char *hex, int size){
    char binary[32];
    int32_t value = 0;
    // HexadecimalAdder(hex, "1", nhex);
    HexadecimalToBinary(hex, binary, size);
    int s = strlen(binary), tmp;
    for(int i = 0; i<s; i++)
    {
        tmp = (binary[s - i - 1] == '0') ? 0 : 1;
        if(i+1 < s)
        value += tmp*pow(2, i);
        else
        value -= tmp*pow(2, i);
    }

    return value;
}
int32_t BinaryToSignedDecimal(char *binary){
    int32_t value = 0;
    int s = strlen(binary), tmp;
    for(int i = 0; i<s; i++)
    {
        tmp = (binary[s - i - 1] == '0') ? 0 : 1;
        if(i+1 < s)
        value += tmp*pow(2, i);
        else
        value -= tmp*pow(2, i);
    }

    return value;
}
uint32_t BinaryToDecimal(char *binary){
    uint32_t value = 0;
    int s = strlen(binary), tmp;
    for(int i = 0; i<s; i++)
    {
        tmp = (binary[s - i - 1] == '0') ? 0 : 1;
        value += tmp*pow(2, i);
    }

    return value;
}
void ValueInterpreter(char *value, char binaryString[], unsigned short size, void (*error)(char*)){
        if(*value == '\0' && error != NULL) error("empty value provided\n");

    if(strlen(value) > 2 && value[0] == '0' && value[1] == 'x'){
        if(isHexadecimal(value + 2)){
            HexadecimalToBinary(value+2, binaryString, size);
        }else if(error != NULL) error("Invalid Hex value");
    }else{
        if (isdecimal(value)){
            decimalToBinary(atoi(value), binaryString, size, error);
        }else if(error != NULL) error("Invalid Decimal value");
    }
}