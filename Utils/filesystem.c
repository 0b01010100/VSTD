#include <filesystem.h>
#include <stdio.h>
#include <stdlib.h>

#if defined (U_WINDOWS)
#include <Windows.h>

// Gets the absolute file path
char* abs_path(Path rel) {
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
int is_folder(Path path) {
    DWORD attributes = GetFileAttributes(path);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        printf("Error: %lu\n", GetLastError());
        return -1;
    }
    return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

// bruh why unix. I FUCKING LOVE WINDOWS NOW

 // Apple or Unix
#elif defined(U_APPLE) || defined(U_UNIX) || defined(__MINGW32__) || defined(__MINGW64__)
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#if defined(__MINGW32__) || defined(__MINGW64__)
#include <fileapi.h>
#endif
// Gets the absolute file path
char* abs_path(Path rel) {
    if (rel == NULL) return NULL;

    char *absolute_path = calloc(PATH_MAX, sizeof(char));
    if (absolute_path == NULL) {
        perror("abs_path->calloc");
        return NULL;
    }

    #if defined(__MINGW32__) || defined(__MINGW64__)
    // Use GetFullPathName for MinGW
    if (GetFullPathName(rel, PATH_MAX, absolute_path, NULL) == 0) {
        perror("abs_path->GetFullPathName");
        free(absolute_path);
        return NULL;
    }
    #else
    // Use realpath for Unix-like systems
    if (realpath(rel, absolute_path) == NULL) {
        perror("abs_path->realpath");
        free(absolute_path);
        return NULL;
    }
    #endif

    return absolute_path;
}

// Tells you whether a file is a folder or text file
// 0 for text file, 1 for folder, -1 for error
int is_folder(Path path) {
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