#ifndef __vlogger__
#define __vlogger__
/*
@ref at 
	https://stackoverflow.com/questions/66663271/doing-a-logger-in-c
	https://github.com/theguywhoburns/SklCLib/blob/main/src/sklc_lib/utils/logger.c
	https://github.com/ntpeters/SimpleLogger
*/ 

#include <stdint.h>
#include <stdbool.h>

// Internal size of the logger, equivalent to the size of a pointer
#define VLOGGER_INTERNAL_SIZE (sizeof(void*))

// Maximum length for log messages (1024 characters)
#define VLOG_MSG_MAX_LEN (512 * 2)

// Enable logging for warnings and informational messages
#define VLOG_WARN_ENABLED 1
#define VLOG_INFO_ENABLED 1

// Disable debug and trace logging for release builds
#if defined(DEBUG) || defined(DEBUG)
    // Enable debug and trace logging in debug builds
    #define VLOG_DEBUG_ENABLED 1
    #define VLOG_TRACE_ENABLED 1
#else
    // Disable debug and trace logging in release builds
    #define VLOG_DEBUG_ENABLED 0
    #define VLOG_TRACE_ENABLED 0
#endif

// Enum defining various logging levels
typedef enum VLOG_LEVEL {
    VLOG_LEVEL_FATAL = 0,  // Fatal errors (highest priority)
    VLOG_LEVEL_ERROR = 1,  // Errors that do not stop execution
    VLOG_LEVEL_WARN = 2,   // Warnings that are not necessarily errors
    VLOG_LEVEL_INFO = 3,   // Informational messages
    VLOG_LEVEL_DEBUG = 4,  // Debugging messages (available in debug builds)
    VLOG_LEVEL_TRACE = 5   // Trace messages (available in debug builds)
} VLOG_LEVEL;

/**
 * @brief Logs a message with a specified log level.
 *
 * This function is used to log messages to a given stream with a particular log level.
 * It supports formatted input using variadic arguments, allowing the message to be
 * customized with additional data.
 *
 * @param _Stream The output stream to which the log message is written (e.g., stdout or a file).
 * @param level The log level for the message (e.g., INFO, DEBUG, ERROR).
 * @param message The log message format string (similar to printf).
 * @param ... Variadic arguments to format the log message (if any).
 */
void vloggerLog(const char* _Stream, VLOG_LEVEL level, const char* message, ...);

/**
 * @brief Logs a fatal-level message.
 *
 * This will always log since fatal logs are the highest severity.
 *
 * @param _Stream The output stream to which the fatal message is written.
 * @param message The log message format string.
 * @param ... Variadic arguments to format the fatal message.
 */
#define VFATAL(_Stream, message, ...) vloggerLog(_Stream, VLOG_LEVEL_FATAL, message, ##__VA_ARGS__)

#ifndef VERROR
/**
 * @brief Logs an error-level message.
 *
 * This logs errors that are less severe than fatal but still critical.
 *
 * @param _Stream The output stream to which the error message is written.
 * @param message The log message format string.
 * @param ... Variadic arguments to format the error message.
 */
#define VERROR(_Stream, message, ...) vloggerLog(_Stream, VLOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#endif

#if VLOG_WARN_ENABLED
/**
 * @brief Logs a warning-level message.
 *
 * This logs warnings when `VLOG_WARN_ENABLED` is 1.
 *
 * @param _Stream The output stream to which the warning message is written.
 * @param message The log message format string.
 * @param ... Variadic arguments to format the warning message.
 */
#define VWARN(_Stream, message, ...) vloggerLog(_Stream, VLOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
/**
 * @brief Does nothing if warning-level logging is disabled.
 */
#define VWARN(_Stream, message, ...)
#endif

#if VLOG_INFO_ENABLED
/**
 * @brief Logs an info-level message.
 *
 * This logs informational messages when `VLOG_INFO_ENABLED` is 1.
 *
 * @param _Stream The output stream to which the info message is written.
 * @param message The log message format string.
 * @param ... Variadic arguments to format the info message.
 */
#define VINFO(_Stream, message, ...) vloggerLog(_Stream, VLOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
/**
 * @brief Does nothing if info-level logging is disabled.
 */
#define VINFO(_Stream, message, ...)
#endif

#if VLOG_DEBUG_ENABLED
/**
 * @brief Logs a debug-level message (only enabled in debug builds).
 *
 * This logs debugging information when `VLOG_DEBUG_ENABLED` is 1.
 *
 * @param _Stream The output stream to which the debug message is written.
 * @param message The log message format string.
 * @param ... Variadic arguments to format the debug message.
 */
#define VDEBUG(_Stream, message, ...) vloggerLog(_Stream, VLOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
/**
 * @brief Does nothing if debug-level logging is disabled.
 */
#define VDEBUG(_Stream, message, ...)
#endif

#if VLOG_TRACE_ENABLED
/**
 * @brief Logs a trace-level message (only enabled in debug builds).
 *
 * This logs trace-level information when `VLOG_TRACE_ENABLED` is 1.
 *
 * @param _Stream The output stream to which the trace message is written.
 * @param message The log message format string.
 * @param ... Variadic arguments to format the trace message.
 */
#define VTRACE(_Stream, message, ...) vloggerLog(_Stream, VLOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
/**
 * @brief Does nothing if trace-level logging is disabled.
 */
#define VTRACE(_Stream, message, ...)
#endif


#endif // __vlogger__