#include "shell_fct.h"
#include <errno.h>

pid_t pid;

//fonction chien de garde
void alarmHandler()
{	
		printf("Processus fils trop long : Arrêt forcé du processus fils\n");
		kill(pid, SIGKILL);
}

int exec_cmd(cmd * c)
{

//int execvp(const char*file, char*const argv[]);
//utilise un tableau de pointeurs sur des chaines de caracteres terminées par des caracteres nuls, qui constituent les arguments disponibles pour le programme à exécuter. Le tableau doit se terminer par un pointeur NULL.

	int ** tube;
	int valPipe;
	unsigned int i=0,j;
	
	if (c->nb_membres==0)
		return -1;
	//allocation d'un tableau de ptrs
	tube=(int **)malloc(sizeof(int*)*c->nb_membres-1);
	if(tube==NULL)
	{
		perror("allocation raté // tube");
		exit(EXIT_FAILURE);
	}
	//allocation d'un tableau de 2 cases pour chaque ptr
	for(j=0;j<c->nb_membres;j++)
	{
		tube[j]=(int *)malloc(2*sizeof(int));
		if(tube[j]==NULL)
		{
			perror("allocation raté // tube[j]");
			exit(EXIT_FAILURE);
		}
	}
	while(i<c->nb_membres)
	{
		i++;
		//quand i sera à 3, on ne fait plus le pipe
		if (i<c->nb_membres)
		{
			valPipe=pipe(tube[i-1]);
			if(valPipe==-1)
			{
				perror("pipe raté");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		//code du processus fils		
		if(pid == 0)
		{
			//ne passe pas la première fois, code non réalisé
					
			if (i>1)
			{
				close(tube[i-2][1]);
				dup2(tube[i-2][0], 0);
				close(tube[i-2][0]);
			}
			//ne passe pas la dernière fois, code non réalisé
			if (i<c->nb_membres)
			{
				close(tube[i-1][0]);
				dup2(tube[i-1][1], 1);
				close(tube[i-1][1]);
			}
			if((execvp(c->cmd_args[i-1][0], c->cmd_args[i-1]))==-1)
			{
				perror("execvp");
				exit(errno);
			}
			return 0;
		}
				
		if(i>1)
		{
			close(tube[i-2][0]);
			close(tube[i-2][1]);
		}
		
	}
	
		//code du processus pere
		signal(SIGALRM, alarmHandler);
	
		//déclenchement de l'alarme
		alarm(10);

		for (i=1;i<=c->nb_membres;i++)
			wait(NULL);
		//fin de l'execution du processus fils, on annule l'alarme
		alarm(0);
	//desallocation
	for(j=0;j<c->nb_membres;j++)
	{
		free(tube[j]);
	}
	free(tube);
	return 0;
}


