#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <signal.h>
/*
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
*/


void serveur(int port){

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
	/*
	while(strcmp(commande, "fin")){
		// attente d'une connexion
		if((sockClient = accept(sockServ, (struct sockaddr *)(&adrClient), &adrSize)) == -1) {
			perror("accept");
			return -1;
		}

		// réception des données
		if((lenMsg = recv(sockClient, commande, 256, 0)) == -1) {
			perror("recv");
			return -1;
		}
		commande[lenMsg] = '\0';
		printf("%s\n", commande);

// 		paramCommande = strtok(commande, " ");
// 		printf("\n%s\n%s", commande, paramCommande);

		pid = fork();
		if(pid == 0){
			close(0);
			dup(sockClient);
			close(1);
			dup(sockClient);
			close(2);
			dup(sockClient);
			execlp(commande, commande, NULL);
		}
	}
	// fermeture de la connexion
	close(sockServ);

	return 0;
	*/
}

