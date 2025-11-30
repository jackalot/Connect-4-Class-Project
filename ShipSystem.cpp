#include "ShipSystem.h"
#include "battleship.h"

// ------------------------ ShipPiece Constructor -------------------------
ShipPiece::ShipPiece(int xPos, int yPos, Ship* ship)
    : parentShip(ship)
{
    this->setX(xPos);
    this->setY(yPos);
}

// ------------------------ ShipPiece Methods ----------------------------
void ShipPiece::SetHit() {
    if (!HIT) {
        HIT = true;
        if (parentShip) {
            parentShip->IncreaseHitCount();
        }
    }
}

// ----------------------------- Ship Constructor -------------------------
Ship::Ship(int newInitialX, int newInitialY,
           int newFinalX, int newFinalY,
           battleship* ui)
{
    start.setX(newInitialX);
    start.setY(newInitialY);
    end.setX(newFinalX);
    end.setY(newFinalY);

    parentUI = ui;

    char direction = GetDirection();
    ShipSize = 0;

    switch (direction)
    {
    case 'U':
        for (int y = start.getY(); y >= end.getY(); y--) {
            ShipSize++;
            ourPieces.emplace_back(start.getX(), y, this);
        }
        break;

    case 'D':
        for (int y = start.getY(); y <= end.getY(); y++) {
            ShipSize++;
            ourPieces.emplace_back(start.getX(), y, this);
        }
        break;

    case 'L':
        for (int x = start.getX(); x >= end.getX(); x--) {
            ShipSize++;
            ourPieces.emplace_back(x, start.getY(), this);
        }
        break;

    case 'R':
        for (int x = start.getX(); x <= end.getX(); x++) {
            ShipSize++;
            ourPieces.emplace_back(x, start.getY(), this);
        }
        break;
    }
}

// ----------------------------- Ship Methods ----------------------------
char Ship::GetDirection() const {
    if (start.getX() == end.getX())
        return (start.getY() < end.getY()) ? 'D' : 'U';
    else if (start.getY() == end.getY())
        return (start.getX() < end.getX()) ? 'R' : 'L';
    return 'D';
}

void Ship::IncreaseHitCount() {
    HitCount++;
}

bool Ship::CheckIfSunk() const {
    return HitCount == ShipSize;
}

bool Ship::CheckIfHit(int X, int Y) {
    bool hit = false;

    for (auto &piece : ourPieces) {
        if (piece.getX() == X && piece.getY() == Y) {
            hit = true;
            piece.SetHit();
            break;
        }
    }

    return hit;
}

void Ship::RemoveShipInUI() {
    char direction = GetDirection();

    switch (direction) {
    case 'U':
        for (int y = start.getY(); y >= end.getY(); y--)
            parentUI->HighlightCell(y + 1, start.getX() + 1, 'E');
        break;
    case 'D':
        for (int y = start.getY(); y <= end.getY(); y++)
            parentUI->HighlightCell(y + 1, start.getX() + 1, 'E');
        break;
    case 'L':
        for (int x = start.getX(); x >= end.getX(); x--)
            parentUI->HighlightCell(start.getY() + 1, x + 1, 'E');
        break;
    case 'R':
        for (int x = start.getX(); x <= end.getX(); x++)
            parentUI->HighlightCell(start.getY() + 1, x + 1, 'E');
        break;
    }
}
