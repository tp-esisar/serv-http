#include "header-getter.h"
#include "reader.h"



#define free_builder(T) void free_ ## T(T* obj) {\
    if (obj != NULL) {\
        free_ ## T(obj->next);\
        free(obj);\
    }\
}

free_builder(Host_HS)
Host_HS* get_Host(mapStruct* map) {
    
    Host_HS* headerList = NULL;
    int error = 0;
    void callback(char* buff, unsigned int len) {
        Host_HS* temp = malloc(sizeof(Host_HS));
        if (temp == NULL) {
            error = 1;
            return;
        }
        temp->Host = (StringL){buff,len};
        temp->next = headerList;
        headerList = temp;
    }
    search_map (map, "Host", callback);
    if (error != 0) {
        free_Host_HS(headerList);
        return NULL;
    }
    return headerList;
}


free_builder(Content_Length_HS)
Content_Length_HS* get_Content_Length(mapStruct* map) {
    
    Content_Length_HS* headerList = NULL;
    int error = 0;
    void callback(char* buff, unsigned int len) {
        Content_Length_HS* temp = malloc(sizeof(Content_Length_HS));
        if (temp == NULL) {
            error = 1;
            return;
        }
        temp->Content_Length = toLong((StringL){buff,len});
        temp->next = headerList;
        headerList = temp;
    }
    search_map (map, "Content-Length", callback);
    if (error != 0) {
        free_Content_Length_HS(headerList);
        return NULL;
    }
    return headerList;
}


Pair* parseAndAddCookiePair(StringL* cp, Pair* mm) {
    read_return rrkey = CALL_CLOSURE(get_reader(cookie_name,cp));
    if (rrkey.state == FAIL) {
        fprintf(stderr,"\x1b[31merreur semantique cookie-pair\x1b[0m\n");
        exit(EXIT_FAILURE);
    }
    read_return rrequal_char = CALL_CLOSURE(letter_Builder(cp,'='));
    if (rrequal_char.state == FAIL) {
        fprintf(stderr,"\x1b[31merreur semantique cookie-pair\x1b[0m\n");
        exit(EXIT_FAILURE);
    }
    read_return rrvalue = CALL_CLOSURE(get_reader(cookie_value,cp));
    if (rrvalue.state == FAIL) {
        fprintf(stderr,"\x1b[31merreur semantique cookie-pair\x1b[0m\n");
        exit(EXIT_FAILURE);
    }
    return addPair(mm,rrkey.string,rrvalue.string);
}
void free_Cookie_HS(Cookie_HS* obj) {
    if (obj != NULL) {
        free_Cookie_HS(obj->next);
        free_mini_map(obj->cookie_pair);
        free(obj);
    }
}
Cookie_HS* get_Cookie(mapStruct* map) {
    
    Cookie_HS* headerList = NULL;
    int error = 0;
    void callback(char* buff, unsigned int len) {
        Cookie_HS* temp = malloc(sizeof(Cookie_HS));
        if (temp == NULL) {
            error = 1;
            return;
        }
        StringL wBuff = (StringL){buff,len};
        temp->cookie_pair = parseAndAddCookiePair(&wBuff,temp->cookie_pair);
        reader rcomma = letter_Builder(&wBuff,';');
        reader rspace = get_reader(SP, &wBuff);
        while(CALL_CLOSURE(rcomma).state == SUCC) {
            if (CALL_CLOSURE(rspace).state == FAIL) {
                fprintf(stderr,"\x1b[31merreur semantique cookie-string SP\x1b[0m\n");
                exit(EXIT_FAILURE);
            }
            temp->cookie_pair = parseAndAddCookiePair(&wBuff,temp->cookie_pair);
        }
        
        temp->next = headerList;
        headerList = temp;
    }
    search_map (map, "Cookie", callback);
    if (error != 0) {
        free_Cookie_HS(headerList);
        return NULL;
    }
    return headerList;
}


void free_Connection_HS(Connection_HS* obj) {
    if (obj != NULL) {
        free_Connection_HS(obj->next);
        free_mini_map(obj->connection_option);
        free(obj);
    }
}
Connection_HS* get_Connection(mapStruct* map) {
    
    Connection_HS* headerList = NULL;
    int error = 0;
    void callback(char* buff, unsigned int len) {
        Connection_HS* temp = malloc(sizeof(Connection_HS));
        if (temp == NULL) {
            error = 1;
            return;
        }
        temp->connection_option = NULL;
        StringL wBuff = (StringL){buff,len};
        reader rconnection_option = get_reader(connection_option,&wBuff);
        while (wBuff.len > 0) {
            if(wBuff.s[0]!=',' && wBuff.s[0]!=' ' && wBuff.s[0]!='\t' && wBuff.s[0]!='\r' && wBuff.s[0]!='\n') {
                read_return rrco_opt = CALL_CLOSURE(rconnection_option);
                if (rrco_opt.state == FAIL) {
                    fprintf(stderr,"\x1b[31merreur semantique connection-option \"%s\" %d\x1b[0m\n",buff,wBuff.len);
                    exit(EXIT_FAILURE);
                }
                temp->connection_option = addPair(temp->connection_option,rrco_opt.string,rrco_opt.string);
            }
            else {
                wBuff.s++;
                wBuff.len--;
            }
        }
        temp->next = headerList;
        headerList = temp;
        
    }
    search_map (map, "Connection", callback);
    if (error != 0) {
        free_Connection_HS(headerList);
        return NULL;
    }
    
    return headerList;
}


void free_Transfer_Encoding_HS(Transfer_Encoding_HS* obj) {
    if (obj != NULL) {
        free_Transfer_Encoding_HS(obj->next);
        free_mini_map(obj->transfer_coding);
        free(obj);
    }
}
Transfer_Encoding_HS* get_Transfer_Encoding(mapStruct* map) {
    
    Transfer_Encoding_HS* headerList = NULL;
    int error = 0;
    void callback(char* buff, unsigned int len) {
        Transfer_Encoding_HS* temp = malloc(sizeof(Transfer_Encoding_HS));
        if (temp == NULL) {
            error = 1;
            return;
        }
        temp->transfer_coding = NULL;
        StringL wBuff = (StringL){buff,len};
        reader rtransfer_coding = get_reader(transfer_coding,&wBuff);
        while (wBuff.len > 0) {
            if(wBuff.s[0]!=',' && wBuff.s[0]!=' ' && wBuff.s[0]!='\t' && wBuff.s[0]!='\r' && wBuff.s[0]!='\n') {
                read_return rrco_opt = CALL_CLOSURE(rtransfer_coding);
                if (rrco_opt.state == FAIL) {
                    fprintf(stderr,"\x1b[31merreur semantique transfer_coding \"%s\" %d\x1b[0m\n",buff,wBuff.len);
                    exit(EXIT_FAILURE);
                }
                temp->transfer_coding = addPair(temp->transfer_coding,rrco_opt.string,rrco_opt.string);
            }
            else {
                wBuff.s++;
                wBuff.len--;
            }
        }
        temp->next = headerList;
        headerList = temp;
        
    }
    search_map (map, "Transfer_Encoding", callback);
    if (error != 0) {
        free_Transfer_Encoding_HS(headerList);
        return NULL;
    }
    
    return headerList;
}



void free_Authorization_HS(Authorization_HS* obj) {
    if (obj != NULL) {
        free_Authorization_HS(obj->next);
        free(obj);
    }
}
Authorization_HS* get_Connection(mapStruct* map) {
    
    Authorization_HS* headerList = NULL;
    int error = 0;
    void callback(char* buff, unsigned int len) {
        Authorization_HS* temp = malloc(sizeof(Authorization_HS));
        if (temp == NULL) {
            error = 1;
            return;
        }
        temp->auth_scheme = (StringL){NULL,0};
        temp->token68 = (StringL){NULL,0};
        
        StringL wBuff = (StringL){buff,len};
        reader rauth_scheme = get_reader(auth_scheme,&wBuff);
        reader rtoken68 = get_reader(token68,&wBuff);
        reader rRWS = get_reader(RWS,&wBuff);
        read_return rrtemp;
        if((rrtemp = CALL_CLOSURE(rauth_scheme)).state == FAIL) {
            error = 1;
            return;
        }
        temp->auth_scheme = rrtemp.string;
        if((rrtemp = CALL_CLOSURE(rRWS)).state == FAIL) {
            error = 1;
            return;
        }
        if((rrtemp = CALL_CLOSURE(token68)).state == FAIL) {
            error = 1;
            return;
        }
        temp->token68 = rrtemp.string;
        
        
        temp->next = headerList;
        headerList = temp;
        
    }
    search_map (map, "Connection", callback);
    if (error != 0) {
        free_Authorization_HS(headerList);
        return NULL;
    }
    
    return headerList;
}