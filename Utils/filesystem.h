#include <stdbool.h>
#include <stdint.h>
// #if defined(U_WINDOWS)
// #include <BaseTsd.h>
// typedef SSIZE_T ssize_t;
// #endif
// Reprsents the file 
typedef const char* Path;

/**
 * Gets the absolute file path.
 *
 * @param rel A relative file path.
 * @return A pointer to a string containing the absolute file path.
 */
char* FS_abs(Path rel);

/**
 * Tells you whether a file is a folder or a text file.
 *
 * @param path The file path.
 * @return 0 if the path is a text file, 1 if it is a folder, -1 if there is an error.
 */
int FS_is_folder(Path path);

/**
 * Tells you whether a file exists and if it is a folder or a text file.
 *
 * @param path The file path.
 * @return true if the path exists, false otherwise.
 */
bool FS_exist(Path path);

/**
 * Tells you the size of a file.
 *
 * @param path The file path.
 * @return The size of the file in bytes, or -1 if there is an error.
 */
long FS_FileSize(Path path);
