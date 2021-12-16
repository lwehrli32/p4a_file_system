#include <stdio.h>
#include "udp.h"
#include "mfs.h"

typedef struct block{
	char *data;
}block;


typedef struct inode{
	int size; // number of last byte in file
	int type; // file or dir
	struct block *data[14]; // point to data
}inode;


int *imap;
int imap_size;
struct inode *inodes;
int check_point;

int add_inode_imap(){
	imap_size++;
	
	// add new inode to imap
	int *temp = realloc(imap, imap_size * sizeof(int));
	
	if(temp == NULL){
		return -1;
	}
	
	imap = temp;
	*(imap + imap_size) = 0;

	// add inode to inodes array
	struct inode *tempI = realloc(inodes, imap_size * sizeof(struct inode));
	if (tempI == NULL){
		return -1;
	}

	inodes = tempI;
	return 0;
}

int init_fs(FILE *fs){
    printf("server:: reading file system to memory...\n");

    imap_size = 1;
    imap = malloc(imap_size * sizeof(int));
	if (imap == NULL){
		return -1;
	}
	
	inodes = malloc(imap_size * sizeof(struct inode));
	if (inodes == NULL){
		return -1;
	}
	
	// TODO read in existing fs first
	int readFS = 0;
	
	// set checkpoint region. set it to the last inode
	if (readFS){
		// TODO set checkpoint of actual fs
		check_point = 0;
	}else{
		check_point = 0;
	}

    return 0;
}

/*
*  takes the parent inode number (which should be the inode number of a directory) 
*  and looks up the entry name in it. The inode number of name is returned. 
*	Success: return inode number of name; failure: return -1. 
*	Failure modes: invalid pinum, name does not exist in pinum.
*
*/
int s_mfs_lookup(int pinum, char *name){
	printf("server:: mfs_lookup\n");
	int inode = *(pinum + imap);
	struct inode node = *(inode + inodes);
	if (node.type == MFS_DIRECTORY) {
		
	} else {
		return -1;
	}
 	return 0;
}

int s_mfs_stat(int inum, int type, int size){
	//TODO
	printf("server:: mfs_stat\n");
	//int inode = *(imap + inum);
	//struct inode node = *(inodes + inode);
	//if (node.type == MFS_DIRECTORY) {
		
	//} else if (node.type == MFS_REGULAR_FILE) {
		// wait for piazza
	//}
	return 0;
}

int s_mfs_write(int inum, char *buffer, int block){
	//TODO
	printf("server:: mfs_write\n");
	
	// look at imap using checkpoint
	int inode = *(imap + check_point);
	
	// look at inode
	struct inode node = *(inodes + inode);
	
	// look at the data
	if (node.type == MFS_DIRECTORY){
		return -1;
	}

	if (block > 16 || block < 2)
		return -1;

	node.block[block - 2] = buffer;

	return 0;
}

int s_mfs_unlink(int pinum, char *name){
	//TODO
	printf("server:: mfs_unlink\n");
	return 0;
}

int s_mfs_read(int inum, char *buffer, int block){
	//TODO
    printf("server:: mfs_read\n");

	// get inode index from imap using checkpoint region
	int inode = *(imap + inum);
	
	// get inode
	struct inode node = *(inodes + inode);

	if (node.type == 0){
		// directory

		MFS_DirEnt_t *dir = malloc(sizeof(MFS_DirEnt_t));
		if (dir == NULL){
			return -1;
		}

		// TODO get dir name
		strcpy(dir->name, "dir_name");
		dir->inum = inum;

		// get data
		strcpy(buffer, (char *)dir);

		free(dir);
	}else{
		// file
		
		// get data
		strcpy(buffer, node.block[block]);
	} 
	
	return 0;
}

int s_mfs_create(int pinum, int type, char *name){
	//TODO
	printf("server:: mfs_create\n");
	
	if(add_inode_imap() < 0){
		return -1;
	}

	// create new inode number
	struct inode *new_inode = malloc(sizeof(struct inode));
	if (new_inode == NULL){
		return -1;
	}

	new_inode->type = type;

	// get inode index from imap using checkpoint region
	int pinode = *(imap + pinum);
	
	

	// update checkpoint region
	
	return 0;
}

int s_mfs_shutdown(FILE *fs){
	//TODO: force everything to the fs and exit
	printf("server:: mfs_shutdown\n");
	free(inodes);
	free(imap);
	return 0;
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

	int init_rc = init_fs(fs);

	if (init_rc < 0){
		printf("Could not get server running\n");
		exit(0);
	}

	printf("server:: up and running\n");
	
	while (1) {
		struct sockaddr_in addr;
		struct message *msg = malloc(sizeof(struct message));
		if (msg == NULL){
			printf("cannot allocate\n");
			exit(1);	
		}
		
		printf("server:: waiting...\n");
		
		int rc = UDP_Read(sd, &addr, (char *)&msg, sizeof(struct message));
		printf("server:: got read\n");
	
		struct message *msgs = (struct message *) &msg;
		
		printf("server:: after cast\n");	
		printf("server:: read message [size:%d] call: %i\n", rc, msgs->call);
		printf("server:: after print\n");	
		
		// error here - is not an int
		int call = msgs->call;
		
		printf("server:: before work\n");

		if(call == 0){
			s_mfs_lookup(msg->pinum, msg->name);
		}else if (call == 1){
			s_mfs_stat(msg->inum, msg->type, msg->size);
		}else if (call == 2){
			s_mfs_write(msg->inum, msg->buffer, msg->block);
		}else if(call == 3){
			s_mfs_read(msg->inum, msg->buffer, msg->block);		
		}else if (call == 4){
			s_mfs_create(msg->pinum, msg->type, msg->name);
		}else if (call == 5){
			s_mfs_unlink(msg->pinum, msg->name);		
		}else if (call == 6){
			s_mfs_shutdown(fs);
		}
		
		printf("server:: after work\n");

		if (rc > 0) {
			printf("server:: before call\n");
            rc = UDP_Write(sd, &addr, (char *)&msg, sizeof(struct message));
			printf("server:: reply\n");
		}

		free(msg);
    }

	fclose(fs);
	
    return 0; 
}
