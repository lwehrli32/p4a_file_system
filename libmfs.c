#include "mfs.h"
#include "udp.h"

#define BUFFER_SIZE (1000)

int MFS_Init(char *hostname, int port){
	struct sockaddr_in addrSnd;	

	int sd = UDP_Open(20000);
	int rc = UDP_FillSockAddr(&addrSnd, hostname, port);	
	
	if (rc < 0){
		return -1;
	}

	return sd;
}

int MFS_Stat(int inum, MFS_Stat_t *m){
	// TODO: return info about the file inum, success return 0, -1 otherwise
	
	return 0;
}

int MFS_Write(int inum, char *buffer, int block){
	// TODO: write block of 4096 bytes offset by block.

	return 0;
}

int MFS_Read(int inum, char *buffer, int block){
	// TODO: read the block specified by block into inum

	return 0;
}

int MFS_Creat(int pinum, int type, char *name){
	// TODO: 
	
	return 0;
}

int MFS_Unlink(int pinum, char *name){
	return 0;
}

int MFS_Shutdown(){
	// TODO: force all data to the disk

	exit(0);	
}
