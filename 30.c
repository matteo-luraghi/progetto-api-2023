#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define COMMAND_LENGTH 1000000
#define AGGIUNGI_AUTO '1'
#define ROTTAMA_AUTO '2'
#define AGGIUNGI_STAZIONE '3'
#define DEMOLISCI_STAZIONE '4'
#define PIANIFICA_PERCORSO '5'
#define HASH_SIZE 120000
#define HASH_MOD 119999
#define ARRAY_DIM 60000

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
    int distanza;
} stazione_t;

typedef struct nodo_grafo {
    int key;
    char visited;
    struct nodo_grafo* prev;
    stazione_t* stazione;
} nodo_grafo_t;

typedef struct grafo {
    nodo_grafo_t** items;
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
    while(x != T->nil && data != x->data) {
        if(data < x->data) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    return x;
}

nodo_albero_t* tree_min(stazione_t* T, nodo_albero_t* x) {
    while(x->left != T->nil) {
        x = x->left;
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

nodo_grafo_t* graph_search(grafo_t* GRAPH, int key) {
    int index = key % HASH_MOD;
    while(GRAPH->items[index] != NULL) {
        if(GRAPH->items[index]->key == key) {
            return GRAPH->items[index];
        }
        index++;
        index %= HASH_SIZE;
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
    item->key = key;
    item->stazione = stazione;

    int index = key % HASH_MOD;
    
    while(GRAPH->items[index] != NULL && GRAPH->items[index]->key != -1) {
        index++;
        index %= HASH_SIZE;
    }
    GRAPH->items[index] = item;
}

nodo_grafo_t* graph_delete(grafo_t* GRAPH, int key) {
    int index = key % HASH_MOD;
    while(GRAPH->items[index] != NULL) {
        if(GRAPH->items[index]->key == key) {
            nodo_grafo_t* temp = GRAPH->items[index];
            GRAPH->items[index]->key = -1;
            GRAPH->items[index]->stazione = NULL;
            return temp;
        }
        index++;
        index %= HASH_SIZE;
    }
    return NULL;
}

void BFS(grafo_t* GRAPH, int* nodi, int len) {
    int i;
    nodo_grafo_t* start = graph_search(GRAPH, nodi[0]);
    start->visited = 'B';
    
    lista_t* queue = malloc(sizeof(lista_t));
    queue->head = NULL;
    queue->tail = NULL;
    list_insert_head(queue, nodi[0]);

    int reachable[ARRAY_DIM];
    int reachable_tail = 0;

    nodo_lista_t* curr_lista;
    nodo_grafo_t* curr_grafo;
    nodo_grafo_t* v_grafo;
    
    while(queue->head != NULL) {
        curr_lista = list_remove_tail(queue);
        if(curr_lista != NULL) {
            if(curr_lista->el == nodi[0]) {
                curr_grafo = start;
            }
            else {
                curr_grafo = graph_search(GRAPH, curr_lista->el);
            }

            //find reachable
            reachable_tail = 0;
            for(i = len-1; i >= 0; i--) {
                if(nodi[i] == curr_grafo->key) {
                    break;
                }
                if(nodi[i] > curr_grafo->key && nodi[i] - curr_grafo->key <= curr_grafo->stazione->max){
                    reachable[reachable_tail] = nodi[i];
                    reachable_tail++;
                }
            }

            while(reachable_tail != -1) {
                if(reachable[reachable_tail] > nodi[0] && reachable[reachable_tail] <= nodi[len-1]) {
                    v_grafo = graph_search(GRAPH, reachable[reachable_tail]);
                    if(reachable[reachable_tail] == nodi[len-1]) {
                        v_grafo->prev = curr_grafo;
                        return;
                    }
                    if(v_grafo->visited == 'W') {
                        v_grafo->visited = 'B';
                        v_grafo->prev = curr_grafo;
                        list_insert_head(queue, reachable[reachable_tail]);
                    }
                    
                }
                reachable[reachable_tail] = 0;
                reachable_tail--;
            }
        }
    }
}

void BFS_backwards(grafo_t* GRAPH, int* nodi, int len) {
    int i;
    nodo_grafo_t* end = graph_search(GRAPH, nodi[0]);
    end->visited = 'B';
    
    lista_t* queue = malloc(sizeof(lista_t));
    queue->head = NULL;
    queue->tail = NULL;
    list_insert_head(queue, nodi[0]);

    int reachable[ARRAY_DIM];
    int reachable_tail = 0;

    nodo_lista_t* curr_lista;
    nodo_grafo_t* curr_grafo;
    nodo_grafo_t* i_grafo;
    nodo_grafo_t* v_grafo;

    while(queue->head != NULL) {
        curr_lista = list_remove_tail(queue);
        if(curr_lista != NULL) {
            if(curr_lista->el == nodi[0]) {
                curr_grafo = end;
            }
            else {
                curr_grafo = graph_search(GRAPH, curr_lista->el);
            }

            //find reachable
            reachable_tail = 0;
            for(i = len-1; i >= 0; i--) {
                if(nodi[i] == curr_grafo->key) {
                    break;
                }
                i_grafo = graph_search(GRAPH, nodi[i]);
                if(nodi[i] > curr_grafo->key && nodi[i] - curr_grafo->key <= i_grafo->stazione->max){
                    reachable[reachable_tail] = nodi[i];
                    reachable_tail++;    
                }
            }

            while(reachable_tail != -1) {
                if(reachable[reachable_tail] > nodi[0] && reachable[reachable_tail] <= nodi[len-1]) {
                    v_grafo = graph_search(GRAPH, reachable[reachable_tail]);
                    if(reachable[reachable_tail] == nodi[len-1]) {
                        v_grafo->prev = curr_grafo;
                        return;
                    }
                    if(v_grafo->visited == 'W') {
                        v_grafo->visited = 'B';
                        v_grafo->prev = curr_grafo;
                        list_insert_head(queue, reachable[reachable_tail]);
                    }

                }
                reachable[reachable_tail] = 0;
                reachable_tail--;
            }
        }
    }
}

int main() {
    grafo_t* GRAPH = malloc(sizeof(grafo_t));
    nodo_grafo_t* curr;
    char command[COMMAND_LENGTH];
    char station_distance_str[15];
    char command_number_str[15];
    char data_str[15];
    char command_text;
    int data, i, j, q, station_distance, command_number, count, clean;

    GRAPH->items = malloc(HASH_SIZE*(sizeof(nodo_grafo_t)));
    for(i=0; i<HASH_SIZE;i++) {
        GRAPH->items[i] = NULL;
    }

    while (fgets(command, COMMAND_LENGTH, stdin)!=NULL) {

        for(clean = 0; clean < 15; clean++) {
            station_distance_str[clean] = 0;
            command_number_str[clean] = 0;
        }
               
        i = 0;
        j = 0;
        
        //reading the command
        if(command[0] == 'r') {
            command_text = ROTTAMA_AUTO;
            i = 13;
        }
        else if(command[0] == 'd') {
            command_text = DEMOLISCI_STAZIONE;
            i = 19;
        }
        else if(command[0] == 'p') {
            command_text = PIANIFICA_PERCORSO;
            i = 19;
        }
        else if(command[9] == 's') {
            command_text = AGGIUNGI_STAZIONE;
            i = 18;
        }
        else {
            command_text = AGGIUNGI_AUTO;
            i = 14;
        }

        //reading the station distance / start station
        while(command[i] != ' ' && command[i] != '\n') {
            station_distance_str[j] = command[i];
            i++;
            j++;
        }
        station_distance_str[i] = '\0';
        
        station_distance = atoi(station_distance_str);
 
        j = 0;
        if(command[i] != '\n') {
            i++;
        }
        //reading the number of cars to add / car to add or remove / end station
        while(command[i] != ' ' && command[i] != '\n') {
            command_number_str[j] = command[i];
            i++;
            j++;
        }

        command_number = atoi(command_number_str);

        if(command_text == AGGIUNGI_AUTO) {
            nodo_grafo_t* current_station = graph_search(GRAPH, station_distance);
            if(current_station != NULL) {
                tree_insert(current_station->stazione, command_number);
                printf("aggiunta\n");
            }
            else {
                printf("non aggiunta\n");
            }
        } 
        else if(command_text == AGGIUNGI_STAZIONE) {
            if(graph_search(GRAPH, station_distance) == NULL) {
                graph_insert(GRAPH, station_distance);
                nodo_grafo_t* current_station = graph_search(GRAPH, station_distance);
                count = 0;
                
                //reading all the cars to add to the new station
                while(count < command_number) {
                    for(clean=0;clean<15; clean++) {
                        data_str[clean] = 0;
                    }
                    j = 0;
                    i++;
                    while(command[i] != ' ' && command[i] != '\n') {
                        data_str[j] = command[i];
                        i++;
                        j++;
                    }
                    data = atoi(data_str);
                    tree_insert(current_station->stazione, data);
                    count++;
                } 
                printf("aggiunta\n");
            }
            else {
                printf("non aggiunta\n");
            }
        } 
        else if(command_text == ROTTAMA_AUTO) {
            nodo_grafo_t* current_station = graph_search(GRAPH, station_distance);
            if(current_station != NULL) {
                nodo_albero_t* deleted_auto = rb_delete(current_station->stazione, command_number);
                if(deleted_auto != current_station->stazione->nil) {
                    free(deleted_auto);
                    printf("rottamata\n");
                }
                else {
                    printf("non rottamata\n");
                }
            }
            else {
                printf("non rottamata\n");
            }
        } 
        else if(command_text == DEMOLISCI_STAZIONE) {
            nodo_grafo_t* deleted = graph_delete(GRAPH, station_distance);
            if(deleted != NULL) {
                free(deleted->stazione);
                printf("demolita\n");
            }
            else {
                printf("non demolita\n");
            }
        } 

        else { //pianifica percorso
            int start_num, end_num;           

            start_num = station_distance;
            end_num = command_number;

            int* nodi_utili = malloc(sizeof(int)*HASH_SIZE);
            int nodi_utili_len = 0;

            if(end_num > start_num) {
                nodi_utili[0] = start_num;
                nodi_utili_len++;

                for(int m = 0; m < HASH_SIZE; m++) {
                    if(GRAPH->items[m] != NULL && GRAPH->items[m]->key > start_num && GRAPH->items[m]->key < end_num) {
                        nodi_utili[nodi_utili_len] = GRAPH->items[m]->key;
                        nodi_utili_len++;
                    }
                }
                nodi_utili[nodi_utili_len] = end_num;
                nodi_utili_len++;
            }    
            else {
                nodi_utili[0] = end_num;
                nodi_utili_len++;
                
                for(int m = 0; m < HASH_SIZE; m++) {
                    if(GRAPH->items[m] != NULL && GRAPH->items[m]->key < start_num && GRAPH->items[m]->key > end_num) {
                        nodi_utili[nodi_utili_len] = GRAPH->items[m]->key;
                        nodi_utili_len++;
                    }
                }

                nodi_utili[nodi_utili_len] = start_num;
                nodi_utili_len++;
            }

            int nodi_temp = nodi_utili[0];
            for(int h=1; h<=nodi_utili_len;h++) {
                if(nodi_temp > nodi_utili[h]) {
                    heapsort(nodi_utili, nodi_utili_len-1);
                    break;
                }
                nodi_temp = nodi_utili[h];
            }

            for(q = 0; q < nodi_utili_len; q++) {
                curr = graph_search(GRAPH, nodi_utili[q]);
                curr->visited = 'W';
                curr->prev = NULL;
            }

            if(end_num > start_num) {
                BFS(GRAPH, nodi_utili, nodi_utili_len);
                lista_t* path = malloc(sizeof(lista_t));
                path->head = NULL;
                path->tail = NULL;
                nodo_grafo_t* end_grafo = graph_search(GRAPH, end_num);
                if(end_grafo->prev == NULL) {
                    printf("nessun percorso\n");
                }
                else {
                    while(end_grafo != NULL) {
                        list_insert_head(path, end_grafo->key);
                        end_grafo = end_grafo->prev;
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
                BFS_backwards(GRAPH, nodi_utili, nodi_utili_len);
                lista_t* path = malloc(sizeof(lista_t));
                path->head = NULL;
                path->tail = NULL;
                nodo_grafo_t* start_grafo = graph_search(GRAPH, start_num);
                if(start_grafo->prev == NULL) {
                    printf("nessun percorso\n");
                }
                else {
                    while(start_grafo != NULL) {
                        list_insert_head(path, start_grafo->key);
                        start_grafo = start_grafo->prev;
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
            free(nodi_utili);
            
        } 
    }
    return 0;
}