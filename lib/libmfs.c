#include <stdio.h>
#include "mfs.h"

int MFS_Init(char *hostname, int port){
	printf("init %s on %i\n", hostname, port);
	return 0;
}

int MFS_Stat(int inum, MFS_Stat_t *m){
	return 0;
}

int MFS_Write(int inum, char *buffer, int block){
	return 0;
}

int MFS_Read(int inum, char *buffer, int block){
	return 0;
}

int MFS_Creat(int pinum, int type, char *name){
	return 0;
}

int MFS_Unlink(int pinum, char *name){
	return 0;
}

int MFS_Shutdown(){
	return 0;
}
