#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    uint16_t available_states;
    
    bool collapsed;
    bool is_default;
    int state;
} Cell;

typedef struct {
    Cell cells[9*9];
} Grid;

void init_grid(Grid* grid);
void load_grid(Grid* grid, FILE* fptr);
void dump_grid(Grid* grid, bool display_available);
