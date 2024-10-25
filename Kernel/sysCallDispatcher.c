#include "include/videoDriver.h"
#include "include/keyboard.h"
#include "include/lib.h"
#include "include/time.h"


#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYS_CALLS_QTY 19

extern uint8_t hasregisterInfo;
extern const uint64_t registerInfo[17];
extern int _hlt();

extern Color WHITE;
extern Color BLACK;

static uint64_t sys_read(uint64_t fd, char *buff)
{
    if (fd != 0)
    {
        return -1;
    }

    *buff = getCharFromKeyboard();
    return 0;
}