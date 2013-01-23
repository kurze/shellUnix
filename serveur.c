#include "serveur.h"


void serveur(char * port){

	struct sockaddr_in adrSocket;
// 	pthread_t thread;
	int idSocket;
	int fdSocket;
	unsigned int taille;

	// crée la socket
	idSocket = socket(AF_INET, SOCK_STREAM,0);

	//configure la socket
	adrSocket.sin_family = AF_INET;
	adrSocket.sin_port = htons(atoi(port));
	adrSocket.sin_addr.s_addr = INADDR_ANY;


	// nomme la socket
	if(bind(idSocket, (struct sockaddr *)(&adrSocket), sizeof(adrSocket)) == -1){

	}

	// place en écoute
	listen(idSocket, 10);

	taille = sizeof(struct sockaddr_in);

	while(1){
		fdSocket = accept(idSocket, (struct sockaddr *)(&adrSocket), &taille);
		// traiter la connexion

// 		if(pthread_create(&thread, NULL, (void *)executionCommandeRecu, (void *)fdSocket)){
// 			perror("erreur de création de thread\n");
// 			exit(EXIT_FAILURE);
// 		}

		if(fork()==0){
			executionCommandeRecu(fdSocket);
			exit(0);
		}
		close(fdSocket);
	}
}

void executionCommandeRecu(int fdSocket){
	char commande[256];
// 	int sauvSortie=10;

	cmd * mycmd;
	mycmd = (cmd *) malloc(sizeof(cmd) * 1);
	initCMD(mycmd);

	if(recv(fdSocket, commande, 256, 0) == -1){
		perror("erreur lors de la réception de la commande");
		exit(EXIT_FAILURE);
	}
// 	dup2(1, sauvSortie);
	close(0);
	dup(fdSocket);
	close(1);
	dup(fdSocket);
// 	close(2);
// 	dup(fdSocket);
	close(fdSocket);
	executerCommande(commande, mycmd);
	close(0);
	close(1);
// 	dup2(sauvSortie, 1);
// 	printf("fin de thread executionCommandeRecu\n");
}
