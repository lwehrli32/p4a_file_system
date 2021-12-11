#ifndef __MFS_h__
#define __MFS_h__

#define MFS_DIRECTORY    (0)
#define MFS_REGULAR_FILE (1)

#define MFS_BLOCK_SIZE   (4096)

#include <stdio.h>
#include <stdlib.h>

typedef struct __MFS_Stat_t {
	int type;
	int size;
} MFS_Stat_t;

typedef struct _MFS_DirEnt_t {
	char name[28];
	int inum;
} MFS_DirEnt_t;

typedef struct message{
	int call;
}message;

int MFS_Init(char *hostname, int port);
int MFS_Lookup(int pinum, char *name);
int MFS_Stat(int inum, MFS_Stat_t *m);
int MFS_Write(int inum, char *buffer, int block);
int MFS_Read(int inum, char *buffer, int block);
int MFS_Creat(int pinum, int type, char *name);
int MFS_Unlink(int pinum, char *name);
int MFS_Shutdown();

#endif
