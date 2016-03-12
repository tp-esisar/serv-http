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
        case ALPHA: return charIn(((StringL){"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",52}));
        case alphanum: return or(DIGIT, ALPHA);
        case unreserved: return or(alphanum,charIn(((StringL){"-._~"},4)));
        case tchar: return or(alphanum,charIn(((StringL){"!#$%&'*+-.^_`|~"},15)));
        case token: return concat(tchar,kleene(tchar));
        case field-name return token;
        case VCHAR return charBetween(0x21, 0x7E); //à implémenter : prototype -> reader charBetween(int,int);
        case obs-text return charBetween(0x21, 0x7E); //à implémenter : prototype -> reader charBetween(int,int);
        case field-vchar return or(VCHAR, obs-text);
        case field-content return concat( field-vchar, concat( concat(or(SP, HTAB), kleene(or(SP, HTAB))), field-vchar)); //cette technique de concat(var, kleene(var)) sert à avoir au moins une occurence de var, si on veut au moin n occurences, n défini, faire une fonction ?
        case obs-fold return concat( CRLF, concat(or(SP, HTAB), kleene(or(SP, HTAB))));
        case field-value return kleene(or(field-content, obs-fold));
        case header-field return concat( field-name,concat( letter(':'), concat( OWS, concat( field-value, OWS))));
        case method return token;
        case HEXDIG return or(DIGIT, charIn(((StringL){"ABCDEF",6})));
        case pct-encoded return concat(letter('%'), concat(HEXDIG, HEXDIG));
        case sub-delims return charIn(((StringL){"!$&'()*+,;="},12));
        case pchar return or(unreserved, or( pct-encoded, or( sub-delims, or( letter(':') , letter('@')))));
        case segment return kleene(pchar);
        case absolute-path return concat(segment, kleene(segment));
        case query return kleene(or( pchar, or( letter('/'), letter('?'))));
        case origin-form return concat(absolute-path, optionnal(concat(letter('?'),query))); // à implémenter : prototypr -> reader optionnal(reader);
        default: return bad_symbole();
    }
    
    
}
