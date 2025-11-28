#include "boardgrid.h"
#include <algorithm>

// Default constructor (optional, just initializes empty board)
BoardGrid::BoardGrid() : rows(0), cols(0) {}

// Parameterized constructor
BoardGrid::BoardGrid(int r, int c) : rows(r), cols(c) {
    board = std::vector<std::vector<char>>(rows, std::vector<char>(cols, 'E'));
}

// Getters
int BoardGrid::getRows() const { return rows; }
int BoardGrid::getCols() const { return cols; }

// Get a cell value safely
char BoardGrid::getCell(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) return 'E';
    return board[row][col];
}

// Clear board
void BoardGrid::clearBoard() {
    for (auto &row : board) {
        std::fill(row.begin(), row.end(), 'E');
    }
}

// Check if board is full
bool BoardGrid::isFull() const {
    for (auto &row : board) {
        for (auto &cell : row) {
            if (cell == 'E') return false;
        }
    }
    return true;
}
