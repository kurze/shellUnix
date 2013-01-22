#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <pthread.h>

#include "shell_fct.h"
#include "constante.h"
#include "serveur.h"
#include "client.h"


int main(int argc, char** argv){

	pthread_t threadClient, threadServeur;
	if(argc!=3 || atoi(argv[1])<1024 || atoi(argv[1])>65535){
		printf("./myshell <numéro de port d'écoute> <action>\n\
				\tnuméro de port compris entre 1024 et 65535\n\
				\taction : 0 = client, 1 = serveur\n");
		exit(EXIT_FAILURE);
	}
	if(atoi(argv[2])==0){
		if(pthread_create(&threadClient, NULL, (void *)client, NULL)){
			perror("erreur de création de thread\n");
			exit(EXIT_FAILURE);
		}
		pthread_join(threadClient, NULL);
	}
	if(atoi(argv[2])==1){
		if(pthread_create(&threadServeur, NULL, (void *)serveur, (void *)argv[1])){
			perror("erreur de création de thread\n");
			exit(EXIT_FAILURE);
		}
		pthread_join(threadServeur, NULL);
	}

	return 0;
}
