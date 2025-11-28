#include "battleship.h"
#include "ui_battleship.h"
#include "boardgrid.h"
#include "Coordinates.h"
#include "ShipSystem.h"
#include <vector>
#include <QDebug>
#include <QPushButton>
#include <QPlainTextEdit>
#include <ctime>
#include <cstdlib>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

using namespace std;

// --------------------- Forward declarations ---------------------
class BattleShipBoard;
class AI;
void SendAttack(int Col, int Row, battleship* parentUI);
void swapBoards();

// --------------------- Global Variables ---------------------
BattleShipBoard* PlayerOneBoard = nullptr;
BattleShipBoard* AIBoard = nullptr;
AI* ourAI = nullptr;
QPlainTextEdit* ModeStatusText = nullptr;
QPlainTextEdit* GameStatusText = nullptr;
QPlainTextEdit* ViewStatusText = nullptr;
bool GameOver = false;
bool placeMode = true;
bool PlayerOnesTurn = false;
bool PlayerBoardVisible = true;

// --------------------- BattleShipBoard Class ---------------------
class BattleShipBoard : public BoardGrid {
public:
    battleship* parentUI;
    vector<Ship*> ShipsOnBoard;
    Coordinate OriginalCoords;
    Coordinate FinalCoords;
    bool firstPointPlaced = false;
    bool secondPointPlaced = false;
    vector<int> ShipSizes = {5, 4, 3, 3, 2}; // standard Battleship sizes
    int shipsSunk = 0;

    BattleShipBoard(battleship* ui);
    ~BattleShipBoard();

    void PlaceShip(int Col, int Row);
    bool CheckSlotsIfAvailable(bool Paint);
    void ConfirmShip();
    bool RecieveAttack(int Col, int Row);
    void RemoveLastShip();
    char GetDirection();
    void CreateShip();
    void ResetCoordinates();
    bool CanPlaceShip(Coordinate start, Coordinate end);
    void DisplayShips();
    void DisplayMissesAndHits();
    void HideBoard();
    void clearBoard(char replacementKey);

    bool setCell(int row, int col, char symbol) override; // override virtual
};

// --------------------- Constructor & Destructor ---------------------
BattleShipBoard::BattleShipBoard(battleship* ui)
    : BoardGrid(10, 10), parentUI(ui) {
    ResetCoordinates();
}

BattleShipBoard::~BattleShipBoard() {
    for (auto ship : ShipsOnBoard) delete ship;
}

// --------------------- Override setCell ---------------------
bool BattleShipBoard::setCell(int row, int col, char symbol) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        board[row][col] = symbol;
        return true;
    }
    return false;
}

// --------------------- Coordinate reset ---------------------
void BattleShipBoard::ResetCoordinates() {
    OriginalCoords.setX(-1); OriginalCoords.setY(-1);
    FinalCoords.setX(-1); FinalCoords.setY(-1);
    firstPointPlaced = false;
    secondPointPlaced = false;
}

// --------------------- Determine direction ---------------------
char BattleShipBoard::GetDirection() {
    char direction = 'D'; // default
    if (OriginalCoords.getX() == FinalCoords.getX()) {
        direction = (OriginalCoords.getY() < FinalCoords.getY()) ? 'D' : 'U';
    } else if (OriginalCoords.getY() == FinalCoords.getY()) {
        direction = (OriginalCoords.getX() < FinalCoords.getX()) ? 'R' : 'L';
    }
    return direction;
}

// --------------------- Check placement ---------------------
bool BattleShipBoard::CanPlaceShip(Coordinate start, Coordinate end) {
    int x1 = start.getX(), y1 = start.getY();
    int x2 = end.getX(), y2 = end.getY();
    int dx = (x2 > x1) ? 1 : (x2 < x1 ? -1 : 0);
    int dy = (y2 > y1) ? 1 : (y2 < y1 ? -1 : 0);

    int x = x1, y = y1;
    while (true) {
        if (board[y][x] == 'S') return false; // overlap
        if (x == x2 && y == y2) break;
        x += dx; y += dy;
    }
    return true;
}

// --------------------- Check and optionally paint ---------------------
bool BattleShipBoard::CheckSlotsIfAvailable(bool Paint) {
    if (ShipSizes.empty()) return false;

    char dir = GetDirection();
    int dx = 0, dy = 0, length = 0;

    // Determine direction and length
    switch (dir) {
    case 'R': dx = 1; dy = 0; length = FinalCoords.getX() - OriginalCoords.getX() + 1; break;
    case 'L': dx = -1; dy = 0; length = OriginalCoords.getX() - FinalCoords.getX() + 1; break;
    case 'D': dx = 0; dy = 1; length = FinalCoords.getY() - OriginalCoords.getY() + 1; break;
    case 'U': dx = 0; dy = -1; length = OriginalCoords.getY() - FinalCoords.getY() + 1; break;
    default:
        parentUI->SetGameStatus("Invalid ship direction!");
        return false;
    }

    // Check length matches required size
    if (length != ShipSizes.front()) {
        parentUI->SetGameStatus("Wrong ship size! Need " + std::to_string(ShipSizes.front()));
        return false;
    }

    // Check for overlapping ships
    int x = OriginalCoords.getX();
    int y = OriginalCoords.getY();
    for (int i = 0; i < length; ++i) {
        if (board[y][x] == 'S') {
            parentUI->SetGameStatus("Cannot place ship here! Overlaps another ship.");
            return false;
        }
        x += dx; y += dy;
    }

    // Paint the ship on board if requested
    if (Paint) {
        x = OriginalCoords.getX();
        y = OriginalCoords.getY();
        for (int i = 0; i < length; ++i) {
            setCell(y, x, 'S');
            parentUI->HighlightCell(y + 1, x + 1, 'S');
            x += dx; y += dy;
        }
    }

    return true;
}


// --------------------- Create ship object ---------------------
void BattleShipBoard::CreateShip() {
    Ship* newShip = new Ship(OriginalCoords.getX(), OriginalCoords.getY(),
                             FinalCoords.getX(), FinalCoords.getY(), parentUI);
    ShipsOnBoard.push_back(newShip);
}

// --------------------- Place ship ---------------------
void BattleShipBoard::PlaceShip(int Col, int Row) {
    int x = Col - 1;  // Convert to 0-based
    int y = Row - 1;

    // First point
    if (!firstPointPlaced) {
        if (board[y][x] != 'S') {
            OriginalCoords.setX(x);
            OriginalCoords.setY(y);
            firstPointPlaced = true;
            parentUI->HighlightCell(Row, Col, 'P');
            parentUI->SetGameStatus("Select second point of the ship!");
        } else {
            parentUI->SetGameStatus("First point cannot overlap another ship!");
        }
        return;
    }

    // Second point
    if (!secondPointPlaced) {
        // Must align horizontally or vertically
        if ((x == OriginalCoords.getX() || y == OriginalCoords.getY()) && board[y][x] != 'S') {
            FinalCoords.setX(x);
            FinalCoords.setY(y);
            secondPointPlaced = true;
            parentUI->HighlightCell(Row, Col, 'P');

            // Validate placement
            if (CheckSlotsIfAvailable(true)) {
                CreateShip();
                ShipSizes.erase(ShipSizes.begin()); // Remove first (largest) ship size
                ResetCoordinates();

                // All ships placed
                if (ShipSizes.empty()) {
                    placeMode = false;
                    PlayerOnesTurn = true;
                    parentUI->SetModeStatus("Viewing our board mode");
                    parentUI->SetGameStatus("All ships placed! Swap view to attack.");
                    parentUI->SetViewStatus("You are viewing your board");
                } else {
                    parentUI->SetGameStatus(
                        "Place next ship of size " + std::to_string(ShipSizes.front())
                        );
                }
            } else {
                // Invalid placement, clear highlights
                parentUI->HighlightCell(OriginalCoords.getY() + 1, OriginalCoords.getX() + 1, 'E');
                parentUI->HighlightCell(FinalCoords.getY() + 1, FinalCoords.getX() + 1, 'E');
                ResetCoordinates();
            }
        } else {
            parentUI->SetGameStatus("Second point must be aligned horizontally or vertically and not overlap!");
        }
    }
}


// --------------------- Receive attack ---------------------
bool BattleShipBoard::RecieveAttack(int Col, int Row) {
    int x = Col-1, y = Row-1;
    bool hit = false;
    for (auto &ship : ShipsOnBoard) {
        if (ship->CheckIfHit(x, y)) {
            hit = true;
            setCell(y,x,'H');
            parentUI->HighlightCell(Row,Col,'H');
            if (ship->CheckIfSunk()) {
                shipsSunk++;
                if (shipsSunk == 5) {
                    GameOver = true;
                    parentUI->SetGameStatus("Game Over!");
                }
            }
            break;
        }
    }
    if (!hit) {
        setCell(y,x,'M');
        parentUI->HighlightCell(Row,Col,'M');
    }
    return hit;
}

// --------------------- Remove last ship ---------------------
void BattleShipBoard::RemoveLastShip() {
    if (ShipsOnBoard.empty()) {
        parentUI->SetGameStatus("No ships to remove!");
        return;
    }

    // Remove the last ship object
    Ship* lastShip = ShipsOnBoard.back();

    // Restore ship size at the front
    ShipSizes.insert(ShipSizes.begin(), lastShip->ShipSize);

    // Remove ship from board grid and UI
    for (auto& piece : lastShip->ourPieces) {
        setCell(piece.getY(), piece.getX(), 'E');         // Clear internal board
        parentUI->HighlightCell(piece.getY() + 1, piece.getX() + 1, 'E'); // Clear UI
    }

    delete lastShip;
    ShipsOnBoard.pop_back();

    // Reset placement coordinates
    ResetCoordinates();

    // Update status message
    parentUI->SetGameStatus(
        "Last ship removed. Place a ship of size " + std::to_string(ShipSizes.front())
        );

    // Ensure placement mode is active
    placeMode = true;
}



// --------------------- Display ---------------------
void BattleShipBoard::DisplayShips() {
    for(int y=0; y<rows; y++)
        for(int x=0; x<cols; x++) {
            char c = board[y][x];
            if(c=='S') parentUI->HighlightCell(y+1, x+1,'S');
            else if(c=='H') parentUI->HighlightCell(y+1, x+1,'H');
            else if(c=='M') parentUI->HighlightCell(y+1, x+1,'M');
        }
}

void BattleShipBoard::DisplayMissesAndHits() {
    for(int y=0; y<rows; y++)
        for(int x=0; x<cols; x++) {
            char c = board[y][x];
            if(c=='H') parentUI->HighlightCell(y+1, x+1,'H');
            else if(c=='M') parentUI->HighlightCell(y+1, x+1,'M');
        }
}

void BattleShipBoard::HideBoard() {
    for(int y=0; y<rows; y++)
        for(int x=0; x<cols; x++)
            parentUI->HighlightCell(y+1,x+1,'E');
}

void BattleShipBoard::clearBoard(char replacementKey = 'E') {
    for (int y = 0; y < rows; y++)
        for (int x = 0; x < cols; x++)
            setCell(y, x, replacementKey);
}


// --------------------- AI Class ---------------------
class AI {
public:
    BattleShipBoard* AIBoard;
    AI(BattleShipBoard* board) : AIBoard(board) {}
    void PlaceAllShipsRandomly() {
        if(!AIBoard) return;
        while(!AIBoard->ShipSizes.empty()) {
            int size = AIBoard->ShipSizes.front();
            bool placed = false;
            while(!placed) {
                bool vertical = rand()%2==0;
                int x = rand()%10, y = rand()%10;
                int endX = vertical ? x : x+size-1;
                int endY = vertical ? y+size-1 : y;
                if(endX>=10 || endY>=10) continue;
                AIBoard->OriginalCoords.setX(x);
                AIBoard->OriginalCoords.setY(y);
                AIBoard->FinalCoords.setX(endX);
                AIBoard->FinalCoords.setY(endY);
                if(AIBoard->CanPlaceShip(AIBoard->OriginalCoords,AIBoard->FinalCoords)) {
                    AIBoard->CreateShip();
                    AIBoard->ShipSizes.pop_back();
                    AIBoard->ResetCoordinates();
                    placed = true;
                }
            }
        }
    }

    void MakeRandomAttack() {
        int x = rand()%10 + 1, y = rand()%10 + 1;
        SendAttack(x,y, AIBoard->parentUI);
    }
};

// --------------------- Send Attack ---------------------
void SendAttack(int Col,int Row,battleship* parentUI){
    if(PlayerOnesTurn){
        if(AIBoard->RecieveAttack(Col,Row))
            parentUI->HighlightCell(Row,Col,'R');
        else
            parentUI->HighlightCell(Row,Col,'X');
        PlayerOnesTurn=false;
        swapBoards();
        ourAI->MakeRandomAttack();
    } else {
        if(PlayerOneBoard->RecieveAttack(Col,Row))
            parentUI->HighlightCell(Row,Col,'R');
        else
            parentUI->HighlightCell(Row,Col,'X');
        PlayerOnesTurn=true;
    }
}

// --------------------- Swap Boards ---------------------
void swapBoards(){
    if(PlayerBoardVisible){
        PlayerOneBoard->HideBoard();
        AIBoard->DisplayShips();
        AIBoard->DisplayMissesAndHits();
        PlayerBoardVisible=false;
        if(ModeStatusText) ModeStatusText->setPlainText("Viewing AI Board Mode");
        if(ViewStatusText) ViewStatusText->setPlainText("You are viewing AI Board");
    } else {
        AIBoard->HideBoard();
        PlayerOneBoard->DisplayShips();
        PlayerOneBoard->DisplayMissesAndHits();
        PlayerBoardVisible=true;
        if(ModeStatusText) ModeStatusText->setPlainText("Viewing Our Board Mode");
        if(ViewStatusText) ViewStatusText->setPlainText("You are viewing your board");
    }
}

// --------------------- Battleship QWidget ---------------------
battleship::battleship(QWidget *parent)
    : QDialog(parent), ui(new Ui::battleship)
{
    srand(time(nullptr));
    ui->setupUi(this);

    if(!PlayerOneBoard) PlayerOneBoard = new BattleShipBoard(this);
    if(!AIBoard) AIBoard = new BattleShipBoard(this);

    ourAI = new AI(AIBoard);
    ourAI->PlaceAllShipsRandomly();

    for(int row=1; row<=10; ++row)
        for(int col=1; col<=10; ++col){
            QString name = QString("Coll%1R%2").arg(col).arg(row);
            QPushButton* btn = this->findChild<QPushButton*>(name);
            if(btn){
                connect(btn,&QPushButton::clicked,this,&battleship::onButtonClicked);
                btn->setStyleSheet("background-color:white; color:white;");
            }
        }

    GameStatusText = this->findChild<QPlainTextEdit*>("GameStatus");
    ViewStatusText = this->findChild<QPlainTextEdit*>("ViewStatus");
    ModeStatusText = this->findChild<QPlainTextEdit*>("ModeStatus");

    SetModeStatus("Place Ship Mode");
    SetViewStatus("Current needed size: " + std::to_string(PlayerOneBoard->ShipSizes.front()) + ". Ships left: 5");
    SetGameStatus("Click to place the start position of your ship!");
}

battleship::~battleship() { delete ui; }

// --------------------- Highlight Cell ---------------------
void battleship::HighlightCell(int row, int col, char ColorKey) {
    QString name = QString("Coll%1R%2").arg(col).arg(row);
    QPushButton* btn = this->findChild<QPushButton*>(name);
    if (!btn) return;

    switch (ColorKey) {
    case 'P': btn->setStyleSheet("background-color: purple; color: white;"); break;
    case 'S': btn->setStyleSheet("background-color: green; color: white;"); break;
    case 'E': btn->setStyleSheet("background-color: white; color: white;"); break;
    case 'M': btn->setStyleSheet("background-color: gray; color: white;"); break;
    case 'H': btn->setStyleSheet("background-color: red; color: white;"); break;
    case 'R': btn->setStyleSheet("background-color: orange; color: white;"); break;
    case 'X': btn->setStyleSheet("background-color: black; color: white;"); break;
    }
}

// --------------------- Button clicks ---------------------
void battleship::onButtonClicked() {
    if(GameOver || !PlayerOneBoard || !ourAI) return;

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;

    QString name = btn->objectName();
    static QRegularExpression regex("Coll(\\d+)R(\\d+)");
    QRegularExpressionMatch match = regex.match(name);
    if(!match.hasMatch()) return;

    int col = match.captured(1).toInt();
    int row = match.captured(2).toInt();

    if(!placeMode){
        if(PlayerOnesTurn && !PlayerBoardVisible) SendAttack(col,row,this);
    } else {
        PlayerOneBoard->PlaceShip(col,row);
    }
}

// --------------------- UI setters ---------------------
void battleship::SetGameStatus(std::string text){ if(GameStatusText) GameStatusText->setPlainText(QString::fromStdString(text)); }
void battleship::SetModeStatus(std::string text){ if(ModeStatusText) ModeStatusText->setPlainText(QString::fromStdString(text)); }
void battleship::SetViewStatus(std::string text){ if(ViewStatusText) ViewStatusText->setPlainText(QString::fromStdString(text)); }

void battleship::on_ViewButton_clicked() { if(!placeMode) swapBoards(); }

void battleship::on_ResetButton_clicked() {
    // Reset global game state
    GameOver = false;
    PlayerOnesTurn = false;
    placeMode = true;
    PlayerBoardVisible = true;

    // Delete old boards and AI
    delete ourAI; ourAI = nullptr;
    delete PlayerOneBoard; PlayerOneBoard = nullptr;
    delete AIBoard; AIBoard = nullptr;

    // Create fresh boards
    PlayerOneBoard = new BattleShipBoard(this);
    AIBoard = new BattleShipBoard(this);

    // Recreate AI
    ourAI = new AI(AIBoard);
    ourAI->PlaceAllShipsRandomly();

    // Clear and hide boards
    PlayerOneBoard->clearBoard();
    PlayerOneBoard->HideBoard();
    AIBoard->clearBoard();
    AIBoard->HideBoard();

    // Update UI status
    SetModeStatus("Place Ship Mode");
    SetViewStatus(
        "Next size: " + std::to_string(PlayerOneBoard->ShipSizes.front()) +
        ". Ships left: " + std::to_string(PlayerOneBoard->ShipSizes.size())
        );
    SetGameStatus("Click to place the start position of your ship!");
}


void battleship::on_UndoButton_clicked() {
    if (!PlayerOneBoard) return;
    PlayerOneBoard->RemoveLastShip();
}
