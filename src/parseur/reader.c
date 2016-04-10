#include "reader.h"
#include "gc.h"
#include <stdio.h>
#include <stdlib.h>
#include "base_readers.c"

#define symb(X) get_reader(X,wBuff)
//#define nOccurencesRange(X,n1,n2) nOccurencesMax(nOccurencesMin(X,n1),n2)
#define nOccurencesRange(X,n1,n2) and(nOccurencesMin(X,n1),nOccurencesMin(X,n2))
//temporaire (le _t pour le remplacer facilement)
#define word_t(X) epsilon()

#define header(X,n,Y) concat(concat( word_s(X,n), symb(OWS)),concat( symb(Y), symb(OWS)))
#define word_s(X,n) word_t(((StringL){X,n))
#define list(X) concat( concat( kleene(concat( letter(','), symb(OWS))), symb(X)), kleene(concat( concat( symb(OWS), letter(',')), optionnal(concat( symb(OWS), symb(X))))))

reader get_reader(syntaxe_elem se, StringL* wBuff) {
    
    switch(se) {
        case SP: return letter(' ');
        case HTAB: return letter('\t');
        case OWS: return kleene(or(letter(' '),letter('\t')));
        case CRLF: return concat(letter('\r'),letter('\n')); //CRLF conforme (ne marche pas pour le debug avec l'encodage linux)
        //case CRLF: return or(letter('\n'),concat(letter('\r'),letter('\n'))); //CRLF non conforme à utiliser pour le debug sur linux
        case DIGIT: return charIn(((StringL){"0123456789",10}));
        case ALPHA: return charIn(((StringL){"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",52}));
        case HEXDIG: return or(symb(DIGIT), charIn(((StringL){"ABCDEFabcdef",6})));
        case VCHAR: return charBetween(0x21, 0x7E);
        case OCTET: return charBetween(0x00, 0xFF);
        case BWS: return symb(OWS);
        case DQUOTE: return letter('"');
        case RWS: return nOccurencesMin(or(symb(SP),symb(HTAB)),1);
        case alphanum: return or(symb(DIGIT), symb(ALPHA));
        case unreserved: return or(symb(alphanum),charIn(((StringL){"-._~",4})));
        case tchar: return or(symb(alphanum),charIn(((StringL){"!#$\%&'*+-.^_`|~",15})));
        case token: return nOccurencesMin(symb(tchar),1);
        case field_name: return symb(token);
        case obs_text: return charBetween(0x80, 0xFF); 
        case field_vchar: return or(symb(VCHAR), symb(obs_text));
        //case field_content: return concat( symb(field_vchar), concat( nOccurencesMin(or(symb(SP), symb(HTAB)), 1), symb(field_vchar)));
        //case field_content: return concat(symb(field_vchar),optionnal(concat(nOccurencesMin(or(symb(SP),symb(HTAB)),1),symb(field_vchar)))); //vielle version qui bug
        case field_content: return concat(symb(field_vchar),optionnal(nOccurencesMin(or(symb(SP),or(symb(HTAB),symb(field_vchar))),1)));
        case obs_fold: return concat(symb(OWS),concat( symb(CRLF), nOccurencesMin(or(symb(SP), symb(HTAB)),1)));
        case field_value: return kleene(or(symb(field_content), symb(obs_fold)));
        case header_field: return concat( symb(field_name),concat( letter(':'), concat( symb(OWS), concat( symb(field_value), symb(OWS)))));
        case method: return symb(token);
        case pct_encoded: return concat(letter('%'), concat(symb(HEXDIG), symb(HEXDIG)));
        case sub_delims: return charIn(((StringL){"!$&'()*+,;=",12}));
        case pchar: return or(symb(unreserved), or( symb(pct_encoded), or( symb(sub_delims), or( letter(':') , letter('@')))));
        case segment: return kleene(symb(pchar));
        case absolute_path: return nOccurencesMin(concat(letter('/'),symb(segment)),1);
        case query: return kleene(or( symb(pchar), or( letter('/'), letter('?'))));
        case origin_form: return concat(symb(absolute_path), optionnal(concat(letter('?'),symb(query))));
        case scheme: return concat(symb(ALPHA), or(or(symb(ALPHA), symb(DIGIT)), or(letter('+'), or(letter('-'), letter('.')))));
        case userinfo: return kleene(or(or(symb(unreserved), symb(pct_encoded)), or(symb(sub_delims),letter(':'))));
        case h16: return nOccurencesRange(symb(HEXDIG),1,4);
        case dec_octet: return or(or(symb(DIGIT),concat(charBetween(0x31,0x39),symb(DIGIT))), or(concat(letter('1'),concat(symb(DIGIT),symb(DIGIT))), or(concat(letter('2'),concat(charBetween(0x30,0x34),symb(DIGIT))),concat(letter('2'),concat(letter('5'),charBetween(0x30,0x35))))));
        case IPv4address: return concat(nOccurences(concat(symb(dec_octet),letter('.')),3),symb(dec_octet));
        case ls32: return or(concat( symb(h16), concat( letter(':'), symb(h16))),symb(IPv4address));
        // case IPv6address: {
        //     reader symbPoints = concat( letter(':'), letter(':')); //"::"
        //     reader symbh16 = concat( symb(h16), letter(':')); //h16":"
        //     reader reader1 = nOccurences( symbh16, 6); //6( h16 ":") ls32
        //     reader reader2 = concat( concat( symbPoints, nOccurences( symbh16, 5)), symb(ls32)); //"::"5( h16 ":") ls32
        //     reader reader3 = concat( concat( concat( optionnal(symb(h16)), symbPoints), nOccurences( symbh16, 4)), symb(ls32)); //[ h16 ]"::"4( h16 ":") ls32
        //     reader reader4 = concat( concat( concat( optionnal(concat( nOccurencesMax(symbh16,1),symb(h16))), symbPoints), nOccurences( symbh16, 3)), symb(ls32)); //[*1( h16 ":") h16 ]"::"3( h16 ":") ls32
        //     reader reader5 = concat( concat( concat( optionnal(concat( nOccurencesMax(symbh16,2),symb(h16))), symbPoints), nOccurences( symbh16, 2)), symb(ls32)); //[*2( h16 ":") h16 ]"::"2( h16 ":") ls32
        //     reader reader6 = concat( concat( concat( optionnal(concat( nOccurencesMax(symbh16,3),symb(h16))), symbPoints), symbh16), symb(ls32)); //[*3( h16 ":") h16 ]"::" h16 ":" ls32
        //     reader reader7 = concat( concat( optionnal(concat( nOccurencesMax(symbh16,4),symb(h16))), symbPoints), symb(ls32)); //[*4( h16 ":") h16 ]"::" ls32
        //     reader reader8 = concat( concat( optionnal(concat( nOccurencesMax(symbh16,5),symb(h16))), symbPoints), symb(h16));// [*5( h16 ":") h16 ]"::" h16
        //     reader reader9 = concat( optionnal(concat( nOccurencesMax(symbh16,6),symb(h16))), symbPoints);//[*6( h16 ":") h16 ]"::"
        //     return or(or(or(or( reader1, reader2), reader3),or( reader4, reader5)),or(or( reader6, reader7),or( reader8, reader9)));
        // }
        case IPv6address: return bad_symbole();
        //case IPvFuture: return concat( concat( concat( letter('v'), symb(HEXDIG)), concat( letter('.'), nOccurencesMin(symb(HEXDIG),1))), concat( or( or( symb(unreserved), symb(sub_delims)), letter(':')), nOccurencesMin(or( or( symb(unreserved), symb(sub_delims)), letter(':')),1)));
        case IPvFuture: return concat( letter('v'), concat( nOccurencesMin( symb(HEXDIG), 1), concat( letter('.'), nOccurencesMin( or( or( symb(unreserved), symb(sub_delims)), letter(':')), 1))));
        case IP_literal: return concat(concat( letter('['), or( symb(IPv6address), symb(IPvFuture))), letter(']'));
        case reg_name: return kleene(or(or( symb(unreserved), symb(pct_encoded)), symb(sub_delims)));
        case host: return or(or( symb(IP_literal), symb(IPv4address)), symb(reg_name));
        case port: return kleene(symb(DIGIT));
        case authority: return concat(concat(optionnal(concat(symb(userinfo),letter('@'))), symb(host)), optionnal(concat(letter(':'),symb(port))));
        case path_abempty: return kleene(concat(letter('/'), symb(segment)));
        case segment_nz: return nOccurencesMin(symb(pchar),1);
        //case path_absolute: return concat(concat( letter('/'), symb(segment_nz)), kleene(concat(letter('/'),symb(segment))));
        case path_absolute: return concat(letter('/'),optionnal(concat(symb(segment_nz),kleene(concat(letter('/'),symb(segment_nz))))));
        case path_rootless: return concat( symb(segment_nz), kleene(concat(letter('/'),symb(segment))));
        case path_empty: return epsilon(); //pas sur le la signification du 0!
        case hier_part: return concat( concat(letter('/'),letter('/')), or(or( concat( symb(authority), symb(path_abempty)), symb(path_absolute)),or( symb(path_rootless), symb(path_empty))));
        case absolute_URI: return concat(concat( symb(scheme), letter(':')), concat( symb(hier_part), optionnal(concat(letter('?'),symb(query)))));
        case absolute_form: return symb(absolute_URI);
        case authority_form: return symb(authority);
        case asterisk_form: return letter('*');
        case request_target: return or(or( symb(origin_form), symb(absolute_form)), or( symb(authority_form), symb(asterisk_form)));
        case HTTP_name: return concat(letter('H'),concat(letter('T'),concat(letter('T'),letter('P')))); // à remplacer par word_s("HTTP",4)
        case HTTP_version: return concat(concat(concat( symb(HTTP_name), letter('/')), symb(DIGIT)),concat( letter('.'), symb(DIGIT)));
        case request_line: return concat(concat(concat( symb(method), symb(SP)), concat( symb(request_target), symb(SP))),concat( symb(HTTP_version), symb(CRLF)));
        case status_code: return nOccurences(symb(DIGIT),3);
        case reason_phrase: return kleene(or(or( symb(HTAB), symb(SP)),or( symb(VCHAR), symb(obs_text))));
        case status_line: return concat(concat(concat( symb(HTTP_version), symb(SP)), concat( symb(status_code), symb(SP))),concat( symb(reason_phrase), symb(CRLF)));
        case start_line: return or(symb(request_line), symb(status_line));
        case message_body: return kleene(symb(OCTET));
        case colon: return letter(':');//sale
        case connection_option: return symb(token);
        case Connection: return concat(concat( kleene(concat(letter(','), symb(OWS))), symb(connection_option)), kleene(concat(concat( symb(OWS), letter(',')), optionnal(concat( symb(OWS), symb(connection_option))))));
        case Connection_header: return header("Connection:",11,Connection);
        case Content_Length: return nOccurencesMin(symb(DIGIT),1);
        case Content_Length_header: return header("Content-Length:",15,Content_Length);
        case Trailer: return list(field_name);
        case Trailer_header: return header("Trailer:",8,Trailer);
        case qdtext: return or( or( or( symb(HTAB), symb(SP)), letter('!')), or( or( charBetween(0x23,0x5B), charBetween(0x5D,0x7E)), symb(obs_text)));
        case quoted_pair: return concat( letter('\\'), or( or( symb(HTAB), symb(SP)), or ( symb(VCHAR), symb(obs_text))));
        case quoted_string: return concat( concat( symb(DQUOTE), kleene(or( symb(qdtext), symb(quoted_pair)))), symb(DQUOTE));
        case transfer_parameter: return concat(concat(concat( symb(token), symb(BWS)), letter('=')),concat( symb(BWS), or(symb(token), symb(quoted_string))));
        case transfer_extension: return concat( symb(token), kleene(concat( concat( symb(OWS), letter(';')), concat( symb(OWS), symb(transfer_parameter)))));
        case transfer_coding: return or( or( or( word_s("chunked",7), word_s("compress",8)), word_s("deflate",7)), or( word_s("gzip",4), symb(transfer_extension)));
        case Transfer_Encoding: return list(transfer_coding);
        case Transfer_Encoding_header: return header("Transfer-Encoding:", 18, Transfer_Encoding);
        case protocol_name: return symb(token);
        case protocol_version: return symb(token);
        case protocol: return concat( symb(protocol_name), optionnal(concat( letter('/'), symb(protocol_version))));
        case Upgrade: return list(protocol);
        case Upgrade_header: return header("Upgrade:",8,Upgrade);
        case received_protocol: return concat(optionnal(concat(symb(protocol_name),letter('/'))),symb(protocol_version));
        case pseudonym: return symb(token);
        case uri_host: return symb(host);
        case received_by: return or(concat(symb(uri_host),optionnal(concat(letter(':'),symb(port)))),symb(pseudonym));
        case ctext: return or(or(or( symb(HTAB), symb(SP)), charBetween(0x21,0x27)), or(or( charBetween(0x2A,0x5B), charBetween(0x5D,0x7E)), symb(obs_text)));
        case comment: return concat(concat( letter('('), kleene(or(or( symb(ctext), symb(quoted_pair)), symb(comment)))), letter(')'));
        case Via: concat(concat( kleene(concat(letter(','), symb(OWS))), concat(concat( symb(received_protocol), symb(RWS)), concat( symb(received_by), optionnal(concat(symb(RWS), symb(comment)))))), kleene(concat(concat( symb(OWS), letter(',')), optionnal(concat(concat( symb(received_protocol), symb(RWS)), concat( symb(received_by), optionnal(concat(symb(RWS), symb(comment)))))))));
        case Via_header: return header("Via:",4,Via);
        case delta_seconds: return nOccurencesMin(symb(DIGIT),1);
        case Age: return symb(delta_seconds);
        case Age_header: return header("Age:",4,Age);
        case day_name: return or(or(or( word_s("Mon",3), word_s("Tue",3)), or( word_s("Wed",3), word_s("Thu",3))), or(or( word_s("Fri",3), word_s("Sat",3)), word_s("Sun",3)));
        case day: return nOccurences(symb(DIGIT),2);
        case month: return or(or(or(or( word_s("Jan",3), word_s("Feb",3)),or( word_s("Mar",3), word_s("Apr",3))),or(or( word_s("May",3), word_s("Jun",3)),or( word_s("Jul",3), word_s("Aug",3)))),or(or( word_s("Sep",3), word_s("Oct",3)),or( word_s("Nov",3), word_s("Dec",3))));
        case year: return nOccurences(symb(DIGIT),4);
        case date1: return concat(concat(concat( symb(day), symb(SP)),concat( symb(month), symb(SP))), symb(year));
        case hour: return nOccurences(symb(DIGIT),2);
        case minute: return nOccurences(symb(DIGIT),2);
        case second: return nOccurences(symb(DIGIT),2);
        case time_of_day: return concat(concat(concat( symb(hour), letter(':')),concat( symb(minute), letter(':'))), symb(second));
        case GMT: return word_s("GMT",3);
        case IMF_fixdate: return concat(concat(concat( symb(day_name), letter(',')),concat( symb(SP), symb(date1))), concat(concat( symb(SP), symb(time_of_day)), concat( symb(SP), symb(GMT))));
        case day_name_l: return or(or(or( word_s("Monday",6), word_s("Tuesday",7)), or( word_s("Wednesday",9), word_s("Thursday",8))), or(or( word_s("Friday",6), word_s("Saturday",8)), word_s("Sunday",6)));
        case date2: return concat(concat(concat( symb(day), letter('-')),concat( symb(month), letter('-'))), nOccurences(symb(DIGIT),2));
        case rfc850_date: return concat(concat(concat( symb(day_name_l), letter(',')),concat( symb(SP), symb(date2))), concat(concat( symb(SP), symb(time_of_day)), concat( symb(SP), symb(GMT))));
        case date3: return concat(concat( symb(month), symb(SP)), or( nOccurences(symb(DIGIT),2), concat( symb(SP), symb(DIGIT))));
        case asctime_date: return concat(concat(concat( symb(day_name), symb(SP)),concat( symb(date3), symb(SP))), concat(concat( symb(time_of_day), symb(SP)), symb(year)));
        case obs_date: return or(symb(rfc850_date), symb(asctime_date));
        case HTTP_date: return or(symb(IMF_fixdate),symb(obs_date));
        case Expires: return symb(HTTP_date);
        case Expires_header: return header("Expires:",8,Expires);
        default: return bad_symbole();
    }
}