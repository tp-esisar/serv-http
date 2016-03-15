#include "parseur.h"
#include "reader.h"



int parser (char *buf,unsigned int len,char *search, void (*callback)()) {
  
  return -1;
}

parse_return parse_HTTP_message(StringL* buff) {
    StringL startLine;
    StringL headerField;
    StringL messageBody;
    read_return rr;
    
    rr=read(start_line,buff);
    if(rr.state==FAIL) {
        return (parse_return){FAIL,
    }
}
