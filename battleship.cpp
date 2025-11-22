#include "battleship.h"
#include "ui_battleship.h"
#include <QRegularExpression>
#include "boardgrid.h"
#include <vector>
using namespace std;

// Forward declarations
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
    void SetHit(); // Declaration only
};

class Ship {
public:
    vector<ShipPiece> ourPieces;
    int ShipSize;
    int HitCount = 0;

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

class BattleShipBoard : public BoardGrid {
    vector<Ship> ShipsOnBoard;
    BoardGrid* PlayerBoard;
    BoardGrid* MissesAndHits;
    // The first spot we click for placing a ship
    int originalX = -1;
    int originalY = -1;
public:
    BattleShipBoard() : BoardGrid(10, 10), PlayerBoard(new BoardGrid(10, 10)), MissesAndHits(new BoardGrid(10, 10)) {}

    void PlaceShip(int Col, int Row);
    void SendAttack(int Col, int Row);
    bool RecieveAttack(int Col, int Row);
};

void BattleShipBoard::PlaceShip(int Col, int Row) {
    // Implementation for placing the ship
    if(originalX == -1 && originalY == -1)
    {
        //new cordinate
        originalX = Col;
        originalY = Row;
    }
}

bool BattleShipBoard::RecieveAttack(int Col, int Row) {
    // Implementation to check if attacked
    return false; // Placeholder
}

BattleShipBoard PlayerOneBoard;
BattleShipBoard AIBoard;

battleship::battleship(QWidget *parent)
    : QDialog(parent), ui(new Ui::battleship) {
    ui->setupUi(this);  // Set up the UI
}

battleship::~battleship() {
    delete ui;
}

void battleship::HighlightCell(int row, int col, char ColorKey) {
    QString buttonName = QString("Coll%1R%2").arg(col).arg(row);
    QPushButton* button = this->findChild<QPushButton*>(buttonName);
    button->setStyleSheet("background-color: green; color: white;");
}

bool GameOver = false;
bool placeMode = true;

void battleship::onButtonClicked() {
    if (!GameOver) {
        QPushButton* button = qobject_cast<QPushButton*>(sender());
        if (button) {
            QString buttonName = button->objectName();
            static QRegularExpression regex("Coll(\\d+)R(\\d+)");
            QRegularExpressionMatch match = regex.match(buttonName);

            int col = match.captured(1).toInt();
            int row = match.captured(2).toInt();

            if (!placeMode) {
                HighlightCell(row, col, 'G');
            } else {
                PlayerOneBoard.PlaceShip(col, row);
            }
        }
    }
}
