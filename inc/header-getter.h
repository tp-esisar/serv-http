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


typedef struct Transfer_Encoding_HS {
    Pair* transfer_coding;
    struct Transfer_Encoding_HS* next;
} Transfer_Encoding_HS;
free_decl(Transfer_Encoding_HS)
Transfer_Encoding_HS* get_Transfer_Encoding(mapStruct* map);


typedef struct Authorization_HS {
    StringL auth_scheme;
    StringL token68;
    struct Authorization_HS* next;
} Authorization_HS;
free_decl(Authorization_HS)
Authorization_HS* get_Authorization(mapStruct* map);




#endif