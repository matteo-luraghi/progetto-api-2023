#include<stdio.h>
#include<stdlib.h>
#define COMMAND_LENGTH 1000000
#define AGGIUNGI_AUTO '1'
#define ROTTAMA_AUTO '2'
#define AGGIUNGI_STAZIONE '3'
#define DEMOLISCI_STAZIONE '4'
#define HASH_SIZE 60000
#define HASH_MOD 59989

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

typedef struct nodo_albero {
    struct nodo_albero *right;
    struct nodo_albero *left;
    struct nodo_albero *p;
    int data;
    char color;
} nodo_albero_t;

typedef struct albero {
    nodo_albero_t* root;                
    nodo_albero_t* nil;
    int max;
    //maybe useless
    int distanza;
} stazione_t;

typedef struct nodo_collisioni {
    struct nodo_collisioni* right;
    struct nodo_collisioni* left;
    struct nodo_collisioni* p;
    char color;
    int key;
    char visited;
    stazione_t* stazione;
} nodo_collisioni_t;

typedef struct nodo_grafo {
    nodo_collisioni_t* root;
    nodo_collisioni_t* nil;
} nodo_grafo_t;

typedef struct grafo {
    nodo_grafo_t** items;
    //probably useless
    int size;
    int count;
} grafo_t;

//enqueue
void list_insert_head(lista_t* l, int el) {
    nodo_lista_t* temp = malloc(sizeof(nodo_lista_t));
    temp->el = el;
    temp->next = l->head;
    if(l->head != NULL) {
        l->head->prev = temp;
    }
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

//dequeue
nodo_lista_t* list_remove_tail(lista_t* l) {
    if(l->tail == NULL) {
        return NULL;
    }
    if(l->tail == l->head) {
        nodo_lista_t* tail = l->tail;
        l->head = NULL;
        l->tail = NULL;
        return tail;
    }
    nodo_lista_t* tail = l->tail;
    l->tail = tail->prev;
    l->tail->next = NULL;
    return tail;
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

nodo_collisioni_t* collisioni_search(nodo_grafo_t* T, nodo_collisioni_t* x, int data) {
    if(x == T->nil || data == x->key) {
        return x;
    }
    if(data < x->key) {
        return collisioni_search(T, x->left, data);
    }
    else {
        return collisioni_search(T, x->right, data);
    }
}

nodo_collisioni_t* collisioni_min(nodo_grafo_t* T, nodo_collisioni_t* x) {
    while(x->left != T->nil) {
        x = x->left;
    }
    return x;
}

nodo_collisioni_t* collisioni_max(nodo_grafo_t* T, nodo_collisioni_t* x) {
    while(x->right != T->nil) {
        x = x->right;
    }
    return x;
}

nodo_collisioni_t* tree_successor(nodo_grafo_t* T, nodo_collisioni_t* x) {
    if(x->right != T->nil) {
        return collisioni_min(T, x->right);
    }
    nodo_collisioni_t* y = x->p;
    while(y != T->nil && x == y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

void collisioni_left_rotate(nodo_grafo_t* T, nodo_collisioni_t* x) {
    nodo_collisioni_t* y = x->right;
    
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

void collisioni_right_rotate(nodo_grafo_t* T, nodo_collisioni_t* y) {
    nodo_collisioni_t* x = y->left;
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

void collisioni_insert_fixup(nodo_grafo_t* T, nodo_collisioni_t* z) {
    if(z == T->root) {
        T->root->color = 'B';
    }
    else{                     
        nodo_collisioni_t* x = z->p;              
        if(x->color == 'R') {
            if(x == x->p->left) {           
                nodo_collisioni_t* y = x->p->right;   
                if (y->color == 'R') {
                    x->color = 'B';             
                    y->color = 'B';             
                    x->p->color = 'R';          
                    collisioni_insert_fixup(T, x->p);   
                }
                else {
                    if(z == x->right)  {
                        z = x;                      
                        collisioni_left_rotate(T, z); 
                        x = z->p;           
                    }
                    x->color = 'B';                  
                    x->p->color = 'R';              
                    collisioni_right_rotate(T, x->p);          
                }
            }
            else {
                nodo_collisioni_t* y = x->p->left;
                if (y->color == 'R') {
                    x->color = 'B';
                    y->color = 'B';
                    x->p->color = 'R';
                    collisioni_insert_fixup(T, x->p);
                }
                else {
                    if(z == x->left) {
                        z = x;
                        collisioni_right_rotate(T, z);
                        x = z->p;
                    }  
                    x->color = 'B';
                    x->p->color = 'R';
                    collisioni_left_rotate(T, x->p);
                }
            }
        }
    }
}

void collisioni_insert(nodo_grafo_t* T, int data) {
    if(data > T->max) {
        T->max = data;
    }

    nodo_collisioni_t* z = malloc(sizeof(nodo_collisioni_t));
    z->key = data;

    nodo_collisioni_t* y = T->nil; 
    nodo_collisioni_t* x = T->root;

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
    collisioni_insert_fixup(T, z);
}

void collisioni_delete_fixup(nodo_grafo_t* T, nodo_collisioni_t* x) {
    if (x->color == 'R' || x->p == T->nil) {
        x->color = 'B';
    }
    else if(x == x->p->left) {
        nodo_collisioni_t* w = x->p->right;
        if(w->color == 'R') {
            w->color = 'B';
            x->p->color = 'R';
            collisioni_left_rotate(T, x->p);
            w = x->p->right;
        }
        if(w->left->color == 'B' && w->right->color == 'B') {
            w->color = 'R';
            collisioni_delete_fixup(T, x->p);
        }
        else {
            if(w->right->color == 'B') {
                w->left->color = 'B';
                w->color = 'R';
                collisioni_right_rotate(T, w);
                w = x->p->right;
            }
            w->color = x->p->color;
            x->p->color = 'B';
            w->right->color = 'B';
            collisioni_left_rotate(T, x->p);
        }
    }
    else {
        nodo_collisioni_t* w = x->p->left;
        if(w->color == 'R') {
            w->color = 'B';
            x->p->color = 'R';
            collisioni_right_rotate(T, x->p);
            w = x->p->left;
        }
        if(w->right->color == 'B' && w->left->color == 'B') {
            w->color = 'R';
            collisioni_delete_fixup(T, x->p);
        }
        else {
            if(w->left->color == 'B') {
                w->right->color = 'B';
                w->color = 'R';
                collisioni_left_rotate(T, w);
                w = x->p->left;
            }
            w->color = x->p->color;
            x->p->color = 'B';
            w->left->color = 'B';
           collisioni_right_rotate(T, x->p);
        }
    }
}

nodo_collisioni_t* collisioni_delete(nodo_grafo_t* T, int data) {
    nodo_collisioni_t* z = collisioni_search(T, T->root, data);
    nodo_collisioni_t* x;
    nodo_collisioni_t* y;

    if(z == T->nil) {
        return z;
    }

    //updating the max
    if(data == T->max) {
        if(z->left == T->nil) {
            T->max = z->p->key;    
        }
        else {
            T->max = z->left->key;
        }
    }

    if(z->left == T->nil || z->right == T->nil) {
        y = z;
    }
    else {
        y = collisioni_successor(T, z);
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
        collisioni_delete_fixup(T, x);
    }
    return y;
}

int hash_function(int key) {
    return key % HASH_MOD;
}

nodo_collisioni_t* graph_search(grafo_t* GRAPH, int key) {
    int index = hash_function(key);
    nodo_collisioni_t found = collisioni_search(GRAPH->items[index], GRAPH->items[index]->root, key);
    if(found != GRAPH->items[index]->nil) {
        return found;
    }
    return NULL;
}

void graph_insert(grafo_t* GRAPH, int key) {
    stazione_t* stazione = malloc(sizeof(stazione_t));
    stazione->max = 0;
    stazione->distanza = key;
    stazione->nil = malloc(sizeof(nodo_albero_t));
    stazione->nil->color = 'B';
    stazione->root = stazione->nil;

    nodo_grafo_t* item = malloc(sizeof(nodo_grafo_t));
    item->nil = malloc(sizeof(nodo_collisioni_t));
    item->nil->color = 'B';
    item->root = item->nil;
    collisioni_insert(item, key);

    int index = hash_function(key);
    
    while(GRAPH->items[index] != NULL && GRAPH->items[index]->key != -1) {
        index++;
        index %= HASH_SIZE;
    }
    GRAPH->items[index] = item;
    GRAPH->count++;
}

nodo_grafo_t* graph_delete(grafo_t* GRAPH, int key) {
    int index = hash_function(key);
    while(GRAPH->items[index] != NULL) {
        if(GRAPH->items[index]->key == key) {
            nodo_grafo_t* temp = GRAPH->items[index];
            GRAPH->items[index]->key = -1;
            GRAPH->items[index]->stazione = NULL;
            GRAPH->count--;
            return temp;
        }
        index++;
        index %= HASH_SIZE;
    }
    return NULL;
}

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
        if(graph_search(GRAPH, station_distance) == NULL) {
            graph_insert(GRAPH, station_distance);
        }
        else {
            printf("non aggiunta\n");
            return 0;
        }
    }
    
    else if(command == DEMOLISCI_STAZIONE) {    
        nodo_grafo_t* deleted = graph_delete(GRAPH, station_distance);
        if(deleted != NULL) {
            free(deleted->stazione);
            free(deleted);
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
        nodo_grafo_t* current_station = graph_search(GRAPH, station_distance);
        if(current_station != NULL) {
            if(tree_search(current_station->stazione, current_station->stazione->root, command_number) != current_station->stazione->nil) {
                nodo_albero_t* deleted_auto = rb_delete(current_station->stazione, command_number);
                free(deleted_auto);
                printf("rottamata\n");
                return 0;
            }    
        }
        printf("non rottamata\n");
        return 0;
    }

    else if(command == AGGIUNGI_AUTO) {
        nodo_grafo_t* current_station = graph_search(GRAPH, station_distance);
        if(current_station != NULL) {
            tree_insert(current_station->stazione, command_number);
            printf("aggiunta\n");
        }
        else {
            printf("non aggiunta\n");
        }
        return 0;
    }

    else if(command == AGGIUNGI_STAZIONE) {
        nodo_grafo_t* current_station = graph_search(GRAPH, station_distance);
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
        if(count != command_number) {
            printf("non aggiunta\n");
            return 0;
        }

        printf("aggiunta\n");
        return 0;
    }
    return 1;
}

int main() {
    grafo_t* GRAPH = malloc(sizeof(grafo_t));
    GRAPH->size = HASH_SIZE;
    GRAPH->count = 0;
    GRAPH->items = malloc(HASH_SIZE*(sizeof(nodo_grafo_t)));
    for(int p=0; p<HASH_SIZE;p++) {
        GRAPH->items[p] = NULL;
    }

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
            int start_num, end_num;
            start_num = atoi(start_str);
            end_num = atoi(end_str);
            ////////////////////////////////////////////////////////
            if(start_num >= end_num || end_num >= start_num) {
                printf("nessun percorso\n");
            }
            /*
            if(end_num > start_num) {
                lista_t* path = malloc(sizeof(lista_t));
                path->head = NULL;
                path->tail = NULL;
                nodo_grafo_t* start_grafo = graph_search(GRAPH, GRAPH->root, start_num);
                nodo_grafo_t* end_grafo = graph_search(GRAPH, GRAPH->root, end_num);
                BFS(GRAPH, start_grafo, end_grafo);
                if(end_grafo->predecessor == GRAPH->nil) {
                    printf("nessun percorso\n");
                }
                else {
                    while(end_grafo != GRAPH->nil) {
                        list_insert_head(path, end_grafo->stazione->distanza);
                        end_grafo = end_grafo->predecessor;
                    }
                    print_list(path->head);
                }
                nodo_lista_t* temp = path->head;
                while(temp != NULL) {
                    nodo_lista_t* deleted = temp;
                    temp = temp->next;
                    free(deleted);
                }
                free(path);
                free(temp);
            }    
            else {
                lista_t* path = malloc(sizeof(lista_t));
                path->head = NULL;
                path->tail = NULL;
                nodo_grafo_t* start_grafo = graph_search(GRAPH, GRAPH->root, start_num);
                nodo_grafo_t* end_grafo = graph_search(GRAPH, GRAPH->root, end_num);
                BFS_backwards(GRAPH, start_grafo, end_grafo);
                if(start_grafo->predecessor == GRAPH->nil) {
                    printf("nessun percorso\n");
                }
                else {
                    while(start_grafo != GRAPH->nil) {
                        list_insert_head(path, start_grafo->stazione->distanza);
                        start_grafo = start_grafo->predecessor;
                    }
                    print_list_backwards(path->tail);
                }
                nodo_lista_t* temp = path->head;
                while(temp != NULL) {
                    nodo_lista_t* deleted = temp;
                    temp = temp->next;
                    free(deleted);
                }
                free(path);
                free(temp);
            }
            */
        }

        if(dataret != 0) {
            printf("Errore\n");
        } 
    }
    int w;
    int prev = -1;
    for(w=0;w<HASH_SIZE;w++) {
        if(GRAPH->items[w] != NULL && GRAPH->items[w]->key > 0) {
            prev = GRAPH->items[w]->key;
            break;
        }
    }
    for(int e=w+1; e< HASH_SIZE; e++) {
        if(GRAPH->items[e] != NULL && prev > GRAPH->items[e]->key && GRAPH->items[e]->key > 0) {
            printf("%d:%d %d:%d\n",w, prev, e, GRAPH->items[e]->key );
        }
        if(GRAPH->items[e] != NULL && GRAPH->items[e]->key > 0) {
            prev = GRAPH->items[e]->key;
            w++;
        }
    }
    return 0;
}