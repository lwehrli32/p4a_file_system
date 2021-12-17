#include <stdio.h>
#include "udp.h"
#include "mfs.h"

typedef struct block{
	char *name; // not sure, for unlink
	char *data;
}block;

typedef struct inode{
	int size; // number of last byte in file
	int type; // file or dir
	struct block *data[14]; // point to data
}inode;

struct directory {
	char *name; // not sure, for unlink
} directory;


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

int init_fs(char fname[]){
	return -1;
    printf("server:: reading file system to memory...\n");
    
    FILE *fs;
    fs = fopen(fname, "r");

	char * line = NULL;
    size_t len = 0;
    ssize_t read;
	int lineNum = 0;
	int inode_counter = 0;
	int inode_part = 0;
	struct inode new_inode;

	printf("server:: printing found results in fs:\n");

	while ((read = getline(&line, &len, fs)) != -1) {
		if (line){
			printf("line: %s\n", line);
			if (lineNum == 0){
				// populate checkpoint region and imap_size
				int portion = 0;
				char *found;
				while( (found = strsep(&line,",")) != NULL ){
					if(portion == 0){
						check_point = atoi(found);
						printf("checkpoint: %i\n", check_point);
					}else{
						imap_size = atoi(found);
						printf("imap_size: %i\n", imap_size);
						imap = malloc(imap_size * sizeof(int));
						if (imap == NULL){
							fclose(fs);
							return -1;
						}
						
						inodes = malloc(imap_size * sizeof(struct inode));
						if (inodes == NULL){
							fclose(fs);
							return -1;
						}
					}
					portion++;
				}
			}else if (lineNum == 1){
				// populate imap
				char *found;
				int portion = 0;
				while( (found = strsep(&line,",")) != NULL ){
					*(imap + portion) = atoi(found);
					portion++;
				}
				for (int i = 0; i < imap_size; i++){
					printf("%i,", *(imap + i));
				}
			}else{
				// populate inodes

				// signals end of inode data
				if (strcmp(line, "*&*&*") != 0){
					if (inode_part == 0){
						// get size and type of inode
						char *found;
						int portion = 0;
						while( (found = strsep(&line,",")) != NULL ){
							if (portion == 0){
								new_inode.size = atoi(found);
								printf("inode %i size: %i\n", inode_counter, new_inode.size);
							}else{
								new_inode.type = atoi(found);
								printf("inode %i type: %i\n", inode_counter, new_inode.type);
							}
							portion++;
						}
					}else{
						strcpy(new_inode.data[inode_part - 1]->data, line);
						printf("inode %i data: %s\n", inode_counter, new_inode.data[inode_part - 1]->data);
					}
					inode_part++;
				}else{
					*(inodes + inode_counter) = new_inode;
					inode_counter++;
					inode_part = 0;
				}
			}
		}else{
			break;
		}
		lineNum++;
    }

	printf("after load\n");
	*(inodes + inode_counter) = new_inode;

	if (line)
		free(line);

	// nothing was found in the fs
	if (lineNum == 0){
		printf("Nothing found in fs\n");
		imap_size = 1;
		imap = malloc(imap_size * sizeof(int));
		if (imap == NULL){
			fclose(fs);
			return -1;
		}
		
		inodes = malloc(imap_size * sizeof(struct inode));
		if (inodes == NULL){
			fclose(fs);
			return -1;
		}
	}
	
	fclose(fs);

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
	return -1;
}

int s_mfs_stat(int inum, int type, int size){
	return -1;
}

int s_mfs_write(int inum, char *buffer, int block){
	return -1;
}

int s_mfs_unlink(int pinum, char *name){
	return -1;
}

int s_mfs_read(int inum, char *buffer, int block){
	//TODO
	return -1;
}

int s_mfs_create(int pinum, int type, char *name){
	//TODO
	return -1;
}

int s_mfs_shutdown(char fname[]){
	//TODO: force everything to the fs and exit	
	return -1;
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

	int init_rc = init_fs(argv[2]);

	if (init_rc < 0){
		printf("Could not get server running\n");
		exit(0);
	}

	printf("server:: up and running\n");
	
	while (1) {
		struct sockaddr_in addr;
		struct message msg;
		
		printf("server:: waiting...\n");
		
		int rc = UDP_Read(sd, &addr, (char *)&msg, sizeof(struct message));
		printf("server:: got read\n");
	
		msg = (struct message) msg;
		
		printf("server:: after cast\n");	
		printf("server:: read message [size:%d] call: %i\n", rc, msg.call);
		printf("server:: after print\n");	
		
		// error here - is not an int
		int call = msg.call;
		
		printf("server:: before work\n");

		if(call == 0){
			msg.call = s_mfs_lookup(msg.pinum, msg.name);
		}else if (call == 1){
			msg.call = s_mfs_stat(msg.inum, msg.type, msg.size);
		}else if (call == 2){
			msg.call = s_mfs_write(msg.inum, msg.buffer, msg.block);
		}else if(call == 3){
			msg.call = s_mfs_read(msg.inum, msg.buffer, msg.block);		
		}else if (call == 4){
			msg.call = s_mfs_create(msg.pinum, msg.type, msg.name);
		}else if (call == 5){
			msg.call = s_mfs_unlink(msg.pinum, msg.name);		
		}else if(call == 6){
			msg.call = s_mfs_shutdown(argv[2]);
		}
		
		printf("server:: after work\n");

		if (rc > 0) {
			printf("server:: before call\n");
            rc = UDP_Write(sd, &addr, (char *)&msg, sizeof(struct message));
			printf("server:: reply\n");
		}
		
    }
	
    return 0; 
}
