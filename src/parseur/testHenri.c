/*test(name,symbole,code_retour,buff_départ,str_retour,buff_après)
-name: string nom du test 
-symbole: symbole de l'enum
-code_retour: code de retour attendu du reader (SUCC ou FAIL) c'est pour tester si ton reader fail bien dans certain cas
-buff_entrée: string état du buffer de lecture avant la lecture
-str_retour: string que le reader doit retourner en cas de SUCC (met "" en cas de FAIL)
-buff_après: état du buffer de lecture après la lecture
le but est de réaliser 2 ou 3 test pour les symboles les plus importants (pas tous evidement)*/

test("get message",HTTP_message,SUCC,"GET /path/file.html HTTP/1.0 \r\nFrom: someuser@jmarshall.com\r\nUser-Agent: HTTPTool/1.0\r\n\r\n","GET /path/file.html HTTP/1.0 \r\nFrom: someuser@jmarshall.com\r\nUser-Agent: HTTPTool/1.0\r\n\r\n","");