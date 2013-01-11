#include "cmd.h"

void stringCopy(char ** dest, const char * src){

	size_t taille = 0;
	taille = sizeof(char)*(strlen(src)+1);

// 	if(*dest!=NULL){
// 		free(*dest);
// 	}

	*dest = malloc(taille);
	if(*dest==NULL){
		perror("allocation raté // stringCopy");
		exit(EXIT_FAILURE);
	}
	strcpy(*dest, src);
}

void initCMD(cmd * c){
	c->cmd_initial = NULL;
	c->cmd_membres = NULL;
	c->nb_args_membres = 0;
	c->cmd_args = NULL;
	c->nb_args_membres = NULL;
	c->redirect = NULL;
	c->type_redirect = NULL;
}


void parse_membres(char *chaine,cmd *c){
	unsigned int i=0;
	char * cTmp=NULL, * cTok=NULL;

// 	stringCopy(c->cmd_initial, chaine);
	c->cmd_initial = chaine;
	c->cmd_membres = (char **) malloc(sizeof(char *)*1);
	if(c->cmd_membres==NULL){
		perror("allocation raté // parse_membres");
		exit(EXIT_FAILURE);
	}

	stringCopy(&cTok, c->cmd_initial);

	cTmp = strtok(cTok, "|");
	while( cTmp != NULL){
		// réalocation du tableau
		c->cmd_membres = (char **) realloc(c->cmd_membres, sizeof(char *)*(i+1));
		if(c->cmd_membres==NULL){
			perror("réallocation raté // parse_membres");
			exit(EXIT_FAILURE);
		}

		c->cmd_membres[i]=NULL;
		// affectation
		stringCopy(&(c->cmd_membres[i]), cTmp);

		//extraction du membres suivants
		cTmp = strtok(NULL, "|");

		i++;

	}
	c->nb_membres = i;
	free(cTok);
	free(cTmp);
}

void aff_membres(cmd *c){
	unsigned int i=0;
	printf("\nNombre de membres : %d\n", c->nb_membres);
	while(i < (c->nb_membres)){
		printf("Membre n°%d : %s\n", i, c->cmd_membres[i]);
		i++;
	}
	printf("\n");
}

void free_membres(cmd *c){
	unsigned int i=0;
	if(c->cmd_membres==NULL){
		return;
	}else{
		while( i < c->nb_membres){
			if(c->cmd_membres[i]){
				free(c->cmd_membres[i]);
			}
			i++;
		}
	}
	free(c->cmd_membres);
}


void parse_args(cmd *c){
	unsigned int i, j, redirection;
	char * cTmp=NULL, * cTok=NULL;

	// allocation de la première dimension du tableau
	c->cmd_args = (char ***) malloc(sizeof(char **) * c->nb_membres);
	if(c->cmd_args==NULL)
	{
		perror("allocation raté // parse_args");
		exit(EXIT_FAILURE);
	}

	c->nb_args_membres = (unsigned int *) malloc(sizeof(unsigned int)*c->nb_membres);
	if(c->nb_args_membres==NULL)
	{
		perror("allocation raté // parse_args");
		exit(EXIT_FAILURE);
	}

	for(i=0; i < c->nb_membres; i++)
	{
		j=0;
		c->cmd_args[i] = (char **) malloc(sizeof(char *)*1);
		if(c->cmd_args[i]==NULL){
			perror("allocation raté // parse_args");
			exit(EXIT_FAILURE);
		}

		stringCopy(&cTok, c->cmd_membres[i]);
		cTmp = strtok(cTok, " ");

		redirection = 0;
		while( cTmp != NULL && !redirection){

			// réalocation du tableau
			c->cmd_args[i] = (char **) realloc(c->cmd_args[i], sizeof(char *)*(j+1));
			if(c->cmd_args[i]==NULL){
				perror("réallocation raté // parse_args");
				exit(EXIT_FAILURE);
			}

			// affectation
			c->cmd_args[i][j] = NULL;
			// test si l'argument est une redirection (si oui ->  enregistre NULL)
			if( (i== 0 && index(cTmp, '<') != NULL) ){
				redirection = 1;
			}
			else if( (i==(c->nb_membres-1) && index(cTmp, '>') != NULL) ){
				redirection = 1;
			}
			else{
				stringCopy(&(c->cmd_args[i][j]), cTmp);
			}

			//extraction du membre suivant
			cTmp = strtok(NULL, " ");

			j++;
		}

		if(!redirection){
			c->cmd_args[i] = (char **) realloc(c->cmd_args[i], sizeof(char *)*(j+1));
			if(c->cmd_args[i]==NULL){
				perror("réallocation raté // parse_args");
				exit(EXIT_FAILURE);
			}
			c->cmd_args[i][j] = NULL;

			c->nb_args_membres[i] = j;
		}else{
			c->nb_args_membres[i] = j-1;
		}
		free(cTok);
	}
}

void aff_args(cmd *c){
	unsigned int nbMembres=0, nbArgs=0;
	while(nbMembres< c->nb_membres){
		nbArgs=0;
		printf("nombre d'arguments pour le membre %d: %d\n",nbMembres,  c->nb_args_membres[nbMembres]);
		while(nbArgs < c->nb_args_membres[nbMembres]+1){
			printf("\targument n°%d du menbre n°%d: %s\n", nbArgs, nbMembres, c->cmd_args[nbMembres][nbArgs]);
			nbArgs++;
		}
		nbMembres++;
	}
	printf("\n");
}

void free_args(cmd *c){
	unsigned int i, j;
	if(c->cmd_args!=NULL){
		for(i=0; i<c->nb_membres; i++){
			if(c->cmd_args[i]!=NULL){
				for(j=0; j< c->nb_args_membres[i]; j++){
					if(c->cmd_args[i][j]!=NULL){
						free(c->cmd_args[i][j]);
					}
				}
				free(c->cmd_args[i]);
			}
		}
		free(c->cmd_args);
		free(c->nb_args_membres);
	}
}


void parse_redirect(cmd *c){
	unsigned int i = 0, redirection = 0;
	char * cTmp=NULL, * ptrRedir=NULL, * cTok=NULL;

	if(c->redirect==NULL){
		c->redirect = (char ***) malloc(sizeof(char **) * c->nb_membres);
		if(c->redirect==NULL){
			perror("allocation raté // parse_redirect");
			exit(EXIT_FAILURE);
		}
	}

	for(i=0; i < c->nb_membres; i++){

		c->redirect[i] = (char **) malloc(sizeof(char *) * 3);
		if(c->redirect[i]==NULL){
			perror("allocation raté // parse_redirect");
			exit(EXIT_FAILURE);
		}
		c->redirect[i][STDIN ]=NULL;
		c->redirect[i][STDOUT]=NULL;
		c->redirect[i][STDERR]=NULL;

		stringCopy(&cTok, c->cmd_membres[i]);
		cTmp = strtok(cTok, " ");

		redirection=0;

		while( cTmp != NULL && !redirection){
			if( i == 0 && index(cTmp, '<') != NULL ){ // test si redirection de l'entrée standard
				cTmp = strtok(NULL, " ");
				stringCopy( &(c->redirect[i][STDIN]), cTmp);
				redirection = 1;
			}
			else if( i==c->nb_membres-1 &&  index(cTmp, '>') != NULL ){ // test si redirection de la sortie standard
				cTmp = strtok(NULL, " ");
				stringCopy( &(c->redirect[i][STDOUT]), cTmp);
				redirection = 1;
			}
			else if( i==c->nb_membres-1 && (ptrRedir = index(cTmp, '2')) != NULL && ptrRedir[1] == '>'){ // test si redirection de la sortie d'erreur
				cTmp = strtok(NULL, " ");
				stringCopy( &(c->redirect[i][STDERR]), cTmp);
				redirection = 1;
			}

			cTmp = strtok(NULL, " ");
		}
		free(cTok);
	}
}

void aff_redirect(cmd *c){
	unsigned int numRedirection=0;
	printf("redirection : \n");

	while(numRedirection< c->nb_membres){
		printf("\tmembre : %d\n", numRedirection);

		printf("\t\t entrée standard : %s\n", c->redirect[numRedirection][STDIN ]);
		printf("\t\t sortie standard : %s\n", c->redirect[numRedirection][STDOUT]);
		printf("\t\t sortie erreur   : %s\n", c->redirect[numRedirection][STDERR]);

		numRedirection++;
	}
	printf("\n");
}

void free_redirect(cmd * c){
	unsigned int i, j;
	if(c->redirect!=NULL){
		for(i=0; i< c->nb_membres; i++){
			if(c->redirect[i]!=NULL){
				for(j=0; j<3; j++){
					if(c->redirect[i][j]!=NULL){
						free(c->redirect[i][j]);
					}
				}
				free(c->redirect[i]);
			}
		}
		free(c->redirect);
	}
}
