#ifndef SHELL_FCT_H
#define SHELL_FCT_H

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include "cmd.h"
#include "client.h"

int exec_cmd(cmd* ma_cmd);

#endif
