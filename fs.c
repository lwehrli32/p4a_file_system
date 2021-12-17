#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mfs.h"

typedef struct block{
	char *data;
}block;

typedef struct inode{
	int size; // number of last byte in file
	int type; // file or dir
	struct block *data[14]; // point to data
}inode;

typedef struct Checkpoint {
	int file_size;
	int imap[256];
} Checkpoint;

struct Checkpoint *checkpoint;
int *imap;
int imap_size;
struct inode *inodes;
char *buffer;

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
	if (fs == NULL){
		return -1;
	}
	//fseek(fs, 0L, SEEK_END);
	//size_t file_size = ftell(fs);
	size_t checkpoint_size = sizeof(struct Checkpoint);
	int count = 0;
	char ch;
	char checkBuffer[checkpoint_size];
	while( ( ch = fgetc(fs) ) != EOF || count < checkpoint_size){
	checkBuffer[count++] = ch; // make Buffer global variable
		printf("%c",ch);
	}

	checkpoint = (struct Checkpoint*) checkBuffer;
	
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
	/*	
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
	*/
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
	
	

	free(inodes);
	free(imap);
	return 0;
}

int main(int argc, char *argv[]){
	
	if (argc < 3){
		printf("Need a fs and an operation\n");
		exit(1);
	}

	init_fs(argv[1]);

	if (strcmp(argv[2], "read") == 0){
		// call read
	}else if (strcmp(argv[2], "write") == 0){
		// call write
	}else if (strcmp(argv[2], "create") == 0){
		// call create
	}
	
	return 1;
}
