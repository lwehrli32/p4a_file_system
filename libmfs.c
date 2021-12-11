#include "mfs.h"
#include "udp.h"

#define BUFFER_SIZE (1000)

int sd = -1;

int MFS_Init(char *hostname, int port){
	struct sockaddr_in addrSnd;

	sd = UDP_Open(20000);
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
	// TODO: read the block specified by block into buffer of file/directory inum
	
	struct sockaddr_in addrSnd, addrRcv;
	struct message msg = malloc(sizeof(struct message));
	if (msg == NULL){
		return -1;
	}
	
	rc = UDP_Write(sd, &addrSnd, msg, BUFFER_SIZE);
	if (rc < 0) {
		printf("client:: failed to send\n");
		exit(1);
	}

	printf("client:: wait for reply...\n");
	
	int rc = UDP_Read(sd, &addrRcv, message, BUFFER_SIZE);
	//printf("client:: got reply [size:%d contents:(%s)\n", rc, message);	

	free(msg);	

	return 0;
}

int MFS_Creat(int pinum, int type, char *name){
	// TODO: makes a file (type == MFS_REGULAR_FILE) or directory (type == MFS_DIRECTORY) in the parent directory specified by pinum of name name. Returns 0 on success
	
	return 0;
}

int MFS_Unlink(int pinum, char *name){
	// TODO: removes the file or directory name from the directory specified by pinum. 0 on success, -1 on failure.

	return 0;
}

int MFS_Shutdown(){
	// TODO: force all data to the disk

	exit(0);	
}
