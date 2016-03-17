#ifndef READER_H
#define READER_H

#include "StringL.h"
#include <stdlib.h>

typedef enum {
    FAIL,
    SUCC,
} read_state;


typedef struct {
    read_state state;
    StringL string;
} read_return;

//type de closure de lecture (emulation du comportement d'une closure)
typedef struct {
    void* ctxt;
    read_return (*fun)(void*); //code de la closure
} reader;

//#define CALL_CLOSURE(NAME) NAME ## .closure(( ## NAME ## _context) ## NAME ## .context)
#define CALL_CLOSURE(NAME) NAME.fun(NAME.ctxt)

//symboles syntaxiques simples
typedef enum {
    SP,
    HTAB,
    OWS,
    DIGIT,
    day,
    CRLF,
    ALPHA,
    alphanum,
    unreserved,
    tchar,
    token,
    field_name,
    VCHAR,
    obs_text,
    field_vchar,
    field_content,
    obs_fold,
    field_value,
    header_field,
    method,
    HEXDIG,
    pct_encoded,
    sub_delims,
    pchar,
    segment,
    absolute_path,
    query,
    origin_form,
    scheme,
    userinfo,
    h16,
    dec_octet,
    IPv4address,
    ls32,
    IPv6address,
    IPvFuture,
    IP_literal,
    reg_name,
    host,
    authority,
    path_abempty,
    segment_nz,
    path_absolute,
    path_rootless,
    path_empty,
    hier_part,
    absolute_URI,
    absolute_form,
    authority_form,
    asterisk_form,
    request_target,
    port,
    HTTP_name,
    HTTP_version,
    request_line,
    start_line,
    status_code,
    reason_phrase,
    status_line,
    OCTET,
    colon,
    message_body
} syntaxe_elem;

//foncteur de lecture conditionnel (retourne une fonction qui lit un segment)
reader read(syntaxe_elem se, StringL* buff);


#endif
