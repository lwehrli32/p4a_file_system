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

int MFS_Lookup(int pinum, char *name){
	if (sd == -1){
		printf("Must call MFS_Init before calling other functions\n");
		return -1;
	}

	struct message msg;
	msg.call = 0;
	msg.pinum = pinum;
	strcpy(msg.name, name);

	int rc = UDP_Write(sd, &addrSnd, (char *)&msg, sizeof(struct message));
	if (rc < 0) {
		printf("client:: failed to send\n");
		return -1;
	}

	printf("client:: wait for reply...\n");

	rc = UDP_Read(sd, &addrRcv, (char *)&msg, sizeof(struct message));

	printf("client:: got reply [size:%d contents:(%i)\n", rc, msg.inum);
	
	if (rc < 0){
		return -1;
	}

	return 0;
}

int MFS_Stat(int inum, MFS_Stat_t *m){
	// TODO: return info about the file inum, success return 0, -1 otherwise
	
	if (sd == -1){
		printf("Must call MFS_Init before calling other functions\n");
		return -1;
	}

	struct message msg;
	msg.call = 1;
	msg.inum = inum;
	msg.type = m->type;
	msg.size = m->size;

	int rc = UDP_Write(sd, &addrSnd, (char *)&msg, sizeof(struct message));
	if (rc < 0) {
		printf("client:: failed to send\n");
		return -1;
	}

	printf("client:: wait for reply...\n");

	rc = UDP_Read(sd, &addrRcv, (char *)&msg, sizeof(struct message));

	printf("client:: got reply [size:%d contents:(%i)\n", rc, msg.call);

	if (rc < 0){
		return -1;
	}

	return 0;
}

int MFS_Write(int inum, char *buffer, int block){
	// TODO: write block of 4096 bytes offset by block.

	if (sd == -1){
		printf("Must call MFS_Init before calling other functions\n");
		return -1;
	}

	struct message msg;
	msg.call = 2;
	msg.inum = inum;
	strcpy(msg.buffer, buffer);
	msg.block = block;

	int rc = UDP_Write(sd, &addrSnd, (char *)&msg, sizeof(struct message));
	if (rc < 0) {
		printf("client:: failed to send\n");
		return -1;
	}

	printf("client:: wait for reply...\n");

	rc = UDP_Read(sd, &addrRcv, (char *)&msg, sizeof(struct message));

	printf("client:: got reply [size:%d contents:(%i)\n", rc, msg.call);

	if (rc < 0){
		return -1;
	}

	return 0;
}

int MFS_Read(int inum, char *buffer, int block){
	// TODO: read the block specified by block into buffer of file/directory inum
	printf("client:: MFS_READ\n");
	if (sd == -1){
		printf("Must call MFS_Init before calling other functions\n");
		return -1;
	}
	
	struct message msg;	

	msg.call = 3;
	msg.inum = inum;
	strcpy(msg.buffer, buffer);
	msg.block = block;

	int rc = UDP_Write(sd, &addrSnd, (char *)&msg, sizeof(struct message));
	if (rc < 0) {
		printf("client:: failed to send\n");
		return -1;
	}

	printf("client:: wait for reply...\n");
		
	rc = UDP_Read(sd, &addrRcv, (char *)&msg, sizeof(struct message));
	
	printf("client:: got reply [size:%d contents:(%i)\n", rc, msg.call);		

	if (rc < 0)
		return -1;

	return 0;
}

int MFS_Creat(int pinum, int type, char *name){
	// TODO: makes a file (type == MFS_REGULAR_FILE) or directory (type == MFS_DIRECTORY) in the parent directory specified by pinum of name name. Returns 0 on success
	
	if (sd == -1){
		printf("Must call MFS_Init before calling other functions\n");
		return -1;
	}

	struct message msg;
	msg.call = 4;
	msg.pinum = pinum;
	strcpy(msg.name, name);
	msg.type = type;

	int rc = UDP_Write(sd, &addrSnd, (char *)&msg, sizeof(struct message));
	if (rc < 0) {
		printf("client:: failed to send\n");
		return -1;
	}

	printf("client:: wait for reply...\n");

	rc = UDP_Read(sd, &addrRcv, (char *)&msg, sizeof(struct message));

	printf("client:: got reply [size:%d)\n", rc);

	if (rc < 0)
		return -1;
	
	return 0;
}

int MFS_Unlink(int pinum, char *name){
	// TODO: removes the file or directory name from the directory specified by pinum. 0 on success, -1 on failure.
	
	if (sd == -1){
		printf("Must call MFS_Init before calling other functions\n");
		return -1;
	}

	struct message msg;
	msg.call = 5;
	msg.pinum = pinum;
	strcpy(msg.name, name);

	int rc = UDP_Write(sd, &addrSnd, (char *)&msg, sizeof(struct message));
	if (rc < 0) {
		printf("client:: failed to send\n");
		return -1;
	}

	printf("client:: wait for reply...\n");

	rc = UDP_Read(sd, &addrRcv, (char *)&msg, sizeof(struct message));

	printf("client:: got reply [size:%d)\n", rc);

	if (rc < 0)
		return -1;
	return 0;
}

int MFS_Shutdown(){
	// TODO: force all data to the disk
	
	if (sd == -1){
		printf("Must call MFS_Init before calling other functions\n");
		return -1;
	}

	struct message msg;
	msg.call = 6;
	
	int rc = UDP_Write(sd, &addrSnd, (char *)&msg, sizeof(struct message));
	if (rc < 0) {
		printf("client:: failed to send\n");
		return -1;
	}

	printf("client:: wait for reply...\n");

	rc = UDP_Read(sd, &addrRcv, (char *)&msg, sizeof(struct message));

	printf("client:: got reply [size:%d)\n", rc);

	if (rc < 0)
		return -1;

	exit(0);	
}
