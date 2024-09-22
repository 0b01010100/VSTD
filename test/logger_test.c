#include <vlogger.h>
int main()
{
    VWARN("logged.txt", "YOUR TEST%d", 1);
    VINFO("logged.txt", "YOUR TEST%d", 2);
    return 0;
}