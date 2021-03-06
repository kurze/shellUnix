#ifndef CMD_H
#define CMD_H

#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include "constante.h"


typedef struct commande {
	char* cmd_initial;				/* La chaine initiale tapee par l'utilisateur */
	char **cmd_membres;				/* a chaque case i chaine du membre i */
	unsigned int nb_membres;		/* Nombre de membres */
	char ***cmd_args;				/* cmd_args[i][j] contient le jieme mot du ieme membre */
	unsigned int *nb_args_membres;	/* Nombre d'arguments par membres */
	char ***redirect;				/* Pour stocker le chemin vers le fichier de redirection */
	int **type_redirect;			/* Pour stocker le type de rediction */
	char ***distant;					/* Pour stocker les connexion distante */
} cmd;

void initCMD(cmd * c);

void aff_args(cmd *c);
void aff_membres(cmd *c);
void aff_redirect(cmd *c);
void aff_distant(cmd *c);

void free_args(cmd *c);
void free_membres(cmd *c);
void free_redirect(cmd *c);
void free_distant(cmd *c);

void parse_args(cmd *c);
void parse_membres(char *chaine,cmd *c);
void parse_redirect(cmd *c);
void parse_distant(cmd *c);

#endif
