#include "fcgi.h"
#include "StringL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket.h"
#include "cJSON.h"

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
    FCGI_ParamWrapper* ret;
    unsigned short baseLen = sizeof(unsigned char) + sizeof(unsigned long long);
    unsigned short longueur;
    if(name.len<=127 && value.len <=127) {
        longueur = 2 + name.len + value.len;
        ret = safeMalloc(longueur+baseLen);
        ret->variente = 11;
        ret->totalLen = longueur;
        FCGI_NameValuePair11* temp = (FCGI_NameValuePair11*)&(ret->data);
        temp->nameLength = name.len;
        temp->valueLength = value.len;
        memcpy(&(temp->nameAndValue),name.s,name.len);
        memcpy((char*)&(temp->nameAndValue)+name.len,value.s,value.len);
    }
    else if(name.len>127 && value.len <=127) {
        longueur = 5 + name.len + value.len;
        ret = safeMalloc(longueur+baseLen);
        ret->variente = 41;
        ret->totalLen = longueur;
        FCGI_NameValuePair41* temp = (FCGI_NameValuePair41*)&(ret->data);
        temp->nameLength = name.len;
        temp->valueLength = value.len;
        memcpy(temp+5,name.s,name.len);
        memcpy(temp+5+name.len,value.s,value.len);
    }
    else if(name.len<=127 && value.len >127) {
        longueur = 5 + name.len + value.len;
        ret = safeMalloc(longueur + baseLen);
        ret->variente = 14;
        ret->totalLen = longueur;
        FCGI_NameValuePair14* temp = (FCGI_NameValuePair14*)&(ret->data);
        temp->nameLength = name.len;
        temp->valueLength = value.len;
        memcpy(temp+5,name.s,name.len);
        memcpy(temp+5+name.len,value.s,value.len);
    }
    else if(name.len>127 && value.len >127) {
        longueur = 8 + name.len + value.len;
        ret = safeMalloc(longueur + baseLen);
        ret->variente = 44;
        ret->totalLen = longueur;
        FCGI_NameValuePair44* temp = (FCGI_NameValuePair44*)&(ret->data);
        temp->nameLength = name.len;
        temp->valueLength = value.len;
        memcpy(temp+8,name.s,name.len);
        memcpy(temp+8+name.len,value.s,value.len);
    }
    return ret;
}



int sendStreamChunk(int sock, unsigned char type, unsigned short requestId, StringL buffer) {
    FCGI_Record_generic* record;
    if(buffer.len<=65535) {
        record = safeMalloc(buffer.len + sizeof(FCGI_Header));
        record->header = make_FCGI_Header(type,requestId,buffer.len,0);
        memcpy(&(record->dataAndPad),buffer.s,buffer.len);
        int errsocket = put_fcgi(sock, record);
        if(errsocket == -1) {
            fprintf(stderr,"erreur socket fcgi.c");
            return -1;
        }
        free(record);
    }
    else {
        StringL s1;
        StringL s2;
        s1.s = buffer.s;
        s1.len = 65535;
        s2.s = buffer.s + 65535;
        s2.len = buffer.len - 65535;
        int ret1 = sendStreamChunk(sock, type, requestId, s1);
        if(ret1 == -1) {
            fprintf(stderr,"erreur socket fcgi.c");
            return -1;
        }
        int ret2 = sendStreamChunk(sock, type, requestId, s2);
        if(ret2 == -1) {
            fprintf(stderr,"erreur socket fcgi.c");
            return -1;
        }
    }
    return buffer.len;
}


StringL FCGI_Request(StringL stdinbuff, cJSON* param) {
    if(param->type != cJSON_Object) {
        fprintf(stderr,"erreur fcgi conf json not object");
        return (StringL){NULL,0};
    }
    cJSON* iter;
    cJSON_ArrayForEach(iter, param) {
        StringL name = fromRegularString(iter->string);
        StringL value = fromRegularString(iter->valuestring);
        FCGI_ParamWrapper* param = make_FCGI_ParamWrapper(name,value,1);
        printf("variente : %d\ntotalLength : %d\n",param->variente, param->totalLen);
        free(param);
        free(name.s);
        free(value.s);
        
    }
    return stdinbuff;
    
}

