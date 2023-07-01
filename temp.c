#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define DIM 1000000000

int main() {
    int* l = calloc(DIM, sizeof(int));
    int l_head = DIM/2;
    int l_tail = DIM/2;
    int len = 0;
    char command[20];
    int num;

    while(scanf("%s", command) > 0) {
        scanf("%d", &num);
         if(strcmp(command, "enqueue")==0) {
            l_head--;
            if(l_head < 0) {
                l_head = DIM - 1;
            }
            l[l_head] = num;
            len++;
        }
        else if(strcmp(command, "dequeue")==0) {
            int delted = l[l_tail];
            l[l_tail] = 0;
            l_tail--;
            if(l_tail < 0) {
                l_tail = DIM -1;
            }
            len--;
        }
    }
    printf("LEN:%d\n", len);
    if(l_head < l_tail) {
        for(int p=l_head; p<l_tail;p++) {
            printf("%d\n", l[p]);
        }
    }
    else if( l_head > l_tail) {
        for(int p=l_head;p<DIM;p++) {
            printf("%d\n",l[p]);
        }
        for(int p=0; p<l_tail;p++) {
            printf("%d\n",l[p]);
        }
    }
    return 0;
}