#include <vconsole.h>
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <windows.h>

void VConsole_SetColor(enum VCONSOLE_COLOR bg_color, enum VCONSOLE_COLOR fg_color, enum VCONSOLE_ASCII ascii)
{
  WORD ascii_type = 0;

  switch (ascii) {
  case VCONSOLE_ASCII_NORMAL:
  case VCONSOLE_ASCII_ITALIC:
  case VCONSOLE_ASCII_UNDERLINE:
      ascii_type = 0;
    break;
  case VCONSOLE_ASCII_BOLD:
      ascii_type = FOREGROUND_INTENSITY;
    break;
  default:
    break;
  }

  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)(ascii_type | fg_color | (bg_color << 4)));
}

void VConsole_ResetColor(void) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void VConsole_SetCursorPos(int x, int y) {
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct VCONSOLE_VEC VConsole_GetCursorPos(void) {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  struct VCONSOLE_VEC pos;
  pos.x = csbi.dwCursorPosition.X;
  pos.y = csbi.dwCursorPosition.Y;
  return pos;
}

struct VCONSOLE_VEC VConsole_GetWidthHeight() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  struct VCONSOLE_VEC size;
  size.x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  size.y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  return size;
}

#endif

#if defined(__APPLE__) || defined(_unix) || defined(_unix_)
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

int ToAnsi(VCONSOLE_ASCII ascii) {
    switch (ascii) {
    case VCONSOLE_ASCII_NORMAL:    return 0;
    case VCONSOLE_ASCII_BOLD:      return 1;
    case VCONSOLE_ASCII_ITALIC:    return 3;
    case VCONSOLE_ASCII_UNDERLINE: return 4;
    default: return 0; 
    }
}

const char* ToColorCode(VCONSOLE_COLOR color) {
    switch (color) {
    case VCONSOLE_COLOR_BLACK:        return "\033[30m";
    case VCONSOLE_COLOR_BlUE:         return "\033[34m";
    case VCONSOLE_COLOR_GREEN:        return "\033[32m";
    case VCONSOLE_COLOR_CYAN:         return "\033[36m";
    case VCONSOLE_COLOR_RED:          return "\033[31m";
    case VCONSOLE_COLOR_PURPLE:       return "\033[35m"; // Magenta for most terminals
    case VCONSOLE_COLOR_BROWN:        return "\033[33m"; // Usually represented as yellow in ANSI
    case VCONSOLE_COLOR_LIGHTGRAY:    return "\033[37m";
    case VCONSOLE_COLOR_DARKGRAY:     return "\033[90m";
    case VCONSOLE_COLOR_LIGHTBLUE:    return "\033[94m";
    case VCONSOLE_COLOR_LIGHTGREEN:   return "\033[92m";
    case VCONSOLE_COLOR_LIGHTCYAN:    return "\033[96m";
    case VCONSOLE_COLOR_LIGHTRED:     return "\033[91m";
    case VCONSOLE_COLOR_LIGHTPURPLE:  return "\033[95m";
    case VCONSOLE_COLOR_YELLOW:       return "\033[93m";
    case VCONSOLE_COLOR_WHITE:        return "\033[97m";
    default: return "\033[0m"; // default
    }
}

void VConsole_SetColor
(
    enum VCONSOLE_COLOR bg_color, 
    enum VCONSOLE_COLOR fg_color, 
    enum VCONSOLE_ASCII ascii
)
{
  int attrCode = ToAnsi(ascii);
  int fgCode = ToColorCode(fg_color, 0);
  int bgCode = ToColorCode(bg_color, 1);
  printf("\033[%d;%d;%dm", attrCode, fgCode, bgCode);
}

void VConsole_ResetColor(void)
{
  printf("\033[0m");
}

void VConsole_SetCursorPos(int x, int y) {
  printf("\033[%d;%dH", y + 1, x + 1);
}

struct VCONSOLE_VEC VConsole_GetCursorPos(void) {
  struct CursorPos pos;
  printf("\033[6n");
  fflush(stdout);
  scanf("\033[%d;%dR", &pos.y, &pos.x);
  return pos;
}
struct VCONSOLE_VEC VConsole_GetWidthHeight() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  struct VCONSOLE_VEC size;
  size.x = w.ws_col;
  size.y = w.ws_row;
  return size;
}

#endif