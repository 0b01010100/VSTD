#include <stdbool.h>
#include <stdint.h>
//WHY WINDOWS :=(
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
typedef SIZE_T size_t;
#endif
typedef const char* FS_Path;

#define FS_PATH(str) str

#ifdef __cplusplus
extern "C"{
#endif
/**
 * Gets the absolute file path.
 *
 * @param rel A relative file path.
 * @return A pointer to a string containing the absolute file path.
 */
char* FS_abs(FS_Path rel);

/**
 * Creates a new directory.
 *
 * @param dir_name The path of the directory to create.
 * @return 0 on success, -1 on failure.
 *
 * This function uses the _mkdir function from the Windows API to create a new directory.
 * If the directory creation fails, it prints an error message using perror and returns -1.
 */
int FS_mkdir(FS_Path dir_name);

/**
 * Deletes an existing directory.
 *
 * @param dir_name The path of the directory to delete.
 * @return 0 on success, -1 on failure.
 *
 * This function uses the RemoveDirectory function from the Windows API to delete an existing directory.
 * If the directory deletion fails, it retrieves the last error code using GetLastError, prints an error message,
 * and returns -1.
 */
int FS_rmdir(FS_Path dir_name);

/**
 * Tells you whether a file is a folder or a text file.
 *
 * @param path The file path.
 * @return 0 if the path is a text file, 1 if it is a folder, -1 if there is an error.
 */
int FS_is_dir(FS_Path path);

/**
 * Tells you whether a file exists and if it is a folder or a text file.
 *
 * @param path The file path.
 * @return true if the path exists, false otherwise.
 */
bool FS_exist(FS_Path path);

/**
 * Tells you the size of a file.
 *
 * @param path The file path.
 * @return The size of the file in bytes, or -1 if there is an error.
 */
long FS_FileSize(FS_Path path);


/**
 * Reads the contents of a file into a buffer.
 *
 * @param path The file path.
 * @param buffer A pointer to the buffer where the contents will be stored.
 * @param size The size of the buffer.
 * @return The number of bytes read, or -1 if there is an error.
 */
ssize_t FS_read(FS_Path path, void* buffer, size_t size);

/**
 * Reads all of the contents of a file into a buffer.
 *
 * @param path The file path.
 * @param buffer A pointer to the buffer where the contents will be stored.
 * @return The number of bytes read, or -1 if there is an error.
 */
ssize_t FS_text_to_str(FS_Path path, char** buffer);

#ifdef __cplusplus
}
#endif