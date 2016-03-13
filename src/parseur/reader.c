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
        case VCHAR: return charBetween(0x21, 0x7E); //à implémenter : prototype -> reader charBetween(int,int);
        case obs_text: return charBetween(0x21, 0x7E); //à implémenter : prototype -> reader charBetween(int,int);
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
        case origin_form: return concat(symb(absolute_path), optionnal(concat(letter('?'),symb(query)))); // à implémenter : prototype -> reader optionnal(reader);
        case scheme: return concat(symb(ALPHA), or(or(symb(ALPHA), symb(DIGIT)), or(letter('+'), or(letter('-'), letter('.')))));
        case userinfo: return kleene(or(or(symb(unreserved), symb(pct_encoded)), or(symb(sub_delims),letter(':'))));
        case h16: return concat(nOccurences(symb(HEXDIG),4),kleene(nOccurences(symb(HEXDIG),4))); //à implémenter : prototype -> reader nOccurences(reader, int);
        case dec_octet: return or(or(symb(DIGIT),concat(charBetween(0x31,0x39),symb(DIGIT))), or(concat(letter('1'),concat(symb(DIGIT),symb(DIGIT))), or(concat(letter('2'),concat(charBetween(0x30,0x34),symb(DIGIT))),concat(letter('2'),concat(letter('5'),charBetween(0x30,0x35))))));
        case IPv4address: return concat(nOccurences(concat(symb(dec_octet),letter('.')),3),symb(dec_octet));
        case ls32: return or(concat( symb(h16), concat( letter(':'), symb(h16))),symb(IPv4address));
        case IPv6address: {
            symbPoints = concat( letter(':'), letter(':')); //"::"
            symbh16 = concat( symb(h16), letter(':')); //h16":"
            reader1 = nOccurences( symbh16, 6); //6( h16 ":") ls32
            reader2 = concat( concat( symbPoints, nOccurences( symbh16, 5)), symb(ls32)); //"::"5( h16 ":") ls32
            reader3 = concat( concat( concat( optionnal(symb(h16)), symbPoints), nOccurences( symbh16, 4)), symb(ls32)); //[ h16 ]"::"4( h16 ":") ls32
            reader4 = concat( concat( concat( optionnal(concat(concat( symbh16,kleene(symbh16)),symb(h16))), symbPoints), nOccurences( symbh16, 3)), symb(ls32)); //[*1( h16 ":") h16 ]"::"3( h16 ":") ls32
            reader5 = concat( concat( concat( optionnal(concat(concat( nOccurences(symbh16, 2), kleene(symbh16)),symb(h16))), symbPoints), nOccurences( symbh16, 2)), symb(ls32)); //[*2( h16 ":") h16 ]"::"2( h16 ":") ls32
            reader6 = concat( concat( concat( optionnal(concat(concat( nOccurences(symbh16, 3), kleene(symbh16)),symb(h16))), symbPoints), symbh16), symb(ls32)); //[*3( h16 ":") h16 ]"::" h16 ":" ls32
            reader7 = concat( concat( optionnal(concat(concat( nOccurences(symbh16, 4), kleene(symbh16)),symb(h16))), symbPoints), symb(ls32)); //[*4( h16 ":") h16 ]"::" ls32
            reader8 = concat( concat( optionnal(concat(concat( nOccurences(symbh16, 5), kleene(symbh16)),symb(h16))), symbPoints), symb(h16));// [*5( h16 ":") h16 ]"::" h16
            reader9 = concat( optionnal(concat(concat( nOccurences(symbh16, 6), kleene(symbh16)),symb(h16))), symbPoints);//[*6( h16 ":") h16 ]"::"
            return or(or(or(or(or( reader1, reader2), reader3)),or( reader4, reader5))),or(or(or( reader6, reader7),or( reader8, reader9))))
        }
        case IPvFuture: return concat( concat( concat( letter('v'), symb(HEXDIG)), concat( letter('.'), kleene(symb(HEXDIG)))), concat( or( or( symb(unreserved), symb(sub_delims)), letter(':')), kleene(or( or( symb(unreserved), symb(sub_delims)), letter(':')))));
        case IP_literal: return concat(concat( letter('['), or( symb(IPv6address), symb(IPvFuture)), letter(']'));
        case reg_name: return kleene(or(or( symb(unreserved), symb(pct_encoded)), symb(sub_delims)));
        case host: return or(or( symb(IP_literal), symb(IPv4address)), symb(reg_name));
        case port: return kleene(symb(DIGIT));
        case authority: return concat(concat(optionnal(concat(symb(userinfo),letter('@'))), symb(host)), optionnal(concat(letter(':'),port)));
        case path_abempty: return kleene(concat(letter('/'), symb(segment)));
        case segment_nz: return concat( symb(pchar), kleene(symb(pchar)));
        case path_absolute: return concat(concat( letter('/'), symb(segment_nz)), kleene(concat(letter('/'),symb(segment))));
        case path_rootless: return concat(, symb(segment_nz), kleene(concat(letter('/'),symb(segment))));
        case path_empty: return ;
        case hier_part: return concat( concat(letter('/'),letter('/')), or(or( concat( symb(authority), symb(path_abempty)), symb(path_absolute)),or( symb(path_rootless), symb(path_empty))));
        case absolute_URI: return concat(concat( symb(scheme), letter(':')), concat( symb(hier_part), optionnal(concat(letter('?'),symb(query)))));
        case absolute_form: return symb(absolute_URI);
        case authority_form: return symb(authority);
        case asterisk_form: return letter('*');
        case request_target: return or(or( symb(origin_form), symb(absolute_form)), or( symb(authority_form), symb(asterisk_form)));
        
        default: return bad_symbole();
    }
    
    
}
