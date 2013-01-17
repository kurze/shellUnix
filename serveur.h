#ifndef SERVEUR_H
#define SERVEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <signal.h>
/*
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
*/
void serveur(int port);

#endif