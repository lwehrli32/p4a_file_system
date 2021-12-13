#include <stdio.h>
#include "udp.h"

int mfs_read(int inum, char *buffer, int block){
    printf("server:: mfs_read\n");
	return 1;
}

int mfs_create(int pinum, int type, char *name){
	printf("server:: mfs_create\n");
	return 1;
}

int main(int argc, char *argv[]) {

	if (argc < 3){
		printf("Incorrect number of arguments.\n");
		for (int i = 0; i < argc; i++){
			printf("argv[%i]: %s\n", i, argv[i]);
		}		
		exit(0);
	}
    
	int sd = UDP_Open(atoi(argv[1]));
    assert(sd > -1);
    
	FILE *fs;
    fs = fopen(argv[2], "w+");

	printf("Server:: running\n");
	
	while (1) {
		struct sockaddr_in addr;
		struct message *msg = malloc(sizeof(struct message));
		if (msg == NULL){
			printf("cannot allocate\n");
			exit(1);	
		}
		
		printf("server:: waiting...\n");
		
		int rc = UDP_Read(sd, &addr, msg, sizeof(struct message));
		printf("server:: read message [size:%d] msg->inum: %i\n", rc, msg->inum);

		int call = msg->call;

		if (call == 2){
			mfs_read(msg->inum, msg->buffer, msg->block);
		}else if(call == 3){
			mfs_create(msg->pinum, msg->file_or_dir, msg->name);
		}

		if (rc > 0) {
            rc = UDP_Write(sd, &addr, msg, sizeof(struct message));
			printf("server:: reply\n");
		}

		free(msg);
    }

	fclose(fs);
	
    return 0; 
}
