#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_ROWS 25
#define MAX_COLS 80

int **create_matrix(int m, int n);
void load_field_from_file(int **field, const char *filename, int m, int n);
void print_grid(int **grid, int m, int n);
int count_neighbors(int **field, int row, int col, int m, int n);
void update_field(int **field, int **state, int m, int n);
int count_live_cells(int **field, int m, int n);
void initialize_random_field(int **field, int m, int n);
void process_input(char ch, float *key);
void cleanup(int **field, int **state);

int main(int argc, char *argv[]) {
    int **field = create_matrix(MAX_ROWS, MAX_COLS);
    int **state = create_matrix(MAX_ROWS, MAX_COLS);
    int generation = 0;
    float key = 1.0;

    if (argc > 1) {
        load_field_from_file(field, argv[1], MAX_ROWS, MAX_COLS);
    } else {
        initialize_random_field(field, MAX_ROWS, MAX_COLS);
    }

    initscr();
    resize_term(MAX_ROWS + 5, MAX_COLS + 5);
    cbreak();
    nodelay(stdscr, true);
    noecho();

    while (1) {
        curs_set(0);
        clear();
        printw("Generation: %d\n", generation);
        printw("Alive cells: %d\n", count_live_cells(field, MAX_ROWS, MAX_COLS));
        print_grid(field, MAX_ROWS, MAX_COLS);
        update_field(field, state, MAX_ROWS, MAX_COLS);

        int **temp = field;
        field = state;
        state = temp;

        generation++;
        char ch = getch();
        process_input(ch, &key);

        refresh();
        usleep(100000 * key);
    }

    cleanup(field, state);
    endwin();
    return 0;
}

int count_live_cells(int **field, int m, int n) {
    int count = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            count += field[i][j];
        }
    }
    return count;
}

int **create_matrix(int m, int n) {
    int **data = malloc(m * sizeof(int *));
    data[0] = malloc(m * n * sizeof(int));
    for (int i = 1; i < m; i++) {
        data[i] = data[0] + i * n;
    }
    return data;
}

void load_field_from_file(int **field, const char *filename, int m, int n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(file, "%d", &field[i][j]) != 1) {
                fprintf(stderr, "Ошибка чтения файла\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(file);
}

void print_grid(int **grid, int m, int n) {
    printw("+");
    for (int j = 0; j < n; j++) printw("-");
    printw("+\n");

    for (int i = 0; i < m; i++) {
        printw("|");
        for (int j = 0; j < n; j++) {
            printw("%c", grid[i][j] ? 'O' : ' ');
        }
        printw("|\n");
    }

    printw("+");
    for (int j = 0; j < n; j++) printw("-");
    printw("+\n");
}

int count_neighbors(int **field, int row, int col, int m, int n) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i == row && j == col) continue;
            count += field[(i + m) % m][(j + n) % n];
        }
    }
    return count;
}

void update_field(int **field, int **state, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            int neighbors = count_neighbors(field, i, j, m, n);
            state[i][j] =
                (field[i][j] && (neighbors == 2 || neighbors == 3)) || (!field[i][j] && neighbors == 3);
        }
    }
}

void initialize_random_field(int **field, int m, int n) {
    srand(time(NULL));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            field[i][j] = rand() % 2;
        }
    }
}

void process_input(char ch, float *key) {
    if (ch == 'q') {
        endwin();
        exit(0);
    }
    if (ch == 'z') {
        *key = (*key < 3.0) ? *key + 0.3 : 3.0;
    }
    if (ch == 'x') {
        *key = (*key > 0.1) ? *key * 0.5 : 0.1;
    }
}

void cleanup(int **field, int **state) {
    free(state[0]);
    free(state);
    free(field[0]);
    free(field);
}
