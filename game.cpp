#include <iostream>
#include <vector>

#include "game.h"
#include "ui_game.h"
/*=======================
 * GAME LOGIC:
----------------------*/
Game::Game(QWidget *parent) : QDialog(parent), ui(new Ui::Game) {
  ui->setupUi(this);
}

Game::~Game() { delete ui; }

// Class for board grid to be used for polymorphism if othe games are create ex.
// tic tac toe and battleship
class BoardGrid {
  // Private variables for showcasing encapulation
 protected:
  std::vector<std::vector<char>> board;
  int rows;
  int cols;

 public:
  // Empty Vector for reuse as a 2d game grid
  BoardGrid(int r, int c)
      : rows(r), cols(c), board(r, std::vector<char>(c, ' ')) {}

  ~BoardGrid() = default;

  // Getter for number of rows
  int getRows() { return rows; }

  // Getter for number of columns
  int getCols() { return cols; }

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

  // Display board to console for testing ***THIS CAN BE REMOVED AFTER GUI IS
  // IMPLEMETED***
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

// Showing inheritance and potential for polymorphism
class Connect4Board : public BoardGrid {
 public:
  // Initialize board to be a 6x7 grid
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

  // Method for checking if 4 pieces are connected either 'R' or 'B'
  bool checkWin(char piece) {
    // Check horizontal wins
    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols - 3; c++) {
        if (board[r][c] == piece && board[r][c + 1] == piece &&
            board[r][c + 2] == piece && board[r][c + 3] == piece) {
          return true;
        }
      }
    }

    // Check vertical wins
    for (int r = 0; r < rows - 3; r++) {
      for (int c = 0; c < cols; c++) {
        if (board[r][c] == piece && board[r + 1][c] == piece &&
            board[r + 2][c] == piece && board[r + 3][c] == piece) {
          return true;
        }
      }
    }
    /*
Diagnal Checks
    */
    // Check diagonal wins (top-left to bottom-right)
    for (int r = 0; r < rows - 3; r++) {
      for (int c = 0; c < cols - 3; c++) {
        if (board[r][c] == piece && board[r + 1][c + 1] == piece &&
            board[r + 2][c + 2] == piece && board[r + 3][c + 3] == piece) {
          return true;
        }
      }
    }

    // Check diagonal wins (top-right to bottom-left)
    for (int r = 0; r < rows - 3; r++) {
      for (int c = 3; c < cols; c++) {
        if (board[r][c] == piece && board[r + 1][c - 1] == piece &&
            board[r + 2][c - 2] == piece && board[r + 3][c - 3] == piece) {
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

/*This will be called when we click a button
  to drop a piece. Only game logic side, not UI side.  */
Connect4Board ourGameBoard;

char Player1Piece = 'B';
char Player2Piece = 'R';
// Calls methods needed for column and row calls
void Game::DropInColumn(int column, int row) {
    // For now, we always drop Player 1's piece
    if (ourGameBoard.dropPiece(column, Player1Piece)) {
        HighlightCell(column, row, Player1Piece);  // Method call directly
    }
}
/*=======================
 * UI SECTION:
 * By Chris:
 *  Rows 1-8 event handlers
 *  To quick search, CTRL+F
 *  Then type for ex.:
 *  "Collumn 1 Row 8"
 *  Each handler is commented.
----------------------*/

/*=====================
 * Row 1
 ====================*/


void Game::on_Coll1R1_clicked() {
    DropInColumn(1, 1); // Column 1 Row 1
}

void Game::on_Coll2R1_clicked() {
    DropInColumn(2, 1); // Column 2 Row 1
}

void Game::on_Coll3R1_clicked() {
    DropInColumn(3, 1); // Column 3 Row 1
}

void Game::on_Coll4R1_clicked() {
    DropInColumn(4, 1); // Column 4 Row 1
}

void Game::on_Coll5R1_clicked() {
    DropInColumn(5, 1); // Column 5 Row 1
}

void Game::on_Coll6R1_clicked() {
    DropInColumn(6, 1); // Column 6 Row 1
}

void Game::on_Coll7R1_clicked() {
    DropInColumn(7, 1); // Column 7 Row 1
}

void Game::on_Coll8R1_clicked() {
    DropInColumn(8, 1); // Column 8 Row 1
}

void Game::on_Coll9R1_clicked() {
    DropInColumn(9, 1); // Column 9 Row 1
}

void Game::on_Coll10R1_clicked() {
    DropInColumn(10, 1); // Column 10 Row 1
}

void Game::on_Coll11R1_clicked() {
    DropInColumn(11, 1); // Column 11 Row 1
}

void Game::on_Coll12R1_clicked() {
    DropInColumn(12, 1); // Column 12 Row 1
}

void Game::on_Coll13R1_clicked() {
    DropInColumn(13, 1); // Column 13 Row 1
}

/*=====================
 * Row 2
 ====================*/

void Game::on_Coll1R2_clicked() {
    DropInColumn(1, 2); // Column 1 Row 2
}

void Game::on_Coll2R2_clicked() {
    DropInColumn(2, 2); // Column 2 Row 2
}

void Game::on_Coll3R2_clicked() {
    DropInColumn(3, 2); // Column 3 Row 2
}

void Game::on_Coll4R2_clicked() {
    DropInColumn(4, 2); // Column 4 Row 2
}

void Game::on_Coll5R2_clicked() {
    DropInColumn(5, 2); // Column 5 Row 2
}

void Game::on_Coll6R2_clicked() {
    DropInColumn(6, 2); // Column 6 Row 2
}

void Game::on_Coll7R2_clicked() {
    DropInColumn(7, 2); // Column 7 Row 2
}

void Game::on_Coll8R2_clicked() {
    DropInColumn(8, 2); // Column 8 Row 2
}

void Game::on_Coll9R2_clicked() {
    DropInColumn(9, 2); // Column 9 Row 2
}

void Game::on_Coll10R2_clicked() {
    DropInColumn(10, 2); // Column 10 Row 2
}

void Game::on_Coll11R2_clicked() {
    DropInColumn(11, 2); // Column 11 Row 2
}

void Game::on_Coll12R2_clicked() {
    DropInColumn(12, 2); // Column 12 Row 2
}

void Game::on_Coll13R2_clicked() {
    DropInColumn(13, 2); // Column 13 Row 2
}

/*=====================
 * Row 3
 ====================*/
void Game::on_Coll1R3_clicked() {
    DropInColumn(1, 3); // Column 1 Row 3
}

void Game::on_Coll2R3_clicked() {
    DropInColumn(2, 3); // Column 2 Row 3
}

void Game::on_Coll3R3_clicked() {
    DropInColumn(3, 3); // Column 3 Row 3
}

void Game::on_Coll4R3_clicked() {
    DropInColumn(4, 3); // Column 4 Row 3
}

void Game::on_Coll5R3_clicked() {
    DropInColumn(5, 3); // Column 5 Row 3
}

void Game::on_Coll6R3_clicked() {
    DropInColumn(6, 3); // Column 6 Row 3
}

void Game::on_Coll7R3_clicked() {
    DropInColumn(7, 3); // Column 7 Row 3
}

void Game::on_Coll8R3_clicked() {
    DropInColumn(8, 3); // Column 8 Row 3
}

void Game::on_Coll9R3_clicked() {
    DropInColumn(9, 3); // Column 9 Row 3
}

void Game::on_Coll10R3_clicked() {
    DropInColumn(10, 3); // Column 10 Row 3
}

void Game::on_Coll11R3_clicked() {
    DropInColumn(11, 3); // Column 11 Row 3
}

void Game::on_Coll12R3_clicked() {
    DropInColumn(12, 3); // Column 12 Row 3
}

void Game::on_Coll13R3_clicked() {
    DropInColumn(13, 3); // Column 13 Row 3
}

/*=====================
 * Row 4
 ====================*/

void Game::on_Coll1R4_clicked() {
    DropInColumn(1, 4); // Column 1 Row 4
}

void Game::on_Coll2R4_clicked() {
    DropInColumn(2, 4); // Column 2 Row 4
}

void Game::on_Coll3R4_clicked() {
    DropInColumn(3, 4); // Column 3 Row 4
}

void Game::on_Coll4R4_clicked() {
    DropInColumn(4, 4); // Column 4 Row 4
}

void Game::on_Coll5R4_clicked() {
    DropInColumn(5, 4); // Column 5 Row 4
}

void Game::on_Coll6R4_clicked() {
    DropInColumn(6, 4); // Column 6 Row 4
}

void Game::on_Coll7R4_clicked() {
    DropInColumn(7, 4); // Column 7 Row 4
}

void Game::on_Coll8R4_clicked() {
    DropInColumn(8, 4); // Column 8 Row 4
}

void Game::on_Coll9R4_clicked() {
    DropInColumn(9, 4); // Column 9 Row 4
}

void Game::on_Coll10R4_clicked() {
    DropInColumn(10, 4); // Column 10 Row 4
}

void Game::on_Coll11R4_clicked() {
    DropInColumn(11, 4); // Column 11 Row 4
}

void Game::on_Coll12R4_clicked() {
    DropInColumn(12, 4); // Column 12 Row 4
}

void Game::on_Coll13R4_clicked() {
    DropInColumn(13, 4); // Column 13 Row 4
}
/*=====================
 * Row 5
 ====================*/

void Game::on_Coll1R5_clicked() {
    DropInColumn(1, 5); // Column 1 Row 5
}

void Game::on_Coll2R5_clicked() {
    DropInColumn(2, 5); // Column 2 Row 5
}

void Game::on_Coll3R5_clicked() {
    DropInColumn(3, 5); // Column 3 Row 5
}

void Game::on_Coll4R5_clicked() {
    DropInColumn(4, 5); // Column 4 Row 5
}

void Game::on_Coll5R5_clicked() {
    DropInColumn(5, 5); // Column 5 Row 5
}

void Game::on_Coll6R5_clicked() {
    DropInColumn(6, 5); // Column 6 Row 5
}

void Game::on_Coll7R5_clicked() {
    DropInColumn(7, 5); // Column 7 Row 5
}

void Game::on_Coll8R5_clicked() {
    DropInColumn(8, 5); // Column 8 Row 5
}

void Game::on_Coll9R5_clicked() {
    DropInColumn(9, 5); // Column 9 Row 5
}

void Game::on_Coll10R5_clicked() {
    DropInColumn(10, 5); // Column 10 Row 5
}

void Game::on_Coll11R5_clicked() {
    DropInColumn(11, 5); // Column 11 Row 5
}

void Game::on_Coll12R5_clicked() {
    DropInColumn(12, 5); // Column 12 Row 5
}

void Game::on_Coll13R5_clicked() {
    DropInColumn(13, 5); // Column 13 Row 5
}

/*=====================
 * Row 6
 ====================*/

void Game::on_Coll1R6_clicked() {
    DropInColumn(1, 6); // Column 1 Row 6
}

void Game::on_Coll2R6_clicked() {
    DropInColumn(2, 6); // Column 2 Row 6
}

void Game::on_Coll3R6_clicked() {
    DropInColumn(3, 6); // Column 3 Row 6
}

void Game::on_Coll4R6_clicked() {
    DropInColumn(4, 6); // Column 4 Row 6
}

void Game::on_Coll5R6_clicked() {
    DropInColumn(5, 6); // Column 5 Row 6
}

void Game::on_Coll6R6_clicked() {
    DropInColumn(6, 6); // Column 6 Row 6
}

void Game::on_Coll7R6_clicked() {
    DropInColumn(7, 6); // Column 7 Row 6
}

void Game::on_Coll8R6_clicked() {
    DropInColumn(8, 6); // Column 8 Row 6
}

void Game::on_Coll9R6_clicked() {
    DropInColumn(9, 6); // Column 9 Row 6
}

void Game::on_Coll10R6_clicked() {
    DropInColumn(10, 6); // Column 10 Row 6
}

void Game::on_Coll11R6_clicked() {
    DropInColumn(11, 6); // Column 11 Row 6
}

void Game::on_Coll12R6_clicked() {
    DropInColumn(12, 6); // Column 12 Row 6
}

void Game::on_Coll13R6_clicked() {
    DropInColumn(13, 6); // Column 13 Row 6
}
/*=====================
 * Row 7
 ====================*/

void Game::on_Coll1R7_clicked() {
    DropInColumn(1, 7); // Column 1 Row 7
}

void Game::on_Coll2R7_clicked() {
    DropInColumn(2, 7); // Column 2 Row 7
}

void Game::on_Coll3R7_clicked() {
    DropInColumn(3, 7); // Column 3 Row 7
}

void Game::on_Coll4R7_clicked() {
    DropInColumn(4, 7); // Column 4 Row 7
}

void Game::on_Coll5R7_clicked() {
    DropInColumn(5, 7); // Column 5 Row 7
}

void Game::on_Coll6R7_clicked() {
    DropInColumn(6, 7); // Column 6 Row 7
}

void Game::on_Coll7R7_clicked() {
    DropInColumn(7, 7); // Column 7 Row 7
}

void Game::on_Coll8R7_clicked() {
    DropInColumn(8, 7); // Column 8 Row 7
}

void Game::on_Coll9R7_clicked() {
    DropInColumn(9, 7); // Column 9 Row 7
}

void Game::on_Coll10R7_clicked() {
    DropInColumn(10, 7); // Column 10 Row 7
}

void Game::on_Coll11R7_clicked() {
    DropInColumn(11, 7); // Column 11 Row 7
}

void Game::on_Coll12R7_clicked() {
    DropInColumn(12, 7); // Column 12 Row 7
}

void Game::on_Coll13R7_clicked() {
    DropInColumn(13, 7); // Column 13 Row 7
}

/*=====================
 * Row 8
 ====================*/

void Game::on_Coll1R8_clicked() {
    DropInColumn(1, 8); // Column 1 Row 8
}

void Game::on_Coll2R8_clicked() {
    DropInColumn(2, 8); // Column 2 Row 8
}

void Game::on_Coll3R8_clicked() {
    DropInColumn(3, 8); // Column 3 Row 8
}

void Game::on_Coll4R8_clicked() {
    DropInColumn(4, 8); // Column 4 Row 8
}

void Game::on_Coll5R8_clicked() {
    DropInColumn(5, 8); // Column 5 Row 8
}

void Game::on_Coll6R8_clicked() {
    DropInColumn(6, 8); // Column 6 Row 8
}

void Game::on_Coll7R8_clicked() {
    DropInColumn(7, 8); // Column 7 Row 8
}

void Game::on_Coll8R8_clicked() {
    DropInColumn(8, 8); // Column 8 Row 8
}

void Game::on_Coll9R8_clicked() {
    DropInColumn(9, 8); // Column 9 Row 8
}

void Game::on_Coll10R8_clicked() {
    DropInColumn(10, 8); // Column 10 Row 8
}

void Game::on_Coll11R8_clicked() {
    DropInColumn(11, 8); // Column 11 Row 8
}

void Game::on_Coll12R8_clicked() {
    DropInColumn(12, 8); // Column 12 Row 8
}

void Game::on_Coll13R8_clicked() {
    DropInColumn(13, 8); // Column 13 Row 8
}
