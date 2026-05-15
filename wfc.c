#include "wfc.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

void propagate_row(Grid* grid, int row) {
    uint16_t mask = 0b000000000;
    for (size_t i = 0; i < 9; i++) {
        if (grid->cells[i + row*9].collapsed) {
            mask |= 0b1 << (grid->cells[i + row*9].state - 1);
        }
    }
    for (size_t i = 0; i < 9; i++) {
        grid->cells[i + row*9].available_states &= ~mask;
    }
}

void propagate_column(Grid* grid, int column) {
    uint16_t mask = 0b000000000;
    for (size_t j = 0; j < 9; j++) {
        if (grid->cells[column + j*9].collapsed) {
            mask |= 0b1 << (grid->cells[column + j*9].state - 1);
        }
    }
    for (size_t j = 0; j < 9; j++) {
        grid->cells[column + j*9].available_states &= ~mask;
    }
}

void propagate_block(Grid* grid, int x, int y) {
    uint16_t mask = 0b000000000;
    for (int j = 3*y; j < 3*y+3; j++) {
        for (int i = 3*x; i < 3*x+3; i++) {
            if (grid->cells[i + j*9].collapsed) {
                mask |= 0b1 << (grid->cells[i + j*9].state - 1);
            }
        }
    }
    for (int j = 3*y; j < 3*y+3; j++) {
        for (int i = 3*x; i < 3*x+3; i++) {
            grid->cells[i + j*9].available_states &= ~mask;
        }
    }
}

void propagate(Grid* grid) {
    for (size_t k = 0; k < 9; k++) {
        propagate_row(grid, k);
        propagate_column(grid, k);
    }
    for (size_t y = 0; y < 3; y++) {
        for (size_t x = 0; x < 3; x++) {
            propagate_block(grid, x, y);
        }
    }
}


void find_min_cell(Grid* grid, int* x, int* y) {
    int min_score = 10;
    *x = -1;
    *y = -1;

    for (size_t j = 0; j < 9; j++) {
        for (size_t i = 0; i < 9; i++) {
            Cell cell = grid->cells[i + j*9];
            if (cell.collapsed) continue;
            
            int score = 0;
            for (size_t i = 0; i < 9; i++) {
                if (cell.available_states >> i & 0b1) score++;
            }

            if (score == 1) {
                *x = i;
                *y = j;
                return;
            } else if (score < min_score) {
                min_score = score;
                *x = i;
                *y = j;
            }
        }
    }
}

void save_trace(StackTrace* stack, Grid* grid) {
    Grid trace = {0};
    memcpy(&trace, grid, sizeof(Grid));
    da_append(stack, trace);
}

bool backtrack(StackTrace* stack, Grid* grid) {
    if (stack->count > 0) {
        return false;
    }
    *grid = stack->items[--stack->count];
    return true;
}

bool collapse(StackTrace* stack, Grid* grid, bool* solved) {
    *solved = true;

    int x, y;
    find_min_cell(grid, &x, &y);
    if (x < 0 || y < 0) return false;

    int score = 0;
    for (size_t i = 0; i < 9; i++) {
        if (grid->cells[x + y*9].available_states >> i & 0b1) {
            score++;

            grid->cells[x + y*9].available_states &= ~(0b1 << i);
            grid->cells[x + y*9].collapsed = true;
            grid->cells[x + y*9].state = i+1;
        }
    }

    if (score > 1) {
        save_trace(stack, grid);
    } else if (score == 0) {
        if (!backtrack(stack, grid)) {
            *solved = false;
            return false;
        }
    }

    return true;
}

bool solve(Grid* grid) {
    bool solved;

    StackTrace stack = {0};

    while (collapse(&stack, grid, &solved)) {
        usleep(50000);
        printf("\033[12F\n");
        propagate(grid);
        dump_grid(grid, false);
    }

    return solved;
}
