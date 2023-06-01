#include<stdio.h>
#include<stdlib.h>
#define COMMAND_LENGTH 1024
//----------------------------------------------------
// BST DATA STRUCTURE AND FUNCTIONS
typedef struct nodo_rb {
    struct nodo_rb *right;
    struct nodo_rb *left;
    struct nodo_rb *p;
    int autonomia;
} macchina_t;

typedef struct albero_rb {
    macchina_t* root;
    int max;
} albero_t;

macchina_t* tree_search(macchina_t* x, int autonomia) {
    if(x == NULL || autonomia == x->autonomia) {
        return x;
    }
    if(autonomia < x->autonomia) {
        return tree_search(x->left, autonomia);
    }
    else {
        return tree_search(x->right, autonomia);
    }
}

void tree_insert(albero_t* T, int autonomia) {
    if(autonomia > T->max) {
        T->max = autonomia;
    }

    macchina_t* z = malloc(sizeof(macchina_t));
    z->autonomia = autonomia;
    z->left = NULL;
    z->right = NULL;
    z->p = NULL;

    macchina_t* y = NULL;
    macchina_t* x = T->root;

    while(x != NULL) {
        y = x;
        if(z->autonomia < x->autonomia) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->p = y;
    if(y == NULL) {
        T->root = z;
    }
    else if(z->autonomia < y->autonomia) {
        y->left = z;
    }
    else {
        y->right = z;
    }
}

macchina_t* tree_min(macchina_t* x) {
    while(x->left != NULL) {
        x = x->left;
    }
    return x;
}

macchina_t* tree_successor(macchina_t* x) {
    if(x->right != NULL) {
        return tree_min(x->right);
    }
    macchina_t* y = x->p;
    while(y != NULL && x == y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

void tree_delete(albero_t* T, macchina_t* z) {
    macchina_t* y;
    macchina_t* x;
    if(z->left == NULL || z->right == NULL) {
        y = z;
    } else {
        y = tree_successor(z);
    }
    if(y->left != NULL) {
        x = y->left;
    }
    else {
        x = y->right;
    }
    if(x != NULL) {
        x->p = y->p;
    }
    if(y->p == NULL) {
        T->root = x;
    } else if(y == y->p->left) {
        y->p->left = x;
    } else {
        y->p->right = x;
    }
    if(y != z) {
        z->autonomia = y->autonomia;
    }
    free(y);
}
//----------------------------------------------------

typedef struct command_data{
    int stazione;
    int data_num;
    int data[data_num];
} command_data;

int string_compare(char a[], char b[], int len) {
    int i;
    for(i=0; i<len; i++) {
        if(a[i] != b[i]) { 
            return 0;
        }
    }
    return 1;
}

int string_len(char a[]) {
    int i=0;
    while(a[i] != '\0') {
        i++;
    }
    return i+1;
}

int main() {
    char command[COMMAND_LENGTH], station_distance_str[10], command_number_str[10];
    int i, j, command_number, station_distance;
    albero_t* T = malloc(sizeof(albero_t));
    T->root = NULL;
    T->max = 0;
    int autonomia;

    while (1) {
        int nope = 0;
        scanf("%[^\n]%*c", command);
        if(string_compare(command, "aggiungi-auto", 12) == 1) {
            if(command[13] != ' ' || command[14] == ' ') {    //solo alcuni dei casi possibili per non accettare l'input, da vedere se l'input è sempre corretto
                nope = 1;
            }
            else {
                int count = 0;
                for(i = 14; i<COMMAND_LENGTH && command[i] != ' '; i++) {
                    station_distance_str[count] = command[i];
                    count++;
                }
                
                station_distance = atoi(station_distance_str);
                
                count = 0;
                for(j = i + 1; j<COMMAND_LENGTH && command[j] != ' '; j++) {
                    command_number_str[count] = command[j];
                    count++;
                }
                command_number = atoi(command_number_str);

                int macchine[command_number];
                int k = 0;
                count = 0;
                char data_str[10];
                for(i = j + 1; i<COMMAND_LENGTH && count < command_number; i++) {
                    if(command[i] != ' ') {
                        data_str[k] = command[i];
                        k++;
                    }
                    else {
                        if(atoi(data_str) == 0) {
                            nope = 1;
                            break;
                        }
                        macchine[count] = atoi(data_str);
                        for(j = 0; j < k+1; j++) {
                            data_str[j] = ' ';
                        }
                        k = 0;
                        count++;     
                    }
                }
                //
                for(i=0; i<command_number; i++) {
                    printf("%d ", macchine[i]);
                } printf("\n");

                if(count != command_number) {
                    nope = 1;
                }
                if(nope == 1) {
                    printf("non aggiunta\n");
                }
                else {
                    printf("aggiunta\n");
                }
            }      
        }
        if(string_compare(command, "rottama-auto", 11) == 1) {
            if(command[12] != ' ' || command[13] == ' ') {
                nope = 1;
            }
            else {
                int count = 0;
                for(i = 13; i < COMMAND_LENGTH; )
            }
        }
        nope = 0;
    }
    return 0;
}