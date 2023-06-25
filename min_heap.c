#include<stdio.h>
#include<stdlib.h>

typedef struct min_heap {
    int* array;
    int size;
} min_heap_t;

void min_heapify(min_heap_t* A, int i) {
    int l = 2*i;
    int r = (2*i)+1;
    int min;
    if(l <= A->size && A->array[l] < A->array[i]) {
        min = l;
    }
    else {
        min = i;
    }
    if(r <= A->size && A->array[r] < A->array[min]) {
        min = r;
    }
    if(min != i) {
        int temp = A->array[i];
        A->array[i] = A->array[min];
        A->array[min] = temp;
        min_heapify(A, min);
    }
}

int heap_extract_min(min_heap_t* A) {
    if(A->size >= 1) {
        int min = A->array[1];
        A->array[1] = A->array[A->size];
        A->size--;
        min_heapify(A,1);
        return min;
    }
    return -1;
}

void heap_decrease_key(min_heap_t* A, int i, int key) {
    if(key <= A->array[i]) {
        A->array[i] = key;
        while(i > 1 && A->array[i/2] > A->array[i]) {
            int temp = A->array[i];
            A->array[i] = A->array[i/2];
            A->array[i/2] = temp;
            i = i/2;
        }
    }
}

void min_heap_insert(min_heap_t* A, int key) {
    A->size++;
    //A->array = realloc(A->array, (A->size)*sizeof(int));
    A->array[A->size] = 2147483647;
    heap_decrease_key(A, A->size, key);
}

void print_heap(min_heap_t* A) {
    for(int i=0; i<=A->size; i++) {
        printf("%d ", A->array[i]);
    } printf("\n");
}

int main() {
    min_heap_t* A = malloc(sizeof(min_heap_t));
    A->size = 0;
    //A->array = malloc(sizeof(int));
    A->array = malloc(100*sizeof(int));
    int x = -1;
    while(x != 0) {
        printf("Inserisci un numero: ");
        scanf("%d", &x);
        if(x%2==0) {
            min_heap_insert(A, x);
        }
        else {
            int y = heap_extract_min(A);
            printf("%d\n", y);
        }
        print_heap(A);
    }
    
    return 0;
}