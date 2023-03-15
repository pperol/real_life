#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 116 

void initialize_grid(int grid[GRID_SIZE][GRID_SIZE]) {

    // Seed random number generator with current time:
    srand(time(NULL));
    for (int i = 0; i < GRID_SIZE/5; i++) {
        for (int j = 0; j < GRID_SIZE/5; j++) {
            // Set each cell to a random state (0 or 1):
            grid[i][j] = rand() % 2;
        }
    }
}

void print_grid(int grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == 1)
                // living cell
				write(1, "#", 1);
            else if (grid[i][j] == 2)
                // new cell
				write(1, "O", 1);
            else if (grid[i][j] == -1)
                // dying cell
				write(1, "+", 1);
            else if (grid[i][j] == 3)
               // new born cell
				write(1, "o", 1);
            else
				write(1, " ", 1);
        }
		write(1, "\n", 1);
    }
}

int count_healthy_neighbors(int grid[GRID_SIZE][GRID_SIZE], int row, int col) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE && !(i == row && j == col)) {
                //if (grid[i][j] == 1 || grid[i][j] == 2)  // count living and dying cells
                if (grid[i][j] == 1 || grid[i][j] == 2 || grid[i][j] == -1)  // count living and dying cells
                    count++;
            }
        }
    }
    return count;
}

int count_old_neighbors(int grid[GRID_SIZE][GRID_SIZE], int row, int col) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE && !(i == row && j == col)) {
                if (grid[i][j] == -1)  // count living and dying cells
                    count++;
            }
        }
    }
    return count;
}

void update_grid(int grid[GRID_SIZE][GRID_SIZE]) {
    int new_grid[GRID_SIZE][GRID_SIZE];
	
	// calculate the new state of each cell based on its neighbors
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            int count = count_healthy_neighbors(grid, i, j);
            int count_old = count_old_neighbors(grid, i, j);
            if (grid[i][j] == 0) 
			{ 
                //if (count == 3 || count_old > 3)
                if (count == 3 || count == 6 || count_old >= 3)
                    new_grid[i][j] = 3; // mark as new
				else
					new_grid[i][j] = 0;
			}
			else if (grid[i][j] == -1) // dying cell
            	new_grid[i][j] = 0; // mark as dead
			else if (grid[i][j] == 3) // egg cell
            	new_grid[i][j] = 2; // mark as new cell
			else if (grid[i][j] == 1) // living cell
            	new_grid[i][j] = -1; // dying cell
			else if (grid[i][j] == 2) // new cell
                    new_grid[i][j] = 1; // mark as alive
            else // no cell
			{	
				// living cell
                if (count < 2 || count > 3) 
                    new_grid[i][j] = -1; // mark as dying
			}
        }
    }

	// copy new grid to original grid
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = new_grid[i][j];
        }
    }
	usleep(50000);
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
