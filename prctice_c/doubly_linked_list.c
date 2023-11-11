#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next;
    struct node* prev;
} node;

typedef struct dll {
    struct node* head;
    struct node* tail;  
} dll;


void init_dll(dll* dll_list) {
    dll_list->head = NULL;
    dll_list->tail = NULL;
}


void add_node(dll* dll_list, int data) {
    node* new = (node*)malloc(sizeof(node));
    new->data = data;
    new->next = NULL;
    new->prev = NULL;

    if (dll_list->head == NULL) {
        dll_list->head = new;
        dll_list->tail = new;
        return;
    }
    else {
        dll_list->tail->next = new;
        new->prev = dll_list->tail;

        dll_list->tail = new;
        return;
    }
}

node* find_node(dll* dll_list, int data) {
    node* current_node = (node*)malloc(sizeof(node));
    current_node = dll_list->head;

    while (current_node != NULL) {
        if (current_node->data == data)
            return current_node;
        
        current_node = current_node->next;
    }
    
    if (current_node != NULL) {
        return NULL;
    }
}

void remove_node(dll* dll_list, int data) {
    node* current_node = (node*)malloc(sizeof(node));
    current_node = find_node(dll_list, data);

    if (current_node == NULL) {
        printf("Not found!\n");
        return;
    }
    if (current_node == dll_list->head) {
        dll_list->head = current_node->next;
        current_node->next->prev = NULL;
        free(current_node);
        printf("delete head\n");
    }
    else if (current_node == dll_list->tail) {
        dll_list->tail = current_node->prev;
        current_node->prev->next = NULL;
        free(current_node);
        printf("delete tail\n");
    }
    else {
        current_node->prev->next = current_node->next;
        current_node->next->prev = current_node->prev;
        free(current_node);
        printf("delete complete\n");
    }
    return;
}


void insert_node(dll* dll_list, int num, int data) {
    node* current_node = (node*)malloc(sizeof(node));
    current_node = find_node(dll_list, num);

    if (current_node == NULL) {
        printf("Not found\n");
        return;
    }

    node* new = (node*)malloc(sizeof(node));
    new->data = data;
    new->next = current_node->next;
    new->prev = current_node;

    if (current_node == dll_list->tail) {
        dll_list->tail = new;
    }
    else {
        current_node->next->prev = new;
    }
    current_node->next = new;

    printf("insert complete\n");
    return;
}

void show_dll(dll* dll_list)
{
	node* cur = (node*)malloc(sizeof(node));
	cur = dll_list->head;
	while (cur != NULL)
	{
		printf("%d ", cur->data);
		cur = cur->next;
	}
	printf("\n");
}


int main() {
    dll* new_dll = (dll*)malloc(sizeof(dll));

    init_dll(new_dll);

    add_node(new_dll, 1);
    add_node(new_dll, 4);
    add_node(new_dll, 3);
    add_node(new_dll, 4);
    add_node(new_dll, 15);
    add_node(new_dll, 6);

    int idx;
	printf("Enter number where you insert : ");
	scanf("%d", &idx);

    int number_insert;
	printf("Enter number to insert : ");
	scanf("%d", &number_insert);
	insert_node(new_dll, idx, number_insert);

    show_dll(new_dll);
}
