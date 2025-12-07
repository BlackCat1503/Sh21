#include "list.h"

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAIL 1

int add_door_test() {
    struct door door1 = {1, 0};
    struct door door2 = {2, 0};
    // 1 - надо создать новый список с дверью1
    struct node *root = init(&door1);
    //создаем указатель root на структуру node и инициализируется функ. init, кот. передается указатель на
    //структуру door1
    // 2 - добавляем в список выше дверь2
    add_door(root, &door2);
    //вызываем функ. add_door, кот. передается указатель на структуру node и указатель на структуру door2
    // 3 - проверка правильно ли была добавлена дверь2
    //если следующий элемент после двери1 нет (т.е их не две) или id добавленной двери не 2, то FAIL
    if (root->next == 0 || root->next->door.id != 2) {
        return FAIL;
    }
    destroy(root);
    return SUCCESS;
}

int remove_door_test() {
    struct door door1 = {1, 0};
    struct door door2 = {2, 0};
    // 1 - надо создать новый список с дверью1
    struct node *root = init(&door1);
    // 2 - добавляем в список выше дверь2
    add_door(root, &door2);
    // 3 - найти элемент списка с дверью 1
    struct node *elem = find_door(1, root);
    //создаем указатель elem на структуру node и инициализируется функ. find_door,кот. передается id door1 и
    root = remove_door(elem, root);
    //список с двумя дверями
    // 4 - удалить дверь1
    // 5 - проверка правильно ли была удалена дверь1
    //если id добавленной двери не 2 и следующий элемент есть, то FAIL
    if (root->door.id != 2 || root->next != 0) {
        return FAIL;
    }
    destroy(root);
    return SUCCESS;
}

int main() {
    int result = SUCCESS;
    result |= add_door_test();
    result |= remove_door_test();
    if (result == SUCCESS) {
        printf("SUCCESS\n");
    } else {
        printf("FAIL\n");
    }
    return result;
}