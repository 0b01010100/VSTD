#include <vterm.h>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <windows.h>

void vterm_set_color(enum VTERM_COLOR bg_color, enum VTERM_COLOR fg_color, enum VTERM_ASCII ascii) {
    WORD ascii_type = 0;

    switch (ascii) {
        case VTERM_ASCII_NORMAL:
        case VTERM_ASCII_ITALIC:
        case VTERM_ASCII_UNDERLINE:
            ascii_type = 0;
            break;
        case VTERM_ASCII_BOLD:
            ascii_type = FOREGROUND_INTENSITY;
            break;
        default:
            break;
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)(ascii_type | fg_color | (bg_color << 4)));
}

void vterm_reset_color(void) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void vterm_set_cursor_pos(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct VTERM_VEC vterm_get_cursor_pos(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    struct VTERM_VEC pos;
    pos.x = csbi.dwCursorPosition.X;
    pos.y = csbi.dwCursorPosition.Y;
    return pos;
}

struct VTERM_VEC vterm_get_width_height(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    struct VTERM_VEC size;
    size.x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return size;
}

#endif

#if defined(__APPLE__) || defined(__unix__) || defined(__unix__)
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

static int to_ansi(enum VTERM_ASCII ascii) {
    switch (ascii) {
        case VTERM_ASCII_NORMAL:    return 0;
        case VTERM_ASCII_BOLD:      return 1;
        case VTERM_ASCII_ITALIC:    return 3;
        case VTERM_ASCII_UNDERLINE: return 4;
        default: return 0; 
    }
}

static const char* to_color_code(enum VTERM_COLOR color) {
    switch (color) {
        case VTERM_COLOR_BLACK:        return "\033[30m";
        case VTERM_COLOR_BLUE:         return "\033[34m";
        case VTERM_COLOR_GREEN:        return "\033[32m";
        case VTERM_COLOR_CYAN:         return "\033[36m";
        case VTERM_COLOR_RED:          return "\033[31m";
        case VTERM_COLOR_PURPLE:       return "\033[35m"; // Magenta for most terminals
        case VTERM_COLOR_BROWN:        return "\033[33m"; // Usually represented as yellow in ANSI
        case VTERM_COLOR_LIGHTGRAY:    return "\033[37m";
        case VTERM_COLOR_DARKGRAY:     return "\033[90m";
        case VTERM_COLOR_LIGHTBLUE:    return "\033[94m";
        case VTERM_COLOR_LIGHTGREEN:   return "\033[92m";
        case VTERM_COLOR_LIGHTCYAN:    return "\033[96m";
        case VTERM_COLOR_LIGHTRED:     return "\033[91m";
        case VTERM_COLOR_LIGHTPURPLE:  return "\033[95m";
        case VTERM_COLOR_YELLOW:       return "\033[93m";
        case VTERM_COLOR_WHITE:        return "\033[97m";
        default: return "\033[0m"; // default
    }
}

void vterm_set_color(enum VTERM_COLOR bg_color, enum VTERM_COLOR fg_color, enum VTERM_ASCII ascii) {
    int attrCode = to_ansi(ascii);
    const char* fgCode = to_color_code(fg_color);
    const char* bgCode = to_color_code(bg_color);
    printf("%s%s\033[%dm", fgCode, bgCode, attrCode);
}

void vterm_reset_color(void) {
    printf("\033[0m");
}

void vterm_set_cursor_pos(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

struct VTERM_VEC vterm_get_cursor_pos(void) {
    struct VTERM_VEC pos;
    printf("\033[6n");
    fflush(stdout);
    scanf("\033[%d;%dR", &pos.y, &pos.x);
    return pos;
}

struct VTERM_VEC vterm_get_width_height(void) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    struct VTERM_VEC size;
    size.x = w.ws_col;
    size.y = w.ws_row;
    return size;
}

#endif
