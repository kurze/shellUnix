#include <string.h>
#include "cmd.h"

void parse_membres(char *chaine,cmd *ma_cmd){
	unsigned int i=0;
	char * cTmp;
	
	ma_cmd->cmd_initial = chaine;
	
	ma_cmd->cmd_membres = (char **) malloc(sizeof(char *)*1);
	if(ma_cmd==NULL){
		perror("allocation raté // parse_membres");
		exit(1);
	}
	
	cTmp = strtok(ma_cmd->cmd_initial, "|");
	while( cTmp != NULL){
		// réalocation du tableau
		ma_cmd->cmd_membres = (char **) realloc(ma_cmd->cmd_membres, sizeof(char *)*(i+1));
		if(ma_cmd==NULL){
			perror("réallocation raté // parse_membres");
			exit(1);
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

void free_membres(cmd *ma_cmd){
	int i=0;
	if(ma_cmd->cmd_membres==NULL){
		return;
	}else{
		while( i < ma_cmd->nb_membres){
			free(ma_cmd->cmd_membres[i]);
			i++;
		}
		free(ma_cmd->cmd_membres);
// 		ma_cmd->cmd_membres = NULL;
	}
}