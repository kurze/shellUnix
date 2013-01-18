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
		parse_distant(mycmd);

#ifdef DEBUG_FLAG
		aff_membres(mycmd);
		aff_args(mycmd);
		aff_redirect(mycmd);
		aff_distant(mycmd);
#endif

		exec_cmd(mycmd);
		//on regarde si la chaine est vide ou non, et s'il est nécessaire de l'ajouter à l'historique
		if (strcmp(buf,""))
			add_history(buf);

		//..........
		free(buf);
		free(readlineptr);
		free_distant(mycmd);
		free_redirect(mycmd);
		free_args(mycmd);
		free_membres(mycmd);
		free(mycmd);
		//..........

	}
	//..........
}

int connexionServeur(char * adresseIP, char * port){
	struct sockaddr_in adrServ;
// 	struct pollfd pol[2];
	int sock=0;


	inet_aton(adresseIP, &adrServ.sin_addr);
	adrServ.sin_family = AF_INET;
	adrServ.sin_port = htons(atoi(port));
	bzero(&(adrServ.sin_zero), 8);

	// param de l'attente de réponse
// 	pol[0].fd = 0;
// 	pol[0].events = POLLIN;
// 	pol[0].revents = 0;
// 	pol[1].fd = sock;
// 	pol[1].events = POLLIN;
// 	pol[1].revents = 0;
//
	// creation de la socket
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("erreur de création de la socket client");
		return -1;
	}


	// demande de connexion au serveur
	if(connect(sock, (struct sockaddr *)(&adrServ), sizeof(adrServ)) == -1) {
		perror("erreur de connexion à la socket serveur");
		return -1;
	}

	return sock;
}

void envoieCommande(int socket, char ** commande){
	char com[256];
	unsigned int i= 0;
	unsigned int ptr=0;
	while(commande[i]!=NULL && ptr<256){
		printf("\n\n----> %s <----\n\n", commande[i]);
		strcpy(&(com[ptr]), commande[i]);
		ptr = ptr + sizeof(commande[i]);
		i++;
	}
	printf("\n\n----> %s <----\n\n", com);
	if(send(socket, com, 256, 0) == -1){
		perror("erreur d'envoi de la commande");
	}
}