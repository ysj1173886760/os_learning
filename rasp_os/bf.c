#include "bf.h"

char memory[1024];
int stack[1024];

void bf_simulate(char *program) {
    memset(memory, 0, sizeof(memory));
    memset(stack, 0, sizeof(stack));
    int sp = 0, pc = 0, size = 0, ptr = 0;
    int printx = 0, printy = 0;
    while (program[size] != '\0')
        size++;
    uart_puts("simulation begin\n");
    mvprintw(printy, printx++, "%d", size);
    refresh();
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
            pc = memory[ptr] ? stack[pc - 1]: pc;
            pc -= !memory[ptr];
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