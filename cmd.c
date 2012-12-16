#include <string.h>
#include "cmd.h"

void parse_membres(char *chaine,cmd *ma_cmd){
	unsigned int i=0;
	char * cTmp;

	ma_cmd->cmd_initial = chaine;

	ma_cmd->cmd_membres = (char **) malloc(sizeof(char *)*1);
	if(ma_cmd->cmd_membres==NULL){
		perror("allocation raté // parse_membres");
		exit(EXIT_FAILURE);
	}

	cTmp = strtok(ma_cmd->cmd_initial, "|");
	while( cTmp != NULL){
		// réalocation du tableau
		ma_cmd->cmd_membres = (char **) realloc(ma_cmd->cmd_membres, sizeof(char *)*(i+1));
		if(ma_cmd->cmd_membres==NULL){
			perror("réallocation raté // parse_membres");
			exit(EXIT_FAILURE);
		}

		// affectation
		ma_cmd->cmd_membres[i] = cTmp;

		//extraction du membres suivants
		cTmp = strtok(NULL, "|");

		i++;

	}
	ma_cmd->nb_membres = i;
// 	printf(" ma_cmd->nb_membres = %d", ma_cmd->nb_membres);
}

void aff_membres(cmd *ma_cmd){
	unsigned int i=0;
	printf("nombre de membres : %d\n", ma_cmd->nb_membres);
	while(i < (ma_cmd->nb_membres)){
		printf("membres n°%d : %s\n", i, ma_cmd->cmd_membres[i]);
		i++;
	}
}

void free_membres(cmd *ma_cmd){
	if(ma_cmd->cmd_membres!=NULL){
		/**
		 * ne supprimer que le tableau et pas chaque case, car crée erreur sinon (et passe pas valgrind)
		 */
		free(ma_cmd->cmd_membres);
		ma_cmd->nb_membres = 0;
	}
}