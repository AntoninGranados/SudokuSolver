#include "wfc.h"
#include "grid.h"

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

void propagate_block(Grid* grid, CellCoordinates coord) {
    uint16_t mask = 0b000000000;
    int x = 3 * (coord.x / 3);
    int y = 3 * (coord.y / 3);
    for (int j = y; j < y+3; j++) {
        for (int i = x; i < x+3; i++) {
            if (grid->cells[i + j*9].collapsed) {
                mask |= 0b1 << (grid->cells[i + j*9].state - 1);
            }
        }
    }
    for (int j = y; j < y+3; j++) {
        for (int i = x; i < x+3; i++) {
            grid->cells[i + j*9].available_states &= ~mask;
        }
    }
}

void propagate_from(Grid* grid, CellCoordinates coord) {
    propagate_row(grid, coord.y);
    propagate_column(grid, coord.x);
    propagate_block(grid, coord);
}

void propagate(Grid* grid) {
    for (size_t k = 0; k < 9; k++) {
        propagate_row(grid, k);
        propagate_column(grid, k);
    }
    for (size_t y = 0; y < 3; y++) {
        for (size_t x = 0; x < 3; x++) {
            propagate_block(grid, (CellCoordinates){ x, y });
        }
    }
}


void find_min_cells(Grid* grid, CellCandidates* candidates) {
    int min_states_count = 10;
    candidates->count = 0;

    for (size_t j = 0; j < 9; j++) {
        for (size_t i = 0; i < 9; i++) {
            Cell cell = grid->cells[i + j*9];
            if (cell.collapsed) continue;

            CellCoordinates coord = {
                .x = i,
                .y = j,
            };
            
            int states_count = 0;
            for (size_t i = 0; i < 9; i++) {
                if (cell.available_states >> i & 0b1) states_count++;
            }

            if (states_count == min_states_count) {
                da_append(candidates, coord);
            } else if (states_count < min_states_count) {
                min_states_count = states_count;
                candidates->count = 0;
                da_append(candidates, coord);
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
    if (stack->count == 0) {
        return false;
    }
    *grid = stack->items[--stack->count];
    return true;
}

bool collapse(StackTrace* stack, CellCandidates* candidates, Grid* grid, bool* solved) {
    *solved = true;

    find_min_cells(grid, candidates);
    if (candidates->count == 0) return false;
    // CellCoordinates coord = candidates->items[0];
    CellCoordinates coord = candidates->items[rand() % candidates->count];

    int available_states[9] = {0};
    int states_count = 0;
    for (size_t i = 0; i < 9; i++) {
        if (grid->cells[as_id(coord)].available_states >> i & 0b1) {
            available_states[states_count++] = i+1;
        }
    }

    if (states_count == 0) {
        if (!backtrack(stack, grid)) {
            *solved = false;
            return false;
        } else {
            return true;
        }
    }

    int state = rand() % states_count;
    grid->cells[as_id(coord)].available_states &= ~(0b1 << (available_states[state]-1));
    if (states_count > 1) save_trace(stack, grid);

    grid->cells[as_id(coord)].collapsed = true;
    grid->cells[as_id(coord)].state = available_states[state];


    propagate_from(grid, coord);
    return true;
}

bool solve(Grid* grid) {
    bool solved;

    StackTrace stack = {0};
    CellCandidates candidates = {0};

    while (collapse(&stack, &candidates, grid, &solved)) {
        usleep(10000);
        printf("\033[12F\n");
        dump_grid(grid, false);
    }

    return solved;
}
