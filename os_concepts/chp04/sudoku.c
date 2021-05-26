#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int flag[11];
int map[9][9] = {
    {2, 9, 7, 5, 1, 3, 4, 6, 8},
    {3, 8, 1, 4, 2, 6, 9, 5, 7},
    {6, 5, 4, 9, 8, 7, 1, 2, 3},
    {9, 6, 8, 2, 5, 1, 7, 3, 4},
    {1, 4, 3, 6, 7, 8, 5, 9, 2},
    {5, 7, 2, 3, 9, 4, 8, 1, 6},
    {8, 3, 6, 1, 4, 5, 2, 7, 9},
    {7, 2, 5, 8, 3, 9, 6, 4, 1},
    {4, 1, 9, 7, 6, 2, 3, 8, 5}
};

typedef struct {
    int index;
} parameters;

void *thread(void *args) {
    int index = ((parameters *) args)->index;
    int res = 1;
    int count[9] = {0};
    if (index < 9) {
        int sx = index % 3;
        int sy = index / 3;
        for (int i = sx * 3; i < sx * 3 + 3; i++) {
            for (int j = sy * 3; j < sy * 3 + 3; j++) {
                count[map[i][j]] = 1;
            }
        }
        for (int i = 1; i <= 9; i++) {
            if (!count[i]) {
                res = 0;
                break;
            }
        }
    } else if (index == 9) {
        for (int i = 0; i < 9; i++) {
            memset(count, 0, sizeof(count));
            for (int j = 0; j < 9; j++) {
                count[map[i][j]] = 1;
            }
            for (int j = 1; j <= 9; j++) {
                if (!count[j]) {
                    res = 0;
                    break;
                }
            }
            if (!res)
                break;
        }
    } else {
        for (int i = 0; i < 9; i++) {
            memset(count, 0, sizeof(count));
            for (int j = 0; j < 9; j++) {
                count[map[j][i]] = 1;
            }
            for (int j = 1; j <= 9; j++) {
                if (!count[j]) {
                    res = 0;
                    break;
                }
            }
            if (!res)
                break;
        }
    }
    flag[index] = res;
}

int main() {
    pthread_t tid[11];
    for (int i = 0; i < 11; i++) {
        parameters *data = (parameters *) malloc(sizeof(parameters));
        data->index = i;
        pthread_create(&tid[i], NULL, thread, data);
    }
    for (int i = 0; i < 11; i++) {
        pthread_join(tid[i], NULL);
    }
    int ans = 1;
    for (int i = 0; i < 11; i++) {
        if (!flag[i]) {
            ans = 0;
            break;
        }
    }
    if (ans) 
        printf("Correct\n");
    else
        printf("Wrong\n");
    return 0;
}