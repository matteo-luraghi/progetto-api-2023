#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quick_sort(int* A, int p, int r) {
    if(p < r) {
        int x = A[r];
        int i = p - 1;
        int temp;
        for(int j=p; j<r;j++) {
            if(A[j] >= x) {
                i++;
                temp = A[i];
                A[i] = A[j];
                A[j] = temp;
            }
        }
        temp = A[i+1];
        A[i+1] = A[r];
        A[r] = temp;

        int q = i+1;

        quick_sort(A, p, q-1);
        quick_sort(A, q+1, r);
    }
}

int main() {
    int a[10];
    int i = 0;
    while(i<10) {
        scanf("%d", &a[i]);
        i++;
    }
    quick_sort(a, 0, 9);
    for(i=0; i<10;i++) {
        printf("%d ", a[i]);
    }printf("\n");
    return 0;
}