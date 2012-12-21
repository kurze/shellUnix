#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

#include "shell_fct.h"
#include "constante.h"


int main(void)
{
	//..........
	int ret = MYSHELL_CMD_OK;
	cmd * mycmd;
	char* readlineptr;
	struct passwd* infos;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];

	while(ret != MYSHELL_FCT_EXIT)
	{
		mycmd = (cmd *) malloc(sizeof(cmd) * 1);
		initCMD(mycmd);

		infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);

		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);
		readlineptr = readline(str);

		//..........
		parse_membres(readlineptr, mycmd);
		parse_args(mycmd);
		parse_redirect(mycmd);

#ifdef DEBUG_FLAG
		aff_membres(mycmd);
		aff_args(mycmd);
		aff_redirect(mycmd);
#endif
		//..........exec_cmd(&mycmd);
		//..........

		free(readlineptr);
		free_redirect(mycmd);
		free_args(mycmd);
		free_membres(mycmd);
		free(mycmd);
		//..........

	}
	//..........
	return 0;
}
