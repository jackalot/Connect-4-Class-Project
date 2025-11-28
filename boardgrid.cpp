#include "boardgrid.h"

// Default constructor
BoardGrid::BoardGrid() : rows(0), cols(0) {}

// Parameterized constructor
BoardGrid::BoardGrid(int r, int c)
    : rows(r), cols(c), board(r, vector<char>(c, ' ')) {}

int BoardGrid::getRows() const { return rows; }
int BoardGrid::getCols() const { return cols; }

char BoardGrid::getCell(int row, int col) const {
    if (row >= 0 && row < rows && col >= 0 && col < cols)
        return board[row][col];
    return ' '; // Safe default
}

// Default clearBoard
void BoardGrid::clearBoard() {
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            setCell(y, x, ' ');
}

// Full check
bool BoardGrid::isFull() const {
    for (const auto &row : board)
        for (char cell : row)
            if (cell == ' ')
                return false;
    return true;
}
