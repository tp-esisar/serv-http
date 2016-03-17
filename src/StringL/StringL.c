#include "StringL.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int stringLEq(StringL s1, StringL s2) {
    int i;
  if (s1.len != s2.len)
    return 0;
  else {
    for (i=0;i<s1.len;i++) {
      if (s1.s[i] != s2.s[i])
        return 0;
    }
    return 1;
  }
}

char* toRegularString(StringL s) {
    int i;
  char* r = NULL;
  r = malloc((s.len+1)*sizeof(char));
  if (r == NULL) return NULL;
  else {
    for (i = 0;i<s.len;i++)
      r[i] = s.s[i];
    r[s.len] = '\0';
    return r;
  }
}

StringL fromRegularString(char* s) {
    int i;
  int len = strlen(s);
  StringL r = {NULL,len};
  r.s = malloc((len)*sizeof(char));
  for (i = 0;i<len;i++)
    r.s[i] = s[i];
  return r;
}

StringL extract_fieldValue(StringL field)
{
    int i = 0, j = field.len-1;
    while(field.s[i++] != ':');
	while(field.s[i] == ' ' || field.s[i] == '\t')
		i++ ;
	while(field.s[j] == ' ' || field.s[j] == '\t')
		j-- ;
	field.len = &(field.s[j]) - &(field.s[i]) +1;
	field.s = &(field.s[i]);
    return field;
}
