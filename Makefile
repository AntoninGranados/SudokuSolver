sudoku: build/grid.o build/wfc.o build/sudoku.o
	gcc -o sudoku build/sudoku.o build/grid.o build/wfc.o -Wall -Wextra -Wpedantic -std=c23

build/grid.o: grid.c
	gcc -o build/grid.o -c grid.c -Wall -Wextra -Wpedantic -std=c23

build/wfc.o: wfc.c
	gcc -o build/wfc.o -c wfc.c -Wall -Wextra -Wpedantic -std=c23

build/sudoku.o: sudoku.c
	gcc -o build/sudoku.o -c sudoku.c -Wall -Wextra -Wpedantic -std=c23
