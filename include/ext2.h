// ext2.h
#ifndef EXT2_H
#define EXT2_H

#include <stdint.h>

typedef struct ext2_inode {
    uint32_t mode;
    uint32_t size;
    uint32_t blocks[15];
    // ... 다른 필드들
} ext2_inode_t;

typedef struct ext2_file {
    ext2_inode_t* inode;
    uint32_t pos;
} ext2_file_t;

int ext2_open(const char* path, int flags, mode_t mode);
int ext2_read(int fd, void* buf, size_t count);
int ext2_write(int fd, const void* buf, size_t count);
int ext2_close(int fd);

#endif
