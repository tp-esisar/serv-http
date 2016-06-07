#include "fcgi.h"


int main() {
    FCGI_Header head = make_FCGI_Header(FCGI_GET_VALUES,FCGI_NULL_REQUEST_ID,8,0);
    FCGI_BeginRequestRecord* begin = make_FCGI_BeginRequestRecord(1,FCGI_RESPONDER,0);
    
    return 0;
}