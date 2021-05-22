#include "uart.h"
#include "io.h"
#include "util.h"
#include "lfb.h"
#include "sprintf.h"

extern volatile unsigned char _end;
extern unsigned int width, height, pitch;
extern unsigned char *lfb;

void print_at(int x, int y, char *fmt, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    // we don't have memory allocation yet, so we
    // simply place our string after our code
    char *s = (char*)&_end;
    // use sprintf to format our string
    vsprintf(s,fmt,args);
    // print out as usual
    lfb_proprint(x, y, s);
}

char get_input() {
    char r = 0;
    if (!(*UART0_FR&0x10)) {
        r=(char)(*UART0_DR);
    }
    return r;
}

void mvprintw(int y, int x, char *fmt, ...) {
    x *= CHAR_WIDTH;
    y *= CHAR_HEIGHT;
    y += BASEY;
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    // we don't have memory allocation yet, so we
    // simply place our string after our code
    char *s = (char*)&_end;
    // use sprintf to format our string
    vsprintf(s,fmt,args);
    // print out as usual
    lfb_proprint(x, y, s);
}

void clear()
{
    int y;
    unsigned char *ptr=lfb + height * pitch;

    for(y = 0; y < height; y++) {
        memset(ptr, 0, width * 4);
        ptr += pitch;
    }
}

void refresh() {
    unsigned char *ptr=lfb;
    unsigned char *ptr2 = lfb + height * pitch;
    for (int y = 0; y < height; y++) {
        memcpy(ptr, ptr2, width * 4);
        ptr += pitch;
        ptr2 += pitch;
    }
}
