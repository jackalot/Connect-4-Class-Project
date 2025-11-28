#ifndef SHIPSYSTEM_HPP
#define SHIPSYSTEM_HPP

#include <vector>
#include "Coordinates.h"   // use the templated Coordinate

using std::vector;

// Forward declarations
class Ship;
class battleship;

// ------------------------ ShipPiece -------------------------
class ShipPiece : public Coordinate<int> {
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
    int ShipSize = 0;
    int HitCount = 0;

    Coordinate<int> OriginalCoords;
    Coordinate<int> FinalCoords;
    battleship* parentUI;

    Ship(int newInitialX, int newInitialY,
         int newFinalX, int newFinalY,
         battleship* ui);

    char GetDirection() const;
    void IncreaseHitCount();
    bool CheckIfSunk() const;
    bool CheckIfHit(int X, int Y);
    void RemoveShipInUI();
};

#endif // SHIPSYSTEM_HPP
