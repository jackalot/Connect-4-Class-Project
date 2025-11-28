#ifndef BOARDGRID_H
#define BOARDGRID_H
#include <vector>
using namespace std;

/* Class for board grid to be used for polymorphism if other
games are created ex.tic tac toe and battleship*/
class BoardGrid {

protected:
    int rows;
    int cols;
    vector<vector<char>> board;
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

    //With this, it's an abstract class
    // Sets a cell value
    virtual bool setCell(int row, int col, char symbol) = 0;

    // Resets all cells in 2d vector to empty
    void clearBoard();

    // Check if board is full
    bool isFull() const;
};

#endif // BOARDGRID_H
