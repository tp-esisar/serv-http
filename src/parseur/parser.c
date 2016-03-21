#include "parser.h"
#include "reader.h"
#include "map.h"

#define LIRE(SYMB,VAR) do {\
	tempReader=get_reader(SYMB,buff);\
	rr = CALL_CLOSURE(tempReader);\
    if(rr.state==FAIL) {\
        return (parse_return){PARSE_FAIL,NULL,*buff};\
    }\
	VAR = rr.string;\
} while(0)

void printSL(StringL str) {
	char* temp = toRegularString(str);
	printf("%s\n",temp);
	free(temp);
}

parse_return parse_HTTP_message(StringL* buff) {
    StringL Smethod;
    StringL Srequest_target;
    StringL SHTTP_version;
	StringL Sheader_field;
	StringL Sfield_name;
	StringL saveBuffer;
    read_return rr;
    reader tempReader;
    mapStruct* map;
    reader crlfReader = get_reader(CRLF,buff);
    reader spReader = get_reader(SP,buff);
	
   	LIRE(method,Smethod);
   	CALL_CLOSURE(spReader);
	LIRE(request_target,Srequest_target);
	CALL_CLOSURE(spReader);
	LIRE(HTTP_version,SHTTP_version);
	CALL_CLOSURE(crlfReader);
	
	

	map = init_map(Smethod,Srequest_target,SHTTP_version);
	
	
	while(CALL_CLOSURE(crlfReader).state == FAIL){
		saveBuffer = *buff;
		LIRE(field_name,Sfield_name);
		
		*buff = saveBuffer;
		LIRE(header_field,Sheader_field);
		
		add_field( map, Sfield_name, Sheader_field);
		
		CALL_CLOSURE(crlfReader);
	}
	
	return (parse_return){PARSE_SUCC,map,*buff};
}
