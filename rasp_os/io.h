void print_at(int x, int y, char *fmt, ...);
void mvprintw(int y, int x, char *fmt, ...);
char get_input();

void clear();
void refresh();

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16
#define BASEX 0
#define BASEY 384
#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 24
