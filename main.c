#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include "shell_fct.h"
#include "constante.h"



int main(int argc, char** argv)
{
	//..........
	int ret = MYSHELL_CMD_OK;
	cmd mycmd;
	char* readlineptr;
	struct passwd* infos;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];
	char *buf;

	while(ret != MYSHELL_FCT_EXIT)
	{
 		
    	//rl_bind_key('\t',rl_abort);//disable auto-complete
 	
		infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);
		//met dans la variable str, les infos puis la suite à myshell	
		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);
    
		while((readlineptr = readline(str))!=NULL)	
    	{
        	//on garde en mémoire la commande
			buf=(char *)malloc (sizeof(char)*(strlen(readlineptr)+1));
			strcpy(buf,readlineptr);
		
			parse_membres(readlineptr, &mycmd);
			#ifdef DEBUG_FLAG
			aff_membres(&mycmd);
			#endif
			parse_args(&mycmd);
			#ifdef DEBUG_FLAG
			aff_args(&mycmd);
			#endif 		
			//..........parse_redirect(&mycmd);		
			exec_cmd(&mycmd);
        	//on regarde si la chaine est vide ou non, et s'il est nécessaire de l'ajouter à l'historique
			if (strcmp(buf,""))
				add_history(buf);
    	}
    
		
		//..........
		free(buf);
		free(readlineptr);
		free_args(&mycmd);
		free_membres(&mycmd);
		//..........

	}
	//..........
	return 0;
}
