#include "cmd.h"
#include <fcntl.h>

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
	c->distant = NULL;
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
		// réallocation du tableau
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

	//allocation du tableau de nombre d'arguments
	c->nb_args_membres = (unsigned int *) malloc(sizeof(unsigned int)*c->nb_membres);
	if(c->nb_args_membres==NULL)
	{
		perror("allocation raté // parse_args");
		exit(EXIT_FAILURE);
	}

	// parcours des membres
	for(i=0; i < c->nb_membres; i++)
	{
		j=0;
		c->cmd_args[i] = (char **) malloc(sizeof(char *)*1);
		if(c->cmd_args[i]==NULL){
			perror("allocation raté // parse_args");
			exit(EXIT_FAILURE);
		}

		// découpage du membres
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
			// test si l'argument est une redirection ou une connexion distante(si oui ->  enregistre NULL)
			if( (i== 0 && index(cTmp, '<') != NULL) ){
				redirection = 1;
			}
			else if( (i==(c->nb_membres-1) && index(cTmp, '>') != NULL) ){
				redirection = 1;
			}
			else if(j==0 && strstr(cTmp, "s:")!=NULL){

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
			printf("\targument n°%d du membre n°%d: %s\n", nbArgs, nbMembres, c->cmd_args[nbMembres][nbArgs]);
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
	unsigned int i = 0;
	char * cTmp=NULL, /* * ptrRedir=NULL,*/ * cTok=NULL;

	//Allocation de c->redirect
	if(c->redirect==NULL){
		c->redirect = (char ***) malloc(sizeof(char **) * c->nb_membres);
		if(c->redirect==NULL){
			perror("allocation raté // redirect");
			exit(EXIT_FAILURE);
		}
	}

	//Allocation des type_redirect
	if(c->type_redirect==NULL){
		c->type_redirect = (int **) malloc(sizeof(int *) * c->nb_membres);
		if(c->type_redirect==NULL){
			perror("allocation raté // type_redirect");
			exit(EXIT_FAILURE);
		}
	}

	for(i=0; i < c->nb_membres; i++){

		//Allocation de redirect[]
		c->redirect[i] = (char **) malloc(sizeof(char *) * 3);
		if(c->redirect[i]==NULL){
			perror("allocation raté // parse_redirect");
			exit(EXIT_FAILURE);
		}

		//Allocation de type_redirect[]
		c->type_redirect[i] = (int *) malloc(sizeof(int) * 3);
		if(c->type_redirect[i]==NULL){
			perror("allocation raté // type_direct");
			exit(EXIT_FAILURE);
		}

		//A NULL PAR DEFAUT
		c->redirect[i][STDIN]=NULL;
		c->redirect[i][STDOUT]=NULL;
		c->redirect[i][STDERR]=NULL;
		//NRAPPEND valeur 0
		c->type_redirect[i][STDIN]=NRAPPEND;
		c->type_redirect[i][STDOUT]=NRAPPEND;
		c->type_redirect[i][STDERR]=NRAPPEND;


		stringCopy(&cTok, c->cmd_membres[i]);
		cTmp = strtok(cTok, " ");


		while( cTmp != NULL){
			//on garde i=0
			if( i == 0 && strcmp(cTmp, "<") ==0 )
			{ // test si redirection de l'entrée standard
				cTmp = strtok(NULL, " ");
				stringCopy( &(c->redirect[i][STDIN]), cTmp);
			}
			//a revoir ce test car p-e que ce n'est pas que lorsque i==c->nb_membres-1
			else if(i==c->nb_membres-1 && strcmp(cTmp, ">") == 0)
			{ // test si redirection de la sortie standard
				//redirection++;				
				cTmp = strtok(NULL, " ");
				if (c->redirect[i][STDOUT]!=NULL)
				{
					int fd;
					fd=open(c->redirect[i][STDOUT],O_RDONLY|O_TRUNC|O_CREAT, S_IRWXU);
					if(fd==-1)
						printf("Erreur \n");
					close(fd);
					
				}
				stringCopy( &(c->redirect[i][STDOUT]), cTmp);
			}
			else if(strcmp(cTmp,">>")==0)
			{
				cTmp = strtok(NULL, " ");
				stringCopy( &(c->redirect[i][STDOUT]), cTmp);
				c->type_redirect[i][STDOUT]=RAPPEND;
			}
			else if(strcmp(cTmp,"2>")==0)
			{
				cTmp = strtok(NULL, " ");
				stringCopy( &(c->redirect[i][STDERR]), cTmp);
			}
			else if(strcmp(cTmp,"2>>")==0)
			{
				cTmp = strtok(NULL, " ");
				stringCopy( &(c->redirect[i][STDERR]), cTmp);
				c->type_redirect[i][STDERR]=RAPPEND;
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

		printf("\t\t entrée standard : redirection %d \n", c->type_redirect[numRedirection][STDIN ]);
		printf("\t\t sortie standard : redirection %d \n", c->type_redirect[numRedirection][STDOUT]);
		printf("\t\t sortie erreur   : redirection %d \n", c->type_redirect[numRedirection][STDERR]);


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
	if(c->type_redirect!=NULL){
		for(i=0; i< c->nb_membres; i++){
			if(c->type_redirect[i]!=NULL){
				free(c->type_redirect[i]);
			}
		}
		free(c->type_redirect);
	}
}


void parse_distant(cmd *c){
	unsigned int i = 0;
	char * str, * cTok, * cTmp;
	if(c->distant==NULL){
		c->distant = (char ***) malloc(sizeof(char **) * c->nb_membres);
		if(c->distant==NULL){
			perror("allocation raté // parse_distant");
			exit(EXIT_FAILURE);
		}

		for(i = 0; i<c->nb_membres; i++){
// 			c->distant[i] = NULL;
			c->distant[i] = (char **) malloc(sizeof(char *) * 2);
			if(c->distant[i]==NULL){
				perror("allocation raté // parse_distant");
				exit(EXIT_FAILURE);
			}

			str = strstr(c->cmd_membres[i], "s:");

			if(str!= NULL){

				stringCopy(&cTok, c->cmd_membres[i]);
				cTmp = strtok(cTok, ":");
				cTmp = strtok(NULL, ":");
				stringCopy(&(c->distant[i][0]), cTmp);
				cTmp = strtok(NULL, " ");
				stringCopy(&(c->distant[i][1]), cTmp);
				free(cTok);
			}else{
				c->distant[i][0] = NULL;
				c->distant[i][1] = NULL;
			}
		}
	}
}

void aff_distant(cmd *c){
	unsigned int i = 0;

	printf("connexion distante : \n");

	for(i=0; i<c->nb_membres; i++){
		printf("\tmembre : %d\n", i);

		printf("\t\t adresse serveur : %s\n", c->distant[i][0]);
		printf("\t\t port serveur    : %s\n", c->distant[i][1]);

	}
	printf("\n");
}

void free_distant(cmd *c){
	unsigned int i, j;
	if(c->distant!=NULL){
		for(i=0; i< c->nb_membres; i++){
			if(c->distant[i]!=NULL){
				for(j=0; j<2; j++){
					if(c->distant[i][j]!=NULL){
						free(c->distant[i][j]);
					}
				}
				free(c->distant[i]);
			}
		}
		free(c->distant);
	}
}
