#include "fcgi.h"
#include "StringL.h"
#include <stdio.h>
#include <stdlib.h>

void* safeMalloc(size_t size) {
    void* temp = malloc(size);
    if(temp == NULL) {
        fprintf(stderr,"erreur malloc fcgi.c");
        exit(-1);
    }
    return temp;
}
    

FCGI_Header make_FCGI_Header(unsigned char type, unsigned short requestId, unsigned short contentLength, unsigned char paddingLength) {
    return (FCGI_Header){FCGI_VERSION_1,type,requestId,contentLength,paddingLength,0};
}

FCGI_UnknownTypeRecord* make_FCGI_UnknownTypeRecord(unsigned char type) {
    FCGI_Header header = make_FCGI_Header(FCGI_UNKNOWN_TYPE,FCGI_NULL_REQUEST_ID,8,0);
    FCGI_UnknownTypeRecord* ret = safeMalloc(sizeof(FCGI_Header)+8);
    ret->header = header;
    ret->body.type = type;
    return ret;
}

FCGI_UnknownTypeRecord* make_FCGI_UnknownTypeRecord(unsigned char type) {
    FCGI_Header header = make_FCGI_Header(FCGI_UNKNOWN_TYPE,FCGI_NULL_REQUEST_ID,8,0);
    FCGI_UnknownTypeRecord* ret = safeMalloc(sizeof(FCGI_Header)+8);
    ret->header = header;
    ret->body.type = type;
    return ret;
}

FCGI_BeginRequestRecord* make_FCGI_BeginRequestRecord(unsigned short requestId,unsigned short role, unsigned char flags) {
    FCGI_Header header = make_FCGI_Header(FCGI_BEGIN_REQUEST,requestId,8,0);
    FCGI_BeginRequestRecord* ret = safeMalloc(sizeof(FCGI_Header)+8);
    ret->header = header;
    ret->body.role = role;
    ret->body.flags = flags;
    return ret;
}

FCGI_EndRequestRecord* make_FCGI_EndRequestRecord(unsigned short requestId,unsigned long int appStatus, unsigned char protocolStatus) {
    FCGI_Header header = make_FCGI_Header(FCGI_END_REQUEST,requestId,8,0);
    FCGI_EndRequestRecord* ret = safeMalloc(sizeof(FCGI_Header)+8);
    ret->header = header;
    ret->body.appStatus = appStatus;
    ret->body.protocolStatus = protocolStatus;
    return ret;
}

FCGI_ParamWrapper* make_FCGI_ParamWrapper(StringL name, StringL value, unsigned short requestId) {
    FCGI_Header header;
    FCGI_ParamWrapper* ret;
    unsigned short longueur;
    if(name.len<=127 && value.len <=127) {
        longueur = 2 + name.len + value.len;
        ret = safeMalloc(longueur + 8 + 1);
        ret->variente = 11;
    }
    else if(name.len>127 && value.len <=127) {
        longueur =  + name.len + value.len;
        ret = safeMalloc(longueur + 8 + 1);
        ret->variente = 41;
    }
    else if(name.len<=127 && value.len >127) {
        longueur =  + name.len + value.len;
        ret = safeMalloc(longueur + 8 + 1);
        ret->variente = 14;
    }
    else if(name.len>127 && value.len >127) {
        longueur =  + name.len + value.len;
        ret = safeMalloc(longueur + 8 + 1);
        ret->variente = 44;
    }
    header = make_FCGI_Header(FCGI_PARAMS, requestId, longueur, 0);
    ret->data.header = make_FCGI_Header(FCGI_PARAMS, requestId, longueur, 0);
    return ret;
}