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
}


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

	node.data[block - 2]->data = buffer;

	return 0;
}

int s_mfs_unlink(int pinum, char *name){
	//TODO
	printf("server:: mfs_unlink\n");
	int inode = *(imap + pinum);
	struct inode node = *(inode + inode);
	if (node.type == MFS_DIRECTORY) {
		
	} else {
		return -1;
	}
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
		strcpy(buffer, node.data[block]->data);
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
	//int pinode = *(imap + pinum);
	
	

	// update checkpoint region
	
	return 0;
}

int s_mfs_shutdown(char fname[]){
	//TODO: force everything to the fs and exit	
	printf("server:: mfs_shutdown\n");
	
	FILE *fs;
    fs = fopen(fname, "w");
	
	// print checkpoint region
	fprintf(fs, "%i,%i\n", check_point, imap_size);
	
	// print imap
	for(int i = 0; i < imap_size; i++){
		if (i == imap_size - 1){
			fprintf(fs, "%i", *(imap + i));
		}else{
			fprintf(fs, "%i,", *(imap + i));
		}
	}
	
	fprintf(fs, "\n");
	
	// print inodes
	for (int i = 0; i < imap_size; i++){
		printf("server:: getting inode %i\n", i);
		struct inode node = *(inodes + i);
		fprintf(fs, "%i,", node.size);
		fprintf(fs, "%i\n", node.type);

		for(int jj = 0; jj < 14; jj++){
			if (node.data[i] != NULL){
				fprintf(fs, "%s", node.data[i]->data);
			}else{
				break;
			}
			fprintf(fs, "\n");
		}
		fprintf(fs, "*&*&*\n");
	}
	
	fclose(fs);
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
