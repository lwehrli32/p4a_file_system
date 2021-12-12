#include <stdio.h>
#include "udp.h"

#define BUFFER_SIZE (1000)

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
		struct message *msg = NULL;
		
		printf("server:: waiting...\n");
		
		int rc = UDP_Read(sd, &addr, msg, BUFFER_SIZE);
		printf("server:: read message [size:%d contents:(%i)]\n", rc, msg->inum);

		msg->inum = 69;

		if (rc > 0) {
            rc = UDP_Write(sd, &addr, msg, BUFFER_SIZE);
			printf("server:: reply\n");
		}
    }

	fclose(fs);
	
    return 0; 
}
