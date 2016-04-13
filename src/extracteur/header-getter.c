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
    search_map (map, "Content_Length", callback);
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