#ifndef SHIPSYSTEM_HPP
#define SHIPSYSTEM_HPP

#include <vector>
#include "Coordinates.h"

using std::vector;

// Forward declarations
class Ship;
class battleship;

// ------------------------ ShipPiece -------------------------
class ShipPiece : public Coordinate {
protected:
    bool HIT = false;

public:
    Ship* parentShip;

    ShipPiece(int xPos, int yPos, Ship* ship);

    void SetHit();
};

// ----------------------------- Ship -------------------------
class Ship {
public:
    vector<ShipPiece> ourPieces;
    int ShipSize;
    int HitCount = 0;

    Coordinate OriginalCoords;
    Coordinate FinalCoords;
    battleship* parentUI;

    Ship(int newInitialX, int newInitialY,
         int newFinalX, int newFinalY,
         battleship* ui);

    char GetDirection();
    void IncreaseHitCount();
    bool CheckIfSunk();
    bool CheckIfHit(int X, int Y);
    void RemoveShipInUI();
};

#endif // SHIPSYSTEM_HPP
