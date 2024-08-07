#include <filesystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <Windows.h>
#include <direct.h> // For _mkdir 

char* FS_abs(FS_Path  rel) {
    if (rel == NULL) return NULL;

    char *absolute_path = calloc(MAX_PATH, sizeof(char));
    if (absolute_path == NULL) {
        perror("FS_abs->calloc");
        return NULL;
    }

    if (_fullpath(absolute_path, rel, MAX_PATH) == NULL) {
        perror("FS_abs->_fullpath");
        free(absolute_path);
        return NULL;
    }

    return absolute_path;
}

int FS_mkdir(FS_Path  dir_name) {
    if (!dir_name) return -1;
    if (_mkdir(dir_name) == -1) {
        perror("Error creating directory");
        return -1; 
    }
    return 0; 
}

int FS_rmdir(FS_Path  dir_name) {
    if (!dir_name) return -1;
    if (!RemoveDirectory(dir_name)) {
        DWORD error = GetLastError();
        printf("Error deleting directory: %lu\n", error);
        return -1; 
    }
    return 0; 
}

int FS_is_dir(FS_Path  path) {
    if (!path) return -1;
    DWORD attributes = GetFileAttributes(path);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        printf("Error: %lu\n", GetLastError());
        return -1; 
    }
    return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0; 
}

#elif defined(U_APPLE) || defined(U_UNIX)
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

char* FS_abs(FS_Path  rel) {
    if (rel == NULL) return NULL;

    char *absolute_path = calloc(PATH_MAX, sizeof(char));
    if (absolute_path == NULL) {
        perror("FS_abs->calloc");
        return NULL;
    }

    // Use realpath for Unix-like systems
    if (realpath(rel, absolute_path) == NULL) {
        perror("FS_abs->realpath");
        free(absolute_path);
        return NULL;
    }

    return absolute_path;
}

int FS_mkdir(FS_Path  dir_name) {
    if (!dir_name) return -1;
    if (mkdir(dir_name, 0777) == -1) {
        perror("Error creating directory");
        return -1;
    }
    return 0;
}

int FS_rmdir(FS_Path  dir_name) {
    if (!dir_name) return -1;
    if (rmdir(dir_name) != 0) {
        perror("Error deleting directory");
        return -1;
    }
    return 0; 
}

int FS_is_dir(FS_Path  path) {
    if (!path) return -1;
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        perror("FS_is_dir->stat");
        return -1; 
    }
    return S_ISDIR(path_stat.st_mode); 
}

#else
#error "Platform not supported"
#endif

bool FS_exist(FS_Path  path) {
    if (!path) return false;
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

long FS_FileSize(FS_Path  path) {
    if (!path) return -1;
    FILE *file = fopen(path, "r");
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
