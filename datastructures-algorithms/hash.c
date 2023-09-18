#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 60000
#define HASH 59989

typedef struct item {
    int key;
    int data;
} item_t;

typedef struct nodo_albero {
    struct nodo_albero *right;
    struct nodo_albero *left;
    struct nodo_albero *p;
    int data;
    //item_t* item;
    char color;
} nodo_albero_t;

typedef struct albero {
    nodo_albero_t* root;                
    nodo_albero_t* nil;
} albero_t;

typedef struct HT {
    item_t** items;
    int size;
    int count;
} HT_t;

nodo_albero_t* tree_search(albero_t* T, nodo_albero_t* x, int data) {
    if(x == T->nil || data == x->data) {
        return x;
    }
    if(data < x->data) {
        return tree_search(T, x->left, data);
    }
    else {
        return tree_search(T, x->right, data);
    }
}

nodo_albero_t* tree_min(albero_t* T, nodo_albero_t* x) {
    while(x->left != T->nil) {
        x = x->left;
    }
    return x;
}

nodo_albero_t* tree_max(albero_t* T, nodo_albero_t* x) {
    while(x->right != T->nil) {
        x = x->right;
    }
    return x;
}

nodo_albero_t* tree_successor(albero_t* T, nodo_albero_t* x) {
    if(x->right != T->nil) {
        return tree_min(T, x->right);
    }
    nodo_albero_t* y = x->p;
    while(y != T->nil && x == y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

void tree_left_rotate(albero_t* T, nodo_albero_t* x) {
    nodo_albero_t* y = x->right;
    
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

void tree_right_rotate(albero_t* T, nodo_albero_t* y) {
    nodo_albero_t* x = y->left;
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

void rb_insert_fixup(albero_t* T, nodo_albero_t* z) {
    if(z == T->root) {
        T->root->color = 'B';
    }
    else{                     
        nodo_albero_t* x = z->p;              
        if(x->color == 'R') {
            if(x == x->p->left) {           
                nodo_albero_t* y = x->p->right;   
                if (y->color == 'R') {
                    x->color = 'B';             
                    y->color = 'B';             
                    x->p->color = 'R';          
                    rb_insert_fixup(T, x->p);   
                }
                else {
                    if(z == x->right)  {
                        z = x;                      
                        tree_left_rotate(T, z); 
                        x = z->p;           
                    }
                    x->color = 'B';                  
                    x->p->color = 'R';              
                    tree_right_rotate(T, x->p);          
                }
            }
            else {
                nodo_albero_t* y = x->p->left;
                if (y->color == 'R') {
                    x->color = 'B';
                    y->color = 'B';
                    x->p->color = 'R';
                    rb_insert_fixup(T, x->p);
                }
                else {
                    if(z == x->left) {
                        z = x;
                        tree_right_rotate(T, z);
                        x = z->p;
                    }  
                    x->color = 'B';
                    x->p->color = 'R';
                    tree_left_rotate(T, x->p);
                }
            }
        }
    }
}

void tree_insert(albero_t* T, int data) {

    nodo_albero_t* z = malloc(sizeof(nodo_albero_t));
    z->data = data;

    nodo_albero_t* y = T->nil; 
    nodo_albero_t* x = T->root;

    while(x != T->nil) {
        y = x;
        if(z->data < x->data) {
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
    else if(z->data < y->data) {
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

void rb_delete_fixup(albero_t* T, nodo_albero_t* x) {
    if (x->color == 'R' || x->p == T->nil) {
        x->color = 'B';
    }
    else if(x == x->p->left) {
        nodo_albero_t* w = x->p->right;
        if(w->color == 'R') {
            w->color = 'B';
            x->p->color = 'R';
            tree_left_rotate(T, x->p);
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
                tree_right_rotate(T, w);
                w = x->p->right;
            }
            w->color = x->p->color;
            x->p->color = 'B';
            w->right->color = 'B';
            tree_left_rotate(T, x->p);
        }
    }
    else {
        nodo_albero_t* w = x->p->left;
        if(w->color == 'R') {
            w->color = 'B';
            x->p->color = 'R';
            tree_right_rotate(T, x->p);
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
                tree_left_rotate(T, w);
                w = x->p->left;
            }
            w->color = x->p->color;
            x->p->color = 'B';
            w->left->color = 'B';
           tree_right_rotate(T, x->p);
        }
    }
}

nodo_albero_t* rb_delete(albero_t* T, int data) {
    nodo_albero_t* z = tree_search(T, T->root, data);
    nodo_albero_t* x;
    nodo_albero_t* y;

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
        z->data = y->data;
    }
    if(y->color == 'B') {
        rb_delete_fixup(T, x);
    }
    return y;
}

int hash_function(int key) {
    return key % HASH;
}

item_t* ht_search(HT_t* table, int key) {
    int index = hash_function(key);
    while(table->items[index] != NULL) {
        if(table->items[index]->key == key) {
            return table->items[index];
        }
        index++;
        index %= SIZE;
    }
    return NULL;
}

void ht_insert(HT_t* table, int key, int data) {
    item_t* item = malloc(sizeof(item_t));
    item->data = data;
    item->key = key;

    int index = hash_function(key);
    
    while(table->items[index] != NULL && table->items[index]->key != -1) {
        index++;
        index %= SIZE;
    }
    table->items[index] = item;
    table->count++;
}

item_t* ht_delete(HT_t* table, item_t* item) {
    int index = hash_function(item->key);
    
    while(table->items[index] != NULL) {
        if(table->items[index]->key == item->key) {
            item_t* temp = table->items[index];
            //write as deleted
            table->items[index]->key = -1;
            table->items[index]->data = -1;
            table->count--;
            return temp;
        }
        index++;
        index %= SIZE;
    }
    return NULL;
}

void display(HT_t* table) {
    int i;
    for(i = 0; i<SIZE; i++) {
        if(table->items[i] != NULL) {
            printf("(%d, %d) ", table->items[i]->key, table->items[i]->data);
        }
        else {
            printf(" ## ");
        }
    }
    printf("\nItems:%d\n", table->count);
}

int main() {
    HT_t* table = malloc(sizeof(HT_t));
    table->size = SIZE;
    table->count = 0;
    table->items = malloc(SIZE*sizeof(item_t));
    for(int i=0;i<SIZE;i++) {
        table->items[i] = NULL;
    }

    int key;
    int r;
    char command[100];
    do {
        printf("Inserisci key: ");
        r = scanf("%s %d",command, &key);
        r++;
        printf("%s\n", command);
        if(strcmp(command, "aggiungi")==0) {
            ht_insert(table, key, key);
        }
        else if(strcmp(command, "rimuovi")==0){
            item_t* deleted = ht_search(table, key);
            if(deleted != NULL) {
                ht_delete(table, deleted);
            }
        }
        else {
            break;
        }
    } while(key != 0);
    display(table);
    return 0;
}