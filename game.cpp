#include "game.h"
#include "ui_game.h"
#include <iostream>
#include <vector>
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

// Class for board grid to be used for polymorphism if othe games are create ex. tic tac toe and battleship
class BoardGrid {

//Private variables for showcasing encapulation
protected:
    std::vector<std::vector<char>> board;
    int rows;
    int cols;

public:
    
	//Empty Vector for reuse as a 2d game grid 
    BoardGrid(int r, int c) : rows(r), cols(c), board(r, std::vector<char>(c, ' ')) {}

    
    ~BoardGrid() = default;

    // Getter for number of rows
    int getRows() {
        return rows;
    }

    // Getter for number of columns
    int getCols() {
        return cols;
    }

    // Gets the character at specified cell
    char getCell(int row, int col) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            return board[row][col];
        }
    }

    // Sets a cell value
    bool setCell(int row, int col, char symbol) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            board[row][col] = symbol;
            return true;
        }
        return false;
    }

    // Resets all cells in 2d vector to empty
    void clearBoard() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                board[i][j] = ' ';
            }
        }
    }

    // Display board to console for testing ***THIS CAN BE REMOVED AFTER GUI IS IMPLEMETED***
    void display() {
        std::cout << std::endl << "  ";
        for (int j = 0; j < cols; j++) {
            std::cout << j << " ";
        }

        std::cout << std::endl << "  ";
        for (int j = 0; j < cols; j++) {
            std::cout << "- ";
        }
        std::cout << std::endl;

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

    // Method for checking if board is full
    bool isFull() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board[i][j] == ' ') {
                    return false;
                }
            }
        }
        return true;
    }
};

//Showing inheritance and potential for polymorphism 
class Connect4Board : public BoardGrid {
public:
    
	//Initialize board to be a 6x7 grid
    Connect4Board() : BoardGrid(6, 7) {}

    // Method to define column bounds for pieces
    bool dropPiece(int col, char piece) {
        if (col < 0 || col >= cols) {
            return false;
        }

        // Drop piece to lowest empty space
        for (int row = rows - 1; row >= 0; row--) {
            if (board[row][col] == ' ') {
                board[row][col] = piece;
                return true;
            }
        }

        // Column is full
        return false;
    }

    //Method for checking if 4 pieces are connected either 'R' or 'B'
    bool checkWin(char piece) {
	
        // Check horizontal wins
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols - 3; c++) {
                if (board[r][c] == piece &&
                    board[r][c + 1] == piece &&
                    board[r][c + 2] == piece &&
                    board[r][c + 3] == piece) {
                    return true;
                }
            }
        }

        // Check vertical wins
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 0; c < cols; c++) {
                if (board[r][c] == piece &&
                    board[r + 1][c] == piece &&
                    board[r + 2][c] == piece &&
                    board[r + 3][c] == piece) {
                    return true;
                }
            }
        }

        // Check diagonal wins (top-left to bottom-right)
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 0; c < cols - 3; c++) {
                if (board[r][c] == piece &&
                    board[r + 1][c + 1] == piece &&
                    board[r + 2][c + 2] == piece &&
                    board[r + 3][c + 3] == piece) {
                    return true;
                }
            }
        }

        // Check diagonal wins (top-right to bottom-left)
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 3; c < cols; c++) {
                if (board[r][c] == piece &&
                    board[r + 1][c - 1] == piece &&
                    board[r + 2][c - 2] == piece &&
                    board[r + 3][c - 3] == piece) {
                    return true;
                }
            }
        }

        return false;
    }

    // Checks if top row is full for a draw
    bool isFull() {
        for (int c = 0; c < cols; c++) {
            if (board[0][c] == ' ') {
                return false;
            }
        }
        return true;
    }
};
