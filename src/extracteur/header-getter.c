#include "header-getter.h"


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