typedef struct OpcodeLookup
{
    const char *opcode;
    void (*func)(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int));
}LookupTable;

void UpperImmediateType(char *code, char* mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int));
void ImmediateType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int));
void RegisterType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int));
void LoadType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int));
void StoreType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int));
void JAL(char *code, char* mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int));
void JALR(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int));
void BranchType(char *code, char *mnemonics, void(*Writefunc)(char*), void(*Error)(char*, int));