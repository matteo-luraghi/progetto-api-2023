#include<stdio.h>
#include<stdlib.h>
#define COMMAND_LENGTH 1024
//----------------------------------------------------
// BST DATA STRUCTURE AND FUNCTIONS
typedef struct nodo {
    struct nodo *right;
    struct nodo *left;
    struct nodo *p;
    int autonomia;
} macchina_t;

typedef struct albero {
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

//temp data structure, the data will go directly in the graph
typedef struct command_data{
    int station_distance;
    int data_num;
    int data[512];
    char valid;
} command_data_t;

command_data_t parse_command(char command_text[], char command[], char message[]) {
    command_data_t DATA;
    int i, j, k = 0, count = 0;
    char station_distance_str[10], command_number_str[10];
    int len = string_len(command_text);
    
    for(i = string_len(command); i < len && command_text[i] != ' '; i++) {
        station_distance_str[count] = command_text[i];
        count++;
    }

    DATA.station_distance = atoi(station_distance_str);

    count = 0;
    for(j = i + 1 ; j < len && command_text[j] != ' '; j++) {
        command_number_str[count] = command_text[j];
        count++;
    }

    if(command == "rottama-auto" || command == "demolisci-stazione") {
        DATA.data_num = 1;
        DATA.data[0] = atoi(command_number_str);
        DATA.valid = 'Y';
        return DATA;
    }

    else if(command == "aggiungi-stazione" || command == "aggiungi-auto") {
        DATA.data_num = atoi(command_number_str);
    }

    count = 0;
    char* data_str = (char*)malloc(2*sizeof(char));
    data_str[0] = '\0';
    for(i = j + 1; i < len && count < DATA.data_num; i++) {
        if(command_text[i] != ' ' && command_text[i] != '\0') {
            data_str = (char*)realloc(data_str, (k+2)*sizeof(char));
            data_str[k] = command_text[i];
            data_str[k+1] = '\0';
            k++;
        }
        else {
            data_str = (char*)realloc(data_str, (k+2)*sizeof(char));
            data_str[k] = '\0';
            DATA.data[count] = atoi(data_str);
            if(DATA.data[count] == 0) {
                printf("non %s\n", message);
                DATA.valid = 'N';
                return DATA;
            }
            free(data_str);
            data_str = (char*)malloc(2*sizeof(char));
            data_str[0] = '\0';
            k = 0;
            count++;
        }
    }
    if(count != DATA.data_num) {
        printf("non %s\n", message);
        DATA.valid = 'N';
        return DATA;
    }

    DATA.valid = 'Y';
    printf("%s\n", message);
    return DATA;
}

void printData(command_data_t DATA) {
    printf("Station Distance: %d\n", DATA.station_distance);
    printf("Data Num: %d\n", DATA.data_num);
    for (int i=0; i<DATA.data_num; i++) {
        printf("Data #%d: %d\n", i, DATA.data[i]);
    }
    printf("Valid: %c\n",DATA.valid);
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
        command_data_t DATA;
        scanf("%[^\n]%*c", command);
        if(string_compare(command, "aggiungi-auto", 12) == 1) {
            DATA = parse_command(command, "aggiungi-auto", "aggiunta");
        }
        else if(string_compare(command, "rottama-auto", 11) == 1) {
            DATA = parse_command(command, "rottama-auto", "rottamata");
        }
        else if(string_compare(command, "aggiungi-stazione", 16) == 1) {
            DATA = parse_command(command, "aggiungi-stazione", "aggiunta");
        }
        else if(string_compare(command, "demolisci-stazione", 17) == 1) {
            DATA = parse_command(command, "demolisci-stazione", "demolita");
        }
        printData(DATA);
        nope = 0;
    }
    return 0;
}