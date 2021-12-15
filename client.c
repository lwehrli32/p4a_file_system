#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "mfs.h"

#define BUFFER_SIZE (1000)

// client code
int main(int argc, char *argv[]) {
	int rc = MFS_Init("localhost", 30000);
	
	if(rc < 0){
		exit(1);
	}
	
    char *message = strdup("Hello");
    //sprintf(message, "hello world");
	
	printf("Before mfs_read call. inum: %i\n", 1);
	
	rc = MFS_Read(1, message, 0);
	
	printf("client:: waiting...\n");
	sleep(4);


	printf("client:: before shutdown\n");	
	rc = MFS_Shutdown();

    //printf("client:: send message [%i]\n", );
    //rc = UDP_Write(sd, &addrSnd, message, BUFFER_SIZE);
    //if (rc < 0) {
		//printf("client:: failed to send\n");
		//exit(1);
    //}

    //printf("client:: wait for reply...\n");
    //rc = UDP_Read(sd, &addrRcv, message, BUFFER_SIZE);
    //printf("client:: got reply [size:%d contents:(%s)\n", rc, message);
    return 0;
}

