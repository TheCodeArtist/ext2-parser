/* =============
 * ext copier
 * AUTHOR : CVS
 * =============
 */

#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "inc/types.h"
#include "inc/superblock.h"
#include "inc/blockgroup_descriptor.h"
#include "inc/inode.h"
#include "inc/directoryentry.h"

//#define DEBUG

#ifdef DEBUG
#define debug printf
#else
#define debug // 
#endif

struct os_superblock_t *superblock;
struct os_blockgroup_descriptor_t *blockgroup;
struct os_inode_t *inodes;

void read_superblock(int fd)
{
	superblock = malloc(sizeof(struct os_superblock_t));
	assert(superblock != NULL);
       
	assert(lseek(fd, (off_t)1024, SEEK_SET) == (off_t)1024);
	assert(read(fd, (void *)superblock, sizeof(struct os_superblock_t)) == sizeof(struct os_superblock_t));
}

void read_blockgroup(int fd)
{
	blockgroup = malloc(sizeof(struct os_blockgroup_descriptor_t));
	assert(blockgroup != NULL);
       
	assert(lseek(fd, (off_t)2048, SEEK_SET) == (off_t)2048);
	assert(read(fd, (void *)blockgroup, sizeof(struct os_blockgroup_descriptor_t)) == sizeof(struct os_blockgroup_descriptor_t));
}

void read_inodeTable(int fd)
{
	int i;

	// preparing to cache inode table in inodes
	inodes = (struct os_inode_t*)malloc(superblock->s_inodes_count*superblock->s_inode_size);
	assert(inodes != NULL);

	// seek to start of inode_table
	assert(lseek(fd, (off_t)(blockgroup->bg_inode_table*1024), SEEK_SET) == (off_t)(blockgroup->bg_inode_table*1024));

#if 0
	// read-in every inode into cache
	for(i=0; i<superblock->s_inodes_count;i++) {
		printf("%lld\n", lseek(fd, (off_t)superblock->s_inode_size , SEEK_CUR));
		assert(read(fd, (void *)&inodes[i], sizeof(struct os_inode_t)) == sizeof(struct os_inode_t));
		//assert(lseek(fd, (off_t)superblock->s_inode_size , SEEK_CUR) == (off_t)superblock->s_inode_size);
	}	
#else

	assert(read(fd, (void *)inodes, 0x40000) == 0x40000);

#endif

}

void printInodeType(int inode_type, char* inode_name)
{
	switch(inode_type)
	{
	case 1:
		printf("File");
		break;
	case 2:
		printf("Directory");
		break;
	case 3:
		printf("Char-Dev");
		break;
	case 4:
		printf("Block-Dev");
		break;
	case 5:
		printf("Buffer");
		break;
	case 6:
		printf("Socket");
		break;
	case 7:
		printf("Symlink");
		break;
	default:
		printf("UNKNOWN");
		break;
	}

	printf(" found\t= %s\n", inode_name);
}

void printInodePerm(int fd, int inode_num)
{
	//int curr_pos = lseek(fd, 0, SEEK_CUR);
	short int mode = inodes[inode_num-1].i_mode;

	printf("Permission mask\t= ");
	mode & EXT2_S_IRUSR ? printf("r") : printf("-");
	mode & EXT2_S_IWUSR ? printf("w") : printf("-");
	mode & EXT2_S_IXUSR ? printf("x") : printf("-");
	mode & EXT2_S_IRGRP ? printf("r") : printf("-");
	mode & EXT2_S_IWGRP ? printf("w") : printf("-");
	mode & EXT2_S_IXGRP ? printf("x") : printf("-");
	mode & EXT2_S_IROTH ? printf("r") : printf("-");
	mode & EXT2_S_IWOTH ? printf("w") : printf("-");
	mode & EXT2_S_IXOTH ? printf("x") : printf("-");

	printf("\n");

	//lseek(fd, curr_pos, SEEK_SET);
}

void ls(int fd, int base_inode_num)
{
	char* name;
	int curr_inode_num;
	int curr_inode_type;

	debug("data block addr \t= 0x%x\n\n", inodes[base_inode_num-1].i_block[0]);

	struct os_direntry_t* nextdir = malloc(sizeof(struct os_direntry_t));
	assert (nextdir != NULL);
	assert(lseek(fd, (off_t)(inodes[base_inode_num-1].i_block[0]*1024), SEEK_SET) == (off_t)(inodes[base_inode_num-1].i_block[0]*1024));
	assert(read(fd, (void *)nextdir, sizeof(struct os_direntry_t)) == sizeof(struct os_direntry_t));

	 while (1) {

		if (!nextdir->inode)
			break;

		name = (char*)malloc(nextdir->name_len+1);
		memcpy(name, nextdir->file_name, nextdir->name_len);
		name[nextdir->name_len+1] = '\0';

		curr_inode_num = nextdir->inode;
		curr_inode_type = nextdir->file_type;

		lseek(fd, (nextdir->rec_len - sizeof(struct os_direntry_t)), SEEK_CUR);
		assert(read(fd, (void *)nextdir, sizeof(struct os_direntry_t)) == sizeof(struct os_direntry_t));

		if (name[0] == '.') {
			if ( name[1]=='.' || name[1]=='\0')
				continue;
		} else {
			printf("Current inode\t= %d\n", curr_inode_num);
			debug("rec_len\t\t= %d\n", nextdir->rec_len);
			debug("nextdir->inode\t= %d\n",nextdir->inode);
			printInodeType(curr_inode_type, name);
			printInodePerm(fd, curr_inode_num);
			printf("\n");
		}
	} 

	return;
}

void saveInode(int fd, int inode_num, char* filename)
{
	int wfd = open(filename, O_RDWR | O_CREAT);
	if (wfd == -1) {
		printf("Could NOT open file \"%s\"\n", filename);
	}

	char* buffer = malloc(1024);
	assert (buffer != NULL);
	assert(lseek(fd, (off_t)(inodes[inode_num-1].i_block[0]*1024), SEEK_SET) == (off_t)(inodes[inode_num-1].i_block[0]*1024));
	assert(read(fd, (void *)buffer, 1024) == 1024);

	write(wfd, buffer, inodes[inode_num-1].i_size);
	close(wfd);

}

int main(int argc, char **argv)
{
	int inode = 2;

	// open up the disk file
	if (argc != 4) {
	printf("usage:  ext <file.img> <inode-num> <filename>\n");
		return -1; 
	}

	int fd = open(argv[1], O_RDONLY|O_SYNC);
	if (fd == -1) {
		printf("Could NOT open file \"%s\"\n", argv[1]);
		return -1; 
	}

	if (argv[2]) {
		inode = atoi(argv[2]);
	}

	// reading superblock
	read_superblock(fd);
	printf("block size \t\t= %d bytes\n", 1<<(10 + superblock->s_log_block_size));
	printf("inode count \t\t= 0x%x\n", superblock->s_inodes_count);
	printf("inode size \t\t= 0x%x\n", superblock->s_inode_size);

	// reading blockgroup
	read_blockgroup(fd);
	printf("inode table address \t= 0x%x\n", blockgroup->bg_inode_table);
	printf("inode table size \t= %dKB\n", (superblock->s_inodes_count*superblock->s_inode_size)>>10);

	// reading inode table
	read_inodeTable(fd);

	//ls(fd, inode);
	saveInode(fd, inode, argv[3]);
	close(fd);
}
