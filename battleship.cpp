#include "battleship.h"
#include "ui_battleship.h"
#include <QRegularExpression>
#include "boardgrid.h"
#include "Coordinates.h" // Include the Coordinate header#include <vector>
#include <QDebug>

using namespace std;
class BattleShipBoard; // Forward declaration
class AI;

// Global variable definitions
BattleShipBoard* PlayerOneBoard = nullptr; // Definition
BattleShipBoard* AIBoard = nullptr;         // Definition
AI* ourAI = nullptr;
QPlainTextEdit* ModeStatusText; // obj name ModeStatus
QPlainTextEdit* GameStatusText; // obj name GameStatus
QPlainTextEdit* ViewStatusText; // obj name ViewStatus
bool GameOver = false;
bool placeMode = true;
bool PlayerOnesTurn = false;
bool PlayerBoardVisible = true;
// --------------------- Ship and ShipPiece ---------------------
class Ship; // Forward declaration of Ship
//Each ship is made of ShipPieces
class ShipPiece : public Coordinate {
//This piece needs a Coordinate, let's inherit it
protected:
//we only can change this to SetHit
    bool HIT = false;
public:
    Ship* parentShip;

    // Constructor
    ShipPiece(int xPos, int yPos, Ship* ship)
        : // Initialize ourCoords directly with Coordinate
        parentShip(ship)  // Initialize parentShip
    {
        setX(xPos);
        setY(yPos);
    }
    // SetHit method to mark the piece as hit
    void SetHit();
};


class Ship {
public:
    vector<ShipPiece> ourPieces;
    int ShipSize;
    int HitCount = 0;
    Coordinate OriginalCoords;
    Coordinate FinalCoords;
    battleship* parentUI; // pointer to the battleship UI
    Ship(int newInitialX, int newInitialY, int newFinalX, int newFinalY, battleship* ui) {
        OriginalCoords.setX(newInitialX);
        OriginalCoords.setY(newInitialY);
        FinalCoords.setX(newFinalX);
        FinalCoords.setY(newFinalY);
        parentUI = ui;
        char direction = GetDirection();
        ShipSize = 0;
        switch(direction)
        {
            case 'U':
                for(int yCoord = OriginalCoords.getY(); yCoord >= FinalCoords.getY(); yCoord--)
                {
                    ShipSize++;
                    ShipPiece newPiece(OriginalCoords.getX(), yCoord, this);
                    ourPieces.push_back(newPiece);
                }
                break;
            case 'D':
                for(int yCoord = OriginalCoords.getY(); yCoord <= FinalCoords.getY(); yCoord++)
                {
                    ShipSize++;
                    ShipPiece newPiece(OriginalCoords.getX(), yCoord, this);
                    ourPieces.push_back(newPiece);
                }
                break;
            case 'L':
                for(int xCoord = OriginalCoords.getX(); xCoord >= FinalCoords.getX(); xCoord--)
                {
                    ShipSize++;
                    ShipPiece newPiece(xCoord, OriginalCoords.getY(), this);
                    ourPieces.push_back(newPiece);
                }
                break;
            case 'R':
                for(int xCoord = OriginalCoords.getX(); xCoord <= FinalCoords.getX(); xCoord++)
                {
                    ShipSize++;
                    ShipPiece newPiece(xCoord, OriginalCoords.getY(), this);
                    ourPieces.push_back(newPiece);
                }
                break;
        }
    }
    char GetDirection() {
        char direction = 'D';
        if(OriginalCoords.getX() == FinalCoords.getX())
        {
            if(OriginalCoords.getY() < FinalCoords.getY())
            {
                direction = 'D';
            }
            else
            {
                direction = 'U';
            }
        }
        else if(OriginalCoords.getY() == FinalCoords.getY())
        {
            if(OriginalCoords.getX() < FinalCoords.getX())
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
            for(int yCoord = OriginalCoords.getY(); yCoord >= FinalCoords.getY(); yCoord--)
                {
                    ourPieces.pop_back();
                    parentUI->HighlightCell(yCoord, OriginalCoords.getX(), 'D');
                }
                break;
            case 'D':
                for(int yCoord = OriginalCoords.getY(); yCoord <= FinalCoords.getY(); yCoord++)
                {
                    ourPieces.pop_back();
                    parentUI->HighlightCell(yCoord, OriginalCoords.getX(), 'D');
                }
                break;
            case 'L':
                for(int xCoord = OriginalCoords.getX(); xCoord >= FinalCoords.getY(); xCoord--)
                {
                    ourPieces.pop_back();
                    parentUI->HighlightCell(OriginalCoords.getY(), xCoord, 'D');
                }
                break;
            case 'R':
                for(int xCoord = OriginalCoords.getX(); xCoord <= FinalCoords.getX(); xCoord++)
                {
                    ourPieces.pop_back();
                    parentUI->HighlightCell(OriginalCoords.getY(), xCoord, 'D');
                }
                break;
        }
    }
    bool CheckIfHit(int X, int Y)
    {
        bool hit = false;
        if(OriginalCoords.getX() == X || OriginalCoords.getY() == Y)
        {
            for(int i = 0; i < ShipSize; i++)
            {
                ShipPiece currentPiece = ourPieces[i];
                if(currentPiece.getX() == X && currentPiece.getY() == Y)
                {
                    hit = true;
                    ourPieces[i].SetHit();
                }
            }
        }
        return hit;
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
    /**/
public:
    battleship* parentUI; // pointer to the battleship UI
    vector<Ship> ShipsOnBoard;
    Coordinate OriginalCoords;
    Coordinate FinalCoords;
    bool firstPointPlaced = false;
    bool secondPointPlaced = false;
    vector<int> ShipSizes = {2, 2, 3, 4, 5};
    BattleShipBoard(battleship* ui);  // constructor declaration
    void PlaceShip(int Col, int Row);
    bool CheckSlotsIfAvailable(bool Paint);
    void ConfirmShip();
    bool RecieveAttack(int Col, int Row);
    void RemoveLastShip();
    char GetDirection();
    void CreateShip();
    void ResetCoordinates();
    BoardGrid* GetPlayerBoard()
    {
        return PlayerBoard;
    }
    void DisplayShips() {
        for(int CurrentRow = 1; CurrentRow < PlayerBoard->getRows(); CurrentRow++)
        {
            for(int CurrentCol = 1; CurrentCol < PlayerBoard->getCols(); CurrentCol++)
            {
                if(PlayerBoard->getCell(CurrentRow, CurrentCol) == 'S')
                {
                    parentUI->HighlightCell(CurrentRow, CurrentCol, 'G');
                }
            }
        }
    }
    void DisplayMissesAndHits() {
        for(int CurrentRow = 1; CurrentRow < PlayerBoard->getRows(); CurrentRow++)
        {
            for(int CurrentCol = 1; CurrentCol < PlayerBoard->getCols(); CurrentCol++)
            {
                switch(PlayerBoard->getCell(CurrentRow, CurrentCol))
                {
                    case 'H':
                        parentUI->HighlightCell(CurrentRow, CurrentCol, 'R');
                        break;
                    case 'M':
                        parentUI->HighlightCell(CurrentRow, CurrentCol, 'X');
                    break;
                }
            }
        }
    }
    void HideBoard() {
        for(int CurrentRow = 1; CurrentRow < PlayerBoard->getRows(); CurrentRow++)
        {
            for(int CurrentCol = 1; CurrentCol < PlayerBoard->getCols(); CurrentCol++)
            {
                parentUI->HighlightCell(CurrentRow, CurrentCol, 'D');
            }
        }
    }
    // Replaces each char with an E for empty
    void clearBoard(char replacementKey)
    {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                setCell(i, j, replacementKey);
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
    parentUI(ui)
{
    ResetCoordinates();
}
void BattleShipBoard::ResetCoordinates() {
    OriginalCoords.setX(-1);
    OriginalCoords.setY(-1);
    FinalCoords.setX(-1);
    FinalCoords.setY(-1);
    firstPointPlaced = false;
    secondPointPlaced = false;
}
char BattleShipBoard::GetDirection() {

        char direction = 'D';
        if(OriginalCoords.getX() == FinalCoords.getX())
        {
            if(OriginalCoords.getY() < FinalCoords.getY())
            {
                direction = 'D';
            }
            else
            {
                direction = 'U';
            }
        }
        else if(OriginalCoords.getY() == FinalCoords.getY())
        {
            if(OriginalCoords.getX() < FinalCoords.getX())
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
            for(int xCoord = OriginalCoords.getX(); xCoord <= FinalCoords.getX() && available; xCoord++)
            {
                int result = FinalCoords.getX() - OriginalCoords.getX() + 1; // Inclusive calculation
                if(PlayerBoard->getCell(OriginalCoords.getY(), xCoord) == 'S' || currentSize != result)
                {
                    available = false;
                    if(PlayerBoard->getCell(OriginalCoords.getY(), xCoord) == 'S')
                    {
                        parentUI->SetGameStatus("This selection crosses an existing ship, select a new ship.");
                    }
                    else
                    {
                        parentUI->SetGameStatus("We need a size of " + std::to_string(currentSize) + ", this isn't a big enough ship.");
                    }
                    break;
                }
            }
            if(available)
            {
                // Highlight between the coordinates
                for(int xCoord = OriginalCoords.getX(); xCoord <= FinalCoords.getX() && available; xCoord++)
                {
                    if(Paint)
                    {
                        parentUI->HighlightCell(OriginalCoords.getY(), xCoord, 'G');
                    }
                    PlayerBoard->setCell(OriginalCoords.getY(), xCoord, 'S');
                }
            }
            break;
        case 'L':
            // From right to left
            // check between the coordinates first
            for(int xCoord = OriginalCoords.getX(); xCoord >= FinalCoords.getX() && available; xCoord--)
        {
                 int result = OriginalCoords.getX() - FinalCoords.getX() + 1; // Inclusive calculation
            if(PlayerBoard->getCell(OriginalCoords.getY(), xCoord) == 'S' || currentSize != result)
            {
                available = false;
                if(PlayerBoard->getCell(OriginalCoords.getY(), xCoord) == 'S')
                {
                    parentUI->SetGameStatus("This selection crosses an existing ship, select a new ship.");
                }
                else
                {
                    parentUI->SetGameStatus("We need a size of " + std::to_string(currentSize) + ", this isn't a big enough ship.");
                }
                break;
            }
        }
            if(available)
        {
            // Highlight between the coordinates
            for(int xCoord = OriginalCoords.getX(); xCoord >= FinalCoords.getX() && available; xCoord--)
            {
                if(Paint)
                {
                    parentUI->HighlightCell(OriginalCoords.getY(), xCoord, 'G');
                }
                PlayerBoard->setCell(OriginalCoords.getY(), xCoord, 'S');
            }
        }
            break;
        case 'U':
            // From down to up
            // check between the coordinates first
            for(int yCoord = OriginalCoords.getY(); yCoord >= FinalCoords.getY() && available; yCoord--)
            {
                int result = OriginalCoords.getY() - FinalCoords.getY() + 1; // Inclusive calculation
                if(PlayerBoard->getCell(yCoord, OriginalCoords.getX()) == 'S' || currentSize != result)
                {
                    available = false;
                    if(PlayerBoard->getCell(yCoord, OriginalCoords.getX()) == 'S')
                    {
                        parentUI->SetGameStatus("This selection crosses an existing ship, select a new ship.");
                    }
                    else
                    {
                        parentUI->SetGameStatus("We need a size of " + std::to_string(currentSize) + ", this isn't a big enough ship.");
                    }
                    break;
                }
            }
            if(available)
            {
                // Highlight between the coordinates
                for(int yCoord = OriginalCoords.getY(); yCoord >= FinalCoords.getY(); yCoord--)
                {
                    if(Paint)
                    {
                        parentUI->HighlightCell(yCoord, OriginalCoords.getX(), 'G');
                    }
                    PlayerBoard->setCell(yCoord, OriginalCoords.getX(), 'S');
                }
            }
            break;
        case 'D':
            // from up to down
            // check between the coordinates first
            for(int yCoord = OriginalCoords.getY(); yCoord <= FinalCoords.getY() && available; yCoord++)
            {
                int result = FinalCoords.getY() - OriginalCoords.getY() + 1; // Inclusive calculation
                if(PlayerBoard->getCell(yCoord, OriginalCoords.getX()) == 'S' || currentSize != result)
                {
                    available = false;
                    if(PlayerBoard->getCell(yCoord, OriginalCoords.getX()) == 'S')
                    {
                        parentUI->SetGameStatus("This selection crosses an existing ship.");
                    }
                    else
                    {
                        parentUI->SetGameStatus("We need a size of " + std::to_string(currentSize) + ", this isn't a big enough ship.");
                    }
                    break;
                }
            }
            if(available)
            {
                // Highlight between the coordinates
                for(int yCoord = OriginalCoords.getY(); yCoord <= FinalCoords.getY(); yCoord++)
                {
                    if(Paint)
                    {
                        parentUI->HighlightCell(yCoord, OriginalCoords.getX(), 'G');
                    }
                    PlayerBoard->setCell(yCoord, OriginalCoords.getX(), 'S');
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
    Ship newShip(OriginalCoords.getX(), OriginalCoords.getY(), FinalCoords.getX(), FinalCoords.getY(), parentUI);
    ShipsOnBoard.push_back(newShip);
}
// Place a ship on the board
void BattleShipBoard::PlaceShip(int Col, int Row) {
    if(ShipSizes.size())
    {
        if(secondPointPlaced)
        {
            //Cancel this ship all together
            parentUI->HighlightCell(FinalCoords.getY(), FinalCoords.getX(), 'D');
            parentUI->HighlightCell(OriginalCoords.getY(), OriginalCoords.getX(), 'D');
            ResetCoordinates();
        }
        else if (!firstPointPlaced) {
            if(PlayerBoard->getCell(Row, Col) != 'S')
            {
                // just place the first point
                OriginalCoords.setX(Col);
                OriginalCoords.setY(Row);
                if (parentUI) {
                    parentUI->HighlightCell(OriginalCoords.getY(), OriginalCoords.getX(), 'P');
                }
                firstPointPlaced = true;
                parentUI->SetGameStatus("Now click a button for the second piece (vertically or horizontally)!");
            }
        }
        else
        {
            // We cancel initial ship placement
            if(Col == OriginalCoords.getX() && OriginalCoords.getY() == Row)
            {
                if (parentUI) {
                    parentUI->HighlightCell(Row, Col, 'D');
                }
                parentUI->SetGameStatus("Please click a square to put the start position of your ship!");
                parentUI->SetGameStatus("Click a button to place the first piece!");
                ResetCoordinates();
            }
            else
            {
                //Place the second point
                if (!secondPointPlaced) {
                    if(PlayerBoard->getCell(Row, Col) != 'S')
                    {
                        //if it's on the X or Y coordinate, it's not diagnal
                        if(Col == OriginalCoords.getX() || OriginalCoords.getY() == Row)
                        {
                            FinalCoords.setX(Col);
                            FinalCoords.setY(Row);
                            if (parentUI) {
                                parentUI->HighlightCell(Row, Col, 'P');
                            }
                            secondPointPlaced = true;
                        }
                        else
                        {
                            parentUI->SetGameStatus("You can't place a diagnal ship, sorry I don't make the rules");
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
                parentUI->HighlightCell(FinalCoords.getY(), FinalCoords.getX(), 'D');
                parentUI->HighlightCell(OriginalCoords.getY(), OriginalCoords.getX(), 'D');
                ResetCoordinates();
                //parentUI->SetGameStatus("Please click a square to put the start position of your ship!");
            }
            else
            {
                CreateShip();
                //we created one ship, let's pop the one we used
                ShipSizes.pop_back();
                ResetCoordinates();
                if(ShipSizes.size())
                {
                    parentUI->SetGameStatus("Please click a square to put the start position of your ship!");
                    parentUI->SetViewStatus("The current needed size is " + std::to_string(PlayerOneBoard->ShipSizes.back()) + ". Ship's left: " + std::to_string(ShipSizes.size()));
                }
                else
                {
                    placeMode = false;
                    PlayerOnesTurn = true;
                    parentUI->SetModeStatus("Viewing our board mode");
                    parentUI->SetGameStatus("These are the ships you placed, swap views with the button to the right to start attacking!");
                    parentUI->SetViewStatus("You are viewing your board");
                }
            }
        }
    }
    else {
        //we ran out of ships to do
        placeMode = false;
        PlayerOnesTurn = true;
    }
}

// Placeholder: receive attack
bool BattleShipBoard::RecieveAttack(int Col, int Row) {
    bool attackHit = false;
    for(int i = 0; i < ShipsOnBoard.size(); i++)
    {
        if(ShipsOnBoard[i].CheckIfHit(Col, Row))
        {
            attackHit = true;
        }
    }
    return attackHit;
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
            int X = shipToRemove.ourPieces[i].getX();
            int Y = shipToRemove.ourPieces[i].getY();
            PlayerBoard->setCell(Y, X, 'E');
        }
        ShipsOnBoard.back().RemoveShipInUI();
        ShipsOnBoard.pop_back();
        ResetCoordinates();
        parentUI->SetGameStatus("Please click a square to put the start position of your ship!");
        parentUI->SetViewStatus("The current needed size is " + std::to_string(PlayerOneBoard->ShipSizes.back()) + ". Ship's left: " + std::to_string(ShipSizes.size()));
    }
    else
    {
        parentUI->SetGameStatus("No ship's to remove, please add one!");
    }
}

// --------------------- AI LOGIC ---------------------
class AI {
public:
    BattleShipBoard* AIBoard;
    Coordinate* LastHitCoords = new Coordinate(-1, -1);
    // Constructor takes a pointer to a BattleshipBoard
    AI(BattleShipBoard* newAIBoard) {
        AIBoard = newAIBoard; // Initialize the pointer
    }
    void MakeRandomAttack();
    /* AI SHip Placement code*/
    void MakeProperSize(int shipSize)
    {
        char Direction = AIBoard->GetDirection();
        bool RightSize = false; // Initialize RightSize to false
        switch(Direction)
        {
        case 'U':
            while(!RightSize)
            {
                int result = AIBoard->OriginalCoords.getY() - AIBoard->FinalCoords.getY() + 1; // Inclusive calculation
                if(result == shipSize)
                {
                    RightSize = true;
                }
                else
                {
                    // Adjust coordinates so that it fits
                    if(result < shipSize)
                    {
                        if(AIBoard->FinalCoords.getY() == 1)
                        {
                            AIBoard->OriginalCoords.incrementY();
                        }
                        else
                        {
                            AIBoard->FinalCoords.decrementY();
                        }
                    }
                    else
                    {
                        AIBoard->FinalCoords.incrementY();
                    }
                }
            }
            break;
        case 'D':
            while(!RightSize)
            {
                int result = AIBoard->FinalCoords.getY() - AIBoard->OriginalCoords.getY() + 1; // Inclusive calculation
                if(result == shipSize)
                {
                    RightSize = true;
                }
                else
                {
                    // Adjust coordinates so that it fits
                    if(result < shipSize)
                    {
                        if(AIBoard->OriginalCoords.getY() == 1)
                        {
                            AIBoard->FinalCoords.incrementY();
                        }
                        else
                        {
                            AIBoard->OriginalCoords.decrementY();
                        }
                    }
                    else
                    {
                        AIBoard->FinalCoords.incrementY();
                    }
                }
            }
            break;
        case 'L':
            while(!RightSize)
            {
                int result = AIBoard->OriginalCoords.getX() - AIBoard->FinalCoords.getX() + 1; // Inclusive calculation
                if(result == shipSize)
                {
                    RightSize = true;
                }
                else
                {
                    // Adjust coordinates so that it fits
                    if(result < shipSize)
                    {
                        if(AIBoard->FinalCoords.getX() == 1)
                        {
                            AIBoard->OriginalCoords.incrementX();
                        }
                        else
                        {
                            AIBoard->FinalCoords.decrementX();
                        }
                    }
                    else
                    {
                        AIBoard->FinalCoords.incrementX();
                    }
                }
            }
            break;
        case 'R':
            while(!RightSize)
            {
                int result = AIBoard->FinalCoords.getX() - AIBoard->OriginalCoords.getX() + 1; // Inclusive calculation
                if(result == shipSize)
                {
                    RightSize = true;
                }
                else
                {
                    // Adjust coordinates so that it fits
                    if(result < shipSize)
                    {
                        if(AIBoard->OriginalCoords.getX() == 1)
                        {
                            AIBoard->FinalCoords.incrementX();
                        }
                        else
                        {
                            AIBoard->OriginalCoords.decrementX();
                        }
                    }
                    else
                    {
                        AIBoard->FinalCoords.incrementX();
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
                AIBoard->OriginalCoords.setX(rand() % 10);
                AIBoard->FinalCoords.setX(AIBoard->OriginalCoords.getX());
                AIBoard->FinalCoords.setY(rand() % (10 - shipSize));
                AIBoard->OriginalCoords.setY(rand() % 10);
            }
            else //horizontal
            {
                AIBoard->OriginalCoords.setY(rand() % 10);
                AIBoard->FinalCoords.setY(AIBoard->OriginalCoords.getY());
                AIBoard->FinalCoords.setX(rand() % (10 - shipSize));
                AIBoard->OriginalCoords.setX(rand() % 10);
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
void SendAttack(int Col, int Row, battleship* parentUI) {
    if(PlayerOnesTurn)
    {
        if(ourAI->AIBoard->RecieveAttack(Col, Row))
        {
            parentUI->HighlightCell(Row, Col, 'R');
            BoardGrid* CurrentGrid = ourAI->AIBoard->GetPlayerBoard();
            CurrentGrid->setCell(Row, Col, 'H');
        }
        else
        {
            parentUI->HighlightCell(Row, Col, 'X');
            BoardGrid* CurrentGrid = ourAI->AIBoard->GetPlayerBoard();
            CurrentGrid->setCell(Row, Col, 'M');
        }
        PlayerOnesTurn = false;
        ourAI->MakeRandomAttack();
    }
    else
    {
        if(PlayerOneBoard->RecieveAttack(Col, Row))
        {
            parentUI->HighlightCell(Row, Col, 'R');
            BoardGrid* CurrentGrid = PlayerOneBoard->GetPlayerBoard();
            CurrentGrid->setCell(Row, Col, 'H');
        }
        else
        {
            parentUI->HighlightCell(Row, Col, 'X');
            BoardGrid* CurrentGrid = PlayerOneBoard->GetPlayerBoard();
            CurrentGrid->setCell(Row, Col, 'M');
        }
        PlayerOnesTurn = false;
        // call AI code here
    }
}

void AI::MakeRandomAttack()
{
    SendAttack(1, 1, AIBoard->parentUI);
}
// --------------------- Button click handler ---------------------
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
        if(PlayerOnesTurn && PlayerBoardVisible == false)
        {
        SendAttack(col, row, this); // Mark attack
        }
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
    // Set up ui
    GameStatusText = this->findChild<QPlainTextEdit*>("GameStatus");
    ViewStatusText = this->findChild<QPlainTextEdit*>("ViewStatus");
    ModeStatusText = this->findChild<QPlainTextEdit*>("ModeStatus");
    SetModeStatus("Place Ship Mode");
    SetViewStatus("The current needed size is " + std::to_string(PlayerOneBoard->ShipSizes.back()) + ". Ship's left: 5");
    SetGameStatus("Please click a square to put the start position of your ship!");
}

battleship::~battleship() {
    delete ui;
}
void battleship::SetGameStatus(std::string text)
{
    GameStatusText->setPlainText(QString::fromStdString(text));
}
void battleship::SetModeStatus(std::string text) {
    ModeStatusText->setPlainText(QString::fromStdString(text));
}
void battleship::SetViewStatus(std::string text) {
    ViewStatusText->setPlainText(QString::fromStdString(text));
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

/*
 * Button handlers
 */
void battleship::on_UndoButton_clicked()
{
    if(placeMode)
    {
        if(PlayerOneBoard)
        {
            PlayerOneBoard->RemoveLastShip();
        }
    }
}
void swapBoards()
{
    //swap to the enemy board
    if(PlayerBoardVisible)
    {
        PlayerBoardVisible = false;
        PlayerOneBoard->HideBoard();
        //Temporary
        ourAI->AIBoard->DisplayShips();
        ourAI->AIBoard->DisplayMissesAndHits();
        PlayerOneBoard->parentUI->SetViewStatus("You are viewing the AI board.");
        PlayerOneBoard->parentUI->SetGameStatus("Select a square to attack the AI's board");
        PlayerOneBoard->parentUI->SetModeStatus("Attack Mode");
    }
    else //swap to the Player's board.
    {
        PlayerBoardVisible = true;
        ourAI->AIBoard->HideBoard();
        PlayerOneBoard->DisplayMissesAndHits();
        PlayerOneBoard->DisplayShips();
        PlayerOneBoard->parentUI->SetGameStatus("These are the ships you placed, swap views with the button to the right to start attacking!");
        PlayerOneBoard->parentUI->SetViewStatus("You are viewing your board.");
        PlayerOneBoard->parentUI->SetModeStatus("Viewing our board mode");
    }
}
void battleship::on_ViewButton_clicked()
{
    if(!placeMode)
    {
        swapBoards();
    }
}


void battleship::on_ResetButton_clicked()
{
    PlayerOnesTurn = false;
    placeMode = true;
    PlayerOneBoard->clearBoard('E');
    ourAI->AIBoard->clearBoard('E');
    PlayerOneBoard->HideBoard();
    PlayerOneBoard->ResetCoordinates();
    ourAI->AIBoard->ResetCoordinates();
    delete ourAI;
    delete AIBoard;
    delete PlayerOneBoard;
    PlayerOneBoard = new BattleShipBoard(this);
    AIBoard = new BattleShipBoard(this);
    ourAI = new AI(AIBoard);
    SetModeStatus("Place Ship Mode");
    SetViewStatus("The current needed size is " + std::to_string(PlayerOneBoard->ShipSizes.back()) + ". Ship's left: 5");
    SetGameStatus("Please click a square to put the start position of your ship!");
}

