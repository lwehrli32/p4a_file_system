#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "mfs.h"

typedef struct Block{
	char *data;
}Block;

typedef struct Inode{
	char name[28];
	int size; // number of last byte in file
	int type; // file or dir
	int data_offset[14]; // this points to the data Block
}Inode;

typedef struct Checkpoint {
	int file_size;
	int imap[256]; // This points to the inodes
} Checkpoint;

struct Checkpoint *checkpoint;
void *write_buffer;
size_t total_size;

Inode * get_inode(int offset, FILE *fs);
Block * get_data(int offset, FILE *fs);

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

	total_size = sizeof(struct Block) + sizeof(struct Inode);

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
	FILE *fs = fopen(fname, "r");
	if (fs == NULL) return -1;
	int imap_num = checkpoint->imap[pinum];
	if (imap_num == NULL) return -1;
	Inode *inode = get_inode(imap_num, fs);
	if (inode == NULL) return -1;
	if (inode->type == MFS_DIRECTORY) {
		for (int i = 0; i < 14; i++) {
			// Where an inode designated as a directory is made up of
			// direct pointers to MFS_DirEnt_t objects. If so, check their
			// names and proceed.
			MFS_DirEnt_t *tempDir = inode->data_offset[i];
			if (strcmp(tempDir->name, name)) {
				return tempDir->inum;
			}
			return -1;
		}
	} else return -1;
 	return 0;
}
// returns some information about the file specified by inum. 
// Upon success, return 0, otherwise -1. The exact info returned 
// is defined by MFS_Stat_t. Failure modes: inum does not exist.
// size is defined by MFS_BLOCK_SIZE * #blocks
// Method Definition was originally:
// int MFS_Stat(int inum, MFS_Stat_t *m)
int s_mfs_stat(int inum, int type, int size){
	//TODO
	printf("server:: mfs_stat\n");
	FILE *fs = fopen(fname, "r");
	if (fs == NULL) return -1;
	int imap_num = checkpoint->imap[pinum];
	if (imap_num == NULL) return -1;
	Inode *inode = get_inode(imap_num, fs);
	if (inode == NULL) return -1;
	// For server.c
	// MFS_Stat_t *stat = NULL; 
	// stat->type = inode->type;
	// stat->size = MFS_BLOCK_SIZE * inode->size; 
	type = inode->type;
	size = inode->size;
	//return stat;
	// Cannot currently return any information
	return 0;
}

int s_mfs_write(int inum, char *buffer, int block, char fname[]){
	printf("server:: mfs_write\n");
	FILE *fs = fopen(fname, "r");
	if (fs == NULL){
		return -1;
	}

	int imap_num  = checkpoint->imap[inum];
	if (imap_num ==0){
		return -1;
	}

	Inode *inode = get_inode(imap_num, fs);
	if(inode == NULL){
		return -1;
	}

	if (inode->type == MFS_REGULAR_FILE){
		// TODO file
		Block *b = get_data(inode->data_offset[block], fs);
		strcpy(b->data, buffer);
		
		memcpy(write_buffer, (void *)b->data, sizeof(b->data));
		
		fclose(fs);
		fs = fopen(fname, "a");
		
		fwrite((char *)write_buffer, sizeof(write_buffer) + 1, 1, fs);
		
		write_buffer = NULL;

		// update checkpoint size
		fseek(fs, 0, SEEK_END);
		checkpoint->file_size = ftell(fs);
	}else{
		return -1;
	}

	fclose(fs);
	return 0;
}
// removes the file or directory name from the directory specified by pinum. 
// 0 on success, -1 on failure. Failure modes: pinum does not exist, 
// directory is NOT empty. Note that the name not existing is NOT a failure 
//by our definition (think about why this might be).
int s_mfs_unlink(int pinum, char *name){
	//TODO
	printf("server:: mfs_unlink\n");
	FILE *fs = fopen(fname, "r+");
	if (fs == NULL) return -1;
	int imap_num = checkpoint->imap[pinum];
	if (imap_num == NULL) return -1;
	Inode *inode = get_inode(imap_num, fs);
	if (inode == NULL) return -1;
	if (inode == MFS_REGULAR_FILE) {

	} else if (inode == MFS_DIRECTORY) {

	}
	return 0;
}

int s_mfs_read(int inum, char *buffer, int block, char fname[]){
    printf("server:: mfs_read\n");
	s
	FILE *fs = fopen(fname, "r");
	if (fs == NULL){
		return -1;
	}	

	int imap_num  = checkpoint->imap[inum];
	if (imap_num ==0){
		return -1;
	}

	Inode *inode = get_inode(imap_num, fs);
	if(inode == NULL){
		return -1;
	}	

	if (inode->type == MFS_REGULAR_FILE){ 
		// file
		Block *b = get_data(inode->data_offset[block], fs);
		if(b->data != NULL){
			strcat(buffer, b->data);
		}else{
			return -1;
		}
	}else{
		// dir
		MFS_DirEnt_t dir;
		strcpy(dir.name, inode->name);
		dir.inum = inum;	
		strcat(buffer, (char *)&dir);
	}

	fclose(fs);
	
	return 0;
}

int s_mfs_create(int pinum, int type, char *name, char fname[]){
	//TODO
	printf("server:: mfs_create\n");
	
	//int parent_imap_num = checkpoint->imap[pinum];

	FILE *fs = fopen(fname, "r");
	if (fs == NULL){
		return -1;
	}

	Inode new_node;
	new_node.type = type;
	new_node.size = 1; 
	strcpy(new_node.name,name);	
	
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

Block * get_data(int offset, FILE *fs){
	size_t block_size = sizeof(Block);
	fseek(fs, offset, SEEK_END);

	char block[block_size];
	int counter = 0;
	while(counter < block_size){
		block[counter] = getc(fs);
		counter++;
		ftell(fs);
	}

	Block *b = (Block *)block;

	return b;	
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

	FILE *fs = fopen(argv[2], "a");
	Inode node;
	strcpy(node.name, "HI");
	node.size = 14;
	node.type = 0 ;
	memcpy(write_buffer, (char *)node, sizeof(Inode));
	//memcpy(fs, write_buffer, sizeof(write_buffer));
	fwrite((char *)write_buffer, sizeof(write_buffer) + 1, 1, fs); 
	fclose(fs);
	exit(0);
	if (strcmp(argv[2], "read") == 0){
		// call read
	}else if (strcmp(argv[2], "write") == 0){
		// call write
	}else if (strcmp(argv[2], "create") == 0){
		// call create
	}
	
	return 1;
}
