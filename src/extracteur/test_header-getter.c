#include <stdio.h>
#include <stdlib.h>
#include "header-getter.h"
#include "StringL.h"
#include "parser.h"
#include "map.h"

//printf("\x1b[31mThis is red text\x1b[0m\n");
//printf("\x1b[32mThis is green text\x1b[0m\n");



mapStruct* getMapFromFile(char* fileName, char** ptxt)
{
    FILE* file;
    int i =0;

    file = fopen(fileName, "r");
    if (file == NULL) { perror("Impossible d'ouvrir le fichier ! "); exit(EXIT_FAILURE); }

    while (fgetc(file) != EOF)
        i++;

    *ptxt = malloc((i+1)*sizeof(char));
    if (*ptxt == NULL)
    { perror("Erreur d'allocation memoire"); exit(EXIT_FAILURE); }

    i=0; rewind(file);
    do
    {
        (*ptxt)[i]=fgetc(file);
    } while ((*ptxt)[i++] != EOF);
    (*ptxt)[i]='\0';
    
    StringL buffer = (StringL){*ptxt,i};
    parse_return pr = parse_HTTP_message(&buffer);    
    
    if (pr.state == PARSE_FAIL) {
        exit(EXIT_FAILURE);
    }
    return pr.map;
}

#define test_header_helper(X,Y) \
    mapStruct* map_ ## X = getMapFromFile(Y,&txt);\
    X ## _HS* X ## _val = get_ ## X(map_ ## X);\
    if(X ## _val == NULL) {\
        fprintf(stderr,"\x1b[31mtest ## X ## get NULL FAILED\x1b[0m\n");\
        exit(0);\
    }\

int main() {
    char* txt=NULL;
    
    test_header_helper(Host,"test_req1")
    
    StringL temp = fromRegularString("www.cvedetails.com");
    if(stringLEq(Host_val->Host,temp) != 1) {
        fprintf(stderr,"\x1b[31mtest Host bad Host FAILED\x1b[0m\n");
    }
    else printf("\x1b[32mtest Host OK\x1b[0m\n");
    
    Host_HS* Host_val2 = Host_val->next;
    if (Host_val2 == NULL) {
        fprintf(stderr,"\x1b[31mtest Host NULL Host in second pos FAILED\x1b[0m\n");
        exit(EXIT_FAILURE);
    }
    free(temp.s);
    temp = fromRegularString("www.hostentrop.com");
    if(stringLEq(Host_val2->Host,temp) != 1) {
        fprintf(stderr,"\x1b[31mtest Host->next bad Host FAILED\x1b[0m\n");
    }
    else printf("\x1b[32mtest Host->next OK\x1b[0m\n");
    
    free(temp.s);
    free_map(map_Host);
    free_Host_HS(Host_val);
    free(txt);
    
    test_header_helper(Content_Length,"test_req1")
    if(Content_Length_val->Content_Length != 42) {
        fprintf(stderr,"\x1b[31mtest Content_Length bad Content_Length FAILED\x1b[0m\n");
    }
    else printf("\x1b[32mtest Content_Length OK\x1b[0m\n");
    
    free(temp.s);
    free_map(map_Content_Length);
    free_Content_Length_HS(Content_Length_val);
    free(txt);
    
    test_header_helper(Cookie,"test_req1")
    StringL key1 = fromRegularString("key1");
    StringL key2 = fromRegularString("key2");
    StringL value1 = fromRegularString("value1");
    StringL value2 = fromRegularString("value2"); //segfault magique
    temp = getValue(Cookie_val->cookie_pair, key1);
    if(stringLEq(temp,value1)!= 1) {
        fprintf(stderr,"\x1b[31mtest Cookie bad value1 FAILED\x1b[0m\n");
    }
    temp = getValue(Cookie_val->cookie_pair, key2);
    if(stringLEq(temp,value2)!= 1) {
        fprintf(stderr,"\x1b[31mtest Cookie bad value2 FAILED\x1b[0m\n");
    }
    printf("\x1b[32mtest Cookie OK\x1b[0m\n");
    
    
    return 0;
}