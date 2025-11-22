#ifndef CONNECT4_H
#define CONNECT4_H
#include <QPushButton>
#include <QDialog>
#include <QPlainTextEdit>
#include <QRegularExpression>
using namespace std;

//Declare Connect4Board class
class Connect4Board;

namespace Ui {
class Game;
}

class Game : public QDialog
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr, bool singlePlayerMode = true);
    void HighlightCell(int column, int Row, char ColorKey);
    void ChangePlayerWins(char PlayerKey);
    void ChangeGameStateText(char PlayerKey);
    ~Game();
private slots:

    void onGridCellClicked();

    void on_resetButton_clicked();

private:
    Ui::Game *ui;
    Connect4Board* ourGameBoard;
    char currentPlayerPiece;

    bool gameOver = false;
    bool singlePlayer = true; // ← NEW: true = 1 player vs AI, false = 2 players
    void DropInColumn(int column);
};


#endif // CONNECT4_H
