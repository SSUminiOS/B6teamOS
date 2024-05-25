// api.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_FILENAME 256
#define MAX_CONTENT 1024

struct File {
    char name[MAX_FILENAME];
    char content[MAX_CONTENT];
};

struct BasicFileSystemLayer {
    struct File files[MAX_FILES];
    int fileCount;
};

void initBasicFileSystemLayer(struct BasicFileSystemLayer* fs) {
    fs->fileCount = 0;
}

int createFile(struct BasicFileSystemLayer* fs, char* name, char* content) {
    if (fs->fileCount >= MAX_FILES) {
        return -1; // No space left
    }
    for (int i = 0; i < fs->fileCount; i++) {
        if (strcmp(fs->files[i].name, name) == 0) {
            return -2; // File already exists
        }
    }
    strcpy(fs->files[fs->fileCount].name, name);
    strcpy(fs->files[fs->fileCount].content, content);
    fs->fileCount++;
    return 0; // Success
}

char* readFile(struct BasicFileSystemLayer* fs, char* name) {
    for (int i = 0; i < fs->fileCount; i++) {
        if (strcmp(fs->files[i].name, name) == 0) {
            return fs->files[i].content;
        }
    }
    return NULL; // File not found
}

int deleteFile(struct BasicFileSystemLayer* fs, char* name) {
    for (int i = 0; i < fs->fileCount; i++) {
        if (strcmp(fs->files[i].name, name) == 0) {
            for (int j = i; j < fs->fileCount - 1; j++) {
                fs->files[j] = fs->files[j + 1];
            }
            fs->fileCount--;
            return 0; // Success
        }
    }
    return -1; // File not found
}

void api_main() {
    struct BasicFileSystemLayer fs;
    initBasicFileSystemLayer(&fs);

    // 파일 생성하기
    if (createFile(&fs, "test.txt", "Hello, World!") == 0) {
        printf("File created successfully.\n");
    } else {
        printf("Failed to create file.\n");
    }

    // 파일 읽기
    char* content = readFile(&fs, "test.txt");
    if (content) {
        printf("File content: %s\n", content);
    } else {
        printf("File not found.\n");
    }

    // 파일 삭제하기
    if (deleteFile(&fs, "test.txt") == 0) {
        printf("File deleted successfully.\n");
    } else {
        printf("Failed to delete file.\n");
    }
}