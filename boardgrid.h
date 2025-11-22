#ifndef BOARDGRID_H
#define BOARDGRID_H
#include <vector>
using namespace std;

/* Class for board grid to be used for polymorphism if other
games are created ex.tic tac toe and battleship*/
class BoardGrid {
    // Private variable for showcasing encapsulation
private:
    vector<vector<char>> board;

protected:
    int rows;
    int cols;

public:
    // Default constructor
    BoardGrid();

    // Parameterized constructor
    BoardGrid(int r, int c);

    // Destructor
    ~BoardGrid() = default;

    // Getter methods - make them const
    int getRows() const;
    int getCols() const;

    // Getter for cell - make it const and safe
    char getCell(int row, int col) const;

    // Sets a cell value
    bool setCell(int row, int col, char symbol);

    // Resets all cells in 2d vector to empty
    void clearBoard();

    // Check if board is full
    bool isFull() const;
};

#endif // BOARDGRID_H
