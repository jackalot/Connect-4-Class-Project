#ifndef GAME_H
#define GAME_H
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
    explicit Game(QWidget *parent = nullptr);
    void HighlightCell(int column, int Row, char ColorKey);
    void ChangePlayerWins(char PlayerKey);
    void ChangeGameStateText(char PlayerKey);
    ~Game();

private slots:

    void onGridCellClicked();

private:
    Ui::Game *ui;
    Connect4Board* ourGameBoard;
    char currentPlayerPiece;

    void DropInColumn(int column);
};


#endif // GAME_H
