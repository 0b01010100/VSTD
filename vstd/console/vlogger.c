#include <vlogger.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    #include <Windows.h>   // Use Windows threading API

    HANDLE log_mutex = NULL;

    void init_mutex() {
        log_mutex = CreateMutex(NULL, FALSE, NULL);
    }

    void log_lock() {
        WaitForSingleObject(log_mutex, INFINITE);
    }

    void log_unlock() {
        ReleaseMutex(log_mutex);
    }

    void destroy_mutex() {
        CloseHandle(log_mutex);
    }

#elif defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__linux__)
    #include <pthread.h>    // Use POSIX threads on Unix-based systems

    pthread_mutex_t log_mutex;

    void init_mutex() {
        pthread_mutex_init(&log_mutex, NULL);
    }

    void log_lock() {
        pthread_mutex_lock(&log_mutex);
    }

    void log_unlock() {
        pthread_mutex_unlock(&log_mutex);
    }

    void destroy_mutex() {
        pthread_mutex_destroy(&log_mutex);
    }

#elif defined(__STDC_NO_THREADS__)
    #error "C11 threads are not supported. Please use a different threading API."
#else
    #include <threads.h>

    mtx_t log_mutex;

    void init_mutex() {
        mtx_init(&log_mutex, mtx_plain);
    }

    void log_lock() {
        mtx_lock(&log_mutex);
    }

    void log_unlock() {
        mtx_unlock(&log_mutex);
    }

    void destroy_mutex() {
        mtx_destroy(&log_mutex);
    }

#endif


static void console_write(FILE* _Stream, const char* message, uint8_t colour);

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <Windows.h>

static void console_write(FILE* _Stream, const char* message, uint8_t colour) 
{
    if (colour > 5) {
        return;
    }

    HANDLE console_handle = (colour <= VLOG_LEVEL_ERROR) ? GetStdHandle(STD_ERROR_HANDLE) : GetStdHandle(STD_OUTPUT_HANDLE);
    if (console_handle == INVALID_HANDLE_VALUE) {
        return;
    }

    //[FATAL], [ERROR], [WARN], [INFO], [DEBUG], [TRACE]
    static const uint8_t levels[6] = {64, 4, 6, 2, 1, 8};

    CONSOLE_SCREEN_BUFFER_INFO csbi = {0};
    GetConsoleScreenBufferInfo(console_handle, &csbi);

    SetConsoleTextAttribute(console_handle, levels[colour]);

    OutputDebugStringA(message);

    DWORD length = (DWORD)strlen(message);
    DWORD number_written = 0;
    WriteConsoleA(console_handle, message, length, &number_written, NULL);

    SetConsoleTextAttribute(console_handle, csbi.wAttributes);

    if (_Stream) {
        fwrite(message, sizeof(char), VLOG_MSG_MAX_LEN, _Stream);
        fflush(_Stream);
        fclose(_Stream);
    }
}

#elif defined(__unix__) || defined(_unix) || defined(__APPLE__) || defined(__linux__)
#include <xcb/xcb.h>

static void console_write(FILE* _Stream, const char* message, uint8_t colour) 
{
    if (colour > 5) {
        return;
    }
	//[FATAL], [ERROR], [WARN], [INFO], [DEBUG], [TRACE]
	static const char* levels[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};

	fprintf((colour <= VLOG_LEVEL_ERROR)? stderr : stdout:,"\033[%sm%s\033[0m", levels[colour], message);


    if (_Stream) {
        fwrite(message, sizeof(char), VLOG_MSG_MAX_LEN, _Stream);
        fflush(_Stream);
    }
}

#else

static void console_write(FILE* _Stream, const char* message, uint8_t colour) 
{
    if (colour > 5) {
        return;
    }

    fprintf((colour <= 1) ? stderr : stdout, "%s", message);

    if (_Stream) {
        fwrite(message, sizeof(char), VLOG_MSG_MAX_LEN, _Stream);
        fflush(_Stream);
    }
}

#endif

#ifndef __builtin_va_list
#define __builtin_va_list va_list
#endif

void vloggerLog(const char* _Stream, VLOG_LEVEL level, const char* message, ...)
{
    log_lock();

    const char* lvls[6] = {
        "[FATAL]: ",
        "[ERROR]: ",
        "[WARN]:  ",
        "[INFO]:  ",
        "[DEBUG]: ",
        "[TRACE]: "
    };

    char va_message[VLOG_MSG_MAX_LEN] = {0};
    char out_message[VLOG_MSG_MAX_LEN] = {0};

    va_list arg_ptr;
    va_start(arg_ptr, message);

    vsnprintf(va_message, VLOG_MSG_MAX_LEN, message, arg_ptr);
    va_end(arg_ptr);

    snprintf(out_message, VLOG_MSG_MAX_LEN, "%s%s\n", lvls[level], va_message);
    
    FILE * __stream = NULL;
    
    if(_Stream){
        __stream = fopen(_Stream, "a");
        if(!__stream) {
            console_write(NULL, "[WARN]:  Faild to open or create file", VLOG_LEVEL_WARN);
        }
    }
    console_write(__stream, out_message, level);

    log_unlock();
}

