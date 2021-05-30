#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int left, right;
} parameters;

int a[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int ans[10];

void merge_sort(int *nums, int l, int r) {
    if (l >= r) return;
    int mid = (l + r) / 2;
    int tmp[10];
    merge_sort(nums, l, mid);
    merge_sort(nums, mid + 1, r);
    int i = l, j = mid + 1, cnt = 0;
    while (i <= mid && j <= r) {
        if (nums[i] < nums[j]) tmp[cnt++] = nums[i++];
        else tmp[cnt++] = nums[j++];
    }
    while (i <= mid) tmp[cnt++] = nums[i++];
    while (j <= r) tmp[cnt++] = nums[j++];
    for (int i = 0; i < cnt; i++) {
        nums[l + i] = tmp[i];
    }
}

void *thread(void *args) {
    int left = ((parameters *)args)->left;
    int right = ((parameters *)args)->right;
    merge_sort(a, left, right);
    free(args);
}

int main() {
    pthread_t first, second;

    parameters *first_para = (parameters *) malloc(sizeof(parameters));
    first_para->left = 0;
    first_para->right = 4;
    pthread_create(&first, NULL, thread, first_para);

    parameters *second_para = (parameters *) malloc(sizeof(parameters));
    second_para->left = 5;
    second_para->right = 9;
    pthread_create(&second, NULL, thread, second_para);

    pthread_join(first, NULL);
    pthread_join(second, NULL);

    int i = 0, j = 5, ptr = 0;
    while (i < 5 && j < 10) {
        if (a[i] < a[j]) {
            ans[ptr++] = a[i++];
        } else {
            ans[ptr++] = a[j++];
        }
    }
    while (i < 5)
        ans[ptr++] = a[i++];
    while (j < 10)
        ans[ptr++] = a[j++];
    for (int index = 0; index < 10; index++) {
        printf("%d ", ans[index]);
    }
    return 0;
}