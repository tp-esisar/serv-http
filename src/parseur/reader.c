#include "reader.h"

#define RET_FAIL (read_return){FAIL,wBuff}
//un reader est une fonction qui reconnait un certain pattern

//créé un reader dans un certain context
//cette fonction est récursive, elle se rapelle à chaque fois dans un nouveau context réduit
//un context est définit par la position du buffer
reader read(syntaxe_elem se, StringL* buff) {
    StringL wBuff = *buff; //context actuel pour cette récurtion
    
    //créé un reader dans le même context qui réalise un "ou" des deux reader en paramètre
    reader or(reader readerA, reader readerB) {
        //oui c'est une closure dans une closure, incepclosure!
        read_return closure() {
            read_return current;
            if( (current=readerA()).state == SUCC )
                return current;
            else if( (current=readerB()).state == SUCC )
                return current;
            else
                return RET_FAIL;
                
        }
        return closure;
    }
    
    //créé un reader dans le même context qui réalise la fermeture itérative du patten décrit par le reader en paramètre
    reader kleen(reader unit_reader) {
        read_return closure() {
            read_return current;
            int i = 0;
            while( (current=unit_reader()).state == SUCC ) {
                i += current.string.len;
            }
            return (read_return){SUCC,{wBuff.s,i}};
        }
        return closure;
    }
    
    //créé un reader dans le même context qui réalise la concaténation des 2 patterns en paramètre
    reader concat(reader readerA, reader readerB) {
        read_return closure() {
            StringL save = wBuff;
            read_return a, b;
            if( ( (a=readerA()).state == SUCC ) && ( (b=readerB()).state == SUCC ) )
                return (read_return){SUCC,{wBuff.s,a.string.len+b.string.len}};
            else {
                wBuff = save;
                return RET_FAIL;
            }
        }
        return closure;
    }
    
    //créé un reader qui lis un char en particulier (vous avez compris on est pas dans la performance)
    reader letter(char c) {
        read_return closure() {
            if((wBuff.s[0] == c) && (wBuff.len > 0)) {
                read_return rr = {SUCC,{wBuff.s,1}};
                wBuff.len--;
                if(wBuff.len <= 0)
                    wBuff.s = NULL;
                else
                    wBuff.s++;
                return rr;
            }
            else
                return RET_FAIL;
        }
        return closure;
    }
    
    //renvoi un reader qui crash à tout les coups
    reader bad_symbole() {
        read_return closure() {
            return RET_FAIL;
        }
        return closure;
    }
        
    
    switch(se) {
        case SP: return letter(' ');
        case HTAB: return letter('\t');
        case OWS: return kleen(letter(' '));
        default: return bad_symbole();
    }
}

