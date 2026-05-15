#pragma once

#include "da.h"
#include "grid.h"


typedef struct {
    size_t count;
    size_t capacity;
    Grid* items;
} StackTrace;


void propagate_row(Grid* grid, int row);
void propagate_column(Grid* grid, int column);
void propagate_block(Grid* grid, int x, int y);
void propagate(Grid* grid);

void find_min_cell(Grid* grid, int* x, int* y);

void save_trace(StackTrace* stack, Grid* grid);
bool backtrack(StackTrace* stack, Grid* grid);
bool collapse(StackTrace* stack, Grid* grid, bool* failed);

bool solve(Grid* grid);
