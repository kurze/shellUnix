#include "shell_fct.h"
#include <errno.h>
#include <fcntl.h>

pid_t pid;

//fonction chien de garde
void alarmHandler()
{	
		printf("Processus fils trop long : Arrêt forcé du processus fils\n");
		kill(pid, SIGKILL);
}

int exec_cmd(cmd * c)
{
	int ** tube;
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
		if (i<c->nb_membres && c->nb_membres>1)
		{
			pipe(tube[i-1]);
		}
		//if(c->redirect[i-1][STDOUT]!=NULL)
		pid = fork();
		//code du processus fils		
		if(pid == 0)
		{	
			if(c->redirect[i-1][STDIN]!=NULL)
			{
				int fd;				
				fd=open(c->redirect[i-1][STDIN], O_RDONLY);
				if(fd==-1)
					printf("Erreur \n");
				if(dup2(fd, 0) == -1)
				{ 
					perror("dup2"); 
					exit(errno); 
				}
				close(fd);
			}
			//ne passe pas la première fois, code non réalisé + p-e refaire code sans pipe quand il n'y en a pas
			
			// A FAIRE + voir avec stderr, s'il y a quelque chose à faire
			//if(c->redirect[i-1][STDOUT!=NULL)
			//fd=open(c->redirect[i-1][STDIN], O_RD|O_CREAT); // revoir td3-1 si soucis
			if (c->nb_membres>1)
				{		
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


