#include "reader.h"
#include "gc.h"
#include <stdio.h>
#include <stdlib.h>
#include "base_readers.c"

reader read(syntaxe_elem se, StringL* wBuff) {
    
    switch(se) {
        case SP: return letter(' ');
        case HTAB: return letter('\t');
        case OWS: return kleene(or(letter(' '),letter('\t')));
        case CRLF: return concat(letter('\r'),letter('\n'));
        case DIGIT: return charIn(((StringL){"0123456789",10}));
        case day: {
            reader digit=read(DIGIT,wBuff);
            return concat(digit,digit);
        }
        
        default: return bad_symbole();
    }
    
    
}


