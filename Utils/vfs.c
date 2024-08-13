#include <vfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int vfs_touch(vfs_Path path)
{
    if(!path) return -1;
    FILE* file = fopen(path, "w");
    if(!file) 
    {
        fprintf(stderr, "Error creating file: ", "%s", path);
        return -1;    
    }

    fclose(file);
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <Windows.h>
#include <direct.h> // For _mkdir 

char* vfs_abs(vfs_Path  rel) {
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

int vfs_mkdir(vfs_Path  dir_name) {
    if (!dir_name) return -1;
    if (_mkdir(dir_name) == -1) {
        perror("Error creating directory");
        return -1; 
    }
    return 0; 
}

int vfs_rmdir(vfs_Path  dir_name) {
    if (!dir_name) return -1;
    if (!RemoveDirectory(dir_name)) {
        DWORD error = GetLastError();
        printf("Error deleting directory: %lu\n", error);
        return -1; 
    }
    return 0; 
}

int vfs_isdir(vfs_Path  path) {
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

char* vfs_abs(vfs_Path  rel) {
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

int vfs_mkdir(vfs_Path  dir_name) {
    if (!dir_name) return -1;
    if (mkdir(dir_name, 0777) == -1) {
        perror("Error creating directory");
        return -1;
    }
    return 0;
}

int vfs_rmdir(vfs_Path  dir_name) {
    if (!dir_name) return -1;
    if (rmdir(dir_name) != 0) {
        perror("Error deleting directory");
        return -1;
    }
    return 0; 
}

int vfs_isdir(vfs_Path  path) {
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

bool vfs_exist(vfs_Path  path) {
    if (!path) return false;
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

long vfs_fileSize(vfs_Path  path) {
    if (!path) return -1;

    FILE *file = fopen(path, "rb");
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

    //rewind start pointer back to zero
    fseek(file, 0, SEEK_SET);
    fclose(file);

    return file_size;
}

ssize_t vfs_read(vfs_Path path, void* buffer, size_t size)
{
    if (!path || !buffer) return -1;

    FILE *file = fopen(path, "rb");
    if (file == NULL) return -1;

    //read the file data
    size_t readbytes = fread(buffer, 1, size, file);

    //checks for any read errors
    if(ferror(file)){
        fclose(file);
        return -1;
    }

    fclose(file);
    return (ssize_t)readbytes;
}

ssize_t vfs_readAll(vfs_Path path, char** out)
{
    if (!path || !out) return -1;

    FILE *file = fopen(path, "rb");
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

    char* temp_buffer = (char*)malloc(file_size + 1);
    if (temp_buffer == NULL) {
        fclose(file);
        return -1;
    }

    size_t bytes_read = fread(temp_buffer, 1, file_size, file);
    if (ferror(file)) {
        free(temp_buffer);
        fclose(file);
        return -1;
    }

    temp_buffer[bytes_read] = '\0';

    fclose(file);

    *out = temp_buffer;
    return bytes_read;
}

ssize_t vfs_cat(vfs_Path* path_OR_str, const char* src, bool file)
{
    if(!path_OR_str || !src) return -1;

    size_t out_len;
    size_t src_len = strlen(src);
    char * str = NULL;
    if(file == true)
    {
        FILE* f = fopen(*path_OR_str, "a");
        if (!f) return -1;
        if (fputs(src, f) == EOF) {
            fclose(f);
            return -1; 
        }

        return (size_t)FS_FileSize(*path_OR_str) == -1;
    }
    else
    {
    realloc(*path_OR_str,(out_len + src_len + 1) * sizeof(char));
    char* new_ = memcpy(*path_OR_str, src, src_len);
    if (!new_) return -1;
    memcpy(new_ + out_len, src, src_len);
    new_[out_len + src_len] = '\0';
    
    return out_len + src_len;
    }
}
