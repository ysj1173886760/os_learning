#include "bf.h"

char memory[1024];
int stack[1024];
char board[TERMINAL_HEIGHT][TERMINAL_WIDTH];
char program[1921];

int curx = 0, cury = 0;
int mode = 0;

void bf_simulate(char *program) {
    memset(memory, 0, sizeof(memory));
    memset(stack, 0, sizeof(stack));
    int sp = 0, pc = 0, size = 0, ptr = 0;
    int printx = 0, printy = 0;
    while (program[size] != '\0')
        size++;
    for (; pc < size; pc++) {
        switch(program[pc]) {
        case '[':
            stack[sp] = pc;
            sp++;
            if (sp == 1024) {
                uart_puts("stack over flow");
                return;
            }
            break;
        case ']':
            if (sp == 0) {
                uart_puts("stack over flow");
                return;
            }
            pc = memory[ptr] ? stack[sp - 1]: pc;
            sp -= !memory[ptr];
            break;
        case '>':
            ptr++;
            if (ptr >= 1024) {
                uart_puts("heap over flow");
                return;
            }
            break;
        case '<':
            ptr--;
            if (ptr < 0) {
                uart_puts("heap over flow");
                return;
            }
            break;
        case '+':
            memory[ptr]++;
            break;
        case '-':
            memory[ptr]--;
            break;
        case ',':
            memory[ptr] = uart_getc();
            break;
        case '.':
            mvprintw(printy, printx++, "%c", memory[ptr]);
            refresh();
            if (printx == 80) {
                printx = 0;
                printy++;
            }
            break;
        default:
            break;
        }
    }
}

void bf_init() {
    curx = 0;
    cury = 0;
    for (int x = 0; x < TERMINAL_HEIGHT; x++) {
        for (int y = 0; y < TERMINAL_WIDTH; y++) {
            board[x][y] = ' ';
        }
    }
}

void bf_editor() {
    while(1) {
        char ch = uart_getc();
        if (mode == 0) {
            switch(ch) {
            case 'i':
                mode = 1;
                break;
            case 'h':
                if (cury != 0)
                    cury--;
                break;
            case 'j':
                if (curx < TERMINAL_HEIGHT - 1)
                    curx++;
                break;
            case 'k':
                if (curx > 0)
                    curx--;
                break;
            case 'l':
                if (cury < TERMINAL_WIDTH - 1)
                    cury++;
                break;
            // here, i try to simulate the arrow button in insert mode
            // but i found every time i pressed arrow button, it will send 27 to terminal first, then comes it's real keycode
            // hence, i put it into console mode, after down with cursor moving, i switch back to insert mode
            case 68:
                if (cury != 0)
                    cury--;
                mode = 1;
                break;
            case 66:
                if (curx < TERMINAL_HEIGHT - 1)
                    curx++;
                mode = 1;
                break;
            case 65:
                if (curx > 0)
                    curx--;
                mode = 1;
                break;
            case 67:
                if (cury < TERMINAL_WIDTH - 1)
                    cury++;
                mode = 1;
                break;
            case 'r':
                clear();
                refresh();
                memset(program, 0, sizeof(program));
                memcpy(program, board, sizeof(board));
                program[1920] = '\0';
                bf_simulate(program);
                uart_getc();
                break;
            case 'q':
                return;
            default:
                break;
            }
        } else {
            switch (ch) {
            case 127:
                if (cury != 0) {
                    cury--;
                    board[curx][cury] = ' ';
                }
                break;
            case 10:
                if (curx < TERMINAL_HEIGHT - 1) {
                    curx++;
                    cury = 0;
                }
                break;
            case 27:
                mode = 0;
                break;
            default:
                board[curx][cury] = ch;
                cury++;
                if (cury == TERMINAL_WIDTH) {
                    cury = 0;
                    curx++;
                    if (curx == TERMINAL_HEIGHT) {
                        curx = TERMINAL_HEIGHT - 1;
                        cury = TERMINAL_WIDTH - 1;
                    }
                }
                break;
            }
        }
        clear();
        for (int x = 0; x < TERMINAL_HEIGHT; x++) {
            for (int y = 0; y < TERMINAL_WIDTH; y++) {
                mvprintw(x, y, "%c", board[x][y]);
            }
        }
        // print cursor
        // mvprintw(20, 20, "%d", ch);
        mvprintw(curx, cury, "%c", mode ? '#' : '@');
        refresh();
    }
}