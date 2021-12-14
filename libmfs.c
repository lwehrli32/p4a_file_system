#include <stdio.h>
#include <stdlib.h>
#include "udp.h"
#include "mfs.h"

int sd = -1;
struct sockaddr_in addrSnd, addrRcv;

int MFS_Init(char *hostname, int port){
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

	if (sd == -1){
    printf("Must call MFS_Init before calling other functions\n");
}

struct message *msg = malloc(sizeof(struct message));
if (msg == NULL){
    return -1;
}

msg->call = 2;
msg->inum = inum;
msg->buffer = buffer;
msg->block = block;
msg->pinum = -1;
msg->file_or_dir = -1;

int rc = UDP_Write(sd, &addrSnd, msg, sizeof(struct message));
if (rc < 0) {
    printf("client:: failed to send\n");
    exit(1);
}

printf("client:: wait for reply...\n");

rc = UDP_Read(sd, &addrRcv, msg, sizeof(struct message));

printf("client:: got reply [size:%d contents:(%i)\n", rc, msg->inum);

	free(msg);

	return 0;
}

int MFS_Read(int inum, char *buffer, int block){
	// TODO: read the block specified by block into buffer of file/directory inum

	if (sd == -1){
		printf("Must call MFS_Init before calling other functions\n");
	}
	
	struct message *msg = malloc(sizeof(struct message));	
	if (msg == NULL){
		return -1;
	}

	msg->call = 2;
	msg->inum = inum;
	msg->buffer = buffer;
	msg->block = block;
	msg->pinum = -1;
	msg->file_or_dir = -1;

	int rc = UDP_Write(sd, &addrSnd, msg, sizeof(struct message));
	if (rc < 0) {
		printf("client:: failed to send\n");
		exit(1);
	}

	printf("client:: wait for reply...\n");
		
	rc = UDP_Read(sd, &addrRcv, msg, sizeof(struct message));
	
	printf("client:: got reply [size:%d contents:(%i)\n", rc, msg->inum);		

	free(msg);

	return 0;
}

int MFS_Creat(int pinum, int type, char *name){
	// TODO: makes a file (type == MFS_REGULAR_FILE) or directory (type == MFS_DIRECTORY) in the parent directory specified by pinum of name name. Returns 0 on success
	
	if (sd == -1){
		printf("Must call MFS_Init before calling other functions\n");
	}

	struct message *msg = malloc(sizeof(struct message));
	if (msg == NULL){
		return -1;
	}	

	msg->call = 3;
	msg->pinum = pinum;
	msg->inum = -1;
	msg->name = name;
	msg->buffer = NULL;
	msg->block = -1;
	msg->file_or_dir = type;

	int rc = UDP_Write(sd, &addrSnd, msg, sizeof(struct message));
	if (rc < 0) {
		printf("client:: failed to send\n");
		exit(1);
	}

	printf("client:: wait for reply...\n");

	rc = UDP_Read(sd, &addrRcv, msg, sizeof(struct message));

	printf("client:: got reply [size:%d)\n", rc);

	free(msg);
	
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
