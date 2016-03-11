
//l'utilisation d'un GC est presque indispensable (si le travail avait été fait à la main ça aurait ressemblé à un GC de toute façon)
//de cette manière les contexts peuvent être composés et réutilisés
//l'utilisation de free en cascade (avec une fonction dédié pour chaque reader) 
//necessite de mémoriser si le context à été free ou non pour éviter les double free (lorsque on réutilise un même reader pour la composition)
//du coup, nous avons opté pour le GC hboehm cela permet:
// - eviter de coder une fonction free dédié à chaque closures
// - eviter de coder un système de sauvegarde de references de context
//
//nous avons essayé l'approche avec des fonctions free dédié, le résultat était trop difficile à maintenir et à faire fonctionner correctement


#define RET_FAIL (read_return){FAIL,*wBuff}

reader make_reader(void* ctxt, read_return (*fun)(void*)) {
    return (reader){ctxt,fun};
}
#define make_reader_helper(NAME) make_reader((void*)ctxt,(read_return (*)(void*))NAME ## _closure)

//Closure letter(char c)
#define letter(X) letter_Builder(wBuff,X)
typedef struct {
    StringL* wBuff;
    char c;
} letter_context;
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
    letter_context* ctxt = GC_MALLOC(sizeof(letter_context)); 
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
    charIn_context* ctxt = GC_MALLOC(sizeof(charIn_context));
    ctxt->wBuff = wBuff;
    ctxt->str = str;
    return make_reader_helper(charIn);
}


//Closure kleene(reader r)
#define kleene(X) kleene_Builder(wBuff,X)
typedef struct {
    StringL* wBuff;
    reader r;
} kleene_context;
read_return kleene_closure(kleene_context* ctxt) {
    StringL *wBuff = ctxt->wBuff;
    reader unit_reader = ctxt->r;
    StringL save = *wBuff;
    read_return current;
    int i=0;
    while( (current=CALL_CLOSURE(unit_reader)).state == SUCC ) {
        i += current.string.len;
    }
    return (read_return){SUCC,{save.s,i}};
}
reader kleene_Builder(StringL* wBuff, reader r) {
    kleene_context* ctxt = GC_MALLOC(sizeof(kleene_context));
    ctxt->wBuff = wBuff;
    ctxt->r = r;
    return make_reader_helper(kleene);
}



//Closure or(reader a, reader b)
#define or(X,Y) or_Builder(wBuff,X,Y)
typedef struct {
    StringL* wBuff;
    reader a;
    reader b;
} or_context;
read_return or_closure(or_context* ctxt) {
    StringL *wBuff = ctxt->wBuff;
    reader readerA = ctxt->a;
    reader readerB = ctxt->b;
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
reader or_Builder(StringL* wBuff, reader a, reader b) {
    or_context* ctxt = GC_MALLOC(sizeof(or_context));
    ctxt->wBuff = wBuff;
    ctxt->a = a;
    ctxt->b = b;
    return make_reader_helper(or);
}



//Closure concat(reader a, reader b)
#define concat(X,Y) concat_Builder(wBuff,X,Y)
typedef struct {
    StringL* wBuff;
    reader a;
    reader b;
} concat_context;
read_return concat_closure(concat_context* ctxt) {
    StringL *wBuff = ctxt->wBuff;
    reader readerA = ctxt->a;
    reader readerB = ctxt->b;
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
reader concat_Builder(StringL* wBuff, reader a, reader b) {
    concat_context* ctxt = GC_MALLOC(sizeof(concat_context));
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
read_return bad_symbole_closure(bad_symbole_context* ctxt) {
    StringL *wBuff = ctxt->wBuff;
    return RET_FAIL;
}
reader bad_symbole_Builder(StringL* wBuff) {
    bad_symbole_context* ctxt = GC_MALLOC(sizeof(bad_symbole_context));
    ctxt->wBuff = wBuff;
    return make_reader_helper(letter);
}
