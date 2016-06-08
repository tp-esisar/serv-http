#include "fcgi.h"
#include "StringL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket.h"
#include "cJSON.h"
#include <arpa/inet.h>

void* safeMalloc(size_t size) {
    void* temp = malloc(size);
    if(temp == NULL) {
        fprintf(stderr,"erreur malloc fcgi.c\n");
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
    unsigned short baseLen = sizeof(unsigned char)+sizeof(unsigned long long);
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
        memcpy((char*)&(temp->nameAndValue[name.len]),value.s,value.len);
    }
    else if(name.len>127 && value.len <=127) {
        longueur = 5 + name.len + value.len;
        ret = safeMalloc(longueur+baseLen);
        ret->variente = 41;
        ret->totalLen = longueur;
        FCGI_NameValuePair41* temp = (FCGI_NameValuePair41*)&(ret->data);
        temp->nameLength = htonl(name.len);
        temp->valueLength = value.len;
        memcpy(&(temp->nameAndValue),name.s,name.len);
        memcpy((char*)&(temp->nameAndValue)+name.len,value.s,value.len);
    }
    else if(name.len<=127 && value.len >127) {
        longueur = 5 + name.len + value.len;
        ret = safeMalloc(longueur + baseLen);
        ret->variente = 14;
        ret->totalLen = longueur;
        FCGI_NameValuePair14* temp = (FCGI_NameValuePair14*)&(ret->data);
        temp->nameLength = name.len;
        temp->valueLength = htonl(value.len);
        memcpy(&(temp->nameAndValue),name.s,name.len);
        memcpy((char*)&(temp->nameAndValue)+name.len,value.s,value.len);
    }
    else if(name.len>127 && value.len >127) {
        longueur = 8 + name.len + value.len;
        ret = safeMalloc(longueur + baseLen);
        ret->variente = 44;
        ret->totalLen = longueur;
        FCGI_NameValuePair44* temp = (FCGI_NameValuePair44*)&(ret->data);
        temp->nameLength = htons(name.len);
        temp->valueLength = htons(value.len);
        memcpy(&(temp->nameAndValue),name.s,name.len);
        memcpy((char*)&(temp->nameAndValue)+name.len,value.s,value.len);
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
            fprintf(stderr,"erreur socket fcgi.c\n");
            free(record);
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
            fprintf(stderr,"erreur socket fcgi.c\n");
            return -1;
        }
        int ret2 = sendStreamChunk(sock, type, requestId, s2);
        if(ret2 == -1) {
            fprintf(stderr,"erreur socket fcgi.c\n");
            return -1;
        }
    }
    return buffer.len;
}
int sendEndStream(int sock,unsigned char type, unsigned short requestId) {
    FCGI_Record_generic* record;
    record = safeMalloc(sizeof(FCGI_Header));
    record->header = make_FCGI_Header(type,requestId,0,0);
    int errsocket = put_fcgi(sock, record);
    if(errsocket == -1) {
        fprintf(stderr,"erreur socket fcgi.c\n");
        free(record);
        return -1;
    }
    return 0;
    free(record);
}


StringL FCGI_Request(StringL stdinbuff, cJSON* param) {
    if(param->type != cJSON_Object) {
        fprintf(stderr,"erreur fcgi conf json not object\n");
        return (StringL){NULL,0};
    }
    FCGI_BeginRequestRecord* begin;
	begin = malloc(sizeof(FCGI_BeginRequestRecord));
	begin->header = make_FCGI_Header(FCGI_BEGIN_REQUEST, 1, 8, 0);
	begin->body.role = htons(FCGI_RESPONDER);
	begin->body.flags = htons(0);
	
	
    int sock = creat_fcgi("127.0.0.1",9000);
    //envoi du begin
    put_fcgi(sock, (FCGI_Record_generic*)begin);
    //envoi des param
    cJSON* iter;
    cJSON_ArrayForEach(iter, param) {
        StringL name = fromRegularString(iter->string);
        StringL value = fromRegularString(iter->valuestring);
        FCGI_ParamWrapper* param = make_FCGI_ParamWrapper(name,value,1);
        StringL buff = (StringL){(char*)&(param->data),param->totalLen};
        
        int err = sendStreamChunk(sock,FCGI_PARAMS,1,buff);
        if(err == -1) {
            fprintf(stderr,"erreur sendStreamChunk fcgi.c\n");
            return (StringL){NULL,0};
        }
        free(param);
        free(name.s);
        free(value.s);
    }
    int err = sendEndStream(sock,FCGI_PARAMS,1);
    if(err == -1) {
        fprintf(stderr,"erreur sendEndStream fcgi.c\n");
        return (StringL){NULL,0};
    }
    
    //envoi du STDIN
    err = sendStreamChunk(sock,FCGI_STDIN,1,stdinbuff);
    if(err == -1) {
        fprintf(stderr,"erreur sendEndStream fcgi.c\n");
        return (StringL){NULL,0};
    }
    err = sendEndStream(sock,FCGI_STDIN,1);
    if(err == -1) {
        fprintf(stderr,"erreur sendEndStream fcgi.c\n");
        return (StringL){NULL,0};
    }
    
    close(sock);
    return stdinbuff;
    
}



