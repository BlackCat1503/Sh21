#include "door_struct.h"

void initialize_doors(struct door* doors);

int main() {
    struct door doors[DOORS_COUNT];

    initialize_doors(doors);

    bubble_sorting(doors);
    doors_status(doors);
    output(doors);

    return 0;
}

void initialize_doors(struct door* doors) {
    srand(time(0));

    int seed = rand() % MAX_ID_SEED;
    for (int i = 0; i < DOORS_COUNT; i++) {
        doors[i].id = (i + seed) % DOORS_COUNT;
        doors[i].status = rand() % 2;
    }
}

void bubble_sorting(struct door* doors) {
    for (int i = 0; i < DOORS_COUNT - 1; i++) {
        for (int j = 0; j < DOORS_COUNT - i - 1; j++) {
            if (doors[j].id > doors[j + 1].id) {
                int temp = doors[j].id;
                doors[j].id = doors[j + 1].id;
                doors[j + 1].id = temp;
            }
        }
    }
}

void doors_status(struct door* doors) {
    for (int i = 0; i < DOORS_COUNT; i++) {
        // 0 - закрыто, 1 - открыто + для всех дверей проставить статус "закрыта"
        if (doors[i].status == 1) {
            doors[i].status = 0;
        }
    }
}

void output(struct door* doors) {
    for (int i = 0; i < DOORS_COUNT; i++) printf("%d, %d\n", doors[i].id, doors[i].status);
}