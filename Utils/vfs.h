/**
 * @brief Provides extended functionality for manipulating file
 *
 * @note that this is not part of the C standard library but a separate extension. Have Fun
*/

#ifndef __vfs__
#define __vfs__
#include <stdbool.h>
#include <stdint.h>
//WHY WINDOWS :=(
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
typedef SIZE_T size_t;
#endif
typedef const char* vfs_Path;

#define vfs_PATH(str) str

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Creates a new file or updates the timestamp of an existing file at the specified path.
 *
 * This function attempts to create a new file at the given `path`. If the file already exists,
 * it updates the file's last modified timestamp to the current time, similar to the Unix `touch` command.
 *
 * @param path The file path where the new file should be created or the existing file's timestamp updated.
 * @return 0 on success, or a negative error code if an error occurs.
 */
int vfs_touch(vfs_Path path);

/**
 * Gets the absolute file path.
 *
 * @param rel A relative file path.
 * @return A pointer to a string containing the absolute file path.
 */
char* vfs_abs(vfs_Path rel);

/**
 * Creates a new directory.
 *
 * @param dir_name The path of the directory to create.
 * @return 0 on success, -1 on failure.
 *
 * This function uses the _mkdir function from the Windows API to create a new directory.
 * If the directory creation fails, it prints an error message using perror and returns -1.
 */
int vfs_mkdir(vfs_Path dir_name);

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
int vfs_rmdir(vfs_Path dir_name);

/**
 * Tells you whether a file is a folder or a text file.
 *
 * @param path The file path.
 * @return 0 if the path is a text file, 1 if it is a folder, -1 if there is an error.
 */
int vfs_is_dir(vfs_Path path);

/**
 * Tells you whether a file exists and if it is a folder or a text file.
 *
 * @param path The file path.
 * @return true if the path exists, false otherwise.
 */
bool vfs_exist(vfs_Path path);

/**
 * Tells you the size of a file.
 *
 * @param path The file path.
 * @return The size of the file in bytes, or -1 if there is an error.
 */
long vfs_fileSize(vfs_Path path);


/**
 * Reads the contents of a file into a buffer.
 *
 * @param path The file path.
 * @param buffer A pointer to the buffer where the contents will be stored.
 * @param size The size of the buffer.
 * @return The number of bytes read, or -1 if there is an error.
 */
ssize_t vfs_read(vfs_Path path, void* buffer, size_t size);

/**
 * Reads all of the contents of a file into a buffer.
 *
 * @param path The file path.
 * @param buffer A pointer to the buffer where the contents will be stored.
 * @return The number of bytes read, or -1 if there is an error.
 */
ssize_t vfs_text_to_str(vfs_Path path, char** out);

/**
 * Concatenates the src string to the out path or appends text to a file.
 *
 * @param out The file path or a string buffer.
 * @param src A pointer to the src string to append to the out path.
 * @param file If true, appends text to the file at the path specified by `out`; 
 *             if false, appends `src` to the string `out`.
 * @return The new length of the path or string, or -1 if there is an error.
 */
ssize_t vfs_cat(vfs_Path* out, const char* src, bool file);


#ifdef __cplusplus
} // __cplusplus
#endif
#endif // __vfs__