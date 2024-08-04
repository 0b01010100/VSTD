#include <filesystem.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <Windows.h>

char* FS_abs(Path rel) {
    if (rel == NULL) return NULL;

    char *absolute_path = calloc(MAX_PATH, sizeof(char));
    if (absolute_path == NULL) {
        perror("abs_path->calloc");
        return NULL;
    }

    if (_fullpath(absolute_path, rel, MAX_PATH) == NULL) {
        perror("abs_path->_fullpath");
        free(absolute_path);
        return NULL;
    }

    return absolute_path;
}

// Tells you whether a file is a folder or text file
// 0 for text file, 1 for folder, -1 for error
int FS_is_folder(Path path) {
    DWORD attributes = GetFileAttributes(path);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        printf("Error: %lu\n", GetLastError());
        return -1;
    }
    return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

// bruh why unix. I FUCKING LOVE WINDOWS NOW

 // Apple or Unix
#elif defined(U_APPLE) || defined(U_UNIX)
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
// Gets the absolute file path
char* FS_abs(Path rel) {
    if (rel == NULL) return NULL;

    char *absolute_path = calloc(PATH_MAX, sizeof(char));
    if (absolute_path == NULL) {
        perror("abs_path->calloc");
        return NULL;
    }
    // Use realpath for Unix-like systems
    if (realpath(rel, absolute_path) == NULL) {
        perror("abs_path->realpath");
        free(absolute_path);
        return NULL;
    }

    return absolute_path;
}

int FS_is_folder(Path path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        perror("stat");
        return -1;
    }
    return S_ISDIR(path_stat.st_mode);
}

#else
#error "None found for you compiler"
#endif

bool FS_exist(Path path)
{
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        fclose(file);
        return 1; 
    }
    return 0; 
}

long FS_FileSize(Path path)
{
    FILE* file = fopen(path, "r");
    if (file == NULL) return -1;

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return -1;
    }

    long file_size = ftell(file);
    if (file_size == -1L) {
        fclose(file);
        return -1;
    }

    fseek(file, 0, SEEK_SET);
    fclose(file);

    return file_size;
}