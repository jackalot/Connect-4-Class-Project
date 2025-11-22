#include "battleship.h"
#include "ui_battleship.h"
#include <QRegularExpression>
#include "boardgrid.h"
#include <vector>
#include <QDebug>

using namespace std;

// --------------------- Ship and ShipPiece ---------------------
class Ship;

class ShipPiece {
private:
    int X;
    int Y;
    bool HIT = false;
public:
    Ship* parentShip;

    ShipPiece(int xPos, int yPos, Ship* ship) : X(xPos), Y(yPos), parentShip(ship) {}

    int getXPos() { return X; }
    int getYPos() { return Y; }
    void SetHit();
};

class Ship {
public:
    vector<ShipPiece> ourPieces;
    int ShipSize;
    int HitCount = 0;
    int initialX = -1;
    int initailY = -1;
    int FinalX = -1;
    int FinalY = -1;
    Ship(int newSize) : ShipSize(newSize) {}

    void IncreaseHitCount() { HitCount++; }
    bool CheckIfSunk() { return HitCount == ShipSize; }
    void AddPiece(int x, int y) {
        ourPieces.emplace_back(x, y, this);
    }
};

// Implement SetHit after Ship is fully defined
void ShipPiece::SetHit() {
    HIT = true;
    if (parentShip) {
        parentShip->IncreaseHitCount();
    }
}

// --------------------- BattleShipBoard ---------------------
class BattleShipBoard : public BoardGrid {
    battleship* parentUI; // pointer to the battleship UI
    vector<Ship> ShipsOnBoard;
    BoardGrid* PlayerBoard;
    BoardGrid* MissesAndHits;
    int originalX = -1;
    int originalY = -1;
    int FinalX = -1;
    int FinalY = -1;
public:
    BattleShipBoard(battleship* ui);  // constructor declaration
    void PlaceShip(int Col, int Row);
    void ConfirmShip();
    void SendAttack(int Col, int Row);
    bool RecieveAttack(int Col, int Row);
};

// Constructor definition
BattleShipBoard::BattleShipBoard(battleship* ui)
    : BoardGrid(10, 10),
    PlayerBoard(new BoardGrid(10, 10)),
    MissesAndHits(new BoardGrid(10, 10)),
    parentUI(ui)
{}

// Place a ship on the board
void BattleShipBoard::PlaceShip(int Col, int Row) {
    if (originalX == -1 && originalY == -1) {
        originalX = Col;
        originalY = Row;
        if (parentUI) {
            parentUI->HighlightCell(originalY, originalX, 'P');
        }
    }
    else
    {
        // We cancel ship placement
        if(Col == originalX && originalY == Row)
        {
            originalX = -1;
            originalY = -1;
            if (parentUI) {
                parentUI->HighlightCell(Row, Col, 'X');
            }
        }
        else
        {
            FinalX = Col;
            FinalY = Row;
            if (parentUI) {
                parentUI->HighlightCell(Row, Col, 'P');
            }
        }

    }
}

// Placeholder: receive attack
bool BattleShipBoard::RecieveAttack(int Col, int Row) {
    return false;
}

void BattleShipBoard::SendAttack(int Col, int Row) {
    // Implement sending attack logic here
}

// --------------------- Global Boards ---------------------
BattleShipBoard* PlayerOneBoard = nullptr;
BattleShipBoard* AIBoard = nullptr;

// --------------------- battleship UI ---------------------
battleship::battleship(QWidget *parent)
    : QDialog(parent), ui(new Ui::battleship)
{
    ui->setupUi(this);

    // Initialize boards and pass this UI pointer
    if (!PlayerOneBoard)
        PlayerOneBoard = new BattleShipBoard(this);

    if (!AIBoard)
        AIBoard = new BattleShipBoard(this);
    // Connect all grid buttons to onButtonClicked()
    for (int row = 1; row <= 10; ++row) {
        for (int col = 1; col <= 10; ++col) {
            QString buttonName = QString("Coll%1R%2").arg(col).arg(row);
            QPushButton* button = this->findChild<QPushButton*>(buttonName);
            if (button) {
                connect(button, &QPushButton::clicked, this, &battleship::onButtonClicked);
                button->setStyleSheet("background-color: gray; color: white;");
            } else {
                qDebug() << "Button not found:" << buttonName;
            }
        }
    }
}

battleship::~battleship() {
    delete ui;
}

// Highlight a cell on the UI
void battleship::HighlightCell(int row, int col, char ColorKey) {
    QString buttonName = QString("Coll%1R%2").arg(col).arg(row);
    QPushButton* button = this->findChild<QPushButton*>(buttonName);
    if (!button) {
        qDebug() << "ERROR: No button named" << buttonName;
        return;
    }

    // Example: color green for 'P' or 'G'
    if (ColorKey == 'P') {
        button->setStyleSheet("background-color: blue; color: white;");
    } else if (ColorKey == 'G') {
        button->setStyleSheet("background-color: green; color: white;");
    } else if (ColorKey == 'X') { //X for canceling ships, default
        button->setStyleSheet("background-color: gray; color: white;");
    }
}

// --------------------- Button click handler ---------------------
bool GameOver = false;
bool placeMode = true;

void battleship::onButtonClicked() {
    if (GameOver) return;

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString buttonName = button->objectName();
    static QRegularExpression regex("Coll(\\d+)R(\\d+)");
    QRegularExpressionMatch match = regex.match(buttonName);

    if (!match.hasMatch()) return;

    int col = match.captured(1).toInt();
    int row = match.captured(2).toInt();

    if (!placeMode) {
        HighlightCell(row, col, 'G'); // Mark attack
    } else {
        PlayerOneBoard->PlaceShip(col, row); // Place ship
    }
}

void battleship::on_ConfirmButton_clicked()
{
    if(placeMode)
    {
        //PlayerOneBoard->
    }
}

