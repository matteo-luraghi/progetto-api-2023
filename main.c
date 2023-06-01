#include<stdio.h>
#include<stdlib.h>

typedef struct nodo_rb {
    struct nodo_rb *right;
    struct nodo_rb *left;
    struct nodo_rb *p;
    char color;
    int autonomia;
} macchina_t;

typedef struct albero_rb {
    macchina_t* root;
    int max;
} albero_t;

//-------------------------------------------------------
// TO REMOVE BEFORE UPLOAD
#define COUNT 10
void print2DUtil(macchina_t* root, int space){
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    print2DUtil(root->right, space);
 
    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d - %c\n", root->autonomia, root->color);
 
    // Process left child
    print2DUtil(root->left, space);
}
 
// Wrapper over print2DUtil()
void print2D(macchina_t* root){
    // Pass initial space count as 0
    print2DUtil(root, 0);
}
//-------------------------------------------------------

void left_rotate(albero_t* T, macchina_t* x) {
    macchina_t* y = x->right;
    x->right = y->left;
    if(y->left != NULL) {
        y->left->p = x;
    }
    y->p = x->p;
    if(x->p == NULL) {
        T->root = y;
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

void right_rotate(albero_t* T, macchina_t* y) {
    macchina_t* x = y->left;
    y->left = x->right;
    if(x->right != NULL) {
        x->right->p = y;
    }
    x->p = y->p;
    if(y->p == NULL) {
        T->root = x;
    }
    else if(y == y->p->left) {
        y->p->left = x;
    }
    else {
        y->p->right = x;
    }
    x->right = y;
    y->p = x;
}

void rb_insert_fixup(albero_t* T, macchina_t* x) {
    macchina_t* parent_pt = NULL;
    macchina_t* grand_parent_pt = NULL;

    while((x != T->root) && (x->color != 'B') && (x->p->color == 'R')) {
        parent_pt = x->p;
        grand_parent_pt = x->p->p;

        if(parent_pt == grand_parent_pt->left) {
            macchina_t* uncle_pt = grand_parent_pt->right;
            if(uncle_pt != NULL && uncle_pt->color == 1) {
                grand_parent_pt->color = 'R';
                parent_pt->color = 'B';
                uncle_pt->color = 'B';
                x = grand_parent_pt;
            }
            else {
                if(x == parent_pt->right) {
                    left_rotate(T, parent_pt);
                    x = parent_pt;
                    parent_pt = x->p;
                }

                right_rotate(T, grand_parent_pt);
                char temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                x = parent_pt;
            }
        }

        else {
            macchina_t* uncle_pt = grand_parent_pt->left;
            if((uncle_pt != NULL) && (uncle_pt->color == 'R')) {
                grand_parent_pt->color = 'R';
                parent_pt->color = 'B';
                uncle_pt->color = 'B';
                x = grand_parent_pt;
            }
            else {
                if(x == parent_pt->l) {
                    right_rotate(parent_pt);
                    x = parent_pt;
                    parent_pt = x->p;
                }

                left_rotate(grand_parent_pt);
                char temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                x = parent_pt;
            }
        }
    }
}

void rb_insert(albero_t* T, int autonomia) {
    macchina_t* z = malloc(sizeof(macchina_t));
    z->autonomia = autonomia;
    z->left = NULL;
    z->right = NULL;
    z->p = NULL;
    z->color = 'R';

    if(z->autonomia > T->max) {
        T->max = z->autonomia;
    }

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
    rb_insert_fixup(T, z);
}

int main() {
    //char input[1024];
    //char* s;
    albero_t* T = malloc(sizeof(albero_t));
    T->root = NULL;
    T->max = 0;
    int autonomia;

    do {
        printf("Inserisci autonomia: ");
        scanf("%d", &autonomia);
        rb_insert(T, autonomia);
        print2D(T->root);
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