#include<stdio.h>
#include<stdlib.h>
#define COMMAND_LENGTH 1000
#define AGGIUNGI_AUTO '1'
#define ROTTAMA_AUTO '2'
#define AGGIUNGI_STAZIONE '3'
#define DEMOLISCI_STAZIONE '4'
//----------------------------------------------------
//DATA STRUCTURE AND FUNCTIONS
typedef struct nodo_lista {
    int el;
    struct nodo_lista *next;
    struct nodo_lista *prev;
} nodo_lista_t;

typedef struct lista {
    struct nodo_lista* head;
    struct nodo_lista* tail;
    struct lista* next;
} lista_t;

typedef struct lista_liste {
    lista_t* head;
} lista_liste_t;

typedef struct nodo_albero {
    struct nodo_albero *right;
    struct nodo_albero *left;
    struct nodo_albero *p;
    int data;
    char color;
} nodo_albero_t;

typedef struct albero {
    nodo_albero_t* root;                //albero delle macchine
    nodo_albero_t* nil;
    struct albero* nodi_ragg;    //albero dei nodi raggiungibili
    int max;
    int distanza;
} stazione_t;

typedef struct nodo_grafo {
    struct nodo_grafo *right;
    struct nodo_grafo *left;
    struct nodo_grafo *p;
    stazione_t* stazione;
    char color;
    char visited;
} nodo_grafo_t;

typedef struct grafo {
    nodo_grafo_t* root;  
    nodo_grafo_t* nil;     
} grafo_t;

void list_insert_head(lista_t* l, int el) {
    nodo_lista_t* temp = malloc(sizeof(nodo_lista_t));
    temp->el = el;
    temp->next = l->head;
    temp->prev = NULL;
    l->head = temp;
    if(l->tail == NULL) {
        l->tail = temp;
    }
}

void list_insert_tail(lista_t* l, int el) {
    if(l->head == NULL) {
        list_insert_head(l, el);
    }
    else {
        nodo_lista_t* temp = malloc(sizeof(nodo_lista_t));
        temp->el = el;
        temp->next = NULL;
        temp->prev = l->tail;
        temp->prev->next = temp;
        l->tail = temp;
    }
}

nodo_lista_t* dequeue(lista_t* l) {
    if(l == NULL) {
        return NULL;
    }
    nodo_lista_t* front = l->head;
    l->head = l->head->next;
    if(l->head == NULL) {
        l->tail = NULL;
    }
    return front;
}

void print_list(nodo_lista_t* x) {
    while (x != NULL) {
        if(x->next != NULL) {
            printf("%d ", x->el);
        }
        else {
            printf("%d", x->el);
        }
        x = x->next;
    } printf("\n");
}

void print_list_backwards(nodo_lista_t* x) {
    while(x != NULL) {
        if(x->prev != NULL) {
            printf("%d ", x->el);
        }
        else {
            printf("%d", x->el);
        }
        x = x->prev;
    } printf("\n");
}

void list_list_insert_head(lista_liste_t* l, lista_t* el) {
    el->next = l->head;
    l->head = el;
}

lista_t* copy_list(nodo_lista_t* curr) {
    lista_t* copied = malloc(sizeof(lista_t));
    copied->head = NULL;
    copied->tail = NULL;
    while(curr != NULL) {
        list_insert_tail(copied, curr->el);
        curr = curr->next;
    }
    return copied;
}

int list_length(nodo_lista_t* x) {
    int count = 0;
    while( x != NULL) {
        count++;
        x = x->next;
    }
    return count;
}

nodo_albero_t* tree_search(stazione_t* T, nodo_albero_t* x, int data) {
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

nodo_albero_t* tree_min(stazione_t* T, nodo_albero_t* x) {
    while(x->left != T->nil) {
        x = x->left;
    }
    return x;
}

nodo_albero_t* tree_max(stazione_t* T, nodo_albero_t* x) {
    while(x->right != T->nil) {
        x = x->right;
    }
    return x;
}

nodo_albero_t* tree_successor(stazione_t* T, nodo_albero_t* x) {
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

void tree_left_rotate(stazione_t* T, nodo_albero_t* x) {
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

void tree_right_rotate(stazione_t* T, nodo_albero_t* y) {
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

void graph_left_rotate(grafo_t* GRAPH, nodo_grafo_t* x) {
    nodo_grafo_t* y = x->right;
    
    x->right = y->left;
    if(y->left != GRAPH->nil) {
        y->left->p = x;
    }
    y->p = x->p;
    if(x->p == GRAPH->nil) {
        GRAPH->root = y;
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

void graph_right_rotate(grafo_t* GRAPH, nodo_grafo_t* y) {
    nodo_grafo_t* x = y->left;
    y->left = x->right;
    if(x->right != GRAPH->nil) {
        x->right->p = y;
    }
    x->p = y->p;
    if(y->p == GRAPH->nil) {
        GRAPH->root = x;
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

void rb_insert_fixup(stazione_t* T, nodo_albero_t* z) {
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

void tree_insert(stazione_t* T, int data) {
    if(data > T->max) {
        T->max = data;
    }

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

void rb_delete_fixup(stazione_t* T, nodo_albero_t* x) {
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

nodo_albero_t* rb_delete(stazione_t* T, int data) {
    nodo_albero_t* z = tree_search(T, T->root, data);
    nodo_albero_t* x;
    nodo_albero_t* y;

    if(z == T->nil) {
        return z;
    }

    //updating the max
    if(data == T->max) {
        if(z->left == T->nil) {
            T->max = z->p->data;    
        }
        else {
            T->max = z->left->data;
        }
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

void graph_find_reachable(nodo_grafo_t* curr, nodo_grafo_t* modified, nodo_grafo_t* NIL, char command) {
    if(curr->left != NIL) {  
        graph_find_reachable(curr->left, modified, NIL, command);
    }
    int distanza_modulo = 0;
        
    if(command == AGGIUNGI_STAZIONE || command == AGGIUNGI_AUTO){

        if(curr->stazione->distanza > modified->stazione->distanza) {
            distanza_modulo = curr->stazione->distanza - modified->stazione->distanza;
        }

        else if(modified->stazione->distanza > curr->stazione->distanza) {
            distanza_modulo = modified->stazione->distanza - curr->stazione->distanza;
        }

        if(distanza_modulo != 0 && distanza_modulo <= modified->stazione->max && tree_search(modified->stazione->nodi_ragg, modified->stazione->nodi_ragg->root, curr->stazione->distanza)==modified->stazione->nodi_ragg->nil) {
            tree_insert(modified->stazione->nodi_ragg, curr->stazione->distanza);
        }
        if(distanza_modulo != 0 && distanza_modulo <= curr->stazione->max && tree_search(curr->stazione->nodi_ragg, curr->stazione->nodi_ragg->root, modified->stazione->distanza)==curr->stazione->nodi_ragg->nil) {
            tree_insert(curr->stazione->nodi_ragg, modified->stazione->distanza);
        }
    }
      
    else if(command == ROTTAMA_AUTO) {
        if(curr->stazione->distanza > modified->stazione->distanza) {
            distanza_modulo = curr->stazione->distanza - modified->stazione->distanza;
        } 
        else if(modified->stazione->distanza > curr->stazione->distanza) {
            distanza_modulo = modified->stazione->distanza - curr->stazione->distanza;
        }
        
        if(distanza_modulo != 0 && distanza_modulo > modified->stazione->max) {
            rb_delete(modified->stazione->nodi_ragg, curr->stazione->distanza);
        }
        if(distanza_modulo != 0 && distanza_modulo > curr->stazione->max) {
            rb_delete(curr->stazione->nodi_ragg, modified->stazione->distanza);
        }
    } 

    if(curr->right != NIL) {        
        graph_find_reachable(curr->right, modified, NIL, command);
    }
}

void graph_find_reachable_demolished(nodo_grafo_t* curr, int distanza, nodo_grafo_t* NIL) {
    if(curr->left != NIL) {  
        graph_find_reachable_demolished(curr->left, distanza, NIL);
    }
    if(tree_search(curr->stazione->nodi_ragg, curr->stazione->nodi_ragg->root, distanza) != curr->stazione->nodi_ragg->nil) {
        rb_delete(curr->stazione->nodi_ragg, distanza);
    } 
    if(curr->right != NIL) {  
        graph_find_reachable_demolished(curr->right, distanza, NIL);
    }
}

nodo_grafo_t* graph_search(grafo_t* GRAPH, nodo_grafo_t* x, int stazione) {
    if(x == GRAPH->nil || stazione == x->stazione->distanza) {
        return x;
    }
    if(stazione < x->stazione->distanza) {
        return graph_search(GRAPH, x->left, stazione);
    }
    else {
        return graph_search(GRAPH, x->right, stazione);
    }
}

nodo_grafo_t* graph_min(grafo_t* GRAPH, nodo_grafo_t* x) {
    while(x->left != GRAPH->nil) {
        x = x->left;
    }
    return x;
}

nodo_grafo_t* graph_max(grafo_t* GRAPH, nodo_grafo_t* x) {
    while(x->right != GRAPH->nil) {
        x = x->right;
    }
    return x;
}

nodo_grafo_t* graph_successor(grafo_t* GRAPH, nodo_grafo_t* x) {
    if(x->right != GRAPH->nil) {
        return graph_min(GRAPH, x->right);
    }
    nodo_grafo_t* y = x->p;
    while(y != GRAPH->nil && x == y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

void graph_insert_fixup(grafo_t* GRAPH, nodo_grafo_t* z) {
    if(z == GRAPH->root) {
        GRAPH->root->color = 'B';
    }
    else{                     
        nodo_grafo_t* x = z->p;              
        if(x->color == 'R') {
            if(x == x->p->left) {           
                nodo_grafo_t* y = x->p->right;   
                if (y->color == 'R') {
                    x->color = 'B';             
                    y->color = 'B';             
                    x->p->color = 'R';          
                    graph_insert_fixup(GRAPH, x->p);   
                }
                else {
                    if(z == x->right)  {
                        z = x;                      
                        graph_left_rotate(GRAPH, z); 
                        x = z->p;           
                    }
                    x->color = 'B';                  
                    x->p->color = 'R';              
                    graph_right_rotate(GRAPH, x->p);          
                }
            }
            else {
                nodo_grafo_t* y = x->p->left;
                if (y->color == 'R') {
                    x->color = 'B';
                    y->color = 'B';
                    x->p->color = 'R';
                    graph_insert_fixup(GRAPH, x->p);
                }
                else {
                    if(z == x->left) {
                        z = x;
                        graph_right_rotate(GRAPH, z);
                        x = z->p;
                    }  
                    x->color = 'B';
                    x->p->color = 'R';
                    graph_left_rotate(GRAPH, x->p);
                }
            }
        }
    }
}

void graph_insert(grafo_t* GRAPH, int stazione) {
    nodo_grafo_t* z = malloc(sizeof(nodo_grafo_t));
    z->stazione = malloc(sizeof(stazione_t));
    z->stazione->nil = malloc(sizeof(nodo_albero_t));
    z->stazione->nil->color = 'B';
    z->stazione->root = z->stazione->nil;
    z->stazione->distanza = stazione;
    z->stazione->max = 0;
    z->stazione->nodi_ragg = malloc(sizeof(stazione_t));
    z->stazione->nodi_ragg->nil = malloc(sizeof(nodo_albero_t));
    z->stazione->nodi_ragg->nil->data = -1;
    z->stazione->nodi_ragg->nil->color = 'B';
    z->stazione->nodi_ragg->root = z->stazione->nodi_ragg->nil;

    nodo_grafo_t* y = GRAPH->nil;
    nodo_grafo_t* x = GRAPH->root;

    while(x != GRAPH->nil) {
        y = x;
        if(z->stazione->distanza < x->stazione->distanza) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->p = y;
    if(y == GRAPH->nil) {
        GRAPH->root = z;
    }
    else if(z->stazione->distanza < y->stazione->distanza) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    z->left = GRAPH->nil;
    z->right = GRAPH->nil;
    z->color = 'R';
    graph_insert_fixup(GRAPH, z);
}

void graph_delete_fixup(grafo_t* T, nodo_grafo_t* x) {
    if (x->color == 'R' || x->p == T->nil) {
        x->color = 'B';
    }
    else if(x == x->p->left) {
        nodo_grafo_t* w = x->p->right;
        if(w->color == 'R') {
            w->color = 'B';
            x->p->color = 'R';
            graph_left_rotate(T, x->p);
            w = x->p->right;
        }
        if(w->left->color == 'B' && w->right->color == 'B') {
            w->color = 'R';
            graph_delete_fixup(T, x->p);
        }
        else {
            if(w->right->color == 'B') {
                w->left->color = 'B';
                w->color = 'R';
                graph_right_rotate(T, w);
                w = x->p->right;
            }
            w->color = x->p->color;
            x->p->color = 'B';
            w->right->color = 'B';
            graph_left_rotate(T, x->p);
        }
    }
    else {
        nodo_grafo_t* w = x->p->left;
        if(w->color == 'R') {
            w->color = 'B';
            x->p->color = 'R';
            graph_right_rotate(T, x->p);
            w = x->p->left;
        }
        if(w->right->color == 'B' && w->left->color == 'B') {
            w->color = 'R';
            graph_delete_fixup(T, x->p);
        }
        else {
            if(w->left->color == 'B') {
                w->right->color = 'B';
                w->color = 'R';
                graph_left_rotate(T, w);
                w = x->p->left;
            }
            w->color = x->p->color;
            x->p->color = 'B';
            w->left->color = 'B';
            graph_right_rotate(T, x->p);
        }
    }
}

nodo_grafo_t* graph_delete(grafo_t* T, int distanza) {
    nodo_grafo_t* z = graph_search(T, T->root, distanza);
    nodo_grafo_t* x;
    nodo_grafo_t* y;
    if(z == T->nil) {
        return z;
    }

    if(z->left == T->nil || z->right == T->nil) {
        y = z;
    }
    else {
        y = graph_successor(T, z);
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
        z->stazione = y->stazione;
    }
    if(y->color == 'B') {
        graph_delete_fixup(T, x);
    }
    return y;
}

//-------------------------------------------------------
// TO REMOVE BEFORE UPLOAD
#define COUNT 10
void print2DUtilTree(nodo_albero_t* root, int space, nodo_albero_t* NIL){
    if (root == NIL)
        return;
    space += COUNT;
    print2DUtilTree(root->right, space, NIL);
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d - %c\n", root->data, root->color);
    print2DUtilTree(root->left, space, NIL);
}
 
// Wrapper over print2DUtil()
void print2DTree(nodo_albero_t* root, nodo_albero_t* NIL){
    // Pass initial space count as 0
    print2DUtilTree(root, 0, NIL);
}
///////////////////////////////////////////////////////////////
void print2DUtilGraph(nodo_grafo_t* root, int space, nodo_grafo_t* NIL){
    if (root == NIL)
        return;
    space += COUNT;
    print2DUtilGraph(root->right, space, NIL);
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d - %c\n", root->stazione->distanza, root->color);
    print2DUtilGraph(root->left, space, NIL);
}
 
// Wrapper over print2DUtilGraph()
void print2DGRAPH(nodo_grafo_t* root, nodo_grafo_t* NIL){
    // Pass initial space count as 0
    print2DUtilGraph(root, 0, NIL);
}

void graph_walk_print(nodo_grafo_t* x, nodo_grafo_t* NIL) {
    if(x != NIL) {
        graph_walk_print(x->left, NIL);
        printf("Albero: %d, max: %d\n", x->stazione->distanza, x->stazione->max);
        printf("Macchine:\n");
        print2DTree(x->stazione->root, x->stazione->nil);
        printf("Nodi Ragg:\n");
        print2DTree(x->stazione->nodi_ragg->root, x->stazione->nodi_ragg->nil);
        graph_walk_print(x->right, NIL);
    }
}
//-------------------------------------------------------

//----------------------------------------------------

char string_compare(char a[], char b[], int len) {
    int i;
    for(i=0; i<len; i++) {
        if(a[i] != b[i]) { 
            return '0';
        }
    }
    return '1';
}

int string_len(char a[]) {
    int i=0;
    while(a[i] != '\0') {
        i++;
    }
    return i+1;
}

int update_graph(grafo_t* GRAPH, char command_text[], char command, int command_len) {
    int i, j, k = 0, count = 0;
    char station_distance_str[10], command_number_str[10];
    int len = string_len(command_text);
    
    for(i = command_len; i < len && command_text[i] != ' '; i++) {
        station_distance_str[count] = command_text[i];
        count++;
    }

    int station_distance = atoi(station_distance_str);

    if(command == AGGIUNGI_STAZIONE) {
        if(graph_search(GRAPH, GRAPH->root, station_distance) == GRAPH->nil) {
            graph_insert(GRAPH, station_distance);
        }
        else {
            printf("non aggiunta\n");
            return 0;
        }
    }
    
    else if(command == DEMOLISCI_STAZIONE) {
        if(graph_search(GRAPH, GRAPH->root, station_distance) != GRAPH->nil) {
            nodo_grafo_t* deleted = graph_delete(GRAPH, station_distance);
            free(deleted);
            graph_find_reachable_demolished(GRAPH->root, station_distance, GRAPH->nil);
            printf("demolita\n");
        }
        else {
            printf("non demolita\n");
        }
        return 0;
    } 

    count = 0;
    for(j = i + 1 ; j < len && command_text[j] != ' '; j++) {
        command_number_str[count] = command_text[j];
        count++;
    }

    int command_number = atoi(command_number_str);

    if(command == ROTTAMA_AUTO) {
        nodo_grafo_t* current_station = graph_search(GRAPH, GRAPH->root, station_distance);
        if(current_station != GRAPH->nil) {
            int prev_max = current_station->stazione->max;
            if(tree_search(current_station->stazione, current_station->stazione->root, command_number) != current_station->stazione->nil) {
                nodo_albero_t* deleted_auto = rb_delete(current_station->stazione, command_number);
                if(deleted_auto->data == prev_max) {
                    graph_find_reachable(GRAPH->root, current_station, GRAPH->nil, ROTTAMA_AUTO);
                }
                free(deleted_auto);
                printf("rottamata\n");
                return 0;
            }    
        }
        printf("non rottamata\n");
        return 0;
    }

    else if(command == AGGIUNGI_AUTO) {
        nodo_grafo_t* current_station = graph_search(GRAPH, GRAPH->root, station_distance);
        if(current_station != GRAPH->nil) {
            int prev_max = current_station->stazione->max;
            tree_insert(current_station->stazione, command_number);
            if(current_station->stazione->max != prev_max) {
                graph_find_reachable(GRAPH->root, current_station, GRAPH->nil, AGGIUNGI_AUTO);
            }
            printf("aggiunta\n");
        }
        else {
            printf("non aggiunta\n");
        }
        return 0;
    }

    else if(command == AGGIUNGI_STAZIONE) {
        nodo_grafo_t* current_station = graph_search(GRAPH, GRAPH->root, station_distance);
        count = 0;
        char* data_str = (char*)malloc(2*sizeof(char));
        int data;
        data_str[0] = '\0';
        for(i = j + 1; i < len && count < command_number; i++) {
            if(command_text[i] != ' ' && command_text[i] != '\0') {
                data_str = (char*)realloc(data_str, (k+2)*sizeof(char));
                data_str[k] = command_text[i];
                data_str[k+1] = '\0';
                k++;
            }
            else {
                data_str = (char*)realloc(data_str, (k+2)*sizeof(char));
                data_str[k] = '\0';
                data = atoi(data_str);
                tree_insert(current_station->stazione, data);
                free(data_str);
                data_str = (char*)malloc(2*sizeof(char));
                data_str[0] = '\0';
                k = 0;
                count++;
            }
        }
        graph_find_reachable(GRAPH->root, current_station, GRAPH->nil, AGGIUNGI_STAZIONE);
        if(count != command_number) {
            printf("non aggiunta\n");
            return 0;
        }

        printf("aggiunta\n");
        return 0;
    }
    return 1;
}

void whiten(grafo_t* GRAPH, nodo_grafo_t* x) {
    if(x != GRAPH->nil) {
        whiten(GRAPH, x->left);
    }

    x->visited = 'W';

    if(x != GRAPH->nil) {
        whiten(GRAPH, x->right);
    }
}

void cerca_per_vicini(grafo_t* GRAPH, nodo_albero_t* curr, int start, int end, lista_t* road, lista_liste_t* percorsi, char direzione) {
    if(curr->data != -1) {
        cerca_per_vicini(GRAPH, curr->left, start, end, road, percorsi, direzione);

        nodo_grafo_t* curr_grafo = graph_search(GRAPH, GRAPH->root, curr->data);
        if(curr->data == end) {
            list_insert_tail(road, end);

            if(percorsi->head != NULL && list_length(percorsi->head->head) > list_length(road->head)) {
                free(percorsi->head);
                percorsi->head = NULL;
            }

            list_list_insert_head(percorsi, road);
        }

        if(curr_grafo->visited == 'W' && ((direzione=='>' && curr->data > road->tail->el) || (direzione=='<' && road->tail->el > curr->data))) {
            lista_t* temp = copy_list(road->head);
            list_insert_tail(temp, curr->data);
            curr_grafo->visited = 'B';
            cerca_per_vicini(GRAPH, curr_grafo->stazione->nodi_ragg->root, start, end, temp, percorsi, direzione);
        }

        cerca_per_vicini(GRAPH, curr->right, start, end, road, percorsi, direzione);
    }
}

void percorso_fixup(lista_t* percorso, grafo_t* GRAPH) {
    nodo_lista_t* x = percorso->head;
    while(x != NULL) {
        nodo_grafo_t* curr = graph_search(GRAPH, GRAPH->root, x->el);
        if(x->next != NULL) {
            nodo_lista_t* y = x->next->next;
            while(y != NULL) {
                if(tree_search(curr->stazione->nodi_ragg, curr->stazione->nodi_ragg->root, y->el) != curr->stazione->nodi_ragg->nil) {
                    nodo_lista_t* temp = y->prev->prev;
                    while(temp != x && temp != NULL) {
                        free(temp->next);
                        temp = temp->prev;
                    }
                    x->next = y;
                    y->prev = x;
                }
                y = y->next;
            }
            x = x->next;
        }
        else {
            x = NULL;
        }
    }
}

lista_t* pianifica_percorso(grafo_t* GRAPH, int start, int end, char direzione) {
    whiten(GRAPH, GRAPH->root);
    nodo_grafo_t* station_start = graph_search(GRAPH, GRAPH->root, start);
    nodo_grafo_t* station_end = graph_search(GRAPH, GRAPH->root, end);
    if(station_start == GRAPH->nil || station_end == GRAPH->nil) {
        return NULL;
    }
    
    lista_t* path = malloc(sizeof(lista_t));
    path->head = NULL;
    path->tail = NULL;
    if(tree_search(station_start->stazione->nodi_ragg, station_start->stazione->nodi_ragg->root, end) != station_start->stazione->nodi_ragg->nil) {
        list_insert_head(path, start);
        list_insert_tail(path, end);
        return path;
    }
    lista_liste_t* percorsi = malloc(sizeof(lista_liste_t));
    percorsi->head = NULL;
    list_insert_head(path, start);
    cerca_per_vicini(GRAPH, station_start->stazione->nodi_ragg->root, start, end, path, percorsi, direzione);

    if(percorsi->head != NULL) {
        lista_t* a = percorsi->head;
        
        a = NULL;
        while(a != NULL) {
            percorso_fixup(a, GRAPH);
            print_list(a->head);
            a = a->next;
        }
    }
    return percorsi->head;
} 


int main() {
    grafo_t* GRAPH = malloc(sizeof(grafo_t));
    GRAPH->nil = malloc(sizeof(nodo_grafo_t));
    GRAPH->nil->color = 'B';
    GRAPH->root = GRAPH->nil;
    char command[COMMAND_LENGTH];
    while (fgets(command, COMMAND_LENGTH, stdin)!=NULL) {
        int dataret = 0;
        if(string_compare(command, "aggiungi-auto", 12) == '1') {
            dataret = update_graph(GRAPH,command, AGGIUNGI_AUTO, 14);
        }
        else if(string_compare(command, "rottama-auto", 11) == '1') {
            dataret = update_graph(GRAPH,command, ROTTAMA_AUTO, 13);
        }
        else if(string_compare(command, "aggiungi-stazione", 16) == '1') {
            dataret = update_graph(GRAPH,command, AGGIUNGI_STAZIONE, 18);
        }
        else if(string_compare(command, "demolisci-stazione", 17) == '1') {
            dataret = update_graph(GRAPH,command, DEMOLISCI_STAZIONE, 19);
        }
        else if(string_compare(command, "pianifica-percorso", 17) == '1') {
            char start_str[30], end_str[30];
            int len = string_len(command), i, j=0, k, l=0;
            for(i = 19; i<len && command[i] != ' '; i++) {
                start_str[j] = command[i];
                j++;
            } 
            start_str[j] = '\0';
            for(k= i+1; k<len; k++) {
                end_str[l] = command[k];
                l++;
            }
            end_str[l] = '\0';
            if(atoi(end_str) > atoi(start_str)) {
                //lista_t* l = pianifica_percorso(GRAPH, atoi(start_str), atoi(end_str), '>');
                lista_t* l = NULL;
                if(l == NULL) {
                    printf("nessun percorso\n");
                }
                else {
                    print_list(l->head);
                }
            }    
            else {
                lista_t* l = pianifica_percorso(GRAPH, atoi(end_str), atoi(start_str), '<');
                if(l == NULL) {
                    printf("nessun percorso\n");
                }
                else {
                    print_list_backwards(l->tail);
                }
            }
        }

        if(dataret != 0) {
            printf("Errore\n");
        }
        else {
            //print2DGRAPH(GRAPH->root, GRAPH->nil);
            //graph_walk_print(GRAPH->root, GRAPH->nil);
            //printf("------------------------------------------------------------\n");
        }
    }
    return 0;
}