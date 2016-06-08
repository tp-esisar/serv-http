#include "StringL.h"
#include "cJSON.h"


#ifndef FCGI_H
#define FCGI_H



typedef struct {
    unsigned char nameLength;
    unsigned char valueLength;
    unsigned char nameAndValue[];
} __attribute__((packed,aligned(1))) FCGI_NameValuePair11;

typedef struct {
    unsigned char nameLength;
    unsigned long int valueLength;
    unsigned char nameAndValue[];
} __attribute__((packed,aligned(1))) FCGI_NameValuePair14;

typedef struct {
    unsigned long int nameLength;
    unsigned char valueLength;
    unsigned char nameAndValue[];
} __attribute__((packed,aligned(1))) FCGI_NameValuePair41;

typedef struct {
    unsigned long int nameLength;
    unsigned long int valueLength;
    unsigned char nameAndValue[];
} __attribute__((packed,aligned(1))) FCGI_NameValuePair44;


/*
 * Listening socket file number
 */
#define FCGI_LISTENSOCK_FILENO 0

typedef struct {
    unsigned char version;
    unsigned char type;
    unsigned short requestId;
    unsigned short contentLength;
    unsigned char paddingLength;
    unsigned char reserved;
} __attribute__((packed,aligned(1))) FCGI_Header;

FCGI_Header make_FCGI_Header(unsigned char type, unsigned short requestId, unsigned short contentLength, unsigned char paddingLength);

typedef struct {
    FCGI_Header header;
    unsigned char dataAndPad[];
} __attribute__((packed,aligned(1))) FCGI_Record_generic;


/*
 * Number of bytes in a FCGI_Header.  Future versions of the protocol
 * will not reduce this number.
 */
#define FCGI_HEADER_LEN  8

/*
 * Value for version component of FCGI_Header
 */
#define FCGI_VERSION_1           1

/*
 * Values for type component of FCGI_Header
 */
#define FCGI_BEGIN_REQUEST       1
#define FCGI_ABORT_REQUEST       2
#define FCGI_END_REQUEST         3
#define FCGI_PARAMS              4
#define FCGI_STDIN               5
#define FCGI_STDOUT              6
#define FCGI_STDERR              7
#define FCGI_DATA                8
#define FCGI_GET_VALUES          9
#define FCGI_GET_VALUES_RESULT  10
#define FCGI_UNKNOWN_TYPE       11
#define FCGI_MAXTYPE (FCGI_UNKNOWN_TYPE)

/*
 * Value for requestId component of FCGI_Header
 */
#define FCGI_NULL_REQUEST_ID     0

typedef struct {
    unsigned short role;
    unsigned char flags;
    unsigned char reserved[5];
} __attribute__((packed,aligned(1))) FCGI_BeginRequestBody;

typedef struct {
    FCGI_Header header;
    FCGI_BeginRequestBody body;
} __attribute__((packed,aligned(1))) FCGI_BeginRequestRecord;
FCGI_BeginRequestRecord* make_FCGI_BeginRequestRecord(unsigned short requestId,unsigned short role, unsigned char flags);
/*
 * Mask for flags component of FCGI_BeginRequestBody
 */
#define FCGI_KEEP_CONN  1

/*
 * Values for role component of FCGI_BeginRequestBody
 */
#define FCGI_RESPONDER  1
#define FCGI_AUTHORIZER 2
#define FCGI_FILTER     3

typedef struct {
    unsigned long int appStatus;
    unsigned char protocolStatus;
    unsigned char reserved[3];
} __attribute__((packed,aligned(1))) FCGI_EndRequestBody;

typedef struct {
    FCGI_Header header;
    FCGI_EndRequestBody body;
} __attribute__((packed,aligned(1))) FCGI_EndRequestRecord;
FCGI_EndRequestRecord* make_FCGI_EndRequestRecord(unsigned short requestId,unsigned long int appStatus, unsigned char protocolStatus);
/*
 * Values for protocolStatus component of FCGI_EndRequestBody
 */
#define FCGI_REQUEST_COMPLETE 0
#define FCGI_CANT_MPX_CONN    1
#define FCGI_OVERLOADED       2
#define FCGI_UNKNOWN_ROLE     3

/*
 * Variable names for FCGI_GET_VALUES / FCGI_GET_VALUES_RESULT records
 */
#define FCGI_MAX_CONNS  "FCGI_MAX_CONNS"
#define FCGI_MAX_REQS   "FCGI_MAX_REQS"
#define FCGI_MPXS_CONNS "FCGI_MPXS_CONNS"

typedef struct {
    unsigned char type;    
    unsigned char reserved[7];
} __attribute__((packed,aligned(1))) FCGI_UnknownTypeBody;

typedef struct {
    FCGI_Header header;
    FCGI_UnknownTypeBody body;
} __attribute__((packed,aligned(1))) FCGI_UnknownTypeRecord;

FCGI_UnknownTypeRecord* make_FCGI_UnknownTypeRecord(unsigned char type);

typedef struct {
    char variente;
    unsigned long long totalLen;
    char data[];
} __attribute__((packed,aligned(1))) FCGI_ParamWrapper;


typedef struct {
    StringL stdout;
    StringL stderr;
    int status;
} AppResult;


int sendStreamChunk(int sock, unsigned char type, unsigned short requestId, StringL buffer);
AppResult FCGI_Request(StringL stdinbuff, cJSON* param);


#endif
