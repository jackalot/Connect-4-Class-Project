#ifndef BOARDGRID_H
#define BOARDGRID_H

#include <vector>
using namespace std;

// Base class for any board game
class BoardGrid {
protected:
    int rows;
    int cols;
    vector<vector<char>> board;

public:
    BoardGrid();                    // Default constructor
    BoardGrid(int r, int c);        // Parameterized constructor
    virtual ~BoardGrid() = default; // Virtual destructor

    int getRows() const;
    int getCols() const;

    char getCell(int row, int col) const;

    // Pure virtual function to override
    virtual bool setCell(int row, int col, char symbol) = 0;

    // Can override in derived class if needed
    virtual void clearBoard();

    bool isFull() const;
};

#endif
