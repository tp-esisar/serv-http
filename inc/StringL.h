#ifndef STRINGL_H
#define STRINGL_H

typedef struct {
    char* s;
    unsigned int len;
} StringL;

int stringLEq(StringL s1, StringL s2);
char* toRegularString(StringL s);
StringL fromRegularString(char* s);
StringL extract_fieldValue(StringL field);
void extract_stringL (StringL field, void (*callback)(char* found, unsigned int len));

#endif
