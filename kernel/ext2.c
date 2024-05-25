// ext2.c
#include "ext2.h"
#include "blockdev.h"

int ext2_open(const char* path, int flags, mode_t mode) {
    // inode 할당 및 초기화
    ext2_file_t* file = malloc(sizeof(ext2_file_t));
    file->inode = get_inode_from_path(path);
    file->pos = 0;
    return (int)file;
}

int ext2_read(int fd, void* buf, size_t count) {
    ext2_file_t* file = (ext2_file_t*)fd;
    uint32_t remaining = file->inode->size - file->pos;
    if (remaining == 0) {
        return 0; // 파일 끝에 도달함
    }
    size_t bytes_to_read = (remaining < count) ? remaining : count;

    // 데이터 블록에서 읽기
    read_blocks(file->inode->blocks, file->pos, buf, bytes_to_read);
    file->pos += bytes_to_read;
    return bytes_to_read;
}

int ext2_write(int fd, const void* buf, size_t count) {
    ext2_file_t* file = (ext2_file_t*)fd;
    if (file->pos + count > file->inode->size) {
        return -1; // 파일 크기를 초과함
    }

    // 데이터 블록에 쓰기
    write_blocks(file->inode->blocks, file->pos, buf, count);
    file->pos += count;
    return count;
}

int ext2_close(int fd) {
    ext2_file_t* file = (ext2_file_t*)fd;
    free(file);
    return 0;
}