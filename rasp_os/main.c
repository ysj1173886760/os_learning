/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

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

char program[] = {
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
                bf_simulate(program);
                ch = uart_getc();
            default:
                break;
        }
    }
}
