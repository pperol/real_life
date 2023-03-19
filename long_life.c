#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "./mlx/mlx.h"

#define GRID_SIZE 115

void	initialize_grid(int grid[GRID_SIZE][GRID_SIZE])
{
	int	i;
	int	j;
	int	count;
	int num_to_fill;

	count = 0;
	// Seed the random number generator
    srand(time(NULL));

	// Calculate the number of elements to fill
    num_to_fill = GRID_SIZE * GRID_SIZE / 6;

    // Loop through the grid and fill the required number of elements
    i = 0;
	while (i < GRID_SIZE) 
	{
        j = 0;
		while (j < GRID_SIZE) 
		{
            // try also %4 or %5 etc.
			if (count < num_to_fill && rand() % 4 == 0) 
			{
                // Fill this element with a random number from (-1) 0 to 3
                grid[i][j] = rand() % 5 - 1;
                //grid[i][j] = rand() % 5;
                count++;
            } 
			else 
                // Otherwise, fill with 0
                grid[i][j] = 0;
           j++;
        }
		i++;
    }
}

void	print_grid(int grid[GRID_SIZE][GRID_SIZE]) 
{
	int i;
	int j;

	i = 0;
	while (i < GRID_SIZE)
	{
		j = 0;
		while (j < GRID_SIZE)
		{
			if (grid[i][j] == 1)
				// living cell
				write(1, "#", 1);
			else if (grid[i][j] == 2)
				// growing cell
				write(1, "O", 1);
			else if (grid[i][j] == -1)
				// dying cell
				write(1, "+", 1);
			else if (grid[i][j] == 3)
				// new born cell
				write(1, "o", 1);
			else
				write(1, " ", 1);
			j++;
		}
		i++;
		write(1, "\n", 1);
    }
}

int	count_spaces(int grid[GRID_SIZE][GRID_SIZE], int row, int col) 
{
	int count;
	int i;
	int j;

	count  = 0;
	i = row - 1;
	while (i <= row + 1)
	{
		j = col - 1;
		while (j <= col + 1)
		{		
			if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE && !(i == row && j == col)) 
			{
                // count spaces:
                if (grid[i][j] == 0)
                    count++;
            }
			j++;
        }
		i++;
    }
    return (count);
}

int	count_cells(int grid[GRID_SIZE][GRID_SIZE], int row, int col) 
{
	int count;
	int i;
	int j;

	count  = 0;
	i = row - 1;
	while (i <= row + 1)
	{
		j = col - 1;
		while (j <= col + 1)
		{		
			if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE && !(i == row && j == col)) 
			{
                // count cells:
                if (grid[i][j] != 0)
                    count++;
            }
			j++;
        }
		i++;
    }
    return (count);
}

int	count_healthy_neighbors(int grid[GRID_SIZE][GRID_SIZE], int row, int col)
{
	int count;
	int i;
	int j;

	count  = 0;
	i = row - 1;
	while (i <= row + 1)
	{
		j = col - 1;
		while (j <= col + 1)
		{		
			if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE && !(i == row && j == col)) 
			{
				if (grid[i][j] == 1 || grid[i][j] == 2 || grid[i][j] == -1)
				// count fertile cells
                    count++;
            }
			j++;
        }
		i++;
    }
    return (count);
}

int	count_too_many_cells(int grid[GRID_SIZE][GRID_SIZE], int row, int col)
{
	int count;
	int i;
	int j;

	count  = 0;
	i = row - 1;
	while (i <= row + 1)
	{
		j = col - 1;
		while (j <= col + 1)
		{		
			if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE && !(i == row && j == col)) 
			{
				if (grid[i][j] == 1 && grid[i][j] == -1 && grid [i][j] == 2 && grid[i][j] == 3)
				// count total cells
                    count++;
            }
			j++;
        }
		i++;
    }
    return (count);
}

int	count_old_neighbors(int grid[GRID_SIZE][GRID_SIZE], int row, int col) 
{
	int count;
	int i;
	int j;

	count  = 0;
	i = row - 1;
	while (i <= row + 1)
	{
		j = col - 1;
		while (j <= col + 1)
		{		
			if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE && !(i == row && j == col)) 
			{
				if (grid[i][j] == -1)
				// count dying cells
                    count++;
            }
			j++;
        }
		i++;
    }
    return (count);
}

void update_grid(int grid[GRID_SIZE][GRID_SIZE])
{
	int new_grid[GRID_SIZE][GRID_SIZE];
	int count;
	int	cells_number;
	int	space_number;
    int count_old;
	int i;
	int j;
	int n;
	int m;
	
	// calculate the new state of each cell based on its neighbors
	i = 0; 
    while (i < GRID_SIZE) 
	{
		j = 0;
		//for (int j = 0; j < GRID_SIZE; j++) {
		while(j < GRID_SIZE) 
		{
			cells_number = count_cells(grid, i, j);
			space_number = count_spaces(grid, i, j);
			if (space_number > cells_number * 1.6) 
            	count = count_healthy_neighbors(grid, i, j);
			if (space_number < cells_number) 
            	count = count_too_many_cells(grid, i, j);
            	count_old = count_old_neighbors(grid, i, j);
            if (grid[i][j] == 0) 
			{ 
                if (count == 3 || count_old > 3)
                //if (count == 3 || count == 6 || count_old >= 3)
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
					//new_grid[j][i] = -1; // mark as dying
			}
			j++;
		}
		i++;
	}
	// copy new grid to original grid
	m = 0;
    while (m < GRID_SIZE)
	{
		n = 0;
		while (n < GRID_SIZE)
		{
            grid[m][n] = new_grid[m][n];
			n++;
    	}
		m++;
	}
	usleep(100000);
}


int main() {
	int	grid[GRID_SIZE][GRID_SIZE];
	initialize_grid(grid);
	{	
		print_grid(grid);
		while (1)
		{
			update_grid(grid);
			print_grid(grid);
    	}
	}
    return (0);
}
