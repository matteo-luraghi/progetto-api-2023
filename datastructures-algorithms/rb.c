#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct nodo_rb {
    struct nodo_rb *p;
    struct nodo_rb *right;
    struct nodo_rb *left;
    char color;
    int key;
} nodo_t;

typedef struct albero_rb {
    nodo_t* root;
    nodo_t* nil;
    int max;
} albero_t;

//-------------------------------------------------------
// TO REMOVE BEFORE UPLOAD
#define COUNT 10
void print2DUtil(nodo_t* root, int space, nodo_t* NIL){
    // Base case
    if (root == NIL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    print2DUtil(root->right, space, NIL);
 
    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d - %c\n", root->key, root->color);
 
    // Process left child
    print2DUtil(root->left, space, NIL);
}
 
// Wrapper over print2DUtil()
void print2D(nodo_t* root, nodo_t* NIL){
    // Pass initial space count as 0
    print2DUtil(root, 0, NIL);
}
//-------------------------------------------------------

nodo_t* tree_search(albero_t* T, nodo_t* x, int key) {
    if(x == T->nil || key == x->key) {
        return x;
    }
    if(key < x->key) {
        return tree_search(T, x->left, key);
    }
    else {
        return tree_search(T, x->right, key);
    }
}
nodo_t* tree_min(albero_t* T, nodo_t* x) {
    while(x->left != T->nil) {
        x = x->left;
    }
    return x;
}

nodo_t* tree_successor(albero_t* T, nodo_t* x) {
    if(x->right != T->nil) {
        return tree_min(T, x->right);
    }
    nodo_t* y = x->p;
    while(y != T->nil && x == y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

void left_rotate(albero_t* T, nodo_t* x) {
    nodo_t* y = x->right;
    
    x->right = y->left;
    if(y->left != T->nil) {
        y->left->p = x;
    }
    y->p = x->p;
    if(x->p == T->nil) {
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

void right_rotate(albero_t* T, nodo_t* y) {
    nodo_t* x = y->left;
    y->left = x->right;
    if(x->right != T->nil) {
        x->right->p = y;
    }
    x->p = y->p;
    if(y->p == T->nil) {
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

void rb_insert_fixup(albero_t* T, nodo_t* z) {
    if(z == T->root) {
        T->root->color = 'B';
    }
    else{                     
        nodo_t* x = z->p;              
        if(x->color == 'R') {
            if(x == x->p->left) {           
                nodo_t* y = x->p->right;   
                if (y->color == 'R') {
                    x->color = 'B';             
                    y->color = 'B';             
                    x->p->color = 'R';          
                    rb_insert_fixup(T, x->p);   
                }
                else {
                    if(z == x->right)  {
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
                nodo_t* y = x->p->left;
                if (y->color == 'R') {
                    x->color = 'B';
                    y->color = 'B';
                    x->p->color = 'R';
                    rb_insert_fixup(T, x->p);
                }
                else {
                    if(z == x->left) {
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
}

void rb_insert(albero_t* T, int key) {
    nodo_t* z = malloc(sizeof(nodo_t));
    
    z->key = key;

    if(z->key > T->max) {
        T->max = z->key;
    }

    nodo_t* y = T->nil;
    nodo_t* x = T->root;

    while(x != T->nil) {
        y = x;
        if(z->key < x->key) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->p = y;
    if(y == T->nil) {
        T->root = z;
    }
    else if(z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    z->left = T->nil;
    z->right = T->nil;
    z->color = 'R';
    rb_insert_fixup(T, z);
}

void rb_delete_fixup(albero_t* T, nodo_t* x) {
    if (x->color == 'R' || x->p == T->nil) {
        x->color = 'B';
    }
    else if(x == x->p->left) {
        nodo_t* w = x->p->right;
        if(w->color == 'R') {
            w->color = 'B';
            x->p->color = 'R';
            left_rotate(T, x->p);
            w = x->p->right;
        }
        if(w->left->color == 'B' && w->right->color == 'B') {
            w->color = 'R';
            rb_delete_fixup(T, x->p);
        }
        else {
            if(w->right->color == 'B') {
                w->left->color = 'B';
                w->color = 'R';
                right_rotate(T, w);
                w = x->p->right;
            }
            w->color = x->p->color;
            x->p->color = 'B';
            w->right->color = 'B';
            left_rotate(T, x->p);
        }
    }
    else {
        nodo_t* w = x->p->left;
        if(w->color == 'R') {
            w->color = 'B';
            x->p->color = 'R';
            right_rotate(T, x->p);
            w = x->p->left;
        }
        if(w->right->color == 'B' && w->left->color == 'B') {
            w->color = 'R';
            rb_delete_fixup(T, x->p);
        }
        else {
            if(w->left->color == 'B') {
                w->right->color = 'B';
                w->color = 'R';
                left_rotate(T, w);
                w = x->p->left;
            }
            w->color = x->p->color;
            x->p->color = 'B';
            w->left->color = 'B';
            right_rotate(T, x->p);
        }
    }
}

nodo_t* rb_delete(albero_t* T, int key) {
    nodo_t* z = tree_search(T, T->root, key);
    nodo_t* x;
    nodo_t* y;
    if(z == T->nil) {
        return z;
    }

    if(z->left == T->nil || z->right == T->nil) {
        y = z;
    }
    else {
        y = tree_successor(T, z);
    }
    if(y->left != T->nil) {
        x = y->left;
    }
    else {
        x = y->right;
    }
    x->p = y->p;
    if(y->p == T->nil) {
        T->root = x;
    }
    else if(y == y->p->left) {
         y->p->left = x;        
    }
    else {
        y->p->right = x;
    }
    if(y != z) {
        z->key = y->key;
    }
    if(y->color == 'B') {
        rb_delete_fixup(T, x);
    }
    return y;
}

int main() {
    albero_t* T = malloc(sizeof(albero_t));
    T->nil = malloc(sizeof(nodo_t));
    T->nil->color = 'B';
    T->root = T->nil;
    T->max = 0;
    int key;
    int r;
    char command[100];

    do {
        printf("Inserisci key: ");
        r = scanf("%s %d",command, &key);
        r++;
        printf("%s\n", command);
        if(strcmp(command, "aggiungi")==0) {
            rb_insert(T, key);
        }
        else if(strcmp(command, "rimuovi")==0){
            rb_delete(T, key);
        }
        else {
            break;
        }
    } while(key != 0);
    print2D(T->root, T->nil);    
    return 0;
}