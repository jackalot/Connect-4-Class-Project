#include "battleship.h"
#include "ui_battleship.h"
#include <QRegularExpression>
#include "boardgrid.h"
#include <vector>
// Set up class and methods
battleship::battleship(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::battleship)
{
    ui->setupUi(this);

    // Use a regex to find buttons with names that match the pattern
    QRegularExpression regex("Coll\\d+R\\d+");
    // Find all QPushButtons that match the regex
    QList<QPushButton*> gridButtons = this->findChildren<QPushButton*>(regex);

    // Connect each button's clicked signal to the onButtonClicked slot
    for (QPushButton* button : gridButtons) {
        connect(button, &QPushButton::clicked, this, &battleship::onButtonClicked);
    }
}

class BattleShipBoard : public BoardGrid {
    vector<Ship> ShipsOnBoard;
    int SunkCount;
    int BoardColSize = 10;
    int BoardRowSize = 10;
    //Stores misses, ship locations, and hits
    BoardGrid PlayerBoard(BoardRowSize, BoardColSize);
    // Enemy Board
    BoardGrid MissesAndHits(BoardRowSize, BoardColSize);     //Stores misses, and hits for enemy ship
    /* ^ Board codes:
     * M: Miss
     * S: Ship
     * H: Hit
     * E: Empty
     */
public:
    // Initialize board to be a 10x10 grid
    BattleShipBoard() : BoardGrid(BoardRowSize, BoardColSize) {
    }
    /* Attack/Recieve attack
     * Board codes:
     * M: Miss
     * S: Ship
     * H: Hit
     * E: Empty
     */
    //Send Attack
    void SendAttack(int Row, int Col)
    {
        MissesAndHits.RecieveAttack(Row, Col);
        // change our hits/misses if we won or not?
    }
    // Did we get hit?
    bool RecieveAttack(int Row, int Col)
    {
        bool attempt;
        // check if we got hit
        return attempt;
    }
};
BattleShipBoard PlayerOneBoard;
BattleShipBoard AIBoard;

class Ship {
public:
    vector<ShipPiece> ourPieces;
    int ShipSize;
    int HitCount = 0;
    Ship(int newSize)
    {
        ShipSize = newSize;
    }
    void IncreaseHitCount()
    {
        HitCount++;
    }
    bool CheckIfSunk()
    {
        return HitCount == ShipSize;
    }
};

class ShipPiece {
private:
    int X;
    int Y;
    bool HIT = false;
public:
    Ship parentShip;
    ShipPiece(int xPos, int yPos, Ship &ship)
    {
        X = xPos;
        Y = yPos;
        parentShip = &ship;
    }
    int getXPos()
    {
        return X;
    }
    int getYPos()
    {
        return Y;
    }
    void SetHit()
    {
        HIT = true;
        parentShip.IncreaseHitCount();
    }
};

// Highlights the proper scell we need
void battleship::HighlightCell(int row, int col)
{
    /*Create the object of the clicked button building
    the row and collumn into "Coll%1R%2"*/
    QString buttonName = QString("Coll%1R%2").arg(col).arg(row);

    //Find the button for the newly created object
    QPushButton* button = this->findChild<QPushButton*>(buttonName);
    /* We need the following kinds of styles:
     * Selecting a slot for creating a ship
     * Confirmed ships
     * misses
     * hits
     */
    button->setStyleSheet("background-color: green; color: white;");
}
battleship::~battleship()
{
    delete ui;
}
bool gameOver = false;
void battleship::onButtonClicked() {
    if(gameOver == false)
    {
        QPushButton* button = qobject_cast<QPushButton*>(sender());
        if (button) {
            QString buttonName = button->objectName();
            //Use regex to parse out row and collumn
            static QRegularExpression regex("Coll(\\d+)R(\\d+)");
            QRegularExpressionMatch match = regex.match(buttonName);
            // Your code based on buttonName
            int col = match.captured(1).toInt();
            int row = match.captured(2).toInt();
            HighlightCell(row, col);
        }
    }
}
