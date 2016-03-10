#include "stringL.h"
#include <string.h>
#include <stdlib.h>

int stringLEq(StringL s1, StringL s2) {
  if (s1.len != s2.len)
    return 0;
  else {
    for (int i=0;i<s1.len;i++) {
      if (s1.s[i] != s2.s[i])
        return 0;
    }
    return 1;
  }
}

char* toRegularString(StringL s) {
  char* r = NULL;
  r = malloc((s.len + 1)*sizeof(char));
  if (r == NULL) return NULL;
  else {
    for (int i = 0;i<s.len;i++)
      r[i] = s.s[i];
    r[s.len] = '\0';
    return r;
  }
}

StringL fromRegularString(char* s) {
  int len = strlen(s);
  StringL r = {NULL,len};
  r.s = malloc((len)*sizeof(char));
  for (int i = 0;i<len;i++)
    r.s[i] = s[i];
  return r;
}