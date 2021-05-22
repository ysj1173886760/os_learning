#include "uart.h"
#include "rand.h"
#include "delays.h"
#include "gpio.h"
#include "lfb.h"
#include "tetris.h"
#include "io.h"

struct tetris_level {
    int score;
    int nsec;
};

struct tetris_block {
    char data[5][5];
    int w;
    int h;
};

struct tetris {
    char game[30][30];
    int w;
    int h;
    int level;
    int gameover;
    int score;
    int x;
    int y;
    struct tetris_block current;
};

struct tetris_block blocks[] =
{
    {{"##", 
      "##"},
    2, 2
    },
    {{" X ",
      "XXX"},
    3, 2
    },
    {{"@@@@"},
        4, 1},
    {{"OO",
      "O ",
      "O "},
    2, 3},
    {{"&&",
      " &",
      " &"},
    2, 3},
    {{"ZZ ",
      " ZZ"},
    3, 2}
};

void
tetris_init(struct tetris *t,int w,int h) {
    int x, y;
    t->level = 1;
    t->score = 0;
    t->gameover = 0;
    t->w = w;
    t->h = h;
    for (x=0; x < w; x++) {
        for (y=0; y < h; y++)
            t->game[x][y] = ' ';
    }
}

void
tetris_print(struct tetris *t) {
    clear();
    int x,y;
    int posx = BASEX, posy = BASEY;
    print_at(posx, posy, "[LEVEL: %d | SCORE: %d]", t->level, t->score);
    posy += CHAR_HEIGHT;
    for (x=0; x<2*t->w+2; x++) {
        print_at(posx, posy, "~");
        posx += CHAR_WIDTH;
    }
    posx = BASEX;
    posy += CHAR_HEIGHT;
    for (y=0; y<t->h; y++) {
        print_at(posx, posy, "!");
        posx += CHAR_WIDTH;
        for (x=0; x<t->w; x++) {
            if (x>=t->x && y>=t->y 
                    && x<(t->x+t->current.w) && y<(t->y+t->current.h) 
                    && t->current.data[y-t->y][x-t->x]!=' ') {
                print_at(posx, posy, "%c ", t->current.data[y-t->y][x-t->x]);
                posx += CHAR_WIDTH * 2;
            } else {
                print_at(posx, posy, "%c ", t->game[x][y]);
                posx += CHAR_WIDTH * 2;
            }
        }
        print_at(posx, posy, "!");
        posx = BASEX;
        posy += CHAR_HEIGHT;
    }
    for (x=0; x<2*t->w+2; x++) {
        print_at(posx, posy, "~");
        posx += CHAR_WIDTH;
    }
    refresh();
}

int
tetris_hittest(struct tetris *t) {
    int x,y,X,Y;
    struct tetris_block b=t->current;
    for (x=0; x<b.w; x++)
        for (y=0; y<b.h; y++) {
            X=t->x+x;
            Y=t->y+y;
            if (X<0 || X>=t->w)
                return 1;
            if (b.data[y][x]!=' ') {
                if ((Y>=t->h) || 
                        (X>=0 && X<t->w && Y>=0 && t->game[X][Y]!=' ')) {
                    return 1;
                }
            }
        }
    return 0;
}

void
tetris_new_block(struct tetris *t) {
    t->current=blocks[rand() % 6];
    t->x=(t->w/2) - (t->current.w/2);
    t->y=0;
    if (tetris_hittest(t)) {
        t->gameover=1;
    }
    // rotate initially
    int rotate = rand() % 4;
    while (rotate--)
        tetris_rotate(t);
}

void
tetris_print_block(struct tetris *t) {
    int x,y;
    struct tetris_block b=t->current;
    for (x=0; x<b.w; x++)
        for (y=0; y<b.h; y++) {
            if (b.data[y][x]!=' ')
                t->game[t->x+x][t->y+y]=b.data[y][x];
        }
}

void
tetris_rotate(struct tetris *t) {
    struct tetris_block b=t->current;
    struct tetris_block s=b;
    int x,y;
    b.w=s.h;
    b.h=s.w;
    for (x=0; x<s.w; x++)
        for (y=0; y<s.h; y++) {
            b.data[x][y]=s.data[s.h-y-1][x];
        }
    x=t->x;
    y=t->y;
    t->x-=(b.w-s.w)/2;
    t->y-=(b.h-s.h)/2;
    t->current=b;
    if (tetris_hittest(t)) {
        t->current=s;
        t->x=x;
        t->y=y;
    }
}

void
tetris_gravity(struct tetris *t) {
    t->y++;
    if (tetris_hittest(t)) {
        t->y--;
        tetris_print_block(t);
        tetris_new_block(t);
    }
}

void
tetris_fall(struct tetris *t, int l) {
    int x,y;
    for (y=l; y>0; y--) {
        for (x=0; x<t->w; x++)
            t->game[x][y]=t->game[x][y-1];
    }
    for (x=0; x<t->w; x++)
        t->game[x][0]=' ';
}

void
tetris_check_lines(struct tetris *t) {
    int x,y,l;
    int p=100;
    for (y=t->h-1; y>=0; y--) {
        l=1;
        for (x=0; x<t->w && l; x++) {
            if (t->game[x][y]==' ') {
                l=0;
            }
        }
        if (l) {
            t->score += p;
            p*=2;
            tetris_fall(t, y);
            y++;
        }
    }
}

void
tetris_run(int w, int h) {
    struct tetris t;
    char cmd;
    int count=0;
    tetris_init(&t, w, h);

    tetris_new_block(&t);
    while (!t.gameover) {
        wait_msec(1000);
        count++;
        if (count%50 == 0) {
            tetris_print(&t);
        }
        if (count%350 == 0) {
            tetris_gravity(&t);
            tetris_check_lines(&t);
        }
        while ((cmd=get_input())>0) {
            switch (cmd) {
                case 'a':
                    t.x--;
                    if (tetris_hittest(&t))
                        t.x++;
                    break;
                case 'd':
                    t.x++;
                    if (tetris_hittest(&t))
                        t.x--;
                    break;
                case 's':
                    tetris_gravity(&t);
                    break;
                case ' ':
                    tetris_rotate(&t);
                    break;
            }
        }
    }

    tetris_print(&t);
}