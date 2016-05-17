#include <stdio.h>
#include <stdlib.h>

#include "StringL.h"
#include "process.h"

int test(char* name, StringL reqTarg, URI_Info etalon) {
    int err = 0;
    URI_Info result = extractInfoFromURI(reqTarg);
    if(result.scheme.s != etalon.scheme.s || result.scheme.len != etalon.scheme.len) {
        fprintf(stderr, "\x1b[31mtest \"%s\" FAIL scheme ", name);
        if(result.scheme.s == NULL && etalon.scheme.s == NULL) {
            fprintf(stderr, "is {NULL,%d} insted of {NULL,%d}\x1b[0m\n",result.scheme.len, etalon.scheme.len);
        }
        else if(result.scheme.s == NULL && etalon.scheme.s != NULL){
            char* reguString = toRegularString(etalon.scheme);
            fprintf(stderr, "is {NULL,%d} insted of {%s,%d}\x1b[0m\n",result.scheme.len, reguString, etalon.scheme.len);
            free(reguString);
        }
        else if(result.scheme.s != NULL && etalon.scheme.s == NULL){
            char* reguString = toRegularString(result.scheme);
            fprintf(stderr, "is {%s,%d} insted of {NULL,%d}\x1b[0m\n",reguString, result.scheme.len, etalon.scheme.len);
            free(reguString);
        }
        else if(result.scheme.s != NULL && etalon.scheme.s != NULL){
            char* reguString = toRegularString(result.scheme);
            char* reguString2 = toRegularString(etalon.scheme);
            fprintf(stderr, "is {%s,%d} insted of {%s,%d}\x1b[0m\n",reguString, result.scheme.len, reguString2, etalon.scheme.len);
            free(reguString);
            free(reguString2);
        }
        err++;
        
        
    }
    if(result.path.s != etalon.path.s || result.path.len != etalon.path.len) {
        fprintf(stderr, "\x1b[31mtest \"%s\" FAIL path ", name);
        if(result.path.s == NULL && etalon.path.s == NULL) {
            fprintf(stderr, "is {NULL,%d} insted of {NULL,%d}\x1b[0m\n",result.path.len, etalon.path.len);
        }
        else if(result.path.s == NULL && etalon.path.s != NULL){
            char* reguString = toRegularString(etalon.path);
            fprintf(stderr, "is {NULL,%d} insted of {%s,%d}\x1b[0m\n",result.path.len, reguString, etalon.path.len);
            free(reguString);
        }
        else if(result.path.s != NULL && etalon.path.s == NULL){
            char* reguString = toRegularString(result.path);
            fprintf(stderr, "is {%s,%d} insted of {NULL,%d}\x1b[0m\n",reguString, result.path.len, etalon.path.len);
            free(reguString);
        }
        else if(result.path.s != NULL && etalon.path.s != NULL){
            char* reguString = toRegularString(result.path);
            char* reguString2 = toRegularString(etalon.path);
            fprintf(stderr, "is {%s,%d} insted of {%s,%d}\x1b[0m\n",reguString, result.path.len, reguString2, etalon.path.len);
            free(reguString);
            free(reguString2);
        }
        err++;
        
        
    }
    if(err == 0) {
        printf("\x1b[32mtest \"%s\" OK\x1b[0m\n",name);
    }
    return err;
}


int main() {
    int ok = 0;
    ok += test("test comme get1",(StringL){"/",1},(URI_Info){\
        {NULL,0},\
        {NULL,0},\
        {NULL,0},\
        {"/",1},\
        {NULL,0},\
        {NULL,0}\
    });
    
    
    
    return ok;
}