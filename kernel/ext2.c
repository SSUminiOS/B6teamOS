// ext2.c
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ext2.h"
#include "blockdev.h"
#define BLOCK_SIZE 4096
#define NUM_BLOCKS 1024

int ext2_open(const char* path, int flags, mode_t mode) {
    ext2_file_t* file = malloc(sizeof(ext2_file_t));
    if (file == NULL) {
        // 메모리 할당 실패 처리
        return -1;
    }
    if (get_inode_from_path(path, &file->inode) < 0) {
        // get_inode_from_path 실패 처리
        free(file);
        return -1;
    }
    file->pos = 0;
    return (int)(uintptr_t)file; // 안전한 형식 캐스팅
}

int ext2_read(int fd, void* buf, size_t count) {
    ext2_file_t* file = (ext2_file_t*)fd;
    uint32_t remaining = file->inode->size - file->pos;
    if (remaining == 0) {
        return 0; // 파일 끝에 도달함
    }
    size_t bytes_to_read = (remaining < count) ? remaining : count;

    // 데이터 블록에서 읽기
    read_blocks(file->inode, buf, bytes_to_read, file->pos);
    file->pos += bytes_to_read;
    return bytes_to_read;
}

int ext2_write(int fd, const void* buf, size_t count) {
    ext2_file_t* file = (ext2_file_t*)fd;
    if (file->pos + count > file->inode->size) {
        return -1; // 파일 크기를 초과함
    }

    // 데이터 블록에 쓰기
    write_blocks(file->inode, buf, count, file->pos);
    file->pos += count;
    return count;
}

int ext2_close(int fd) {
    ext2_file_t* file = (ext2_file_t*)fd;
    free(file->inode);
    free(file);
    return 0;
}

// inode 테이블 및 관련 함수
#define MAX_FILES 10
static inode_t inode_table[MAX_FILES];
static bool inode_table_initialized = false;

int get_inode_from_path(const char* path, inode_t* inode) {
    if (!inode_table_initialized) {
        // inode_table 초기화 (간단한 예제를 위해)
        for (int i = 0; i < MAX_FILES; i++) {
            inode_table[i].block_start = i * 10; // 각 파일은 10 블록씩 떨어져 있음
            inode_table[i].size = BLOCK_SIZE * 2; // 각 파일은 2 블록 크기
        }
        inode_table_initialized = true;
    }

    // 여기서는 단순화를 위해 항상 첫 번째 inode를 반환합니다.
    if (inode) {
        *inode = inode_table[0];
        return 0; // 성공
    }
    return -1; // 실패
}

// 연속된 블록을 읽는 함수
int read_blocks(inode_t* inode, void* buf, size_t size, uint32_t start_block) {
    if (start_block + (size / BLOCK_SIZE) > NUM_BLOCKS) {
        fprintf(stderr, "read_blocks: Request exceeds block device size\n");
        return -1;
    }

    size_t total_bytes_read = 0;
    uint32_t current_block = start_block;
    while (total_bytes_read < size) {
        size_t bytes_to_read = (size - total_bytes_read > BLOCK_SIZE) ? BLOCK_SIZE : size - total_bytes_read;
        if (blockdev_read(current_block, (char*)buf + total_bytes_read, bytes_to_read) < 0) {
            return -1; // blockdev_read 실패
        }
        total_bytes_read += bytes_to_read;
        current_block++;
    }
    return total_bytes_read;
}

// 연속된 블록에 데이터를 쓰는 함수
int write_blocks(inode_t* inode, const void* buf, size_t size, uint32_t start_block) {
    if (start_block + (size / BLOCK_SIZE) > NUM_BLOCKS) {
        fprintf(stderr, "write_blocks: Request exceeds block device size\n");
        return -1;
    }

    size_t total_bytes_written = 0;
    uint32_t current_block = start_block;
    while (total_bytes_written < size) {
        size_t bytes_to_write = (size - total_bytes_written > BLOCK_SIZE) ? BLOCK_SIZE : size - total_bytes_written;
        if (blockdev_write(current_block, (const char*)buf + total_bytes_written, bytes_to_write) < 0) {
            return -1; // blockdev_write 실패
        }
        total_bytes_written += bytes_to_write;
        current_block++;
    }
    return total_bytes_written;
}