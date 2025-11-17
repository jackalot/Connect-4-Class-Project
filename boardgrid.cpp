#include "boardgrid.h"

// Default constructor implementation
BoardGrid::BoardGrid() : rows(0), cols(0) {}

// Parameterized constructor implementation
BoardGrid::BoardGrid(int r, int c)
    : rows(r), cols(c), board(r, std::vector<char>(c, ' ')) {}

// Const getter implementations
int BoardGrid::getRows() const { return rows; }
int BoardGrid::getCols() const { return cols; }

// Safe cell getter
char BoardGrid::getCell(int row, int col) const {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return board[row][col];
    }
    return ' '; // or throw an exception
}

// Cell setter
bool BoardGrid::setCell(int row, int col, char symbol) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        board[row][col] = symbol;
        return true;
    }
    return false;
}

// Clear board implementation
void BoardGrid::clearBoard() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            setCell(i, j, ' ');
        }
    }
}

// Full board check
bool BoardGrid::isFull() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}
