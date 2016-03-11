#ifndef STRINGL
#define STRINGL

typedef struct {
    char* s;
    unsigned int len;
} StringL;

int stringLEq(StringL s1, StringL s2);
char* toRegularString(StringL s);
StringL fromRegularString(char* s);


#endif