#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILES 100
#define MAX_FILENAME 256
#define MAX_CONTENT 1024
#define MAX_DIRECTORIES 100
#define MAX_DIR_NAME 256

struct File {
    char name[MAX_FILENAME];
    char content[MAX_CONTENT];
};

struct Directory {
    char name[MAX_DIR_NAME];
    struct File files[MAX_FILES];
    int fileCount;
};

struct FileSystem {
    struct Directory directories[MAX_DIRECTORIES];
    int dirCount;
};

void initFileSystem(struct FileSystem* fs) {
    fs->dirCount = 0;
}

int createDirectory(struct FileSystem* fs, char* name) {
    if (fs->dirCount >= MAX_DIRECTORIES) {
        return -1; // No space left
    }

    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, name) == 0) {
            return -2; // Directory already exists
        }
    }

    strcpy(fs->directories[fs->dirCount].name, name);
    fs->directories[fs->dirCount].fileCount = 0;
    fs->dirCount++;
    return 0; // Success
}

int createFile(struct FileSystem* fs, char* dirName, char* fileName, char* content) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        return -1; // Directory not found
    }

    struct Directory* dir = &fs->directories[dirIndex];

    if (dir->fileCount >= MAX_FILES) {
        return -2; // No space left
    }

    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i].name, fileName) == 0) {
            return -3; // File already exists
        }
    }
    strncpy(dir->files[dir->fileCount].name, fileName, MAX_FILENAME - 1);
    dir->files[dir->fileCount].name[MAX_FILENAME - 1] = '\0';
    strncpy(dir->files[dir->fileCount].content, content, MAX_CONTENT - 1);
    dir->files[dir->fileCount].content[MAX_CONTENT - 1] = '\0';
    dir->fileCount++;
    return 0; // Success
}


char* readFile(struct FileSystem* fs, char* dirName, char* fileName) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        return NULL; // Directory not found
    }

    struct Directory* dir = &fs->directories[dirIndex];

    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i].name, fileName) == 0) {
            return dir->files[i].content;
        }
    }
    return NULL; // File not found
}

int updateFile(struct FileSystem* fs, char* dirName, char* fileName, char* newContent) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        return -1; // Directory not found
    }

    struct Directory* dir = &fs->directories[dirIndex];

    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i].name, fileName) == 0) {
            strncpy(dir->files[i].content, newContent, MAX_CONTENT -1);
            dir ->files[i].content[MAX_CONTENT-1]='\0';
            return 0; // Success
        }
    }

    return -2; // File not found
}

int deleteFile(struct FileSystem* fs, char* dirName, char* fileName) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        return -1; // Directory not found
    }

    struct Directory* dir = &fs->directories[dirIndex];

    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i].name, fileName) == 0) {
            for (int j = i; j < dir->fileCount - 1; j++) {
                dir->files[j] = dir->files[j + 1];
            }
            dir->fileCount--;
            return 0; // Success
        }
    }

    return -2; // File not found
}

void listFiles(struct FileSystem* fs, char* dirName) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        printf("Directory not found.\n");
        return;
    }

    struct Directory* dir = &fs->directories[dirIndex];

    printf("Files in directory '%s':\n", dirName);
    for (int i = 0; i < dir->fileCount; i++) {
        printf("%s\n", dir->files[i].name);
    }
}

int getFileSize(struct FileSystem* fs, char* dirName, char* fileName) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        return -1; // Directory not found
    }

    struct Directory* dir = &fs->directories[dirIndex];

    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i].name, fileName) == 0) {
            return strlen(dir->files[i].content);
        }
    }

    return -2; // File not found
}

void searchFiles(struct FileSystem* fs, char* dirName, char* keyword) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        printf("Directory not found.\n");
        return;
    }

    struct Directory* dir = &fs->directories[dirIndex];

    printf("Files containing '%s' in directory '%s':\n", keyword, dirName);
    for (int i = 0; i < dir->fileCount; i++) {
        if (strstr(dir->files[i].content, keyword) != NULL) {
            printf("%s\n", dir->files[i].name);
        }
    }
}

int copyFile(struct FileSystem* fs, char* srcDirName, char* srcFileName, char* destDirName, char* destFileName) {
    int srcDirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, srcDirName) == 0) {
            srcDirIndex = i;
            break;
        }
    }

    if (srcDirIndex == -1) {
    return -1; // Source directory not found
}

int destDirIndex = -1;
for (int i = 0; i < fs->dirCount; i++) {
    if (strcmp(fs->directories[i].name, destDirName) == 0) {
        destDirIndex = i;
        break;
    }
}

if (destDirIndex == -1) {
    return -2; // Destination directory not found
}

struct Directory* srcDir = &fs->directories[srcDirIndex];
struct Directory* destDir = &fs->directories[destDirIndex];

for (int i = 0; i < srcDir->fileCount; i++) {
    if (strcmp(srcDir->files[i].name, srcFileName) == 0) {
        if (destDir->fileCount >= MAX_FILES) {
            return -3; // No space left in destination directory
        }

        for (int j = 0; j < destDir->fileCount; j++) {
            if (strcmp(destDir->files[j].name, destFileName) == 0) {
                return -4; // File already exists in destination directory
            }
        }

        strncpy(destDir->files[destDir->fileCount].name, destFileName, MAX_FILENAME-1);
        destDir->files[destDir->fileCount].name[MAX_FILENAME-1]='\0';
        strncpy(destDir->files[destDir->fileCount].content, srcDir->files[i].content,MAX_CONTENT-1);
        destDir->files[destDir->fileCount].content[MAX_CONTENT-1]='\0';
        destDir->fileCount++;
        return 0; // Success
    }
}

return -5; // Source file not found
}

void api_main() {
    struct FileSystem fs;
    initFileSystem(&fs);
    printf("create direc");

    // 디렉토리 생성하기
    if (createDirectory(&fs, "root") == 0) {
        printf("Directory created successfully.\n");
    } else {
        printf("Failed to create directory.\n");
    }
    printf("create file");
    // 파일 생성하기
    if (createFile(&fs, "root", "test.txt", "Hello, World!") == 0) {
        printf("File created successfully.\n");
    } else {
        printf("Failed to create file.\n");
    }

    // 파일 읽기
    char* content = readFile(&fs, "root", "test.txt");
    if (content) {
        printf("File content: %s\n", content);
    } else {
        printf("File not found.\n");
    }

    // 파일 수정하기
    if (updateFile(&fs, "root", "test.txt", "Updated content!") == 0) {
        printf("File updated successfully.\n");
    } else {
        printf("Failed to update file.\n");
    }

    // 파일 목록 조회하기
    listFiles(&fs, "root");

    // 파일 크기 반환하기
    int size = getFileSize(&fs, "root", "test.txt");
    if (size >= 0) {
        printf("File size: %d\n", size);
    } else {
        printf("Failed to get file size.\n");
    }

    // 파일 검색하기
    searchFiles(&fs, "root", "Updated");

    // 파일 복사하기
    if (copyFile(&fs, "root", "test.txt", "root", "test_copy.txt") == 0) {
        printf("File copied successfully.\n");
    } else {
        printf("Failed to copy file.\n");
    }

    // 파일 삭제하기
    if (deleteFile(&fs, "root", "test.txt") == 0) {
        printf("File deleted successfully.\n");
    } else {
        printf("Failed to delete file.\n");
    }
}
