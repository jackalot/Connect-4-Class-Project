#include <vector>
#include "game.h"
#include "ui_game.h"

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

    // Empty Vector for reuse as a 2d game grid
    BoardGrid(int r, int c)
        : rows(r), cols(c), board(r, vector<char>(c, ' ')) {}

    ~BoardGrid() = default;

    // Getter for number of rows
    int getRows() { return rows; }

    // Getter for number of columns
    int getCols() { return cols; }

    char getCell(int row, int col) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            return board[row][col];
        }
        // Add a default return when out of bounds
        return ' '; // or throw an exception if you prefer
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
                setCell(i, j, ' ');
            }
        }
    }

    /* Method for checking if board is full but only makes sense
    for tic tac toe since connect 4 only need to check top row*/
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

    // Initialize board to be a 8x13 grid
    Connect4Board() : BoardGrid(8, 13) {}

    // Method to define column bounds for pieces
    bool dropPiece(int col, char piece, int &droppedRow) {
        if (col < 0 || col >= cols) {
            return false;
        }

        // Drop piece to lowest empty space and return row
        for (int row = rows - 1; row >= 0; row--) {
            if (getCell(row, col) == ' ') {
                setCell(row, col, piece);
                droppedRow = row;
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
                if (getCell(r, c) == piece && getCell(r, c + 1) == piece &&
                    getCell(r, c + 2) == piece && getCell(r, c + 3) == piece) {
                    return true;
                }
            }
        }

        // Check vertical wins
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 0; c < cols; c++) {
                if (getCell(r, c) == piece && getCell(r + 1, c) == piece &&
                    getCell(r + 2, c) == piece && getCell(r + 3, c) == piece) {
                    return true;
                }
            }
        }

        // Check diagonal wins (top-left to bottom-right)
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 0; c < cols - 3; c++) {
                if (getCell(r, c) == piece && getCell(r + 1, c + 1) == piece &&
                    getCell(r + 2, c + 2) == piece && getCell(r + 3, c + 3) == piece) {
                    return true;
                }
            }
        }

        // Check diagonal wins (top-right to bottom-left)
        for (int r = 0; r < rows - 3; r++) {
            for (int c = 3; c < cols; c++) {
                if (getCell(r, c) == piece && getCell(r + 1, c - 1) == piece &&
                    getCell(r + 2, c - 2) == piece && getCell(r + 3, c - 3) == piece) {
                    return true;
                }
            }
        }

        return false;
    }

    // Checks if top row is full for a draw
    bool isFull() {
        for (int c = 0; c < cols; c++) {
            if (getCell(0, c) == ' ') {
                return false;
            }
        }
        return true;
    }
};

//Call Gmae to create a new game window and start with player 1 Blue
Game::Game(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Game),
    currentPlayerPiece('B'){
    ui->setupUi(this);

// Create new instance of game
ourGameBoard = new Connect4Board();

/* Connect all grid buttons and use regex to parse
cell name for button that was clicked
\\d+ matches and captures the numbers of named button*/
QRegularExpression regex("Coll(\\d+)R(\\d+)");
QList<QPushButton*> gridButtons = this->findChildren<QPushButton*>(regex);

// Once button is clicked call onGridCellClicked function
    for (QPushButton* button : gridButtons) {
        connect(button, &QPushButton::clicked, this, &Game::onGridCellClicked);
    }
}

Game::~Game() { delete ui; }

void Game::onGridCellClicked()
{
    //Check which button was clicked
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    //Check the name of the button
    QString buttonName = button->objectName();

    //Use regex to parse out row and collumn
    static QRegularExpression regex("Coll(\\d+)R(\\d+)");
    QRegularExpressionMatch match = regex.match(buttonName);

    //Get the column number and convert text to int
    int col = match.captured(1).toInt();

    //Drop piece
    DropInColumn(col);
}

void Game::HighlightCell(int column, int Row, char ColorKey)
{
    /*Create the object of the clicked button building
    the row and collumn into "Coll%1R%2"*/
    QString buttonName = QString("Coll%1R%2").arg(column).arg(Row);

    //Find the button for the newly created object
    QPushButton* button = this->findChild<QPushButton*>(buttonName);

    //If button is found change it's color based on player turn B or R
    if (button) {
        if (ColorKey == 'B') {
            button->setStyleSheet("background-color: blue; color: white;");
        } else if (ColorKey == 'R') {
            button->setStyleSheet("background-color: red; color: white;");
        }
    }
}
int Player1Wins = 0;
int Player2Wins = 0;
void Game::ChangePlayerWins(char PlayerKey)
{
    if(PlayerKey == 'R')
    {
        Player1Wins++;
        QPlainTextEdit* player1WinsText = this->findChild<QPlainTextEdit*>("Player1WinText");
        if (player1WinsText) {
            player1WinsText->setPlainText("Player 1's Wins: " + QString::number(Player1Wins));
        }
    }
    if(PlayerKey == 'B')
    {
        Player2Wins++;
        QPlainTextEdit* player2WinsText = this->findChild<QPlainTextEdit*>("Player2WinText");
        if (player2WinsText) {
            player2WinsText->setPlainText("Player 2's Wins: " + QString::number(Player2Wins));
        }
    }
}
void ChangeGameStateText(char PlayerKey)
{
     QPlainTextEdit* PlayerTurnText = this->findChild<QPlainTextEdit*>("PlayerTurnText");
    switch(PlayerKey)
    {
        case 'R':
                PlayerTurnText->setPlainText("Player 1's turn");
        case 'B':
                PlayerTurnText->setPlainText("Player 2's turn");
        default:
            PlayerTurnText->setPlainText("Player 1's turn");
    }
}

//Make sure to -1 from row and columnso it starts at index 0
void Game::DropInColumn(int column) {
    int highlightCol = column - 1;
    int highlightDroppedRow;

    // Call ourGameBoard dropPiece function
    if (ourGameBoard->dropPiece(highlightCol, currentPlayerPiece, highlightDroppedRow)) {

        // Highlight correct location of collumn and row
        int ui_row = highlightDroppedRow + 1;
        HighlightCell(column, ui_row, currentPlayerPiece);

        // Check for win/draw
        if (ourGameBoard->checkWin(currentPlayerPiece)) {
            // Need to create dialogue box to show winner and turn off buttons
            ChangePlayerWins(currentPlayerPiece);

        } else if (ourGameBoard->isFull()) {
            // Need to create dialogue box to show draw and turn off buttons

        } else {
            // Switch player
            if (currentPlayerPiece == 'B') {
                currentPlayerPiece = 'R';
            } else {
                currentPlayerPiece = 'B';
            }
        }
    } else {
        // Need to create dialogue box to state that collumn is full and to try another
    }

}
