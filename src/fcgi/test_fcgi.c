#include "fcgi.h"


int main() {
    FCGI_Header head = make_FCGI_Header(FCGI_GET_VALUES,FCGI_NULL_REQUEST_ID,8,0);
    FCGI_NameValuePair11 p11;
    return 0;
}