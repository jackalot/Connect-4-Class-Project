#include <vector>
#include <QRandomGenerator>
#include <QtGlobal>
#include <QMessageBox>
#include "Connect4.h"
#include "ui_Connect4.h"
#include "boardgrid.h"
using namespace std;

// Showing inheritance and potential for polymorphism
class Connect4Board : public BoardGrid {
public:
    // Initialize board to be a 8x13 grid
    Connect4Board() : BoardGrid(8, 13) {}
    bool setCell(int row, int col, char symbol) override;
    // Returns true if the column is valid and not full
    bool dropPiece(int col, char piece, int &droppedPiece) {
        if (col < 0 || col >= cols) {
            return false;
        }
        // Drop piece to lowest empty space and return row
        for (int row = rows - 1; row >= 0; row--) {

            // Use getCell/setCell because 'board' is private in BoardGrid
            if (getCell(row, col) == 'E') {
                setCell(row, col, piece);
                droppedPiece = row;
                return true;
            }
        }
        //Column is full
        return false;
    }

    /* Method for checking if 4 pieces are connected
    also uses proper inheritance*/
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
            if (getCell(0, c) == 'E') {
                return false;
            }
        }
        return true;
    }
};
bool Connect4Board::setCell(int row, int col, char symbol) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        board[row][col] = symbol;
        return true;
    }
    return false;
}

//Boris' AI code - removed duplicate code - made variables private
//to keep their values protected - modified code to use Qt libraries
//instead of C++ ie. ctime, cstdlib for QRandomGenerator and QtGlobal
class BasicAI {
private:
    char m_ai;
    char m_human;

    //Method for ai to determine the best move based on score
    //if no pieces around score = 0. Places piece based on highest score
    int countAdjacents(Connect4Board &b, int r, int c, char piece) {
        int score = 0;
        auto addIf = [&](int rr, int cc) {
            if (rr >= 0 && rr < b.getRows() &&
                cc >= 0 && cc < b.getCols() &&
                b.getCell(rr, cc) == piece) {
                score++;
            }
        };
        addIf(r, c - 1); addIf(r, c + 1); // Horizontal
        addIf(r - 1, c); addIf(r + 1, c); // Vertical
        return score;
    }

public:
    BasicAI(char aiPiece, char humanPiece)
        : m_ai(aiPiece), m_human(humanPiece) {}

    int chooseMove(Connect4Board &board) {
        int cols = board.getCols();

        // Method to win this turn if possible
        for (int c = 0; c < cols; ++c) {
            if (board.getCell(0, c) != 'E') continue;
            Connect4Board temp = board;
            int rTmp = -1;
            if (temp.dropPiece(c, m_ai, rTmp) && temp.checkWin(m_ai)) return c;
        }

        // Method to block opponent if they are about to win
        for (int c = 0; c < cols; ++c) {
            if (board.getCell(0, c) != 'E') continue;
            Connect4Board temp = board;
            int rTmp = -1;
            if (temp.dropPiece(c, m_human, rTmp) && temp.checkWin(m_human)) return c;
        }

        //Method to choose a good move based on score (countAdjacents)
        //if win or block aren't available
        vector<int> validCols;
        for (int c = 0; c < cols; ++c) {
            if (board.getCell(0, c) == 'E') validCols.push_back(c);
        }
        if (validCols.empty()) return -1;

        int bestScore = -1;
        vector<int> bestCols;
        int center = cols / 2;

        for (int c : validCols) {
            int score = 10 - qAbs(c - center);
            Connect4Board temp = board;
            int rTmp = -1;
            if (temp.dropPiece(c, m_ai, rTmp)) {
                score += countAdjacents(temp, rTmp, c, m_ai);
                if (score > bestScore) {
                    bestScore = score;
                    bestCols.clear();
                    bestCols.push_back(c);
                } else if (score == bestScore) {
                    bestCols.push_back(c);
                }
            }
        }
        if (bestCols.empty()) return -1;
        return bestCols[QRandomGenerator::global()->bounded(static_cast<int>(bestCols.size()))];
    }
};

BasicAI g_ai('R', 'B');

Connect4::Connect4(QWidget *parent, bool singlePlayerMode) :
    QDialog(parent),
    ui(new Ui::Connect4),
    currentPlayerPiece('B'),
    gameOver(false),
    singlePlayer(singlePlayerMode)
{
    ui->setupUi(this);

    // Create new instance of game board
    ourGameBoard = new Connect4Board();

    // Connect all grid buttons
    QList<QPushButton*> gridButtons = this->findChildren<QPushButton*>();
    for (QPushButton* button : gridButtons) {
        if (button->objectName().startsWith("Coll")) {
            connect(button, &QPushButton::clicked, this, &Connect4::onGridCellClicked);
        }
    }
}

// Destructor for Game and delete ourGameBoard and ui
Connect4::~Connect4() {
    delete ourGameBoard;
    delete ui;
}

void Connect4::onGridCellClicked() {
    if(!gameOver) {

        //Check which button was clicked
        QPushButton* button = qobject_cast<QPushButton*>(sender());

        //Check the name of the button
        QString buttonName = button->objectName();

        //Use regex to parse out row and collumn
        static QRegularExpression regex("Coll(\\d+)R(\\d+)");
        QRegularExpressionMatch match = regex.match(buttonName);

        if(match.hasMatch()){

            //Get the column number and convert text to int then drop piece
            int col = match.captured(1).toInt();
            DropInColumn(col);
        }
    }
}

/*Make sure to -1 from row and columnso it starts at index 0
also mdoified logic to flow better with 2 player and AI game modes*/
void Connect4::DropInColumn(int column) {
    if (gameOver) return;

    int colIndex = column - 1;       // 0-based for board logic
    int droppedRow = -1;

    // Drop current player's piece
    if (!ourGameBoard->dropPiece(colIndex, currentPlayerPiece, droppedRow)) {
        return; // Column full
    }

    // Highlight the new piece
    HighlightCell(column, droppedRow + 1, currentPlayerPiece); // buttons are 1-based

    // Check win
    if (ourGameBoard->checkWin(currentPlayerPiece)) {
        ChangePlayerWins(currentPlayerPiece);
        ChangeGameStateText('G'); // gameOver = true
        return;
    }

    // Check draw
    if (ourGameBoard->isFull()) {
        ChangeGameStateText('F');
        return;
    }

    // Switch players
    if (!singlePlayer) {
        currentPlayerPiece = (currentPlayerPiece == 'B') ? 'R' : 'B';
        ChangeGameStateText(currentPlayerPiece);
        return;
    }

    // Single player mode - AI turn
    int aiCol = g_ai.chooseMove(*ourGameBoard);
    if (aiCol == -1) { ChangeGameStateText('F'); return; }

    int aiRow = -1;
    ourGameBoard->dropPiece(aiCol, 'R', aiRow);
    HighlightCell(aiCol + 1, aiRow + 1, 'R');

    if (ourGameBoard->checkWin('R')) {
        ChangePlayerWins('R');
        QPlainTextEdit* text = this->findChild<QPlainTextEdit*>("PlayerTurnText");
        if (text) text->setPlainText("AI Wins!");
        gameOver = true;
        return;
    }

    if (ourGameBoard->isFull()) {
        ChangeGameStateText('F');
        return;
    }

    // Human's turn again
    currentPlayerPiece = 'B';
    ChangeGameStateText(currentPlayerPiece);
}


void Connect4::HighlightCell(int column, int Row, char ColorKey) {

    /*Create the object of the clicked button building
    the row and collumn into "Coll%1R%2"*/
    QString buttonName = QString("Coll%1R%2").arg(column).arg(Row);

    //Find the button for the newly created object
    QPushButton* button = this->findChild<QPushButton*>(buttonName);

    //If button is found change it's color based on player turn B or R
    if (button) {
        if (ColorKey == 'B'){
            button->setStyleSheet("background-color: blue; color: white;");
        } else if (ColorKey == 'R'){
            button->setStyleSheet("background-color: red; color: white;");
        }
    else { button->setStyleSheet("background-color: gray; color: white;");
    }
    }
}

//Display win dialogue box - moved variables inside of function
void Connect4::ChangePlayerWins(char PlayerKey) {

    int Player1Wins = 0;
    int Player2Wins = 0;

    if(PlayerKey == 'B') {
        Player1Wins++;
        QPlainTextEdit* player1WinsText = this->findChild<QPlainTextEdit*>("Player1WinText");
        if (player1WinsText){ player1WinsText->setPlainText("Player 1's Wins: " + QString::number(Player1Wins));
    } else if(PlayerKey == 'R') {
        Player2Wins++;
        QPlainTextEdit* Player2WinsText = this->findChild<QPlainTextEdit*>("Player2WinText");
        if (Player2WinsText) Player2WinsText->setPlainText("Player 2's Wins: " + QString::number(Player2Wins));
    }
    }
}

void Connect4::ChangeGameStateText(char PlayerKey)
{
    QPlainTextEdit* PlayerTurnText = this->findChild<QPlainTextEdit*>("PlayerTurnText");
    switch(PlayerKey)
    {
    case 'R':
        PlayerTurnText->setPlainText("Player 2's turn");
        break;
    case 'B':
        PlayerTurnText->setPlainText("Player 1's turn");
        break;
    case 'F':
        PlayerTurnText->setPlainText("Board Full");
        gameOver = true;
        break;
    default:
        gameOver = true;
        PlayerTurnText->setPlainText("Game Over");
    }
}

void Connect4::on_resetButton_clicked() {
    ourGameBoard->clearBoard();

    // Highlight all buttons gray ('G')
    for (int r = 0; r < ourGameBoard->getRows(); ++r) {
        for (int c = 0; c < ourGameBoard->getCols(); ++c) {
            HighlightCell(c + 1, r + 1, 'G'); // 1-based for buttons
        }
    }

    // Reset state
    currentPlayerPiece = 'B';
    gameOver = false;
    ChangeGameStateText('B');
}

