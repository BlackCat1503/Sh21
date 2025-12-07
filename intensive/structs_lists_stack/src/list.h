#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <stdlib.h>

struct door {
    int id;
    int status;
};

struct node {
    struct door door;
    //указатель на структуру next
    struct node* next;
};

//инициализация структуры односвязного списка на базе переданной struct door
struct node* init(struct door* door);
//вставка нового элемента struct door в односвязный список после переданного узла elem
struct node* add_door(struct node* elem, struct door* door);
// //поиск двери в списке по её id
struct node* find_door(int door_id, struct node* root);
// удаление элемента списка
struct node* remove_door(struct node* elem, struct node* root);
//освобождение памяти, занимаемой структурой списка
void destroy(struct node* root);

#endif