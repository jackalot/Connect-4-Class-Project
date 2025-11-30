#ifndef CONNECT4_H
#define CONNECT4_H
#include <QPushButton>
#include <QDialog>
#include <QPlainTextEdit>
#include <QRegularExpression>
using namespace std;

namespace Ui {
class Connect4;
}

class Connect4Board;

class Connect4 : public QDialog
{
    Q_OBJECT

public:
    explicit Connect4(QWidget *parent = nullptr, bool singlePlayerMode = true);
    void HighlightCell(int column, int Row, char ColorKey);
    void ChangePlayerWins(char PlayerKey);
    void ChangeGameStateText(char PlayerKey);
    ~Connect4();
private slots:

    void onGridCellClicked();

    void on_resetButton_clicked();

private:
    int Player1Wins = 0;
    int Player2Wins = 0;

    Ui::Connect4 *ui;
    Connect4Board* ourGameBoard;
    char currentPlayerPiece;

    bool gameOver = false;
    bool singlePlayer = true; // ← NEW: true = 1 player vs AI, false = 2 players
    void DropInColumn(int column);
};


#endif // CONNECT4_H
