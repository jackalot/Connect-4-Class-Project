#include "tictactoe.h"
#include "ui_tictactoe.h"
#include <QRegularExpression>
#include <QRandomGenerator>
#include <QMessageBox>
using namespace std;

TicTacToe::TicTacToe(QWidget *parent, bool singlePlayer) :
    QDialog(parent),
    ui(new Ui::TicTacToe),
    isSinglePlayer(singlePlayer),
    currentPlayer('X'),
    gameOver(false)
{
    ui->setupUi(this);
    board = new TicTacToeBoard();

    /* Connect all grid buttons and use regex to parse
    cell name for button that was clicked
    \\d+ matches and captures the numbers of named button*/
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    QRegularExpression regex("Coll(\\d+)R(\\d+)");

    // Once button is clicked call onGridCellClicked function
    for (QPushButton* btn : buttons) {
        if (regex.match(btn->objectName()).hasMatch()) {
            connect(btn, &QPushButton::clicked, this, &TicTacToe::onGridCellClicked);
        }
    }

    if(ui->StatusText) ui->StatusText->setPlainText("Player X's Turn");
}

TicTacToe::~TicTacToe() {
    delete board;
    delete ui;
}

void TicTacToe::onGridCellClicked() {
    if (gameOver) return;

    //Check which button was clicked
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    //Check the name of the button
    QString name = button->objectName();
    static QRegularExpression regex("Coll(\\d+)R(\\d+)");
    QRegularExpressionMatch match = regex.match(name);

    //Get the column and row number and convert text to int
    if (match.hasMatch()) {
        int col = match.captured(1).toInt();
        int row = match.captured(2).toInt();

        // Make sure to start at index 0
        if (board->getCell(row - 1, col - 1) == ' ') {
            handleMove(col, row);
        }
    }
}

void TicTacToe::handleMove(int col, int row) {

    board->setCell(row - 1, col - 1, currentPlayer);

    // Call method for visual update
    updateUI(col, row, currentPlayer);

    // Call method for checking game state
    checkGameState(currentPlayer);
    if (gameOver) return;

    // Switch players
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    if(ui->StatusText) ui->StatusText->setPlainText(QString("Player %1's Turn").arg(currentPlayer));
    if (isSinglePlayer && currentPlayer == 'O') {
        aiMove();
    }
}

void TicTacToe::aiMove() {

    if (board->isFull() || gameOver) return;

    int score = -1;
    int row = -1;
    int col = -1;

    // Setup a score based loop
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {


            if (board->getCell(r, c) == ' ') {
                int currentScore = 0;

                // Check for instant win
                board->setCell(r, c, 'O');
                if (board->checkWin('O')) {
                    currentScore += 4;
                }
                board->setCell(r, c, ' ');

                // Check for X to see if we need to block
                board->setCell(r, c, 'X');
                if (board->checkWin('X')) {
                    currentScore += 3;
                }
                board->setCell(r, c, ' ');

                // Take center if available
                if (r == 1 && c == 1) {
                    currentScore += 2;
                }

                // Take corners if available
                else if ((r == 0 || r == 2) && (c == 0 || c == 2)) {
                    currentScore += 1;
                }

                // Place wherever based on score priority
                else {
                    currentScore += 0;
                }

                // Prioritze scores
                if (currentScore > score) {
                    score = currentScore;
                    row = r;
                    col = c;
                }
            }
        }
    }

    if (row != -1 && col != -1) {
        handleMove(col + 1, row + 1);
    }
}

void TicTacToe::updateUI(int col, int row, char player) {

    /* Make sure to to set text so board state visually changes
    and set style sheet to change text color*/
    QString btnName = QString("Coll%1R%2").arg(col).arg(row);
    QPushButton* btn = this->findChild<QPushButton*>(btnName);
    if (btn) {
        btn->setText(QChar(player));
        btn->setStyleSheet(player == 'X' ? "color: blue;" : "color: red;");
    }
}

void TicTacToe::checkGameState(char player) {
    if (board->checkWin(player)) {
        if(ui->StatusText) ui->StatusText->setPlainText(QString("Player %1 Wins!").arg(player));        gameOver = true;
    } else if (board->isFull()) {
        if(ui->StatusText) ui->StatusText->setPlainText("Draw!");
        gameOver = true;
    }
}

//Setup button to manually reset the game board
void TicTacToe::on_resetButton_clicked() {
    board->clearBoard();
    currentPlayer = 'X';
    gameOver = false;
    if(ui->StatusText) ui->StatusText->setPlainText("Player X's Turn");

    //Clears the text in cells
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    for (auto btn : buttons) {
        if (btn->objectName().startsWith("Coll")) {
            btn->setText("");
        }
    }
}
