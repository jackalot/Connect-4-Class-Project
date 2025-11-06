#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);
}

Game::~Game()
{
    delete ui;
}
#include <iostream>
#include <vector>

class Connect4Board {
//Private variables for showcasing encapulation
private:
    std::vector<std::vector<char>> board;
    int rows;
    int cols;

public:
    // Initialize vector to have 6 rows and 7 columns with empty space.
    Connect4Board() : rows(6), cols(7), board(6, std::vector<char>(7, ' ')) {}

    // Getter for number of rows
    int getRows() const {
        return rows;
    }

    // Getter for number of columns
    int getCols() {
        return cols;
    }

    // Gets the character which will be 'R' or 'B' at specified cell
    char getCell(int row, int col) {
        return board[row][col];
    }

    // Resets all cells in 2d vector to empty
    void clearBoard() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                board[i][j] = ' ';
            }
        }
    }

    //Virtual destructor does nothing yet but can be used for inheritance and polymorphism
    virtual ~Connect4Board() = default;

	// Displays a board for testing in console ***Is not needed after GUI and player input is implemented***
    void display() {

		//Displays white space for peices
        std::cout << std::endl << "  ";
        for (int j = 0; j < cols; j++) {
            std::cout << j << " ";
        }

		//Displays border for rows
        std::cout << std::endl << "  ";
        for (int j = 0; j < cols; j++) {
            std::cout << "- ";
        }
        std::cout << std::endl;

		//Displays border for columns
        for (int i = 0; i < rows; i++) {
            std::cout << i << "|";
            for (int j = 0; j < cols; j++) {
                std::cout << board[i][j] << "|";
            }
            std::cout << std::endl;
        }
        std::cout << "  ";
        for (int j = 0; j < cols; j++) {
            std::cout << "- ";
        }
        std::cout << std::endl << std::endl;
    }

    // Bool to drop peice in a column and check if it's full
    bool dropPiece(int col, char symbol) {

		//Checks row 1 by 1 to drop peice to lowest empty space until full
        for (int row = rows - 1; row >= 0; row--) {
            if (board[row][col] == ' ') {
                board[row][col] = symbol;
                return true;
            }
        }

		//Column is full and won't allow placement
        return false;
    }

    // Method checks if player symbol 'R' or 'B' won
    bool checkWin(char symbol) {

        // Check horizontal wins
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols - 3; c++) {
                if (board[r][c] == symbol &&
                    board[r][c + 1] == symbol &&
                    board[r][c + 2] == symbol &&
                    board[r][c + 3] == symbol) {
                    return true;
                }
            }
        }

        // Check vertical wins
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 0; c < cols; c++) {
                if (board[r][c] == symbol &&
                    board[r + 1][c] == symbol &&
                    board[r + 2][c] == symbol &&
                    board[r + 3][c] == symbol) {
                    return true;
                }
            }
        }

        // Check diagonal wins (top-left to bottom-right)
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 0; c < cols - 3; c++) {
                if (board[r][c] == symbol &&
                    board[r + 1][c + 1] == symbol &&
                    board[r + 2][c + 2] == symbol &&
                    board[r + 3][c + 3] == symbol) {
                    return true;
                }
            }
        }

        // Check diagonal wins (top-right to bottom-left)
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 3; c < cols; c++) {
                if (board[r][c] == symbol &&
                    board[r + 1][c - 1] == symbol &&
                    board[r + 2][c - 2] == symbol &&
                    board[r + 3][c - 3] == symbol) {
                    return true;
                }
            }
        }

        return false;
    }

    // Checks if the board is completely full for a draw
    bool isFull() {
        for (int c = 0; c < cols; c++) {
            if (board[0][c] == ' ') {
                return false;
            }
        }
        return true;
    }
};

int main() {

    Connect4Board myGame;

    //Test to display board
    myGame.display();

    //Test drop piece method can change column between 0-6 and use 'R' or 'B'
    myGame.dropPiece(3, 'R');
    myGame.display();

    //Same test but for next player
    myGame.dropPiece(3, 'B');
    myGame.display();

    return 0;
}