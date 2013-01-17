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
	int ** tube;//peut-etre a mettre ailleurs
	unsigned int i=0,j;

	if (c->nb_membres==0)
		return -1;
	//allocation d'un tableau de ptrs --> a revoir, pas necessaire si pas de pipe
	tube=(int **)malloc(sizeof(int*)*c->nb_membres-1);
	if(tube==NULL)
	{
		perror("allocation raté // tube");
		exit(EXIT_FAILURE);
	}
	
	if ((strcmp(c->cmd_initial,"exit"))==0)
		exit(0);
	if (c->nb_membres==0)
		return -1;

	while(i<c->nb_membres)
	{
		i++;
		//quand i sera à 3, on ne fait plus le pipe
		if (i<c->nb_membres && c->nb_membres>1)
		{
			tube[i-1]=(int *)malloc(2*sizeof(int));
			if(tube[i-1]==NULL)
			{
				perror("allocation raté // tube[i]");
				exit(EXIT_FAILURE);
			}
			pipe(tube[i-1]);

		}
		
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
			
			if(c->redirect[i-1][STDOUT]!=NULL)
			{
				int fd2=0;
				//le fichier crée est utilisable en lecture/ecriture/execution
				//Le fichier est effacé en entier a son ouverture --> O_TRUNC
				if(c->type_redirect[i-1][STDOUT]==NRAPPEND)
				{
					fd2=open(c->redirect[i-1][STDOUT],O_RDWR|O_TRUNC|O_CREAT, S_IRWXU);
					if(fd2==-1)
						printf("Erreur \n");
				}
				else if (c->type_redirect[i-1][STDOUT]==RAPPEND)
				{
					fd2=open(c->redirect[i-1][STDOUT],O_RDWR| O_APPEND);
					if(fd2==-1)
						printf("Erreur \n");
				}	
 
				if(dup2(fd2, 1) == -1)
				{ 
					perror("dup2"); 
					exit(errno); 
				}
				close(fd2);
			}
			if (c->redirect[i-1][STDERR]!=NULL)
			{
				int fd3=0;
				if(c->type_redirect[i-1][STDERR]==NRAPPEND)
				{
					fd3=open(c->redirect[i-1][STDERR],O_RDWR|O_TRUNC|O_CREAT, S_IRWXU);
					if(fd3==-1)
						printf("Erreur \n");
				}
				else if (c->type_redirect[i-1][STDERR]==RAPPEND)
				{
					fd3=open(c->redirect[i-1][STDERR],O_RDWR| O_APPEND);
					if(fd3==-1)
						printf("Erreur \n");
				}	
 
				if(dup2(fd3, 2) == -1)
				{ 
					perror("dup2"); 
					exit(errno); 
				}
				close(fd3);
			
			}
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
			if(c->cmd_args[i-1][0]!=NULL){ // execution locale
				if((execvp(c->cmd_args[i-1][0], c->cmd_args[i-1]))==-1)
				{
					perror("Commande inconnue \n");
					exit(errno);
				}
			}else{ // cas de connexion distante
				if((execvp(c->cmd_args[i-1][1], &(c->cmd_args[i-1][1])))==-1)
				{
					perror("Commande inconnue \n");
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
	if(c->nb_membres>1)
	{	
		for(i=0;i<c->nb_membres-1;i++)
		{
			free(tube[i]);
		}
		free(tube);
	}
	return 0;
}


