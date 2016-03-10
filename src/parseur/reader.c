#include "reader.h"
#include <stdio.h>
#include <stdlib.h>


#define RET_FAIL (read_return){FAIL,*wBuff}


void safe_free(struct mult_free* context_MF) {
    if(context_MF->freed == 0) {
        free(context_MF->ctxt);
        context_MF->freed = 1;
    }
}

reader make_reader(void* ctxt, read_return (*fun)(void*), void (*freeFun)(struct mult_free*)) {
    return (reader){(struct mult_free){ctxt,0},fun,freeFun};
}
#define make_reader_helper(NAME) make_reader((void*)ctxt,(read_return (*)(void*))NAME ## _closure,(void (*)(struct mult_free*))NAME ## _free)

//Closure letter(char c)
#define letter(X) letter_Builder(wBuff,X)
typedef struct {
    StringL* wBuff;
    char c;
} letter_context;
void letter_free(struct mult_free* context_MF) {safe_free(context_MF);}
read_return letter_closure(letter_context* ctxt) {
    char c = ctxt->c;
    StringL *wBuff = ctxt->wBuff;
    if((wBuff->s == NULL) || (wBuff->len == 0)) {
        return RET_FAIL;
    }
    else {
        if(wBuff->s[0] == c) {
            read_return rr = {SUCC,{wBuff->s,1}};
            wBuff->s++; 
            wBuff->len--;
            return rr;
        }
        return RET_FAIL;
    }
}
reader letter_Builder(StringL* wBuff, char c) {
    letter_context* ctxt = malloc(sizeof(letter_context));
    ctxt->wBuff = wBuff;
    ctxt->c = c;
    return make_reader_helper(letter);
}


//Closure charIn(StringL str)
#define charIn(X) charIn_Builder(wBuff,X)
typedef struct {
    StringL* wBuff;
    StringL str;
} charIn_context;
void charIn_free(struct mult_free* context_MF) {safe_free(context_MF);}
read_return charIn_closure(charIn_context* ctxt) {
    StringL str = ctxt->str;
    StringL *wBuff = ctxt->wBuff;
    if((wBuff->s == NULL) || (wBuff->len == 0)) {
        return RET_FAIL;
    }
    else {
        for(int i=0;i<str.len;i++) {
            if(wBuff->s[0] == str.s[i]) {
                read_return rr = {SUCC,{wBuff->s,1}};
                wBuff->s++; 
                wBuff->len--;
                return rr;
            }
        }
        return RET_FAIL;
    }
}
reader charIn_Builder(StringL* wBuff, StringL str) {
    charIn_context* ctxt = malloc(sizeof(charIn_context));
    ctxt->wBuff = wBuff;
    ctxt->str = str;
    return make_reader_helper(charIn);
}


//Closure kleene(reader r)
#define kleene(X) kleene_Builder(wBuff,X)
typedef struct {
    StringL* wBuff;
    reader* r;
} kleene_context;
void kleene_free(struct mult_free* context_MF) {
    kleene_context* ctxt = (kleene_context*)context_MF->ctxt;
    FREE_CLOSURE(ctxt->r);
    safe_free(context_MF);
}
read_return kleene_closure(kleene_context* ctxt) {
    StringL *wBuff = ctxt->wBuff;
    reader unit_reader = *ctxt->r;
    StringL save = *wBuff;
    read_return current;
    int i=0;
    while( (current=CALL_CLOSURE(unit_reader)).state == SUCC ) {
        i += current.string.len;
    }
    return (read_return){SUCC,{save.s,i}};
}
reader kleene_Builder(StringL* wBuff, reader* r) {
    kleene_context* ctxt = malloc(sizeof(kleene_context));
    ctxt->wBuff = wBuff;
    ctxt->r = r;
    return make_reader_helper(kleene);
}



//Closure or(reader a, reader b)
#define or(X,Y) or_Builder(wBuff,X,Y)
typedef struct {
    StringL* wBuff;
    reader* a;
    reader* b;
} or_context;
void or_free(struct mult_free* context_MF) {
    or_context* ctxt = (or_context*)context_MF->ctxt;
    FREE_CLOSURE(ctxt->a);
    FREE_CLOSURE(ctxt->b);
    safe_free(context_MF);
}
read_return or_closure(or_context* ctxt) {
    StringL *wBuff = ctxt->wBuff;
    reader readerA = *ctxt->a;
    reader readerB = *ctxt->b;
    read_return current;
    if( (current=CALL_CLOSURE(readerA)).state == SUCC ) {
        return current;
    }
    else if( (current=CALL_CLOSURE(readerB)).state == SUCC ) {
        return current;
    }
    else {
        return RET_FAIL;
    }
}
reader or_Builder(StringL* wBuff, reader* a, reader* b) {
    or_context* ctxt = malloc(sizeof(or_context));
    ctxt->wBuff = wBuff;
    ctxt->a = a;
    ctxt->b = b;
    return make_reader_helper(or);
}



//Closure concat(reader a, reader b)
#define concat(X,Y) concat_Builder(wBuff,X,Y)
typedef struct {
    StringL* wBuff;
    reader* a;
    reader* b;
} concat_context;
void concat_free(struct mult_free* context_MF) {
    concat_context* ctxt = (concat_context*)context_MF->ctxt;
    FREE_CLOSURE(ctxt->a);
    FREE_CLOSURE(ctxt->b);
    safe_free(context_MF);
}
read_return concat_closure(concat_context* ctxt) {
    StringL *wBuff = ctxt->wBuff;
    reader readerA = *ctxt->a;
    reader readerB = *ctxt->b;
    StringL save = *wBuff;
    read_return a, b;
    if( ( (a=CALL_CLOSURE(readerA)).state == SUCC ) && ( (b=CALL_CLOSURE(readerB)).state == SUCC ) ) {
        return (read_return){SUCC,{save.s,a.string.len+b.string.len}};
    }
    else {
        *wBuff = save;
        return RET_FAIL;
    }
}
reader concat_Builder(StringL* wBuff, reader* a, reader* b) {
    concat_context* ctxt = malloc(sizeof(concat_context));
    ctxt->wBuff = wBuff;
    ctxt->a = a;
    ctxt->b = b;
    return make_reader_helper(concat);
}



//Closure bad_symbole()
#define bad_symbole() bad_symbole_Builder(wBuff)
typedef struct {
    StringL* wBuff;
} bad_symbole_context;
void bad_symbole_free(struct mult_free* context_MF) {safe_free(context_MF);}
read_return bad_symbole_closure(bad_symbole_context* ctxt) {
    StringL *wBuff = ctxt->wBuff;
    return RET_FAIL;
}
reader bad_symbole_Builder(StringL* wBuff) {
    bad_symbole_context* ctxt = malloc(sizeof(bad_symbole_context));
    ctxt->wBuff = wBuff;
    return make_reader_helper(letter);
}




reader read(syntaxe_elem se, StringL* wBuff) {
    
    switch(se) {
        case SP: return letter(' ');
        case HTAB: return letter('\t');
        case OWS: {
            reader space=letter(' ');
            reader tab=letter('\t');
            reader temp=or((&space),(&tab));
            return kleene((&temp));
        }
        case DIGIT: return charIn(fromRegularString("0123456789"));
        case day: {
            reader digit=read(DIGIT,wBuff);
            return concat((&digit),(&digit));
        }
        default: return bad_symbole();
    }
    
    
}


