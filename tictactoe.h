#ifndef TICTACTOE_H
#define TICTACTOE_H
#include <QDialog>
#include <QPushButton>
#include "boardgrid.h"
using namespace std;

namespace Ui {
class TicTacToe;
}

// Applying inheritance for board grid
class TicTacToeBoard : public BoardGrid {
public:
    TicTacToeBoard() : BoardGrid(3, 3) {}

    bool checkWin(char p) {
        // Check Rows & Columns for wins
        for (int i = 0; i < 3; i++) {
            if (getCell(i, 0) == p && getCell(i, 1) == p && getCell(i, 2) == p) return true;
            if (getCell(0, i) == p && getCell(1, i) == p && getCell(2, i) == p) return true;
        }
        // Check diagonals for wins
        if (getCell(0, 0) == p && getCell(1, 1) == p && getCell(2, 2) == p) return true;
        if (getCell(0, 2) == p && getCell(1, 1) == p && getCell(2, 0) == p) return true;
        return false;
    }
    bool setCell(int row, int col, char symbol) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw std::out_of_range("TicTacToeBoard::setCell coordinates out of range");
        }
        board[row][col] = symbol;
        return true;
    }

};

class TicTacToe : public QDialog
{
    Q_OBJECT

public:
    explicit TicTacToe(QWidget *parent = nullptr, bool singlePlayer = false);
    ~TicTacToe();

private slots:
    void onGridCellClicked();
    void on_resetButton_clicked();

private:
    Ui::TicTacToe *ui;
    TicTacToeBoard* board;
    bool isSinglePlayer;
    char currentPlayer;
    bool gameOver;

    void handleMove(int col, int row);
    void aiMove();
    void updateUI(int col, int row, char player);
    void checkGameState(char player);
};

#endif
