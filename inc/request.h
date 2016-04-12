/** 
\file request.h 
\author Quentin Giorgi 
\version   1.0
\date       13 Decembre 2015

\brief      Interface de traitement des requetes et reponses HTTP.
 
Fichier d'interface entre la bibliothèque et votre programme.
Votre programme doit inclure ce fichier d'entete  #include <request.h> 
La compilation doit inclure l'option -L. -lrequest
*/

#define MAXCLIENT 10

/**
 * \struct message
 * \brief Objet message qui peut etre soit une requete, soit une reponse. utilisé pour communiquer entre le programme et la bibliothèque.
 *
 * Pour les requetes l'allocation du pointeur buf est fait par la librairie, le programme recoit l'information par la fonction getRequest, et une fois le message traité par le programme, il doit libérer explicitement le message par la fonction freeRequest. 
 * 
 * Pour les reponses, la librairie recopie les données pointées dans une structure de message interne, le programme peut donc liberer la memoire si necessaire tout de suite après l'appel à sendReponse. 


*/

typedef struct {
	char *buf;				/**< buf Un pointeur vers le message recu */
	unsigned int len;			/**< len La longueur du message recu */
	unsigned int clientId;			/**< clientId identifiant du client, recu dans une requete, doit etre recopier dans la reponse correspondante*/
	struct sockaddr_in *clientAddress; 	/**< iclientAddress pointeur vers une structure permettant de recuperer l'adresse IP et le port du client HTTP */
} message;


/** 
\fn message *getRequest(short int port)
 * \brief Fonction de recuperation d'une requete, cette fonction est bloquante et doit etre appeler dans la boucle principale du programme.
 *  Cette fonction essaie de recuperer une requete entière pour la livrer à votre programme, mais si des cas d'ereur se produise, livre ce qui a été recu à l'instant t sans filtrage, c'est votre programme qui devra traiter ces cas d'erreurs. 
 * \param port port d'ecoute de la socket, utilisé qu'au premier appel de la fonction, ensuite ce parametre est ignoré dans les appels successifs.  
 * \return un pointeur vers une structure message.
 */

message *getRequest(short int port); 

/** 
\fn void freeRequest(message *r)
* \brief Procedure de liberation de la memoire quand le programme en a fini avec une requete (message *). 
* \param r Le message à libérer. 
*/
void freeRequest(message *r); 

/** 
\fn void sendReponse(message *r)
* \brief Procedure d'envoi d'un mesage au client HTTP 
* \param r Le message à envoyer (recopié par la bibliothèque) 
*/
void sendReponse(message *r); 


/** 
\fn void requestShutdownSocket(int i)
* \brief Procedure de demande de cloture de la connexion, si la bibliothèque à encore des données à envoyer d'un sendReponse précédent, la connexion ne sera fermée qu'à la fin de cet envoi.  
* \param i L'Id du client dont on doit fermer la connexion. 
*/
void requestShutdownSocket(int i); 



