#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_SIZE 2048
#define GEN_NUM 2000
#define THREAD_NUM 8
#define LIVING 1
#define DEAD 0

int living_num_global = 0;

void init_grid(int **grid) {    
    int i, j;

    for (i = 0; i < MAX_SIZE; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            grid[i][j] = DEAD;
        }
    }

    //Glider
    int row = 1;
    int col = 1;
    grid[row][col + 1] = LIVING;
    grid[row + 1][col + 2] = LIVING;
    grid[row + 2][col] = LIVING;
    grid[row + 2][col + 1] = LIVING;
    grid[row + 2][col + 2] = LIVING;

    //R-pentonimo
    row = 10;
    col = 30;
    grid[row][col + 1] = LIVING;
    grid[row][col + 2] = LIVING;
    grid[row + 1][col] = LIVING;
    grid[row + 1][col + 1] = LIVING;
    grid[row + 2][col + 1] = LIVING;
}

int getNeighbors(int **grid, int i, int j) {  
    int prev_i = i - 1;
    int prev_j = j - 1;
    int next_i = i + 1;
    int next_j = j + 1;

    //loop through the board's edges
    if (prev_i < 0) {
        prev_i = MAX_SIZE - 1;
    }
    if (prev_j < 0) {
        prev_j = MAX_SIZE - 1;
    }
    if (next_i > MAX_SIZE - 1) {
        next_i = 0;
    }
    if (next_j > MAX_SIZE - 1) {
        next_j = 0;
    }

    //count number of living neighbors
    int neighbor_num = 0;
    if (grid[prev_i][prev_j] == LIVING) {
        neighbor_num++;
    }
    if (grid[prev_i][j] == LIVING) {
        neighbor_num++;
    }
    if (grid[prev_i][next_j] == LIVING) {
        neighbor_num++;
    }
    if (grid[i][prev_j] == LIVING) {
        neighbor_num++;
    }
    if (grid[i][next_j] == LIVING) {
        neighbor_num++;
    }
    if (grid[next_i][prev_j] == LIVING) {
        neighbor_num++;
    }
    if (grid[next_i][j] == LIVING) {
        neighbor_num++;
    }
    if (grid[next_i][next_j] == LIVING) {
        neighbor_num++;
    }

    return neighbor_num;
}

void fill_newgrid(int **grid, int **newgrid) {
    int i, j;

    for (i = 0; i < MAX_SIZE; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            int neighbor_num = getNeighbors(grid, i, j);

            if (grid[i][j] == LIVING && (neighbor_num < 2 || neighbor_num > 3)) {
                newgrid[i][j] = DEAD;
            } else if (grid[i][j] == DEAD && neighbor_num == 3) {
                newgrid[i][j] = LIVING;
            } else {
                newgrid[i][j] = grid[i][j];
            }
        }
    }
}

void copy_newgrid(int **grid, int **newgrid) {
    int i, j;

    for (i = 0; i < MAX_SIZE; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            grid[i][j] = newgrid[i][j];
        }
    }
}

int count_living(int **grid) {
    int i, j;
    int living_num = 0;
    for (i = 0; i < MAX_SIZE; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            if (grid[i][j] == LIVING) {
                living_num++;
            }
        }
    }
    return living_num;
}

int count_living_global(int **grid) {
    int i, j;
    for (i = 0; i < MAX_SIZE; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            if (grid[i][j] == LIVING) {
                living_num_global++;
            }
        }
    }
    return living_num_global;
}

//for visualization only
void show_grid(int **mat) {
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            if (mat[i][j] == DEAD) {
                printf("%c", 176);
            } else {
                printf("%c", 219);
            }
        }
        printf("\n");
    }
}

int main() {
    int i;
    int **grid = (int**) malloc(MAX_SIZE * sizeof(int*));

    for (i = 0; i < MAX_SIZE; i++) {
        grid[i] = (int*) malloc(MAX_SIZE * sizeof(int));
    }

    int **newgrid = (int**) malloc(MAX_SIZE * sizeof(int*));

    for (i = 0; i < MAX_SIZE; i++) {
        newgrid[i] = (int*) malloc(MAX_SIZE * sizeof(int));
    }
    
    init_grid(grid);
    printf("Initial state: %d\n", count_living(grid));   

    for (int i = 0; i < GEN_NUM - 1; i++) {
        fill_newgrid(grid, newgrid);
        copy_newgrid(grid, newgrid);
        printf("Gen %d: %d\n", i + 1, count_living(grid));
    }

    //last gen
    fill_newgrid(grid, newgrid);
    copy_newgrid(grid, newgrid);

    double start_time = omp_get_wtime();
    printf("Gen %d: %d\n", GEN_NUM, count_living_global(grid));
    double end_time = omp_get_wtime();

    printf("%f\n", end_time - start_time);

    return 0;
}