#include <string.h>
#include "cmd.h"

void parse_membres(char *chaine,cmd *c){
	unsigned int i=0;
	char * cTmp;

	c->cmd_initial = chaine;

	c->cmd_membres = (char **) malloc(sizeof(char *)*1);
	if(c->cmd_membres==NULL){
		perror("allocation raté // parse_membres");
		exit(EXIT_FAILURE);
	}

	cTmp = strtok(c->cmd_initial, "|");
	while( cTmp != NULL){
		// réalocation du tableau
		c->cmd_membres = (char **) realloc(c->cmd_membres, sizeof(char *)*(i+1));
		if(c->cmd_membres==NULL){
			perror("réallocation raté // parse_membres");
			exit(EXIT_FAILURE);
		}

		// affectation
		c->cmd_membres[i] = cTmp;

		//extraction du membres suivants
		cTmp = strtok(NULL, "|");

		i++;

	}
	c->nb_membres = i;
// 	printf(" c->nb_membres = %d", c->nb_membres);
}

void aff_membres(cmd *c){
	unsigned int i=0;
	printf("nombre de membres : %d\n", c->nb_membres);
	while(i < (c->nb_membres)){
		printf("membres n°%d : %s\n", i, c->cmd_membres[i]);
		i++;
	}
	printf("\n");
}

void free_membres(cmd *c){
	if(c->cmd_membres!=NULL){
		//ne supprimer que le tableau et pas chaque case, car crée erreur sinon (et passe pas valgrind)
		free(c->cmd_membres);
		c->nb_membres = 0;
	}
}

void parse_args(cmd *c){
	unsigned int i, j, redirection;
	char * cTmp;

	// allocation de la première dimension du tableau
	c->cmd_args = (char ***) malloc(sizeof(char **) * c->nb_membres);
	if(c->cmd_args==NULL){
		perror("allocation raté // parse_args");
		exit(EXIT_FAILURE);
	}

	c->nb_args_membres = (unsigned int *) malloc(sizeof(unsigned int)*c->nb_membres);
	if(c->nb_args_membres==NULL){
		perror("allocation raté // parse_args");
		exit(EXIT_FAILURE);
	}

	for(i=0; i < c->nb_membres; i++){
		j=0;

		c->cmd_args[i] = (char **) malloc(sizeof(char *)*1);
		if(c->cmd_args[i]==NULL){
			perror("allocation raté // parse_args");
			exit(EXIT_FAILURE);
		}


		cTmp = strtok(c->cmd_membres[i], " ");

		redirection = 0;
		while( cTmp != NULL && !redirection){
			// réalocation du tableau
			c->cmd_args[i] = (char **) realloc(c->cmd_args[i], sizeof(char *)*(j+1));
			if(c->cmd_args[i]==NULL){
				perror("réallocation raté // parse_args");
				exit(EXIT_FAILURE);
			}

			// affectation
			// test si l'argument est une redirection (si oui ->  enregistre NULL)
			if(index(cTmp, '>') == NULL && index(cTmp, '<') == NULL){
				c->cmd_args[i][j] = cTmp;
			}else{
				c->cmd_args[i][j] = NULL;
				redirection = 1;
			}

			//extraction du membres suivants
			cTmp = strtok(NULL, " ");

			j++;
		}

		c->nb_args_membres[i] = j;
	}
}

void aff_args(cmd *c){
	unsigned int nbMembres=0, nbArgs=0;
	while(nbMembres< c->nb_membres){
		nbArgs=0;
		printf("nombre d'arguments pour le membres %d: %d\n",nbMembres,  c->nb_args_membres[nbMembres]);
		while(nbArgs < c->nb_args_membres[nbMembres]){
			printf("\targument n°%d du menbres n°%d: %s\n", nbArgs, nbMembres, c->cmd_args[nbMembres][nbArgs]);
			nbArgs++;
		}
		nbMembres++;
	}
	printf("\n");
}

void free_args(cmd *c);