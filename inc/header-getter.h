#ifndef HEADERGETTER_H
#define HEADERGETTER_H

#include "StringL.h"
#include "map.h"
//structures des headers

#define free_decl(T) void free_ ## T(T* obj);

typedef struct Host_HS {
    StringL Host;
    struct Host_HS* next;
} Host_HS;
free_decl(Host_HS)
Host_HS* get_Host(mapStruct* map);


typedef struct Content_Length_HS {
    long Content_Length;
    struct Content_Length_HS* next;
} Content_Length_HS;
free_decl(Content_Length_HS)
Content_Length_HS* get_Content_Length(mapStruct* map);




#endif