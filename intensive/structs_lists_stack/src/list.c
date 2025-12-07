#include "list.h"

struct node* init(struct door* door) {
    const struct door* const_door = door;  // Локальная константная переменная
    struct node* root = (struct node*)malloc(sizeof(struct node));
    if (root != NULL) {
        root->door = *const_door;
        root->next = NULL;
    }
    return root;
}

struct node* add_door(struct node* elem, struct door* door) {
    const struct door* const_door = door;  // Локальная константная переменная
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    if (new_node != NULL) {
        new_node->door = *const_door;
        new_node->next = elem->next;
        elem->next = new_node;
    }
    return new_node;
}

struct node* find_door(int door_id, struct node* root) {
    struct node* current = root;
    while (current != NULL) {
        if (current->door.id == door_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

struct node* remove_door(struct node* elem, struct node* root) {
    if (elem == root) {
        root = elem->next;
        free(elem);
        return root;
    }
    struct node* current = root;
    while (current != NULL && current->next != elem) {
        current = current->next;
    }
    if (current != NULL) {
        current->next = elem->next;
        free(elem);
    }
    return root;
}

void destroy(struct node* root) {
    while (root != NULL) {
        struct node* temp = root;
        root = root->next;
        free(temp);
    }
}