#ifndef _SHELLUPDATE_H_
#define _SHELLUPDATE_H_
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
}

typedef enum{
	newShellType = 0,
	backupShellType
}shellType;

void updateShell();
int addShellUpdateNum(shellType type);
#endif

