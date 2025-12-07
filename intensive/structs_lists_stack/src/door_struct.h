#ifndef DOOR_STRUCT_H_
#define DOOR_STRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DOORS_COUNT 15
#define MAX_ID_SEED 1000

struct door {
    int id;
    int status;
};

void initialize_doors(struct door* doors);
void bubble_sorting(struct door* doors);
void doors_status(struct door* doors);
void output(struct door* doors);

#endif