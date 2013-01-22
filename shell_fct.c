#include "shell_fct.h"

pid_t pid;

//fonction chien de garde
void alarmHandler()
{
		printf("Processus fils trop long : Arrêt forcé du processus fils\n");
		kill(pid, SIGKILL);
}

void redirection(char valeur[7], char nomFichier[1024], int type)
{
	int fd;
	// redirection <
	if (strcmp(valeur, "STDIN")==0)
	{
		fd=open(nomFichier, O_RDONLY);
		if(fd==-1)
			printf("Erreur \n");
		if(dup2(fd, 0) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(fd);
	}
	//meme cas pour stdout et stderr, sauf redirection
	else if (strcmp(valeur,"STDOUT")==0 || strcmp(valeur,"STDERR")==0)
	{
		//redirection > ou 2>
		if(type==0)
		{
			fd=open(nomFichier,O_RDWR|O_TRUNC|O_CREAT, S_IRWXU);
			if(fd==-1)
				printf("Erreur \n");
		}
		//redirection >> ou 2>>
		else if (type==1)
		{
			fd=open(nomFichier,O_RDWR| O_APPEND);
			if(fd==-1)
				printf("Erreur \n");
		}
		if (strcmp(valeur,"STDOUT")==0)
		{
			if(dup2(fd, 1) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if(dup2(fd, 2) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
		}
		close(fd);
	}
}

int exec_cmd(cmd * c)
{
	int ** tube=NULL;
	unsigned int i=0;
	int retour;
	int socket=-1;
	short continuation; // boleen
	char buffer[1]="";

	if (c->nb_membres==0)
		return -1;

	if (c->nb_membres>1)
	{
		tube=(int **)malloc(sizeof(int*)*c->nb_membres-1);
		if(tube==NULL)
		{
			perror("allocation raté // tube");
			exit(EXIT_FAILURE);
		}
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
			//allocation des tubes
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
			//redirections
			if(c->redirect[i-1][STDIN]!=NULL)
				redirection("STDIN", c->redirect[i-1][STDIN], 0);
			if(c->redirect[i-1][STDOUT]!=NULL)
			{
				if (c->type_redirect[i-1][STDOUT]==NRAPPEND)
					redirection("STDOUT", c->redirect[i-1][STDOUT], 0);
				else if(c->type_redirect[i-1][STDOUT]==RAPPEND)
					redirection("STDOUT", c->redirect[i-1][STDOUT], 1);
			}
			if (c->redirect[i-1][STDERR]!=NULL)
			{
				if (c->type_redirect[i-1][STDERR]==NRAPPEND)
					redirection("STDERR",c->redirect[i-1][STDERR], 0);
				else if(c->type_redirect[i-1][STDERR]==RAPPEND)
					redirection("STDERR", c->redirect[i-1][STDERR], 1);
			}

			//tubes
			if (c->nb_membres>1)
			{
				//ne passe pas la première fois car on ne fait que de l'écriture
				if (i>1)
				{
					close(tube[i-2][1]);
					dup2(tube[i-2][0], 0);
					close(tube[i-2][0]);
				}
				//ne passe pas la dernière fois, on n'écrie plus
				if (i<c->nb_membres)
				{
					close(tube[i-1][0]);
					dup2(tube[i-1][1], 1);
					close(tube[i-1][1]);
				}
			}

			if(c->cmd_args[i-1][0]!=NULL)
			{ // execution locale
				if((execvp(c->cmd_args[i-1][0], c->cmd_args[i-1]))==-1)
				{
					perror("Commande inconnue \n");
					exit(EXIT_FAILURE);
				}
			}
			else
			{ // cas de connexion distante
				socket = connexionServeur(c->distant[i-1][0], c->distant[i-1][1]);
				envoieCommande(socket, &(c->cmd_args[i-1][1]));


				continuation=1;
				while(i>1 && continuation){
					retour = read(0, (void *)buffer, 1);
					if(retour != -1 && retour != 0){
						write(socket, (void *)buffer, 1);
					}
					else if(retour != -1){
						continuation = 0;
					}else{
						perror("erreur de lecture buffer");
						exit(EXIT_FAILURE);
					}
				}
				close(0);
				retour = fcntl(socket, F_SETFL, O_RDONLY);
				if(retour == -1){
					perror("erreur changement sur la socket");
					exit(EXIT_FAILURE);
				}

				continuation=1;
				while(continuation){
					retour = read(socket, (void *)buffer, 1);
					if(retour != -1 && retour != 0){
						write(1, (void *)buffer, 1);
					}
					else if(retour != -1){
						continuation = 0;
					}else{
						perror("erreur de lecture socket");
						exit(EXIT_FAILURE);
					}
				}
				close(1);
				close(socket);
			}
			return 0;
		}

		if(i>1)
		{
			close(tube[i-2][0]);
			close(tube[i-2][1]);
		}

	}
	signal(SIGALRM, alarmHandler);

	//déclenchement de l'alarme
	alarm(10);

	//attente de la fin des processus fils
	for (i=1;i<=c->nb_membres;i++)
		wait(NULL);

	//fin des processus fils, on annule l'alarme
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


