#ifndef SHELL_FCT_H
#define SHELL_FCT_H

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "cmd.h"

int exec_cmd(cmd* ma_cmd);

#endif