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

void print_games() {
    mvprintw(2, 20, "Press T to launch tetris");
    mvprintw(4, 20, "Press B to launch bird");
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
            default:
                break;
        }
    }
}
