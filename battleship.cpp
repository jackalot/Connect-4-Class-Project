#include "battleship.h"
#include "ui_battleship.h"
#include <QRegularExpression>
#include "boardgrid.h"
#include "Coordinates.h"
#include <vector>
#include <QDebug>
#include "ShipSystem.h"
#include <ctime>
#include <cstdlib>

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
    BoardGrid* PlayerBoard;

public:
    battleship* parentUI;
    int shipsSunk = 0;
    vector<Ship*> ShipsOnBoard;
    Coordinate OriginalCoords;
    Coordinate FinalCoords;
    bool firstPointPlaced = false;
    bool secondPointPlaced = false;
    vector<int> ShipSizes = {2, 2, 3, 4, 5};

    BattleShipBoard(battleship* ui);
    ~BattleShipBoard(); // destructor added

    void PlaceShip(int Col, int Row);
    bool CheckSlotsIfAvailable(bool Paint);
    void ConfirmShip();
    bool RecieveAttack(int Col, int Row);
    void RemoveLastShip();
    char GetDirection();
    void CreateShip();
    void ResetCoordinates();
    bool CanPlaceShip(Coordinate start, Coordinate end);
    BoardGrid* GetPlayerBoard() { return PlayerBoard; }

    void DisplayShips();
    void DisplayMissesAndHits();
    void HideBoard();
    void clearBoard(char replacementKey);
};

// --------------------- Destructor ---------------------
BattleShipBoard::~BattleShipBoard() {
    delete PlayerBoard; // important to free memory
}

// --------------------- Constructor ---------------------
BattleShipBoard::BattleShipBoard(battleship* ui)
    : BoardGrid(10, 10),
    PlayerBoard(new BoardGrid(10, 10)),
    parentUI(ui)
{
    ResetCoordinates();
}

// --------------------- Coordinate reset ---------------------
void BattleShipBoard::ResetCoordinates() {
    OriginalCoords.setX(-1);
    OriginalCoords.setY(-1);
    FinalCoords.setX(-1);
    FinalCoords.setY(-1);
    firstPointPlaced = false;
    secondPointPlaced = false;
}

// --------------------- Direction ---------------------
char BattleShipBoard::GetDirection() {
    char direction = 'D';
    if (OriginalCoords.getX() == FinalCoords.getX()) {
        direction = (OriginalCoords.getY() < FinalCoords.getY()) ? 'D' : 'U';
    } else if (OriginalCoords.getY() == FinalCoords.getY()) {
        direction = (OriginalCoords.getX() < FinalCoords.getX()) ? 'R' : 'L';
    }
    return direction;
}

// --------------------- Check if ship can be placed ---------------------
bool BattleShipBoard::CanPlaceShip(Coordinate start, Coordinate end) {
    int x1 = start.getX();
    int y1 = start.getY();
    int x2 = end.getX();
    int y2 = end.getY();

    int dx = (x2 > x1) ? 1 : (x2 < x1 ? -1 : 0);
    int dy = (y2 > y1) ? 1 : (y2 < y1 ? -1 : 0);

    int x = x1;
    int y = y1;

    while(true) {
        if(PlayerBoard->getCell(y, x) == 'S') return false; // overlap
        if(x == x2 && y == y2) break;
        x += dx;
        y += dy;
    }
    return true;
}

// --------------------- Check slots and optionally paint ---------------------
bool BattleShipBoard::CheckSlotsIfAvailable(bool Paint) {
    bool available = true;
    char direction = GetDirection();
    if (ShipSizes.empty()) return false;

    int currentSize = ShipSizes.back();
    int dx = 0, dy = 0, length = 0;

    switch (direction) {
    case 'R': dx = 1; dy = 0; length = FinalCoords.getX() - OriginalCoords.getX() + 1; break;
    case 'L': dx = -1; dy = 0; length = OriginalCoords.getX() - FinalCoords.getX() + 1; break;
    case 'D': dx = 0; dy = 1; length = FinalCoords.getY() - OriginalCoords.getY() + 1; break;
    case 'U': dx = 0; dy = -1; length = OriginalCoords.getY() - FinalCoords.getY() + 1; break;
    default: available = false;
    }

    if (length != currentSize) {
        available = false;
        parentUI->SetGameStatus("Wrong ship size! Need " + std::to_string(currentSize));
        return false;
    }

    int x = OriginalCoords.getX();
    int y = OriginalCoords.getY();
    for (int i = 0; i < length; ++i) {
        if (PlayerBoard->getCell(y, x) == 'S') {
            available = false;
            parentUI->SetGameStatus("This selection crosses an existing ship.");
            break;
        }
        x += dx;
        y += dy;
    }

    if (Paint && available) {
        x = OriginalCoords.getX();
        y = OriginalCoords.getY();
        for (int i = 0; i < length; ++i) {
            PlayerBoard->setCell(y, x, 'S'); // confirmed ship
            parentUI->HighlightCell(y + 1, x + 1, 'S');
            x += dx;
            y += dy;
        }
    }

    return available;
}


// --------------------- Create ship object ---------------------
void BattleShipBoard::CreateShip() {
    Ship* newShip = new Ship(OriginalCoords.getX(), OriginalCoords.getY(),
                             FinalCoords.getX(), FinalCoords.getY(), parentUI);
    ShipsOnBoard.push_back(newShip);
}

// --------------------- Place ship ---------------------
void BattleShipBoard::PlaceShip(int Col, int Row) {
    int x = Col - 1; // 0-based
    int y = Row - 1;

    if (ShipSizes.empty()) return;

    if (!firstPointPlaced) {
        if (PlayerBoard->getCell(y, x) != 'S') {
            OriginalCoords.setX(x);
            OriginalCoords.setY(y);
            parentUI->HighlightCell(Row, Col, 'P'); // preview
            firstPointPlaced = true;
            parentUI->SetGameStatus("Click for second square (vertical/horizontal)!");
        }
    } else if (!secondPointPlaced) {
        if ((x == OriginalCoords.getX() || y == OriginalCoords.getY()) && PlayerBoard->getCell(y, x) != 'S') {
            FinalCoords.setX(x);
            FinalCoords.setY(y);
            parentUI->HighlightCell(Row, Col, 'P'); // preview
            secondPointPlaced = true;
        } else {
            parentUI->SetGameStatus("Invalid ship placement.");
            return;
        }

        if(firstPointPlaced && secondPointPlaced){
            if(!CheckSlotsIfAvailable(true)){
                // Invalid placement, reset previews
                parentUI->HighlightCell(OriginalCoords.getY()+1, OriginalCoords.getX()+1,'E');
                parentUI->HighlightCell(FinalCoords.getY()+1, FinalCoords.getX()+1,'E');
            } else {
                // Confirm ship
                CreateShip(); // internal vector
                ShipSizes.pop_back();

                // Update the board visually to confirmed color
                char direction = GetDirection();
                int x = OriginalCoords.getX();
                int y = OriginalCoords.getY();
                int dx = 0, dy = 0, length = 0;

                switch(direction){
                case 'R': dx=1; dy=0; length=FinalCoords.getX()-OriginalCoords.getX()+1; break;
                case 'L': dx=-1; dy=0; length=OriginalCoords.getX()-FinalCoords.getX()+1; break;
                case 'D': dx=0; dy=1; length=FinalCoords.getY()-OriginalCoords.getY()+1; break;
                case 'U': dx=0; dy=-1; length=OriginalCoords.getY()-FinalCoords.getY()+1; break;
                }

                for(int i=0;i<length;++i){
                    PlayerBoard->setCell(y,x,'S');   // internal board
                    parentUI->HighlightCell(y+1,x+1,'G'); // green confirmed
                    x += dx;
                    y += dy;
                }

                ResetCoordinates();
                if(ShipSizes.empty()){
                    placeMode=false;
                    PlayerOnesTurn=true;
                    parentUI->SetModeStatus("Viewing our board mode");
                    parentUI->SetGameStatus("Ships placed. Swap view to start attacking!");
                    parentUI->SetViewStatus("You are viewing your board");
                } else {
                    parentUI->SetGameStatus("Place next ship.");
                    parentUI->SetViewStatus("Next size: " + std::to_string(ShipSizes.back()));
                }
            }
            ResetCoordinates();
        }

    }
}


// --------------------- Receive attack ---------------------
bool BattleShipBoard::RecieveAttack(int Col, int Row) {
    int x = Col - 1;
    int y = Row - 1;
    Ship* CurrentShip = nullptr;
    bool hit = false;
    for (auto &ship : ShipsOnBoard) {
        if (ship->CheckIfHit(x, y)) {
            CurrentShip = ship; // store pointer
            hit = true;
            break;               // stop after first hit
        }
    }

    if (CurrentShip) {
        PlayerBoard->setCell(y, x, 'H');
        parentUI->HighlightCell(Row, Col, 'H');
        if (CurrentShip->CheckIfSunk()) {
            shipsSunk++;
            if (shipsSunk == 5) {
                GameOver = true;
                parentUI->SetGameStatus("Game Over, click reset below the board");
                parentUI->SetViewStatus("Game Over");
            }
        }
    } else {
        PlayerBoard->setCell(y, x, 'M');
        parentUI->HighlightCell(Row, Col, 'M');
    }
    return hit;
}


// --------------------- Remove last ship ---------------------
void BattleShipBoard::RemoveLastShip() {
    if (ShipsOnBoard.empty()) {
        parentUI->SetGameStatus("No ships to remove!");
        return;
    }

    Ship* lastShip = ShipsOnBoard.back(); // lastShip is a pointer
    ShipSizes.push_back(lastShip->ShipSize);

    // Clear GUI
    lastShip->RemoveShipInUI();
    for (auto& piece : lastShip->ourPieces)
        PlayerBoard->setCell(piece.getY(), piece.getX(), 'E');

    delete lastShip;        // remove pointer from vector
    ShipsOnBoard.pop_back();
    ResetCoordinates();

    parentUI->SetGameStatus("Last ship removed.");
}



// --------------------- Display ---------------------
void BattleShipBoard::DisplayShips() {
    for (int y = 0; y < 10; y++)
        for (int x = 0; x < 10; x++) {
            char cell = PlayerBoard->getCell(y, x);
            if (cell == 'S') parentUI->HighlightCell(y + 1, x + 1, 'S');
            else if (cell == 'H') parentUI->HighlightCell(y + 1, x + 1, 'H');
            else if (cell == 'M') parentUI->HighlightCell(y + 1, x + 1, 'M');
        }
}


void BattleShipBoard::DisplayMissesAndHits() {
    for(int y=0; y<10; y++)
        for(int x=0; x<10; x++)
            switch(PlayerBoard->getCell(y,x)) {
            case 'H': parentUI->HighlightCell(y+1, x+1, 'H'); break; // hit
            case 'M': parentUI->HighlightCell(y+1, x+1, 'M'); break; // miss
            }
}


void BattleShipBoard::HideBoard() {
    for (int y = 0; y < 10; y++)
        for (int x = 0; x < 10; x++)
            parentUI->HighlightCell(y + 1, x + 1, 'E');
}


void BattleShipBoard::clearBoard(char replacementKey){
    for(int y=0;y<rows;y++)
        for(int x=0;x<cols;x++)
            setCell(y,x,replacementKey);
}

// --------------------- AI Class ---------------------
class AI{
public:
    BattleShipBoard* AIBoard;
    Coordinate LastHitCoords;

    AI(BattleShipBoard* newAIBoard): AIBoard(newAIBoard), LastHitCoords(-1,-1){}

    void MakeRandomAttack(){
        if(!AIBoard || !AIBoard->parentUI) return;
        int X = (rand()%10)+1;
        int Y = (rand()%10)+1;
        SendAttack(X,Y,AIBoard->parentUI);
    }

    void PlaceAllShipsRandomly(){
        if(!AIBoard) return;

        while(!AIBoard->ShipSizes.empty()){
            int shipSize = AIBoard->ShipSizes.back();
            bool placed=false;
            while(!placed){
                bool vertical = rand()%2==0;
                int startX = rand()%10;
                int startY = rand()%10;
                int endX = startX + (vertical?0:shipSize-1);
                int endY = startY + (vertical?shipSize-1:0);

                if(endX>=10 || endY>=10) continue;

                AIBoard->OriginalCoords.setX(startX);
                AIBoard->OriginalCoords.setY(startY);
                AIBoard->FinalCoords.setX(endX);
                AIBoard->FinalCoords.setY(endY);

                if(AIBoard->CheckSlotsIfAvailable(false) && AIBoard->CanPlaceShip(AIBoard->OriginalCoords,AIBoard->FinalCoords)){
                    AIBoard->CheckSlotsIfAvailable(true);
                    AIBoard->CreateShip();
                    AIBoard->ShipSizes.pop_back();
                    AIBoard->ResetCoordinates();
                    placed=true;
                }
            }
        }
    }
};

// --------------------- Send Attack ---------------------
void SendAttack(int Col,int Row,battleship* parentUI){
    if(!PlayerOneBoard || !ourAI || !ourAI->AIBoard) return;
    if(Col<1 || Col>10 || Row<1 || Row>10) return;

    if(PlayerOnesTurn){
        if(ourAI->AIBoard->RecieveAttack(Col,Row)){
            parentUI->HighlightCell(Row,Col,'R');
            ourAI->AIBoard->GetPlayerBoard()->setCell(Row-1,Col-1,'H');
        } else {
            parentUI->HighlightCell(Row,Col,'X');
            ourAI->AIBoard->GetPlayerBoard()->setCell(Row-1,Col-1,'M');
        }
        PlayerOnesTurn=false;
        swapBoards();
        ourAI->MakeRandomAttack();
    } else {
        if(PlayerOneBoard->RecieveAttack(Col,Row)){
            parentUI->HighlightCell(Row,Col,'R');
            PlayerOneBoard->GetPlayerBoard()->setCell(Row-1,Col-1,'H');
        } else {
            parentUI->HighlightCell(Row,Col,'X');
            PlayerOneBoard->GetPlayerBoard()->setCell(Row-1,Col-1,'M');
        }
        PlayerOnesTurn=true;
    }
}

// --------------------- Swap Boards ---------------------
void swapBoards(){
    if(!PlayerOneBoard || !AIBoard) return;

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

// --------------------- battleship constructor ---------------------
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
    SetViewStatus("Current needed size: " + std::to_string(PlayerOneBoard->ShipSizes.back()) + ". Ships left: 5");
    SetGameStatus("Click to place the start position of your ship!");
}

// --------------------- Destructor ---------------------
battleship::~battleship() {
    delete ui;
}

// --------------------- Highlight Cell ---------------------
void battleship::HighlightCell(int row, int col, char ColorKey) {
    QString name = QString("Coll%1R%2").arg(col).arg(row);
    QPushButton* btn = this->findChild<QPushButton*>(name);
    if (!btn) return;

    switch (ColorKey) {
    case 'P': btn->setStyleSheet("background-color: purple; color: white;"); break; // preview
    case 'S': btn->setStyleSheet("background-color: green; color: white;"); break;  // confirmed ship
    case 'E': btn->setStyleSheet("background-color: white; color: white;"); break;  // empty
    case 'M': btn->setStyleSheet("background-color: gray; color: white;"); break;   // miss
    case 'H': btn->setStyleSheet("background-color: red; color: white;"); break;    // hit
    }
}




// --------------------- onButtonClicked ---------------------
void battleship::onButtonClicked(){
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

// --------------------- UI Setters ---------------------
void battleship::SetGameStatus(std::string text){ if(GameStatusText) GameStatusText->setPlainText(QString::fromStdString(text)); }
void battleship::SetModeStatus(std::string text){ if(ModeStatusText) ModeStatusText->setPlainText(QString::fromStdString(text)); }
void battleship::SetViewStatus(std::string text){ if(ViewStatusText) ViewStatusText->setPlainText(QString::fromStdString(text)); }

void battleship::on_ViewButton_clicked() {
    if(!placeMode) swapBoards();
}

void battleship::on_ResetButton_clicked() {
    GameOver = false;
    PlayerOnesTurn = false;
    placeMode = true;
    PlayerBoardVisible = true;

    // Delete old objects safely
    delete ourAI; ourAI = nullptr;
    delete AIBoard; AIBoard = nullptr;
    delete PlayerOneBoard; PlayerOneBoard = nullptr;

    // Create new boards and AI
    PlayerOneBoard = new BattleShipBoard(this);
    AIBoard = new BattleShipBoard(this);
    ourAI = new AI(AIBoard);
    ourAI->PlaceAllShipsRandomly();

    // Clear GUI highlights
    PlayerOneBoard->clearBoard('E');
    PlayerOneBoard->HideBoard();
    AIBoard->clearBoard('E');
    AIBoard->HideBoard();

    // Reset UI text
    SetModeStatus("Place Ship Mode");
    SetViewStatus("Next size: " + std::to_string(PlayerOneBoard->ShipSizes.back()) + ". Ships left: 5");
    SetGameStatus("Click to place the start position of your ship!");
}

void battleship::on_UndoButton_clicked() {
    if (!PlayerOneBoard) return;
    if(placeMode)
    {
        PlayerOneBoard->RemoveLastShip();

        // Update view status
        if (!PlayerOneBoard->ShipSizes.empty()) {
            int nextSize = PlayerOneBoard->ShipSizes.back();
            SetViewStatus("Next ship size: " + std::to_string(nextSize) +
                          ". Ships left: " + std::to_string(PlayerOneBoard->ShipSizes.size()));
            SetGameStatus("Place the ship of size " + std::to_string(nextSize) + ".");
        } else {
            SetViewStatus("All ships removed.");
            SetGameStatus("No ships placed. Click to start placing a ship.");
        }
    }
}



