#include "reader.h"
#include "gc.h"
#include <stdio.h>
#include <stdlib.h>
#include "base_readers.c"

#define symb(X) read(X,wBuff)

reader read(syntaxe_elem se, StringL* wBuff) {
    
    switch(se) {
        case SP: return letter(' ');
        case HTAB: return letter('\t');
        case OWS: return kleene(or(letter(' '),letter('\t')));
        case CRLF: return concat(letter('\r'),letter('\n'));
        case DIGIT: return charIn(((StringL){"0123456789",10}));
        case day: return concat(symb(DIGIT),symb(DIGIT));
        case ALPHA: return charIn(((StringL){"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",52}));
        case alphanum: return or(symb(DIGIT), symb(ALPHA));
        case unreserved: return or(symb(alphanum),charIn(((StringL){"-._~",4})));
        case tchar: return or(symb(alphanum),charIn(((StringL){"!#$%&'*+-.^_`|~",15})));
        case token: return concat(symb(tchar),kleene(symb(tchar)));
        case field_name: return symb(token);
        //case VCHAR: return charBetween(0x21, 0x7E); //à implémenter : prototype -> reader charBetween(int,int);
        //case obs_text: return charBetween(0x21, 0x7E); //à implémenter : prototype -> reader charBetween(int,int);
        case field_vchar: return or(symb(VCHAR), symb(obs_text));
        case field_content: return concat( symb(field_vchar), concat( concat(or(symb(SP), symb(HTAB)), kleene(or(symb(SP), symb(HTAB)))), symb(field_vchar))); //cette technique de concat(var, kleene(var)) sert à avoir au moins une occurence de var, si on veut au moin n occurences, n défini, faire une fonction ?
        case obs_fold: return concat( symb(CRLF), concat(or(symb(SP), symb(HTAB)), kleene(or(symb(SP), symb(HTAB)))));
        case field_value: return kleene(or(symb(field_content), symb(obs_fold)));
        case header_field: return concat( symb(field_name),concat( letter(':'), concat( symb(OWS), concat( symb(field_value), symb(OWS)))));
        case method: return symb(token);
        case HEXDIG: return or(symb(DIGIT), charIn(((StringL){"ABCDEF",6})));
        case pct_encoded: return concat(letter('%'), concat(symb(HEXDIG), symb(HEXDIG)));
        case sub_delims: return charIn(((StringL){"!$&'()*+,;=",12}));
        case pchar: return or(symb(unreserved), or( symb(pct_encoded), or( symb(sub_delims), or( letter(':') , letter('@')))));
        case segment: return kleene(symb(pchar));
        case absolute_path: return concat(symb(segment), kleene(symb(segment)));
        case query: return kleene(or( symb(pchar), or( letter('/'), letter('?'))));
        //case origin_form: return concat(symb(absolute_path), optionnal(concat(letter('?'),symb(query)))); // à implémenter : prototype -> reader optionnal(reader);
        case scheme: concat(symb(ALPHA), or(or(symb(ALPHA), symb(DIGIT)), or(letter('+'), or(letter('-'), letter('.')))));
        case userinfo: kleene(or(or(symb(unreserved), symb(pct_encoded)), or(symb(sub_delims),letter(':'))));
        //case h16: concat(nOccurences(symb(HEXDIG),4),kleene(nOccurences(symb(HEXDIG),4))); //à implémenter : prototype -> reader nOccurences(reader, int);
        //case IPv6address: 
        default: return bad_symbole();
    }
    
    
}
