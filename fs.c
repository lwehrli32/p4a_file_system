#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mfs.h"

typedef struct Block{
	char *data;
}Block;

typedef struct Inode{
	char name[26];
	int size; // number of last byte in file
	int type; // file or dir
	int *data_offset[14]; // point to data
}Inode;

typedef struct Imap{
	int offset[256];
}Imap;

typedef struct Checkpoint {
	int file_size;
	int imap[256];
} Checkpoint;

struct Checkpoint *checkpoint;
void *write_buffer;
size_t total_size;

Inode * get_inode(int offset, FILE *fs);
Imap * get_imap(int offset, FILE *fs);
int find_empty_index(Imap *imap);

int init_fs(char fname[]){
    printf("server:: reading file system to memory...\n");
    
    FILE *fs;
    fs = fopen(fname, "r");
	if (fs == NULL){
		return -1;
	}
	
	size_t checkpoint_size = sizeof(struct Checkpoint);
	int count = 0;
	char ch;
	char checkBuffer[checkpoint_size];
	while( ( ch = fgetc(fs) ) != EOF || count < checkpoint_size){
		checkBuffer[count++] = ch;
	}

	checkpoint = (struct Checkpoint*) checkBuffer;

	fclose(fs);

	total_size = sizeof(struct Block) + sizeof(struct Imap) + sizeof(struct Inode);

	write_buffer = malloc(total_size);

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
 	return 0;
}

int s_mfs_stat(int inum, int type, int size){
	//TODO
	printf("server:: mfs_stat\n");
	return 0;
}

int s_mfs_write(int inum, char *buffer, int block, char fname[]){
	//TODO
	printf("server:: mfs_write\n");
	FILE *fs = fopen(fname, "a");
	if(fs == NULL){
		return -1;
	}
	
	int end = checkpoint->file_size;

	//buffer = realloc(buffer, 
	
	
	fseek(fs, 0L, SEEK_END);
	size_t file_size = ftell(fs);

	fclose(fs);
	return 0;
}

int s_mfs_unlink(int pinum, char *name){
	//TODO
	printf("server:: mfs_unlink\n");
	return 0;
}

int s_mfs_read(int inum, char *buffer, int block, char fname[]){
	//TODO
    printf("server:: mfs_read\n");
	
	FILE *fs = fopen(fname, "r");

	int imap_num  = checkpoint->imap[inum];

	Imap *imap = get_imap(imap_num, fs);

	Inode *inode = get_inode(imap->offset[inum], fs);

	if (inode->type == MFS_REGULAR_FILE){ 
		// file
	}else{
		// dir
	}

	fclose(fs);
	
	return 0;
}

int s_mfs_create(int pinum, int type, char *name, char fname[]){
	//TODO
	printf("server:: mfs_create\n");
	
	int parent_imap_num = checkpoint->imap[pinum];

	FILE *fs = fopen(fname, "r");

	Imap *pimap = get_imap(parent_imap_num, fs);
	
	int empty_index = find_empty_index(pimap);
	
	

	//Inode *pinode = get_inode(pimap>offset, fs);
	
	//if (pinode->type != MFS_REGULAR_FILE){
		//return -1;
	//}

	
	
	fclose(fs);

	return 0;
}

int s_mfs_shutdown(char fname[]){
	//TODO: force everything to the fs and exit	
	printf("server:: mfs_shutdown\n");
	//fsync();
	free(write_buffer);
	return 0;
}

int find_empty_index(Imap *imap){
	for (int i = 0; i < 256; i++){
		if(imap->offset[i] == 0){
			return 1;
		}
	}
	return -1;
}

Imap * get_imap(int offset, FILE *fs){
	size_t imap_size = sizeof(Imap);
	fseek(fs, offset, SEEK_END);

	char imap[imap_size];
	int counter = 0;
	while(counter < imap_size){
		imap[counter] = getc(fs);
		counter++;
		ftell(fs);
	}
	
	Imap *i = (Imap *)imap;

	return i;
}

Inode * get_inode(int offset, FILE *fs){
	size_t inode_size = sizeof(Inode);
	fseek(fs, offset, SEEK_END);
	
	char inode[inode_size];
	int counter = 0;
	while(counter < inode_size){
		inode[counter] = getc(fs);
		counter++;
		ftell(fs);
	}

	Inode *i = (Inode *)inode;

	return i;
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
