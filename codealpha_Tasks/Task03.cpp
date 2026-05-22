// task 3: sudoku solver
// codealpha internship - c++ programming
// solves a 9x9 sudoku puzzle using backtracking

#include <iostream>
#include <iomanip>

using namespace std;

const int grid_size = 9;

// prints the sudoku grid in a nice format
void print_grid(int grid[grid_size][grid_size]) {
    cout << "\n+-------+-------+-------+\n";
    for (int row = 0; row < grid_size; row++) {
        cout << "| ";
        for (int col = 0; col < grid_size; col++) {
            if (grid[row][col] == 0)
                cout << ". ";
            else
                cout << grid[row][col] << " ";
            if ((col + 1) % 3 == 0) cout << "| ";
        }
        cout << endl;
        if ((row + 1) % 3 == 0)
            cout << "+-------+-------+-------+\n";
    }
}

// checks if placing num at grid[row][col] is valid
bool is_safe(int grid[grid_size][grid_size], int row, int col, int num) {
    // check the row
    for (int j = 0; j < grid_size; j++) {
        if (grid[row][j] == num) return false;
    }

    // check the column
    for (int i = 0; i < grid_size; i++) {
        if (grid[i][col] == num) return false;
    }

    // check the 3x3 subgrid
    int start_row = row - row % 3;
    int start_col = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[start_row + i][start_col + j] == num) return false;
        }
    }

    return true;
}

// finds the next empty cell (value 0) in the grid
bool find_empty(int grid[grid_size][grid_size], int& row, int& col) {
    for (row = 0; row < grid_size; row++) {
        for (col = 0; col < grid_size; col++) {
            if (grid[row][col] == 0) return true;
        }
    }
    return false; // no empty cell found, puzzle is solved
}

// solves the sudoku using backtracking
bool solve_sudoku(int grid[grid_size][grid_size]) {
    int row, col;

    // if no empty cell, we're done
    if (!find_empty(grid, row, col)) return true;

    // try numbers 1-9
    for (int num = 1; num <= 9; num++) {
        if (is_safe(grid, row, col, num)) {
            grid[row][col] = num; // place the number

            if (solve_sudoku(grid)) return true; // recurse

            grid[row][col] = 0; // backtrack
        }
    }

    return false; // trigger backtracking
}

// allows user to input their own puzzle
void input_puzzle(int grid[grid_size][grid_size]) {
    cout << "enter the sudoku puzzle row by row (use 0 for empty cells):\n";
    for (int i = 0; i < grid_size; i++) {
        cout << "row " << i + 1 << ": ";
        for (int j = 0; j < grid_size; j++) {
            cin >> grid[i][j];
        }
    }
}

int main() {
    cout << "========================================\n";
    cout << "          sudoku solver\n";
    cout << "          codealpha internship\n";
    cout << "========================================\n";

    int grid[grid_size][grid_size];
    int choice;

    cout << "\n1. use sample puzzle\n";
    cout << "2. enter your own puzzle\n";
    cout << "enter choice: ";
    cin >> choice;

    if (choice == 1) {
        // a sample sudoku puzzle (0 means empty)
        int sample[grid_size][grid_size] = {
            {5, 3, 0, 0, 7, 0, 0, 0, 0},
            {6, 0, 0, 1, 9, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {8, 0, 0, 0, 6, 0, 0, 0, 3},
            {4, 0, 0, 8, 0, 3, 0, 0, 1},
            {7, 0, 0, 0, 2, 0, 0, 0, 6},
            {0, 6, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 4, 1, 9, 0, 0, 5},
            {0, 0, 0, 0, 8, 0, 0, 7, 9}
        };
        for (int i = 0; i < grid_size; i++)
            for (int j = 0; j < grid_size; j++)
                grid[i][j] = sample[i][j];
    }
    else {
        input_puzzle(grid);
    }

    cout << "\nunsolved puzzle:";
    print_grid(grid);

    if (solve_sudoku(grid)) {
        cout << "\nsolved puzzle:";
        print_grid(grid);
        cout << "\npuzzle solved successfully!\n";
    }
    else {
        cout << "\nno solution exists for this puzzle.\n";
    }

    return 0;
}
