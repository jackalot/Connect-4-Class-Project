#include "battleship.h"
#include "ui_battleship.h"
#include <QRegularExpression>
#include "boardgrid.h"
#include <vector>
#include <QDebug>

using namespace std;
class BattleShipBoard; // Forward declaration
class AI;

// Global variable definitions
BattleShipBoard* PlayerOneBoard = nullptr; // Definition
BattleShipBoard* AIBoard = nullptr;         // Definition
AI* ourAI = nullptr;
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
    battleship* parentUI; // pointer to the battleship UI
    Ship(int newInitialX, int newInitialY, int newFinalX, int newFinalY, battleship* ui) {
        initialX = newInitialX;
        initailY = newInitialY;
        FinalX = newFinalX;
        FinalY = newFinalY;
        parentUI = ui;
        char direction = GetDirection();
        ShipSize = 0;
        switch(direction)
        {
            case 'U':
                for(int yCoord = initailY; yCoord >= FinalY; yCoord--)
                {
                    ShipSize++;
                    ShipPiece newPiece(initialX, yCoord, this);
                    ourPieces.push_back(newPiece);
                }
                break;
            case 'D':
                for(int yCoord = initailY; yCoord <= FinalY; yCoord++)
                {
                    ShipSize++;
                    ShipPiece newPiece(initialX, yCoord, this);
                    ourPieces.push_back(newPiece);
                }
                break;
            case 'L':
                for(int xCoord = initialX; xCoord >= FinalX; xCoord--)
                {
                    ShipSize++;
                    ShipPiece newPiece(xCoord, initailY, this);
                    ourPieces.push_back(newPiece);
                }
                break;
            case 'R':
                for(int xCoord = initialX; xCoord <= FinalX; xCoord++)
                {
                    ShipSize++;
                    ShipPiece newPiece(xCoord, initailY, this);
                    ourPieces.push_back(newPiece);
                }
                break;
        }
    }
    char GetDirection() {
        char direction = 'D';
        if(initialX == FinalX)
        {
            if(initailY < FinalY)
            {
                direction = 'D';
            }
            else
            {
                direction = 'U';
            }
        }
        else if(initailY == FinalY)
        {
            if(initialX < FinalX)
            {
                direction = 'R';
            }
            else
            {
                direction = 'L';
            }
        }
        return direction;
    }
    void IncreaseHitCount() { HitCount++; }
    bool CheckIfSunk() { return HitCount == ShipSize; }
    void RemoveShipInUI()
    {
        char direction = GetDirection();
        switch(direction)
        {
            case 'U':
                for(int yCoord = initailY; yCoord >= FinalY; yCoord--)
                {
                    ourPieces.pop_back();
                    parentUI->HighlightCell(yCoord, initialX, 'D');
                }
                break;
            case 'D':
                for(int yCoord = initailY; yCoord <= FinalY; yCoord++)
                {
                    ourPieces.pop_back();
                    parentUI->HighlightCell(yCoord, initialX, 'D');
                }
                break;
            case 'L':
                for(int xCoord = initialX; xCoord >= FinalX; xCoord--)
                {
                    ourPieces.pop_back();
                    parentUI->HighlightCell(initailY, xCoord, 'D');
                }
                break;
            case 'R':
                for(int xCoord = initialX; xCoord <= FinalX; xCoord++)
                {
                    ourPieces.pop_back();
                    parentUI->HighlightCell(initailY, xCoord, 'D');
                }
                break;
        }
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
    BoardGrid* PlayerBoard;
    BoardGrid* EnemyBoard;
    /**/
public:
    battleship* parentUI; // pointer to the battleship UI
    vector<Ship> ShipsOnBoard;
    int originalX = -1;
    int originalY = -1;
    bool firstPointPlaced = false;
    int FinalX = -1;
    int FinalY = -1;
    bool secondPointPlaced = false;
    vector<int> ShipSizes = {2, 2, 3, 4, 5};
    BattleShipBoard(battleship* ui);  // constructor declaration
    void PlaceShip(int Col, int Row);
    bool CheckSlotsIfAvailable(bool Paint);
    void ConfirmShip();
    void SendAttack(int Col, int Row);
    bool RecieveAttack(int Col, int Row);
    void RemoveLastShip();
    char GetDirection();
    void CreateShip();
    void ResetCoordinates();
    void SetEnemyBoard(BoardGrid enemyGrid)
    {
        EnemyBoard = &enemyGrid;
    }
    BoardGrid GetPlayerBoard()
    {
        return *PlayerBoard;
    }
    void DisplayPlayerBoardGrid() {
        for(int CurrentRow = 1; CurrentRow < PlayerBoard->getRows(); CurrentRow++)
        {
            for(int CurrentCol = 1; CurrentCol < PlayerBoard->getCols(); CurrentCol++)
            {
                switch(PlayerBoard->getCell(CurrentRow, CurrentCol))
                {
                    case 'S':
                    parentUI->HighlightCell(CurrentRow, CurrentRow, 'G');
                    break;
                }
            }
        }
    }
};
// --------------------- BattleShipBoard methods ---------------------
// Constructor definition
BattleShipBoard::BattleShipBoard(battleship* ui)
    : BoardGrid(10, 10),
    // code side comes from 0-9,
    // UI is 1-10
    // increase size by one
    PlayerBoard(new BoardGrid(11, 11)), // Our ships, where enemy hit's, misses
    EnemyBoard(new BoardGrid(11, 11)), // where we hit, miss
    parentUI(ui)
{}
void BattleShipBoard::ResetCoordinates() {
    originalX = -1;
    originalY = -1;
    FinalX = -1;
    FinalY = -1;
    firstPointPlaced = false;
    secondPointPlaced = false;
}
char BattleShipBoard::GetDirection() {

        char direction = 'D';
        if(originalX == FinalX)
        {
            if(originalY < FinalY)
            {
                direction = 'D';
            }
            else
            {
                direction = 'U';
            }
        }
        else if(originalY == FinalY)
        {
            if(originalX < FinalX)
            {
                direction = 'R';
            }
            else
            {
                direction = 'L';
            }
        }
        return direction;

}
bool BattleShipBoard::CheckSlotsIfAvailable(bool Paint)
{
    bool available = true;
    // Check Vertically
    char direction = GetDirection();
    int currentSize = ShipSizes.back();
    switch(direction)
    {
        case 'R':
            // from left to right
            // check between the coordinates first
            for(int xCoord = originalX; xCoord <= FinalX && available; xCoord++)
            {
                int result = FinalX - originalX + 1; // Inclusive calculation
                if(PlayerBoard->getCell(originalY, xCoord) == 'S' || currentSize != result)
                {
                    available = false;
                    if(PlayerBoard->getCell(originalY, xCoord) == 'S')
                    {
                        /*
                        * ERROR
                        * This selection crosses an existing ship.
                        */
                    }
                    else
                    {
                        /*
                        * ERROR
                        * We need a size of currentSize, this isn't a big enough ship
                        */
                    }
                    break;
                }
            }
            if(available)
            {
                // Highlight between the coordinates
                for(int xCoord = originalX; xCoord <= FinalX && available; xCoord++)
                {
                    if(Paint)
                    {
                        parentUI->HighlightCell(originalY, xCoord, 'G');
                    }
                    PlayerBoard->setCell(originalY, xCoord, 'S');
                }
            }
            break;
        case 'L':
            // From right to left
            // check between the coordinates first
            for(int xCoord = originalX; xCoord >= FinalX && available; xCoord--)
        {
                 int result = originalX - FinalX + 1; // Inclusive calculation
            if(PlayerBoard->getCell(originalY, xCoord) == 'S' || currentSize != result)
            {
                available = false;
                if(PlayerBoard->getCell(originalY, xCoord) == 'S')
                {
                    /*
                        * ERROR
                        * This selection crosses an existing ship.
                        */
                }
                else
                {
                    /*
                        * ERROR
                        * We need a size of currentSize, this isn't a big enough ship
                        */
                }
                break;
            }
        }
            if(available)
        {
            // Highlight between the coordinates
            for(int xCoord = originalX; xCoord >= FinalX && available; xCoord--)
            {
                if(Paint)
                {
                    parentUI->HighlightCell(originalY, xCoord, 'G');
                }
                PlayerBoard->setCell(originalY, xCoord, 'S');
            }
        }
            break;
        case 'U':
            // From down to up
            // check between the coordinates first
            for(int yCoord = originalY; yCoord >= FinalY && available; yCoord--)
            {
                int result = originalY - FinalY + 1; // Inclusive calculation
                if(PlayerBoard->getCell(yCoord, originalX) == 'S' || currentSize != result)
                {
                    available = false;
                    if(PlayerBoard->getCell(yCoord, originalX) == 'S')
                    {
                        /*
                        * ERROR
                        * This selection crosses an existing ship.
                        */
                    }
                    else
                    {
                        /*
                        * ERROR
                        * We need a size of currentSize, this isn't a big enough ship
                        */
                    }
                    break;
                }
            }
            if(available)
            {
                // Highlight between the coordinates
                for(int yCoord = originalY; yCoord >= FinalY; yCoord--)
                {
                    if(Paint)
                    {
                        parentUI->HighlightCell(yCoord, originalX, 'G');
                    }
                    PlayerBoard->setCell(yCoord, originalX, 'S');
                }
            }
            break;
        case 'D':
            // from up to down
            // check between the coordinates first
            for(int yCoord = originalY; yCoord <= FinalY && available; yCoord++)
            {
                int result = FinalY - originalY + 1; // Inclusive calculation
                if(PlayerBoard->getCell(yCoord, originalX) == 'S' || currentSize != result)
                {
                    available = false;
                    if(PlayerBoard->getCell(yCoord, originalX) == 'S')
                    {
                        /*
                        * ERROR
                        * This selection crosses an existing ship.
                        */
                    }
                    else
                    {
                        /*
                        * ERROR
                        * We need a size of currentSize, this isn't a big enough ship
                        */
                    }
                    break;
                }
            }
            if(available)
            {
                // Highlight between the coordinates
                for(int yCoord = originalY; yCoord <= FinalY; yCoord++)
                {
                    if(Paint)
                    {
                        parentUI->HighlightCell(yCoord, originalX, 'G');
                    }
                    PlayerBoard->setCell(yCoord, originalX, 'S');
                }
            }
            break;
        default:
            available = false;
            break;
    }

    return available;
}

void BattleShipBoard::CreateShip()
{
    Ship newShip(originalX, originalY, FinalX, FinalY, parentUI);
    ShipsOnBoard.push_back(newShip);
}
// Place a ship on the board
void BattleShipBoard::PlaceShip(int Col, int Row) {
    if(ShipSizes.size())
    {
        /*
        * ERROR
        * Please place a ship of size: ShipSizes.back();
        */
        if(secondPointPlaced)
        {
            //Cancel this ship all together
            parentUI->HighlightCell(FinalY, FinalX, 'D');
            parentUI->HighlightCell(originalY, originalX, 'D');
            ResetCoordinates();
        }
        else if (!firstPointPlaced) {
            if(PlayerBoard->getCell(Row, Col) != 'S')
            {
                // just place the first point
                originalX = Col;
                originalY = Row;
                if (parentUI) {
                    parentUI->HighlightCell(originalY, originalX, 'P');
                }
                firstPointPlaced = true;
                /*
                * ERROR
                * Now click a button for the second piece (vertically or hirozontally)
                */
            }
        }
        else
        {
            // We cancel initial ship placement
            if(Col == originalX && originalY == Row)
            {
                if (parentUI) {
                    parentUI->HighlightCell(Row, Col, 'D');
                }
                /*
                * ERROR
                * Click a button to place the first piece
                */
                ResetCoordinates();
            }
            else
            {
                //Place the second point
                if (!secondPointPlaced) {
                    if(PlayerBoard->getCell(Row, Col) != 'S')
                    {
                        //if it's on the X or Y coordinate, it's not diagnal
                        if(Col == originalX || originalY == Row)
                        {
                            FinalX = Col;
                            FinalY = Row;
                            if (parentUI) {
                                parentUI->HighlightCell(Row, Col, 'P');
                            }
                            secondPointPlaced = true;
                        }
                        else
                        {
                            /*
                            * ERROR
                            * You can't place a diagnal ship, sorry I don't make the rules
                            */
                        }
                    }
                }
            }

        }
        if(firstPointPlaced && secondPointPlaced)
        {
            //can we place this ship?
            if(!CheckSlotsIfAvailable(true))
            {
                //Cancel the ship
                parentUI->HighlightCell(FinalY, FinalX, 'D');
                parentUI->HighlightCell(originalY, originalX, 'D');
                ResetCoordinates();
            }
            else
            {
                CreateShip();
                //we created one ship, let's pop the one we used
                ShipSizes.pop_back();
                ResetCoordinates();

            }
        }
    }
}

// Placeholder: receive attack
bool BattleShipBoard::RecieveAttack(int Col, int Row) {
    return false;
}
void BattleShipBoard::RemoveLastShip() {
    if(ShipsOnBoard.size())
    {
        Ship shipToRemove = ShipsOnBoard.back();
        //we popped it when we made the ship
        //let's use that size again
        ShipSizes.push_back(shipToRemove.ShipSize);
        for (int i = 0; i < shipToRemove.ourPieces.size(); i++)
        {
            int X = shipToRemove.ourPieces[i].getXPos();
            int Y = shipToRemove.ourPieces[i].getYPos();
            PlayerBoard->setCell(Y, X, 'E');
        }
        ShipsOnBoard.back().RemoveShipInUI();
        ShipsOnBoard.pop_back();
        ResetCoordinates();
    }
    else
    {
        /*
         * ERROR
         * No ships to remove, place one please!
         */
    }
}

void BattleShipBoard::SendAttack(int Col, int Row) {
    // Implement sending attack logic here
}
// --------------------- AI LOGIC ---------------------
class AI {
public:
    BattleShipBoard* AIBoard;

    // Constructor takes a pointer to a BattleshipBoard
    AI(BattleShipBoard* newAIBoard) {
        AIBoard = newAIBoard; // Initialize the pointer
    }
    void MakeProperSize(int shipSize)
    {
        char Direction = AIBoard->GetDirection();
        bool RightSize = false; // Initialize RightSize to false
        switch(Direction)
        {
        case 'U':
            while(!RightSize)
            {
                int result = AIBoard->originalY - AIBoard->FinalY + 1; // Inclusive calculation
                if(result == shipSize)
                {
                    RightSize = true;
                }
                else
                {
                    // Adjust coordinates so that it fits
                    if(result < shipSize)
                    {
                        if(AIBoard->FinalY == 1)
                        {
                            AIBoard->originalY++;
                        }
                        else
                        {
                            AIBoard->FinalY--;
                        }
                    }
                    else
                    {
                        AIBoard->FinalY++;
                    }
                }
            }
            break;
        case 'D':
            while(!RightSize)
            {
                int result = AIBoard->FinalY - AIBoard->originalY + 1; // Inclusive calculation
                if(result == shipSize)
                {
                    RightSize = true;
                }
                else
                {
                    // Adjust coordinates so that it fits
                    if(result < shipSize)
                    {
                        if(AIBoard->originalY == 1)
                        {
                            AIBoard->FinalY++;
                        }
                        else
                        {
                            AIBoard->originalY--;
                        }
                    }
                    else
                    {
                        AIBoard->FinalY++;
                    }
                }
            }
            break;
        case 'L':
            while(!RightSize)
            {
                int result = AIBoard->originalX - AIBoard->FinalX + 1; // Inclusive calculation
                if(result == shipSize)
                {
                    RightSize = true;
                }
                else
                {
                    // Adjust coordinates so that it fits
                    if(result < shipSize)
                    {
                        if(AIBoard->FinalX == 1)
                        {
                            AIBoard->originalX++;
                        }
                        else
                        {
                            AIBoard->FinalX--;
                        }
                    }
                    else
                    {
                        AIBoard->FinalX++;
                    }
                }
            }
            break;
        case 'R':
            while(!RightSize)
            {
                int result = AIBoard->FinalX - AIBoard->originalX + 1; // Inclusive calculation
                if(result == shipSize)
                {
                    RightSize = true;
                }
                else
                {
                    // Adjust coordinates so that it fits
                    if(result < shipSize)
                    {
                        if(AIBoard->originalX == 1)
                        {
                            AIBoard->FinalX++;
                        }
                        else
                        {
                            AIBoard->originalX--;
                        }
                    }
                    else
                    {
                        AIBoard->FinalX++;
                    }
                }
            }
            break;
        }
    }

    void PlaceARandomShip()
    {
        while (AIBoard->ShipSizes.size())
        {
            int shipSize = AIBoard->ShipSizes.back();
            int orientation = rand() % 2; // 0 for vertical, 1 for horizontal
            // lets go vertical
            if(orientation == 1)
            {
                AIBoard->originalX = rand() % 10;
                AIBoard->FinalX = AIBoard->originalX;
                AIBoard->FinalY = rand() % (10 - shipSize);
                AIBoard->originalY = rand() % 10;
            }
            else //horizontal
            {
                AIBoard->originalY = rand() % 10;
                AIBoard->FinalY = AIBoard->originalY;
                AIBoard->FinalX = rand() % 10;
                AIBoard->originalX = rand() % 10;
            }
            MakeProperSize(shipSize);
            if(AIBoard->CheckSlotsIfAvailable(false))
            {
                AIBoard->CreateShip();
                AIBoard->ShipSizes.pop_back();
            }
            AIBoard->ResetCoordinates();
        }
    }
    // Destructor to clean up if needed
    ~AI() {
    }
};
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
    ourAI = new AI(AIBoard);
    // Connect all grid buttons to onButtonClicked()
    for (int row = 1; row <= 10; ++row) {
        for (int col = 1; col <= 10; ++col) {
            QString buttonName = QString("Coll%1R%2").arg(col).arg(row);
            QPushButton* button = this->findChild<QPushButton*>(buttonName);
            if (button) {
                connect(button, &QPushButton::clicked, this, &battleship::onButtonClicked);
                button->setStyleSheet("background-color: white; color: white;");
            } else {
                qDebug() << "Button not found:" << buttonName;
            }
        }
    }
    // set up game
    ourAI->PlaceARandomShip();
    ourAI->AIBoard->SetEnemyBoard(PlayerOneBoard->GetPlayerBoard());
    PlayerOneBoard->SetEnemyBoard(ourAI->AIBoard->GetPlayerBoard());
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

    // P is for placing
    if (ColorKey == 'P') {
        button->setStyleSheet("background-color: purple; color: white;");
    } else if (ColorKey == 'G') { // G is for ships that are placed
        button->setStyleSheet("background-color: green; color: white;");
    } else if (ColorKey == 'D') { //D for canceling ships, default color
        button->setStyleSheet("background-color: white; color: white;");
    } else if (ColorKey == 'X') { //X for misses
        button->setStyleSheet("background-color: gray; color: white;");
    } else if (ColorKey == 'R') { //R for hits
        button->setStyleSheet("background-color: red; color: white;");
    }
}


void battleship::on_UndoButton_clicked()
{
    if(PlayerOneBoard)
    {
        PlayerOneBoard->RemoveLastShip();
    }
}

