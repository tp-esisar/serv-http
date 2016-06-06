#include "fcgi.h"
#include "StringL.h"

FCGI_Header make_FCGI_Header(unsigned char type, unsigned short requestId, unsigned short contentLength, unsigned char paddingLength) {
    return (FCGI_Header){FCGI_VERSION_1,type,requestId,contentLength,paddingLength,0};
}

FCGI_UnknownTypeRecord make_FCGI_UnknownTypeRecord(unsigned char type) {
    FCGI_Header header = make_FCGI_Header(FCGI_UNKNOWN_TYPE,FCGI_NULL_REQUEST_ID,8,0);
    return (FCGI_UnknownTypeRecord){header,(FCGI_UnknownTypeBody){type,0,0,0,0,0,0,0}};
}

FCGI_BeginRequestRecord make_FCGI_BeginRequestRecord(unsigned short requestId,unsigned short role, unsigned char flags) {
    FCGI_Header header = make_FCGI_Header(FCGI_BEGIN_REQUEST,requestId,8,0);
    return (FCGI_BeginRequestRecord){header,(FCGI_BeginRequestBody){role,flags,0,0,0,0,0}};
}

FCGI_EndRequestRecord make_FCGI_EndRequestRecord(unsigned short requestId,unsigned long int appStatus, unsigned char protocolStatus) {
    FCGI_Header header = make_FCGI_Header(FCGI_END_REQUEST,requestId,8,0);
    return (FCGI_EndRequestRecord){header,(FCGI_EndRequestBody){appStatus,protocolStatus,0,0,0}};
}

FCGI_ParamWrapper make_FCGI_ParamWrapper(StringL name, StringL value, unsigned short requestId) {
    FCGI_Header header;
    FCGI_ParamWrapper ret;
    unsigned short longueur;
    if(name.len<=127 && value.len <=127) {
        longueur = 2 + name.len + value.len;
    }
    header = make_FCGI_Header(FCGI_PARAMS, requestId, 2 + name.len + value.len);
    return ret;
}