#include "grid.h"

#include "wfc.h"

void init_grid(Grid* grid) {
    for (size_t y = 0; y < 9; y++) {
        for (size_t x = 0; x < 9; x++) {
            grid->cells[x + y*9] = (Cell){
                .available_states = 0b111111111,
                .collapsed = false,
                .is_default = false,
                .state = -1,
            };
        }
    }
}

void load_grid(Grid* grid, FILE* fptr) {
    init_grid(grid);
    size_t i = 0;
    char c;
    while ((c = fgetc(fptr)) != EOF) {
        if ('1' <= c && c <= '9') {
            grid->cells[i++] = (Cell){
                .available_states = 0,
                .collapsed = true,
                .is_default = true,
                .state = c - '0'
            };
        } else if (c == '.') {
            i++;
        }
    }

    propagate(grid);
}

void dump_grid(Grid* grid, bool display_available) {
    for (size_t y = 0; y < 9; y++) {
        if (y > 0 && y % 3 == 0) {
            printf("\033[90m");
            for (size_t i = 0; i < 9; i++) {
                if (i > 0 && i % 3 == 0) {
                    printf("╋");
                }
                printf("━━");
            }
            printf("\033[0m");
            printf("\n");
        }
        
        for (size_t x = 0; x < 9; x++) {
            if (x > 0 && x % 3 == 0) {
                printf("\033[90m┃\033[0m");
            }

            Cell cell = grid->cells[x + y*9];
            if (cell.is_default) {
                printf("\033[90m");
            }
            if (cell.collapsed) {
                printf("%d ", cell.state);
            } else if (display_available) {
                int c = 0;
                for (size_t i = 0; i < 9; i++) {
                    if (cell.available_states >> i & 0b1) c++;
                }
                printf("/%d", c);
            } else {
                printf("  ");
            }
            printf("\033[0m");
        }
        printf("\n");
    }
}
