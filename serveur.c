#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <signal.h>

int main(int argc, char *argv[]){

	struct sockaddr_in adrSocket;
	int idSocket;
	int fdSocket;
	int taille;

	// crée la socket
	ifSocket = socket(AF_INET, SOCK_STREAM,0);

	//configure la socket
	adrSocket.sin_family = AF_UNIX;
	adrSocket.sin_port = htons(1234);
	adrSocket.sin_add_.s_addr(htonl(INADDR_ANY);


	// nomme la socket
	if(bind(idSocket, &adrSocket, sizeof(adrSocket)) == -1){

	}

	// place en écoute
	listen(idSocket, 10);

	while(1){
		taille = sizeof(struct sockaddr_in);
		fdSocket ) accept(idSocket, &adrSocket, &taille);
		// traiter la connexion
	}
}
