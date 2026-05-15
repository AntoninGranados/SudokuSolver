#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "da.h"
#include "grid.h"
#include "wfc.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    if (argc != 2) {
        fprintf(stderr, "Missing the file argument\n");
        fprintf(stderr, "\tUsage: ./sudoku FILE-NAME\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file");
        return 1;
    }
    
    Grid grid = {0};
    load_grid(&grid, file);
    fclose(file);
    
    dump_grid(&grid, false);
    if (!solve(&grid)) {
        fprintf(stderr, "Failed to find a solution\n");
        return 1;
    }
    printf("Sudoku solved!\n");

    return 0;
}
