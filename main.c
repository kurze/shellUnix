#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include "shell_fct.h"
#include "constante.h"
#include <pthread.h>

#include "client.h"
#include "serveur.h"

int main(int argc, char** argv){

	pthread_t threadClient; /*threadServeur;*/
	if(argc!=2 || atoi(argv[1])<1 || atoi(argv[1])>65535){
		printf("./myshell <numéro de port d'écoute>\n\
				\tnuméro de port compris entre 1024 et 65535\n");
		exit(1);
	}

	if(pthread_create(&threadClient, NULL, (void *)client, NULL)){
		perror("erreur de création de thread\n");
		exit(1);
	}
	pthread_join(threadClient, NULL);

	return 0;
}
