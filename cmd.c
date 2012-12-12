#include <string.h>
#include "cmd.h"

void parse_membres(char *chaine,cmd *ma_cmd){
	int i=0;
	
	ma_cmd->cmd_initial = chaine;
	
	do{
		ma_cmd->cmd_membres[i] = strtok(ma_cmd->cmd_initial, "|");
		i++;
	}while(ma_cmd->cmd_membres[i-1]!=NULL);
	
	ma_cmd->nb_membres = i-1;
	
}