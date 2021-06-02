#include "uart.h"
#include "mbox.h"
#include "power.h"
#include "rand.h"
#include "lfb.h"
#include "tetris.h"
#include "bird.h"
#include "io.h"
#include "malloc.h"
#include "bf.h"

char routine[] = {
    "+++++ +++++             initialize counter (cell #0) to 10 \
    [                       use loop to set 70/100/30/10 \
        > +++++ ++              add  7 to cell #1 \
        > +++++ +++++           add 10 to cell #2 \
        > +++                   add  3 to cell #3 \
        > +                     add  1 to cell #4 \
    <<<< -                  decrement counter (cell #0) \
    ] \
    > ++ .                  print 'H' \
    > + .                   print 'e' \
    +++++ ++ .              print 'l' \
    .                       print 'l' \
    +++ .                   print 'o' \
    > ++ .                  print ' ' \
    << +++++ +++++ +++++ .  print 'W' \
    > .                     print 'o' \
    +++ .                   print 'r' \
    ----- - .               print 'l' \
    ----- --- .             print 'd' \
    > + .                   print '!' \
    > .                     print '\n'"
};

void print_games() {
    mvprintw(2, 20, "Press T to launch tetris");
    mvprintw(4, 20, "Press B to launch bird");
    mvprintw(6, 20, "Press E to launch BF interpreter");
}

void main_screen() {
    clear();
    mvprintw(0, 20, "Welcome to the ascii-os");
    print_games();
    refresh();
}

// clear the screen
void init_game() {
    clear();
    refresh();
}

void main()
{
    // set up serial console
    uart_init();
    lfb_init();
    rand_init();
    mem_init();
    mm_init();

    while(1) {
        main_screen();
        char ch = uart_getc();
        switch(ch) {
            case 't':
                init_game();
                tetris_run(12, 15);
                // press any key to continue
                ch = uart_getc();
                break;
            case 'b':
                init_game();
                flappy_bird();
                ch = uart_getc();
                break;
            case 'e':
                init_game();
                bf_init();
                bf_editor();
            default:
                break;
        }
    }
}
