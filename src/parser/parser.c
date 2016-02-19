#include "parser.h"

#define gtch(a) (*a.s)
#define pgtc(a) (*a->s)


char pstepch(StringL* s) {
  char r = pgtc(s);
  s->s++;
  s->len--;
  return r;
}

StringL P_method(StringL* pbuff) {
  StringL window;
  while (pstepch(pbuff) != ' ') {
    
  }
  
  return ;
}

int parser (char *buf,unsigned int len,char *search, void (*callback)()) {
  StringL buff = {buf,len};
  int readnb = 0;
  char test;
  
  test = gtch(buff);
  readnb = P_method(&buff);
  
  return -1;
}

