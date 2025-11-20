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
public:

    // Initialize board to be a 10x10 grid
    BattleShipBoard() : BoardGrid(10, 10) {}

};
class Ship {
public:
    vector<ShipPiece> ourPieces;
    int ShipSize;
    Ship(int newSize)
    {
        ShipSize = newSize;
    }

};

class ShipPiece {
private:
    int X;
    int Y;
    bool HIT = false;
public:
    ShipPiece(int xPos, int yPos)
    {
        X = xPos;
        Y = yPos;
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
