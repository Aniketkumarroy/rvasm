#ifndef	UTIL
#define UTIL

void RemoveSpaces(char **line);
void RemoveComments(char *line, char *comment);
void Preprocess(char **line, char *comment);
void RegisterCode(char* r, char *binary, void (*error)(char*));
#endif
