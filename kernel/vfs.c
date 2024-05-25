#include "vfs.h"
#include "ext2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// vfs_open 함수 구현
int vfs_open(const char* path, int flags, mode_t mode) {
    printf("VFS: Opening file %s with flags %d and mode %d\n", path, flags, mode);
    
    // ext2 파일 시스템의 open 함수 호출
    int fd = ext2_open(path, flags, mode);
    
    if (fd < 0) {
        fprintf(stderr, "VFS: Failed to open file %s\n", path);
        return -1;
    }
    
    printf("VFS: File %s opened successfully with fd %d\n", path, fd);
    return fd;
}

// vfs_read 함수 구현
int vfs_read(int fd, void* buf, size_t count) {
    printf("VFS: Reading %zu bytes from file descriptor %d\n", count, fd);
    
    // ext2 파일 시스템의 read 함수 호출
    int bytesRead = ext2_read(fd, buf, count);
    
    if (bytesRead < 0) {
        fprintf(stderr, "VFS: Failed to read from file descriptor %d\n", fd);
        return -1;
    }
    
    printf("VFS: Read %d bytes from file descriptor %d\n", bytesRead, fd);
    return bytesRead;
}

// vfs_write 함수 구현
int vfs_write(int fd, const void* buf, size_t count) {
    printf("VFS: Writing %zu bytes to file descriptor %d\n", count, fd);
    
    // ext2 파일 시스템의 write 함수 호출
    int bytesWritten = ext2_write(fd, buf, count);
    
    if (bytesWritten < 0) {
        fprintf(stderr, "VFS: Failed to write to file descriptor %d\n", fd);
        return -1;
    }
    
    printf("VFS: Wrote %d bytes to file descriptor %d\n", bytesWritten, fd);
    return bytesWritten;
}

// vfs_close 함수 구현
int vfs_close(int fd) {
    printf("VFS: Closing file descriptor %d\n", fd);
    
    // ext2 파일 시스템의 close 함수 호출
    int result = ext2_close(fd);
    
    if (result < 0) {
        fprintf(stderr, "VFS: Failed to close file descriptor %d\n", fd);
        return -1;
    }
    
    printf("VFS: File descriptor %d closed successfully\n", fd);
    return result;
} 