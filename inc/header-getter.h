#ifndef HEADERGETTER_H
#define HEADERGETTER_H

#include "StringL.h"
#include "map.h"
#include "mini-map.h"

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

typedef struct Cookie_HS {
    Pair* cookie_pair;
    struct Cookie_HS* next;
} Cookie_HS;
free_decl(Cookie_HS)
Cookie_HS* get_Cookie(mapStruct* map);


typedef struct Connection_HS {
    Pair* connection_option;
    struct Connection_HS* next;
} Connection_HS;
free_decl(Connection_HS)
Connection_HS* get_Connection(mapStruct* map);




#endif