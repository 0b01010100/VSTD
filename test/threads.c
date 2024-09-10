#include <stdio.h>
#include <windows.h>
#include <pthread.h>
#include <strsafe.h>
__declspec(thread) int thread_local_var = 0;

DWORD WINAPI my_thread_function(LPVOID arg) {
    thread_local_var = *(int*)arg;
    printf("Thread ID: %lu, thread_local_var: %d\n", GetCurrentThreadId(), thread_local_var);
    return 0;
}

int main() {
    HANDLE threads[2];
    int values[2] = {100, 200};

    threads[0] = CreateThread(NULL, 0, my_thread_function, &values[0], 0, NULL);
    threads[1] = CreateThread(NULL, 0, my_thread_function, &values[1], 0, NULL);

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(threads[0]);
    CloseHandle(threads[1]);

    return 0;
}
