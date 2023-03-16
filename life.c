#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 75 

//int grid[GRID_SIZE][GRID_SIZE];

void initialize_grid(int grid[GRID_SIZE][GRID_SIZE]) {

    // Seed random number generator with current time:
    srand(time(NULL));
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            // Set each cell to a random state (0 or 1):
            grid[i][j] = rand() % 2;
        }
    }
}

void print_grid(int grid[GRID_SIZE][GRID_SIZE]) {
    int i, j;
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) 
		{
			if (grid[i][j])
				write(1, "#", 1);
			else 
				write(1, " ", 1);
        }
		write(1, "\n", 1);
    }
	write(1, "\n", 1);
}
/*
int count_neighbors(int row, int col, int grid[GRID_SIZE][GRID_SIZE]) {
    int count = 0;
    int i, j;
    for (i = row - 1; i <= row + 1; i++) {
        for (j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE && !(i == row && j == col)) {
                count += grid[i][j];
            }
        }
    }
    return count;
}
*/

int count_neighbors(int grid[GRID_SIZE][GRID_SIZE], int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue; // skip the current cell
            }
            int r = (row + i + GRID_SIZE) % GRID_SIZE; // handle wrapping around the edges of the grid
            int c = (col + j + GRID_SIZE) % GRID_SIZE;
            count += grid[r][c];
        }
    }
    return count;
}

void update_grid(int grid[GRID_SIZE][GRID_SIZE]) {
    int i, j;
    int new_grid[GRID_SIZE][GRID_SIZE];
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            int neighbors = count_neighbors(grid, i, j);
            if (grid[i][j]) {
                if (neighbors < 2 || neighbors > 3) {
                    new_grid[i][j] = 0;
                } else {
                    new_grid[i][j] = 1;
                }
            } else {
                if (neighbors == 3 || neighbors == 6) {
                    new_grid[i][j] = 1;
                } else {
                    new_grid[i][j] = 0;
                }
            }
        }
    }
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = new_grid[i][j];
        }
    }
	usleep(120000);
}

int main() {
	int	grid[GRID_SIZE][GRID_SIZE];
    initialize_grid(grid);
	{	
		write(1, "\n\n", 2);
		print_grid(grid);
		while (1)
		{
        	update_grid(grid);
        	print_grid(grid);
    	}
	}
    return 0;
}
