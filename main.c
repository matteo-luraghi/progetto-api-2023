#include<stdio.h>
#include<stdlib.h>

typedef struct nodo_rb {
    struct nodo_rb *right;
    struct nodo_rb *left;
    struct nodo_rb *p;
    char color;
    int autonomia;
} macchina_t;

typedef struct nodo_rb* albero_macchine_t;

void stampa_albero(albero_macchine_t T) {
    if (T != NULL) {
        stampa_albero(T->left);
        printf("%d\n",T->autonomia);
        stampa_albero(T->right);
    }
}

void left_rotate(albero_macchine_t T, macchina_t* x) {
    macchina_t* y = x->right;
    x->right = y->left;
    if(y->left != NULL) {
        y->left->p = x;
    }

    y->p = x->p;
    if(x->p == NULL) {
        T = y;
    }
    else if(x == x->p->left) {
        x->p->left = y;
    }
    else {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;
}

void right_rotate(albero_macchine_t T, macchina_t* x) {
    macchina_t* y = x->left;
    x->left = y->right;
    if(y->right != NULL) {
        y->right->p = x;
    }

    y->p = x->p;
    if(x->p == NULL) {
        T = y;
    }
    else if(x == x->p->right) {
        x->p->right = y;
    }
    else {
        x->p->right = y;
    }
    y->right = x;
    x->p= y;
}

void rb_insert_fixup(albero_macchine_t T, macchina_t* z) {
    macchina_t* y;
    macchina_t* x;

    if(z == T) {
        T->color = 'B';
    }
    else {
        x = z->p;
        if(x->color == 'R') {
            if(x == x->p->left) {       
                y = x->p->right;
                if(y->color == 'R') {       
                    x->color = 'B';
                    y->color = 'B';
                    x->p->color = 'R';
                    rb_insert_fixup(T, x->p);
                }
                else if(z == x->right){
                
                    z = x;
                
                    left_rotate(T, z);
                
                    x = z->p;
                }
                
                x->color = 'B';
                
                x->p->color = 'R';
                
                right_rotate(T, x->p);
            }
        }
        else {
            if(x == x->p->right) {        
                y = x->p->left;
                if(y->color == 'R') {
                    x->color = 'B';
                    y->color = 'B';
                    x->p->color = 'R';
                    rb_insert_fixup(T, x->p);
                }
                else if(z == x->left){
                    z = x;
                    right_rotate(T, z);
                    x = z->p;
                }
                x->color = 'B';
                x->p->color = 'R';
                left_rotate(T, x->p);
            }
        }
    }
}

albero_macchine_t rb_insert(albero_macchine_t T, int autonomia) {
    macchina_t* z = malloc(sizeof(macchina_t));
    z->autonomia = autonomia;
    z->left = NULL;
    z->right = NULL;
    z->p = NULL;
    z->color = 'R';

    macchina_t* y = NULL;
    macchina_t* x = T;
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
        T = z;
    }
    else if(z->autonomia < y->autonomia) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    z->left = NULL;
    z->right = NULL;
    z->color = 'R';
    rb_insert_fixup(T, z);
    return T;
}

int main() {
    char input[1024];
    char* s;
    albero_macchine_t T = NULL;
    int autonomia;

    do {
        printf("Inserisci autonomia: ");
        scanf("%d", &autonomia);
        T = rb_insert(T, autonomia);
        stampa_albero(T);
    } while(autonomia != 0);
    
    return 0;
    /*
    while (1) {
        if (fgets(input, 1024, stdin) != NULL) {
            for (s = input; (*s != '\n') && *s == ' '; s++){}; 
            if (*s == '\n')
                break;
            else
                printf("%s\n", input);
        }
    }
    return 0;*/
}