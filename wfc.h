#pragma once

#include "da.h"
#include "grid.h"


typedef struct {
    size_t count;
    size_t capacity;
    Grid* items;
} StackTrace;

typedef struct {
    int x, y;
} CellCoordinates;
#define as_id(coord) (coord).x + (coord).y * 9

typedef struct {
    size_t count;
    size_t capacity;
    CellCoordinates* items;
} CellCandidates;

void propagate_row(Grid* grid, int row);
void propagate_column(Grid* grid, int column);
void propagate_block(Grid* grid, CellCoordinates coord);
void propagate_from(Grid* grid, CellCoordinates coord);
void propagate(Grid* grid);

void find_min_cells(Grid* grid, CellCandidates* candidates);

void save_trace(StackTrace* stack, Grid* grid);
bool backtrack(StackTrace* stack, Grid* grid);
bool collapse(StackTrace* stack, CellCandidates* candidates, Grid* grid, bool* failed);

bool solve(Grid* grid);
