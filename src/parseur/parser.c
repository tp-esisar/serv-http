#include "parser.h"
#include "reader.h"
#include "../map/map.h"

#define RET_FAIL (parse_return){FAIL,NULL}

#define LIRE(SYMB,VAR) do {\
	rr=read(SYMB,buff);\
    if(rr.state==FAIL) {\
        return RET_FAIL;\
    }\
	VAR = rr.string;\
}while(0)


int parser (char *buf,unsigned int len,char *search, void (*callback)()) {
  
  return -1;
}

parse_return parse_HTTP_message(StringL* buff) {
    StringL methode;
    StringL request_target;
    StringL http_version;
	StringL headerField;
	StringL field_name;
	StringL field_value;
	StringL saveBuffer;
    read_return rr;
	mapStruct* map;
    
    LIRE(methode,methode);
	LIRE(request_target,request_target);
	LIRE(http_version,http_version);

	map = init_map(methode,request_target,http_version);

	while(read(CLRF,buff).state == FAIL){
		saveBuffer = *buff;
		LIRE(field_name,field_name);
		buff = *saveBuffer;
		LIRE(header_field,headerField);
		
		if( add_field( map, field_name, headerField) == 1){
			return -1;
		}
	}
	
}
