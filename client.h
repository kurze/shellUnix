#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "shell_fct.h"
#include "constante.h"



void client(void);
int connexionServeur(char * adresseIP, char * port);
void executerCommande(char * commande, cmd * mycmd);
void envoieCommande(int socket, char ** commande);

#endif