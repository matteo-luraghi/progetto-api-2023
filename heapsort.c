#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void max_heapify(int* A, int i, int heap_size) {
    int l = 2*i;
    int r = (2*i)+1;
    int max;
    if(l<=heap_size && A[l] > A[i]) {
        max = l;
    }
    else {
        max = i;
    }
    if(r<=heap_size && A[r] > A[max]) {
        max = r;
    }
    if(max != i) {
        int temp = A[i];
        A[i] = A[max];
        A[max] = temp;
        max_heapify(A, max, heap_size);
    }
}

void build_max_heap(int* A, int len) {
    for(int i = len/2; i>=0; i--) {
        max_heapify(A, i, len);
    }
}

void heapsort(int* A, int len) {
    build_max_heap(A,len);
    int heap_size = len;
    for(int i=len; i>=1; i--) {
        int temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        heap_size--;
        max_heapify(A, 0, heap_size);
    }
}

int main() {
    int A[5];
    int len = 0;
    while(len != 5) {
        scanf("%d", &A[len]);
        len++;
    }

    heapsort(A, len-1);

    for(int i=0; i<5;i++) {
        printf("%d ", A[i]);
    } printf("\nEnd program\n");

    return 0;
}