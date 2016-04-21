#include "reader.h"
#include "gc.h"
#include <stdio.h>
#include <stdlib.h>
#include "base_readers.c"

#define symb(X) get_reader(X,wBuff)
//#define nOccurencesRange(X,n1,n2) nOccurencesMax(nOccurencesMin(X,n1),n2)
#define nOccurencesRange(X,n1,n2) and(nOccurencesMin(X,n1),nOccurencesMin(X,n2))

#define header(X,n,Y) concat(concat( word_s(X,n), symb(OWS)),concat( symb(Y), symb(OWS)))
#define word_s(X,n) word(((StringL){X,n}))
#define list(X) concat( concat( kleene(concat( letter(','), symb(OWS))), X), kleene(concat( concat( symb(OWS), letter(',')), optionnal(concat( symb(OWS), X)))))

reader get_reader(syntaxe_elem se, StringL* wBuff) {
    
    switch(se) {
        case SP: return letter(' ');
        case HTAB: return letter('\t');
        case OWS: return kleene(or(letter(' '),letter('\t')));
        //case CRLF: return concat(letter('\r'),letter('\n')); //CRLF conforme (ne marche pas pour le debug avec l'encodage linux)
        case CRLF: return or(letter('\n'),concat(letter('\r'),letter('\n'))); //CRLF non conforme à utiliser pour le debug sur linux
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
        case tchar: return or(symb(alphanum),charIn(((StringL){"!#$\%&'*+-.^_`|~",16})));
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
        //case HTTP_name: return concat(letter('H'),concat(letter('T'),concat(letter('T'),letter('P')))); // à remplacer par word_s("HTTP",4)
        case HTTP_name: return word_s("HTTP",4);
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
        case Trailer: return list(symb(field_name));
        case Trailer_header: return header("Trailer:",8,Trailer);
        case qdtext: return or( or( or( symb(HTAB), symb(SP)), letter('!')), or( or( charBetween(0x23,0x5B), charBetween(0x5D,0x7E)), symb(obs_text)));
        case quoted_pair: return concat( letter('\\'), or( or( symb(HTAB), symb(SP)), or ( symb(VCHAR), symb(obs_text)))); //vaut il mieux mettre le code du "\" ... does it even work ?
        case quoted_string: return concat( concat( symb(DQUOTE), kleene(or( symb(qdtext), symb(quoted_pair)))), symb(DQUOTE));
        case transfer_parameter: return concat(concat(concat( symb(token), symb(BWS)), letter('=')),concat( symb(BWS), or(symb(token), symb(quoted_string))));
        case transfer_extension: return concat( symb(token), kleene(concat( concat( symb(OWS), letter(';')), concat( symb(OWS), symb(transfer_parameter)))));
        case transfer_coding: return or( or( or( word_s("chunked",7), word_s("compress",8)), word_s("deflate",7)), or( word_s("gzip",4), symb(transfer_extension)));
        case Transfer_Encoding: return list(symb(transfer_coding));
        case Transfer_Encoding_header: return header("Transfer-Encoding:", 18, Transfer_Encoding);
        case protocol_name: return symb(token);
        case protocol_version: return symb(token);
        case protocol: return concat( symb(protocol_name), optionnal(concat( letter('/'), symb(protocol_version))));
        case Upgrade: return list(symb(protocol));
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
        case Date: return symb(HTTP_date);
        case Date_header: return header("Date:",5,Date);
        case fragment: return kleene(or(or( symb(pchar), letter('/')), letter('?')));
        case URI: return  concat(concat(concat( symb(scheme), letter(':')),concat( symb(hier_part), optionnal(concat( letter('?'), symb(query))))), optionnal(concat( letter('#'), symb(fragment))));
        case segment_nz_nc: return nOccurencesMin(or(or( symb(unreserved), symb(pct_encoded)),or( symb(sub_delims), letter('@'))),1);
        case path_noscheme: return concat( symb(segment_nz_nc), kleene(concat(letter('/'), symb(segment))));
        case relative_part: return concat( word_s("//",2), or(or( concat( symb(authority), symb(path_abempty)), symb(path_absolute)),or( symb(path_noscheme), symb(path_empty))));
        case relative_ref: return concat(concat( symb(relative_part), optionnal(concat( letter('?'), symb(query)))), optionnal(concat( letter('#'), symb(fragment))));
        case URI_reference: return or(symb(URI), symb(relative_ref));
        case Location: return symb(URI_reference);
        case Location_header: return header("Location:",9,Location);
        case delay_seconds: return nOccurencesMin(symb(DIGIT),1);
        case Retry_After: return or(symb(HTTP_date), symb(delay_seconds));
        case Retry_After_header: return header("Retry-After:",12,Retry_After);
        case Vary: return or(letter('*'), concat(concat(kleene(concat(letter(','), symb(OWS))), symb(field_name)), kleene(concat(concat(symb(OWS), letter(',')), optionnal(concat(symb(OWS), symb(field_name)))))));
        case Vary_header: return header("Vary:",5,Vary);
        case warn_code: return nOccurences(symb(DIGIT),3);
        case warn_agent: return or(concat(symb(uri_host),optionnal(concat(letter(':'),symb(port)))),symb(pseudonym));
        case warn_text: return symb(quoted_string);
        case warn_date: return concat(concat(symb(DQUOTE), symb(HTTP_date)), symb(DQUOTE));
        case warning_value: return concat(concat(concat( symb(warn_code), symb(SP)),concat( symb(warn_agent), symb(SP))),concat( symb(warn_text), optionnal(concat(symb(SP), symb(warn_date)))));
        case Warning: return list(symb(warning_value));
        case Warning_header: return header("Warning:",8,Warning);
        case cache_directive: return concat(symb(token), optionnal(concat(letter('='), or(symb(token), symb(quoted_string)))));
        case Cache_Control: return list(symb(cache_directive));
        case Cache_Control_header: return header("Cache-Control:",14,Cache_Control);
        case Expect: return word_s("100-continue",12);
        case Expect_header: return header("Expect:",7,Expect);
        case Host: return concat(symb(uri_host), optionnal(concat(letter(':'), symb(port))));
        case Host_header: return header("Host:",5,Host);
        case Max_Forwards: return nOccurencesMin(symb(DIGIT),1);
        case Max_Forwards_header: return header("Max-Forwards:",13,Max_Forwards);
        case extension_pragma: return concat(symb(token), optionnal(concat(letter('='), or(symb(token), symb(quoted_string)))));
        case pragma_directive: return or(word_s("no-cache",8), symb(extension_pragma));
        case Pragma: return list(symb(Pragma));
        case Pragma_header: return header("Pragma:",7,Pragma);
        case bytes_unit: return word_s("bytes",5);
        case first_byte_pos: return nOccurencesMin(symb(DIGIT),1);
        case last_byte_pos: return nOccurencesMin(symb(DIGIT),1);
        case byte_range_spec: return concat(concat(symb(first_byte_pos), letter('-')), optionnal(symb(last_byte_pos)));
        case suffix_length: return nOccurencesMin(symb(DIGIT),1);
        case suffix_byte_range_spec: return concat(letter('-'), symb(suffix_length));
        case byte_range_set: return list(or(symb(byte_range_spec), symb(suffix_byte_range_spec)));
        case byte_ranges_specifier: return concat(concat(symb(bytes_unit), letter('=')), symb(byte_range_set));
        case other_range_unit: return symb(token);
        case other_range_set: return nOccurencesMin(symb(VCHAR),1);
        case other_ranges_specifier: return concat(concat(symb(other_range_unit), letter('=')), symb(other_range_set));
        case Range: return or(symb(bytes_unit), symb(other_range_unit));
        case Range_header: return header("Range:",6,Range);
        case rank: return or(concat(letter('0'), optionnal(concat(letter('.'), nOccurencesMax(symb(DIGIT),3)))),concat(letter('1'), optionnal(concat(letter('.'), nOccurencesMax(letter('0'),3)))));
        case t_ranking: return concat(concat(concat(symb(OWS), letter(';')),concat(symb(OWS), word_s("q=",2))), symb(rank));
        case t_codings: return or(word_s("trailers",8), concat(symb(transfer_coding), optionnal(symb(t_ranking))));
        case TE: return optionnal(concat(or(letter(','), symb(t_codings)), kleene(concat(concat(symb(OWS), letter(',')), optionnal(concat(symb(OWS), symb(t_codings)))))));
        case TE_header: return header("TE:",3,TE);
        case weak: return word_s("W/",2);
        case etagc: return or(letter('!'), charBetween(0x23,0x7E));
        case opaque_tag: return concat(concat(symb(DQUOTE), kleene(symb(etagc))), symb(DQUOTE));
        case entity_tag: return concat(optionnal(symb(weak)), symb(opaque_tag));
        case If_Match: return or(letter('*'), list(symb(entity_tag)));
        case If_Match_header: return header("If-Match:",9,If_Match);
        case If_None_Match: return or(letter('*'), list(symb(entity_tag)));
        case If_None_Match_header: return header("If-None-Match:",14,If_None_Match);
        case If_Modified_Since: return symb(HTTP_date);
        case If_Modified_Since_header: return header("If-Modified-Since:",18,If_Modified_Since);
        case If_Unmodified_Since: return symb(HTTP_date);
        case If_Unmodified_Since_header: return header("If-Unmodified-Since:",20,If_Unmodified_Since);
        case If_Range: return or(symb(entity_tag), symb(HTTP_date));
        case If_Range_header: return header("If-Range:",9,If_Range);
        case type: return symb(token);
        case subtype: return symb(token);
        case parameter: return concat(concat(symb(token), letter('=')), or(symb(token), symb(quoted_string)));
        case media_range: return concat(or(or(word_s("*/*",3), concat(symb(type), word_s("/*",2))), concat(concat(symb(type), letter('/')), symb(subtype))), kleene(concat(concat(symb(OWS), letter(';')),concat(symb(OWS), symb(parameter)))));
        case qvalue: return or(concat(letter('0'), optionnal(concat(letter('.'), nOccurencesMax(symb(DIGIT),3)))),concat(letter('1'), optionnal(concat(letter('.'), nOccurencesMax(letter('0'),3)))));
        case weight: return concat(concat(concat(symb(OWS), letter(';')),concat(symb(OWS), word_s("q=",2))), symb(qvalue));
        case accept_ext: return concat(concat(concat(symb(OWS), letter(';')),concat(symb(OWS), symb(token))), optionnal(concat(letter('='), or(symb(token),symb(quoted_string)))));
        case accept_params: return concat(symb(weight), kleene(symb(accept_ext)));
        case Accept: return optionnal(concat(or(letter(','), concat(symb(media_range), optionnal(symb(accept_params)))), kleene(concat(concat(symb(OWS), letter(',')), optionnal(concat(symb(OWS), concat(symb(media_range), optionnal(symb(accept_params)))))))));
        case Accept_header: return header("Accept:",7,Accept);
        case charset: return symb(token);
        case Accept_Charset: return list(concat(or(symb(charset), letter('*')), optionnal(symb(weight))));
        case Accept_Charset_header: return header("Accept-Charset:",15,Accept_Charset);
        case content_coding: return symb(token);
        case codings: return or(or(symb(content_coding), word_s("identity",8)), letter('*'));
        case Accept_Encoding: return optionnal(concat(or(letter(','), concat(symb(codings), optionnal(symb(weight)))), kleene(concat(concat(symb(OWS), letter(',')), optionnal(concat(symb(OWS), concat(symb(OWS), optionnal(symb(weight)))))))));
        case Accept_Encoding_header: return header("Accept-Encoding:",16,Accept_Encoding);
        case language_range: return or(concat(nOccurencesRange(symb(ALPHA),1,8), kleene(concat(letter('-'), nOccurencesRange(symb(alphanum),1,8)))), letter('*'));
        case Accept_Language: return list(concat(symb(language_range), optionnal(symb(weight))));
        case Accept_Language_header: return header("Accept-Language:",16,Accept_Language);
        case auth_scheme: return symb(token);
        case token68: return concat(nOccurencesMin(or(or(or(symb(ALPHA),symb(DIGIT)),or(letter('-'),letter('.'))),or(or(letter('_'),letter('~')),or(letter('+'),letter('/')))),1),kleene(letter('=')));
        case auth_param: return concat(concat(concat(symb(token), symb(BWS)), concat(letter('='), symb(BWS))), or(symb(token), symb(quoted_string)));
        case credentials: return concat(symb(auth_scheme), optionnal(concat(nOccurencesMin(symb(SP),1), or(symb(token68), optionnal(concat(or(letter(','), symb(auth_param)), kleene(concat(concat(symb(OWS), letter(',')), optionnal(concat(symb(OWS), symb(auth_param)))))))))));
        case Authorization: return symb(credentials);
        case Authorization_header: return header("Authorization:",14,Authorization);
        case Proxy_Authorization: return symb(credentials);
        case Proxy_Authorization_header: return header("Proxy-Authorization:",20,Proxy_Authorization);
        case partial_URI: return concat(symb(relative_part), optionnal(concat(letter('?'), symb(query))));
        case Referer: return or(symb(absolute_URI), symb(partial_URI));
        case Referer_header: return header("Referer:",8,Referer);
        case product_version: return symb(token);
        case product: return concat(symb(token), optionnal(concat(letter('/'), symb(product_version))));
        case User_Agent: return concat(symb(product), kleene(concat(symb(RWS), or(symb(product), symb(comment)))));
        case cookie_name: return symb(token);
        case cookie_octet: return or(or(or(letter('!'), charBetween(0x23,0x2B)), or(charBetween(0x2D,0x3A), charBetween(0x3C,0x5B))), charBetween(0x5D,0x7E));
        case cookie_value: return or(kleene(symb(cookie_octet)), concat(concat(symb(DQUOTE), symb(cookie_octet)), symb(cookie_octet)));
        case cookie_pair: return concat(concat(symb(cookie_name), letter('=')), symb(cookie_value));
        case cookie_string: return concat(symb(cookie_pair),kleene(concat(concat(letter(';'), symb(SP)), symb(cookie_pair))));
        case cookie_header: return header("Cookie:",7,cookie_string);
        default: return bad_symbole();
    }
}