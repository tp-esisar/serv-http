
#include "api_test.h"
#include "parser.h"

int parser (char *buf, unsigned int len, char *search, void (*callback)(char* found, unsigned int len))
{
	parse_return retour;
	StringL buff ={buf, len};
	int nombre;

	retour = parse_HTTP_message(&buff);
	if (retour.state == FAIL)
		return retour.buff.s - buf;

	else
	{
		nombre = search_map(retour.map, search, callback);
		printf("\n --- %d occurences trouvees ---\n", nombre);
	}

    return -1;
}

void extract_stringL (StringL field, void (*callback)(char* found, unsigned int len))
{
    callback(field.s, field.len);
}


