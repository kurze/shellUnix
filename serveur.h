#ifndef SERVEUR_H
#define SERVEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// #include <linux/in.h>
#include <signal.h>
#include <pthread.h>

#include "client.h"

/*

#include <errno.h>
#include <string.h>
#include <unistd.h>
*/
void serveur(char * port);
void executionCommande(int fdsocket);

#endif