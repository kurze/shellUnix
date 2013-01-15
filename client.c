#include "client.h"


void client(void){
//..........
	int ret = MYSHELL_CMD_OK;
	cmd * mycmd;
	char* readlineptr;
	struct passwd* infos;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];
	char *buf;

	while(ret != MYSHELL_FCT_EXIT)
	{
		mycmd = (cmd *) malloc(sizeof(cmd) * 1);
		initCMD(mycmd);

		//rl_bind_key('\t',rl_abort);//disable auto-complete

		infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);
		//met dans la variable str, les infos puis la suite à myshell
		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);

		readlineptr = readline(str);
		//on garde en mémoire la commande
		buf=(char *)malloc (sizeof(char)*(strlen(readlineptr)+1));
		strcpy(buf,readlineptr);

		parse_membres(readlineptr, mycmd);
		parse_args(mycmd);
		parse_redirect(mycmd);

#ifdef DEBUG_FLAG
		aff_membres(mycmd);
		aff_args(mycmd);
		aff_redirect(mycmd);
#endif

		exec_cmd(mycmd);
		//on regarde si la chaine est vide ou non, et s'il est nécessaire de l'ajouter à l'historique
		if (strcmp(buf,""))
			add_history(buf);

		//..........
		free(buf);
		free(readlineptr);
		free_redirect(mycmd);
		free_args(mycmd);
		free_membres(mycmd);
		free(mycmd);
		//..........

	}
	//..........
}