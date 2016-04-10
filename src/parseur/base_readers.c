
//l'utilisation d'un GC est presque indispensable (si le travail avait été fait à la main ça aurait ressemblé à un GC de toute façon)
//de cette manière les contexts peuvent être composés et réutilisés
//l'utilisation de free en cascade (avec une fonction dédié pour chaque reader) 
//necessite de mémoriser si le context à été free ou non pour éviter les double free (lorsque on réutilise un même reader pour la composition)
//du coup, nous avons opté pour le GC hboehm cela permet:
// - eviter de coder une fonction free dédié à chaque closures
// - eviter de coder un système de sauvegarde de references de context
//
//nous avons essayé l'approche avec des fonctions free dédié, le résultat était trop difficile à maintenir et à faire fonctionner correctement


#define RET_FAIL (read_return){FAIL,{wBuff->s,0}}

reader make_reader(void* ctxt, read_return (*fun)(void*)) {
    return (reader){ctxt,fun};
}
#define make_reader_helper(NAME) make_reader((void*)ctxt,(read_return (*)(void*))NAME ## _closure)


//Closure epsilon()
#define epsilon() epsilon_Builder(wBuff)
typedef struct {
    StringL* wBuff;
} epsilon_context;
read_return epsilon_closure(epsilon_context* ctxt) {
    StringL *wBuff = ctxt->wBuff;
    return (read_return){SUCC,{wBuff->s,0}};
}
reader epsilon_Builder(StringL* wBuff) {
    epsilon_context* ctxt = GC_MALLOC(sizeof(epsilon_context));
    ctxt->wBuff = wBuff;
    return make_reader_helper(epsilon);
}



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
    StringL save = *wBuff;
    reader readerA = ctxt->a;
    reader readerB = ctxt->b;
    read_return retA;
    read_return retB;
    retA=CALL_CLOSURE(readerA);
    *wBuff=save;
    retB=CALL_CLOSURE(readerB);
    *wBuff=save;
    
    if(retA.state == FAIL) {
        wBuff->s += retB.string.len;
        wBuff->len -= retB.string.len;
        return retB;
    }
    if(retB.state == FAIL) {
        wBuff->s += retA.string.len;
        wBuff->len -= retA.string.len;
        return retA;
    }
    if(retA.string.len >= retB.string.len) {
        wBuff->s += retA.string.len;
        wBuff->len -= retA.string.len;
        return retA;
    }
    else {
        wBuff->s += retB.string.len;
        wBuff->len -= retB.string.len;
        return retB;
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



//Closure and(reader a, reader b) !en cas de succès, les 2 readers doivent retourner la même string!!
#define and(X,Y) and_Builder(wBuff,X,Y)
typedef struct {
    StringL* wBuff;
    reader a;
    reader b;
} and_context;
read_return and_closure(and_context* ctxt) {
    StringL *wBuff = ctxt->wBuff;
    reader readerA = ctxt->a;
    reader readerB = ctxt->b;
    StringL save = *wBuff;
    read_return a, b;
    a=CALL_CLOSURE(readerA);
    if (a.state == SUCC) {
        *wBuff = save; //on remet le buffer à 0 (!= concat)
        b=CALL_CLOSURE(readerB);
        if (b.state == SUCC) {
            if (a.string.len == b.string.len) {
                return (read_return){SUCC,{save.s,a.string.len}};
            }
            else {
                *wBuff = save;
                return RET_FAIL;
            }
        }
        else {
            *wBuff = save;
            return RET_FAIL;
        }
    }
    else {
        *wBuff = save;
        return RET_FAIL;
    }
}
reader and_Builder(StringL* wBuff, reader a, reader b) {
    and_context* ctxt = GC_MALLOC(sizeof(and_context));
    ctxt->wBuff = wBuff;
    ctxt->a = a;
    ctxt->b = b;
    return make_reader_helper(and);
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

//Closure untilChar(char c)
#define untilChar(X) untilChar_Builder(wBuff,X)
typedef struct {
    StringL* wBuff;
    char c;
} untilChar_context;
read_return untilChar_closure(untilChar_context* ctxt) {
    char c = ctxt->c;
    StringL *wBuff = ctxt->wBuff;
    if((wBuff->s == NULL) || (wBuff->len == 0)) {
        return RET_FAIL;
    }
    else {
        int i = 0;
        while(i<wBuff->len) {
            if(wBuff->s[i] == c) {
                read_return rr = {SUCC,{wBuff->s,i-1}};
                wBuff->s += i; 
                wBuff->len -= i;
                return rr;
            }
            i++;
        }
        return RET_FAIL;
    }
}
reader untilChar_Builder(StringL* wBuff, char c) {
    untilChar_context* ctxt = GC_MALLOC(sizeof(untilChar_context)); 
    ctxt->wBuff = wBuff;
    ctxt->c = c;
    return make_reader_helper(untilChar);
}

//Closure charBetween(char c, char c)
#define charBetween(X,Y) charBetween_Builder(wBuff,X,Y)
typedef struct {
    StringL* wBuff;
    char a;
    char b;
} charBetween_context;
read_return charBetween_closure(charBetween_context* ctxt) {
    char a = ctxt->a;
    char b = ctxt->b;
    StringL *wBuff = ctxt->wBuff;
    if((wBuff->s == NULL) || (wBuff->len == 0)) {
        return RET_FAIL;
    }
    else {
        if( (wBuff->s[0] >= a) && (wBuff->s[0] <= b) ) {
            read_return rr = {SUCC,{wBuff->s,1}};
            wBuff->s++; 
            wBuff->len--;
            return rr;
        }
        return RET_FAIL;
    }
}
reader charBetween_Builder(StringL* wBuff, char a, char b) {
    charBetween_context* ctxt = GC_MALLOC(sizeof(charBetween_context)); 
    ctxt->wBuff = wBuff;
    ctxt->a = a;
    ctxt->b = b;
    return make_reader_helper(charBetween);
}

//Closure nOccurencesMin(reader r, int n)
#define nOccurencesMin(X,Y) nOccurencesMin_Builder(wBuff,X,Y)
typedef struct {
    StringL* wBuff;
    reader r;
    int n;
} nOccurencesMin_context;
read_return nOccurencesMin_closure(nOccurencesMin_context* ctxt) {
    reader r = ctxt->r;
    int n = ctxt->n;
    StringL *wBuff = ctxt->wBuff;
    StringL save = *wBuff;
    int totalLen = 0;
    read_return rr;
    int i = 0;
    
    while((rr=CALL_CLOSURE(r)).state == SUCC ) {
        totalLen += rr.string.len;
        i++;
    }
    if(i>=n) {
        return (read_return){SUCC,{save.s,totalLen}};
    }
    else {
        *wBuff = save;
        return RET_FAIL;
    }
    
}
reader nOccurencesMin_Builder(StringL* wBuff, reader r, int n) {
    nOccurencesMin_context* ctxt = GC_MALLOC(sizeof(nOccurencesMin_context)); 
    ctxt->wBuff = wBuff;
    ctxt->r = r;
    ctxt->n = n;
    return make_reader_helper(nOccurencesMin);
}


//Closure nOccurencesMax(reader r, int n)
#define nOccurencesMax(X,Y) nOccurencesMax_Builder(wBuff,X,Y)
typedef struct {
    StringL* wBuff;
    reader r;
    int n;
} nOccurencesMax_context;
read_return nOccurencesMax_closure(nOccurencesMax_context* ctxt) {
    reader r = ctxt->r;
    int n = ctxt->n;
    StringL *wBuff = ctxt->wBuff;
    StringL save = *wBuff;
    int totalLen = 0;
    int i=0;
    read_return rr;
    while((rr=CALL_CLOSURE(r)).state == SUCC ) {
        totalLen += rr.string.len;
        i++;
        if(i>n+1) {
            *wBuff = save;
            return RET_FAIL;
        }
    }
    return (read_return){SUCC,{save.s,totalLen}};
}
reader nOccurencesMax_Builder(StringL* wBuff, reader r, int n) {
    nOccurencesMax_context* ctxt = GC_MALLOC(sizeof(nOccurencesMax_context)); 
    ctxt->wBuff = wBuff;
    ctxt->r = r;
    ctxt->n = n;
    return make_reader_helper(nOccurencesMax);
}



//Reader composé nOccurences(reader r, int n)
#define nOccurences(X,Y) and(nOccurencesMax(X,Y),nOccurencesMin(X,Y))

//Reader composé optionnal(reader r)
#define optionnal(X) or(X,epsilon())

#define word(X) word_Builder(wBuff, X)
typedef struct {
    StringL* wBuff;
    StringL str;
} word_context;
read_return word_closure(word_context* ctxt) {
    StringL str = ctxt->str;
    StringL *wBuff = ctxt->wBuff;
    StringL save = *wBuff;
    if((wBuff->s == NULL) || (wBuff->len < str.len)) {
        return RET_FAIL;
    }
    int i;
    for(i=0;i<str.len;i++) {
            if(wBuff->s[i] != str.s[i]) {
                return RET_FAIL;
            }
    }
    wBuff->s += i;
    wBuff->len -= i;
    return (read_return){SUCC,{save.s,str.len}};
}
reader word_Builder(StringL* wBuff, StringL str) {
    word_context* ctxt = GC_MALLOC(sizeof(word_context));
    ctxt->wBuff = wBuff;
    ctxt->str = str;
    return make_reader_helper(word);
}
//fait par henri => pas sur que ça marche !
//et revisité par alexis => sur que ça marche !