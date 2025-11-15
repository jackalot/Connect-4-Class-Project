#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <QMessageBox>
#include "game.h"
#include "ui_game.h"
/*=======================
 * GAME LOGIC:
----------------------*/
Game::Game(QWidget *parent) : QDialog(parent), ui(new Ui::Game) {
    ui->setupUi(this);
    resetBoard();   // we only call the function
}


Game::~Game() { delete ui; }

void Game::setSinglePlayer(bool singlePlayer)
{
    m_singlePlayer = singlePlayer;
    m_currentPlayer = 1;  // 1st player always starts
}


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
    // we use 8 rows x 13 columns
    Connect4Board() : BoardGrid(8, 13) {}

    // We drop the piece and returns row where it falled
    bool dropPiece(int col, char piece, int &outRow) {
        if (col < 0 || col >= cols) {
            return false;
        }

        // from down to top we look for the first empty space
        for (int row = rows - 1; row >= 0; row--) {
            if (board[row][col] == ' ') {
                board[row][col] = piece;
                outRow = row;
                return true;
            }
        }

        //full colum
        return false;
    }

    // old version to not break anything
    bool dropPiece(int col, char piece) {
        int dummyRow;
        return dropPiece(col, piece, dummyRow);
    }

    // Can drop on this colum?
    bool canDrop(int col) const {
        if (col < 0 || col >= cols) return false;
        return board[0][col] == ' ';
    }

    // Méthod to check if  4 in row
    bool checkWin(char piece) {
        // Horizontal
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols - 3; c++) {
                if (board[r][c] == piece && board[r][c + 1] == piece &&
                    board[r][c + 2] == piece && board[r][c + 3] == piece) {
                    return true;
                }
            }
        }

        // Vertical
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 0; c < cols; c++) {
                if (board[r][c] == piece && board[r + 1][c] == piece &&
                    board[r + 2][c] == piece && board[r + 3][c] == piece) {
                    return true;
                }
            }
        }

        // Diagonal ↘
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 0; c < cols - 3; c++) {
                if (board[r][c] == piece && board[r + 1][c + 1] == piece &&
                    board[r + 2][c + 2] == piece && board[r + 3][c + 3] == piece) {
                    return true;
                }
            }
        }

        // Diagonal ↙
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

    // full board (tie)
    bool isFull() {
        for (int c = 0; c < cols; c++) {
            if (board[0][c] == ' ') {
                return false;
            }
        }
        return true;
    }
};
class BasicAI {
public:
    BasicAI(char aiPiece, char humanPiece)
        : m_ai(aiPiece), m_human(humanPiece)
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    // Returns a valid column (0..cols-1) or -1 if there are no moves
    int chooseMove(Connect4Board &board) {
        int cols = board.getCols();
        int rows = board.getRows();

        // 1) Try to win this turn
        for (int c = 0; c < cols; ++c) {
            if (!board.canDrop(c)) continue;

            Connect4Board temp = board;   // we copy the board
            int rTmp = -1;
            temp.dropPiece(c, m_ai, rTmp);
            if (temp.checkWin(m_ai)) {
                return c; // If I win, I play here
            }
        }

        // 2) Block the player if they can win on their next turn
        for (int c = 0; c < cols; ++c) {
            if (!board.canDrop(c)) continue;

            Connect4Board temp = board;
            int rTmp = -1;
            temp.dropPiece(c, m_human, rTmp);
            if (temp.checkWin(m_human)) {
                return c; // block his winning move
            }
        }

        // 3) Choosing a 'good' move with a simple heuristic
        std::vector<int> validCols;
        for (int c = 0; c < cols; ++c) {
            if (board.canDrop(c)) {
                validCols.push_back(c);
            }
        }
        if (validCols.empty()) return -1;

        int bestScore = -1;
        std::vector<int> bestCols;
        int center = cols / 2;

        for (int c : validCols) {
            // base: prefer columns closer to the center
            int score = 10 - std::abs(c - center);

            //We try to place our piece and see how it looks
            Connect4Board temp = board;
            int rTmp = -1;
            temp.dropPiece(c, m_ai, rTmp);

            // score points for our tokens around (horizontally/vertically)
            score += countAdjacents(temp, rTmp, c, m_ai);

            if (score > bestScore) {
                bestScore = score;
                bestCols.clear();
                bestCols.push_back(c);
            } else if (score == bestScore) {
                bestCols.push_back(c);
            }
        }

        // If there are several with the same score, we choose one at random
        int idx = std::rand() % bestCols.size();
        return bestCols[idx];
    }

private:
    char m_ai;
    char m_human;

    // Counts neighboring pieces (up/down/left/right)
    int countAdjacents(Connect4Board &b, int r, int c, char piece) {
        int score = 0;

        auto addIf = [&](int rr, int cc) {
            if (rr >= 0 && rr < b.getRows() &&
                cc >= 0 && cc < b.getCols() &&
                b.getCell(rr, cc) == piece) {
                score++;
            }
        };

        addIf(r, c - 1); // left
        addIf(r, c + 1); // right
        addIf(r - 1, c); // up
        addIf(r + 1, c); // down

        return score;
    }
};


/*This will be called when we click a button
  to drop a piece. Only game logic side, not UI side.  */
Connect4Board ourGameBoard;

char Player1Piece = 'B';   // Blue
char Player2Piece = 'R';   // Red
void Game::resetBoard()
{
    ourGameBoard.clearBoard();
    m_currentPlayer = 1;
    m_gameOver = false;
}

BasicAI g_ai(Player2Piece, Player1Piece);
void Game::DropInColumn(int column, int /*rowClicked*/)
{
    if (m_gameOver) {
        // If the game is over, ignore clicks
        return;
    }

    // GUI columns are 1..13, board columns are 0..12
    int colIndex = column - 1;
    int placedRow = -1;

    // Current player's piece
    char piece = (m_currentPlayer == 1) ? Player1Piece : Player2Piece;

    // Try to drop a piece in that column
    if (!ourGameBoard.dropPiece(colIndex, piece, placedRow)) {
        // Column is full: do nothing
        return;
    }

    // Color the correct cell according to the board (row + 1, col + 1 for the GUI)
    HighlightCell(colIndex + 1, placedRow + 1, piece);

    // 1) Did the player who just moved win?
    if (ourGameBoard.checkWin(piece)) {
        m_gameOver = true;

        if (m_singlePlayer) {
            if (m_currentPlayer == 1) {
                QMessageBox::information(
                    this,
                    "You won!",
                    "🎉 Congratulations! You beat the AI. 😎"
                    );
            } else {
                QMessageBox::information(
                    this,
                    "You lost",
                    "🤖 The AI has won this game. Try again!"
                    );
            }
        } else {
            QString msg;
            if (m_currentPlayer == 1) {
                msg = "🎉 Player 1 (Blue) has won the game.";
            } else {
                msg = "🎉 Player 2 (Red) has won the game.";
            }
            QMessageBox::information(
                this,
                "We have a winner",
                msg
                );
        }

        return;
    }

    // 2) Draw? (board is full)
    if (ourGameBoard.isFull()) {
        m_gameOver = true;
        QMessageBox::information(
            this,
            "Draw",
            "The board is full. It's a draw. 🤝"
            );
        return;
    }

    // 3) If it is single-player mode and the human just played, now the AI plays
    if (m_singlePlayer && m_currentPlayer == 1) {
        m_currentPlayer = 2;  // AI's turn

        int aiCol = g_ai.chooseMove(ourGameBoard);
        if (aiCol >= 0) {
            int aiRow = -1;
            if (ourGameBoard.dropPiece(aiCol, Player2Piece, aiRow)) {
                HighlightCell(aiCol + 1, aiRow + 1, Player2Piece);

                // Check if the AI wins
                if (ourGameBoard.checkWin(Player2Piece)) {
                    m_gameOver = true;
                    QMessageBox::information(
                        this,
                        "You lost",
                        "🤖 The AI has won this game. Keep practicing!"
                        );
                    return;
                }

                // Check for a draw after the AI's move
                if (ourGameBoard.isFull()) {
                    m_gameOver = true;
                    QMessageBox::information(
                        this,
                        "Draw",
                        "The board is full. It's a draw. 🤝"
                        );
                    return;
                }
            }
        }

        // If the game continues, give the turn back to player 1
        m_currentPlayer = 1;
    } else {
        // Two-player mode: alternate between Player 1 and Player 2
        m_currentPlayer = (m_currentPlayer == 1) ? 2 : 1;
    }
}



// Common logic when clicking any box

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
