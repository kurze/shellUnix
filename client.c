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

		executerCommande(buf, mycmd);
		free(buf);
		free(readlineptr);
	}
	//..........
}

void executerCommande(char * commande, cmd * mycmd){
	parse_membres(commande, mycmd);
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
	if (strcmp(commande,""))
		add_history(commande);

	//..........
	free_distant(mycmd);
	free_redirect(mycmd);
	free_args(mycmd);
	free_membres(mycmd);
	free(mycmd);
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

	// creation de la socket
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("erreur de création de la socket client");
		exit(EXIT_FAILURE);
	}


	// demande de connexion au serveur
	if(connect(sock, (struct sockaddr *)(&adrServ), sizeof(adrServ)) == -1) {
		perror("erreur de connexion à la socket serveur");
		exit(EXIT_FAILURE);
	}

	return sock;
}

void envoieCommande(int socket, char ** commande){
	char com[256]="";
	unsigned int i= 0;
	unsigned int ptr=0;
	while(commande[i]!=NULL && ptr<256){
// 		printf("\n----> %s <----", commande[i]);
		strcpy(&(com[ptr]), commande[i]);
		ptr = ptr + strlen(commande[i]);
		strcpy(&(com[ptr]), " ");
		ptr = ptr + 1;
		i++;
	}
// 	printf("\n\n------> %s <------\n\n", com);
	if(send(socket, com, 256, 0) == -1){
		perror("erreur d'envoi de la commande");
		exit(EXIT_FAILURE);
	}
}