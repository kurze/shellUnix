#include "serveur.h"


void serveur(char * port){

	struct sockaddr_in adrSocket;
	pthread_t thread;
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

	while(1){
		taille = sizeof(struct sockaddr_in);
		fdSocket = accept(idSocket, (struct sockaddr *)(&adrSocket), &taille);
		// traiter la connexion
		if(pthread_create(&thread, NULL, (void *)executionCommandeRecu, (void *)fdSocket)){
			perror("erreur de création de thread\n");
			exit(EXIT_FAILURE);
		}
// 		close(fdSocket);
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

void executionCommandeRecu(int fdSocket){
	char commande[256];

	cmd * mycmd;
	mycmd = (cmd *) malloc(sizeof(cmd) * 1);
	initCMD(mycmd);

	if(recv(fdSocket, commande, 256, 0) == -1){
		perror("erreur lors de la réception de la commande");
		exit(EXIT_FAILURE);
	}
// 	printf("%s\n", commande);

	close(0);
	dup(fdSocket);
	close(1);
	dup(fdSocket);
// 	sleep(2);
	executerCommande(commande, mycmd);
// 	sleep(2);
	close(0);
	close(1);
	close(fdSocket);
}
