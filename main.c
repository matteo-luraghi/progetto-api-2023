#include<stdio.h>
#include<stdlib.h>
#define COMMAND_LENGTH 1024
#define AGGIUNGI_AUTO '1'
#define ROTTAMA_AUTO '2'
#define AGGIUNGI_STAZIONE '3'
#define DEMOLISCI_STAZIONE '4'
//----------------------------------------------------
//DATA STRUCTURE AND FUNCTIONS
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
} nodo_grafo_t;

typedef struct grafo {
    nodo_grafo_t* root;  
    nodo_grafo_t* nil;     
} grafo_t;

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
    //update the max
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
    else if(command == DEMOLISCI_STAZIONE) {

    }    
    else if(command == ROTTAMA_AUTO) {
        nodo_albero_t* deleted = NULL;
        if(curr->stazione->distanza > modified->stazione->distanza && curr->stazione->distanza - modified->stazione->distanza > modified->stazione->max) {
            deleted = rb_delete(modified->stazione->nodi_ragg, curr->stazione->distanza);
        }
        if(modified->stazione->distanza > curr->stazione->distanza && modified->stazione->distanza - curr->stazione->distanza > curr->stazione->max) {
            deleted = rb_delete(curr->stazione->nodi_ragg, modified->stazione->distanza);
        }
        free(deleted);
    } 

    if(curr->right != NIL) {        
        graph_find_reachable(curr->right, modified, NIL, command);
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
        print2DTree(x->stazione->nodi_ragg->root, x->stazione->nodi_ragg->nil);
        graph_walk_print(x->right, NIL);
    }
}
//-------------------------------------------------------

//----------------------------------------------------

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
            printf("non aggiunga\n");
            return 0;
        }
    }
    
    else if(command == DEMOLISCI_STAZIONE) {
        if(graph_search(GRAPH, GRAPH->root, station_distance) != GRAPH->nil) {
            nodo_grafo_t* deleted = graph_delete(GRAPH, station_distance);
            graph_find_reachable(GRAPH->root, deleted, GRAPH->nil, DEMOLISCI_STAZIONE);
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
        nodo_grafo_t* current_station = graph_search(GRAPH, GRAPH->root, station_distance);
        int prev_max = current_station->stazione->max;
        nodo_albero_t* deleted_auto = rb_delete(current_station->stazione, command_number);
        if(deleted_auto->data == prev_max) {
            graph_find_reachable(GRAPH->root, current_station, GRAPH->nil, ROTTAMA_AUTO);
        }
        free(deleted_auto);
        return 0;
    }

    else if(command == AGGIUNGI_AUTO) {
        nodo_grafo_t* current_station = graph_search(GRAPH, GRAPH->root, station_distance);
        int prev_max = current_station->stazione->max;
        tree_insert(current_station->stazione, command_number);
        if(current_station->stazione->max != prev_max) {
            graph_find_reachable(GRAPH->root, current_station, GRAPH->nil, AGGIUNGI_AUTO);
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
                if(data == 0) {
                    printf("non aggiunta\n");
                    return 0;
                }
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

int main() {
    grafo_t* GRAPH = malloc(sizeof(grafo_t));
    GRAPH->nil = malloc(sizeof(nodo_grafo_t));
    GRAPH->nil->color = 'B';
    GRAPH->root = GRAPH->nil;
    char command[COMMAND_LENGTH];
    while (fgets(command, COMMAND_LENGTH, stdin)!=NULL) {
        int dataret = 0;
        if(string_compare(command, "aggiungi-auto", 12) == 1) {
            dataret = update_graph(GRAPH,command, AGGIUNGI_AUTO, 14);
        }
        else if(string_compare(command, "rottama-auto", 11) == 1) {
            dataret = update_graph(GRAPH,command, ROTTAMA_AUTO, 13);
        }
        else if(string_compare(command, "aggiungi-stazione", 16) == 1) {
            dataret = update_graph(GRAPH,command, AGGIUNGI_STAZIONE, 18);
        }
        else if(string_compare(command, "demolisci-stazione", 17) == 1) {
            dataret = update_graph(GRAPH,command, DEMOLISCI_STAZIONE, 19);
        }
        if(dataret == 0) {
            //print2DGRAPH(GRAPH->root, GRAPH->nil);
            graph_walk_print(GRAPH->root, GRAPH->nil);
            printf("------------------------------------------------------------\n");
        }
        else {
            printf("Error\n");
        }
    }
    return 0;
}