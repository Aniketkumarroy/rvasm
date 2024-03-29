#ifndef	UTIL
#define UTIL

extern const char* HELP;
void RemoveSpaces(char **line);
void RemoveComments(char *line, char *comment);
void Preprocess(char **line, char *comment);
void RegisterCode(char* r, char *binary, void (*error)(char*, int));
struct Queue{
    char** arr;
    int str_size;
    int size;
    int no_elements;
    int r;
    int f;
};
int isFull(struct Queue* q);
int isEmpty(struct Queue* q);
void enqueue(struct Queue* q, char* val);
char* dequeue(struct Queue* q);
void InitializeQueue(struct Queue *q, int size, int str_size, void (*error)(char*, int));
void DeleteQueue(struct Queue *q);
#endif
