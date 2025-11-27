#include "ShipSystem.h"
#include "battleship.h"
// ------------------------ ShipPiece Constructor -------------------------

ShipPiece::ShipPiece(int xPos, int yPos, Ship* ship)
    : parentShip(ship)
{
    setX(xPos);
    setY(yPos);
}

// ------------------------ ShipPiece Methods ----------------------------

void ShipPiece::SetHit() {
    if (!HIT) {              // Only increment once per piece
        HIT = true;
        if (parentShip) {
            parentShip->IncreaseHitCount();
        }
    }
}


// ----------------------------- Ship Constructor -------------------------

Ship::Ship(int newInitialX, int newInitialY,
           int newFinalX,    int newFinalY,
           battleship* ui)
{
    OriginalCoords.setX(newInitialX);
    OriginalCoords.setY(newInitialY);
    FinalCoords.setX(newFinalX);
    FinalCoords.setY(newFinalY);

    parentUI = ui;

    char direction = GetDirection();
    ShipSize = 0;

    switch (direction)
    {
    case 'U':
        for (int y = OriginalCoords.getY(); y >= FinalCoords.getY(); y--) {
            ShipSize++;
            ourPieces.emplace_back(OriginalCoords.getX(), y, this);
        }
        break;

    case 'D':
        for (int y = OriginalCoords.getY(); y <= FinalCoords.getY(); y++) {
            ShipSize++;
            ourPieces.emplace_back(OriginalCoords.getX(), y, this);
        }
        break;

    case 'L':
        for (int x = OriginalCoords.getX(); x >= FinalCoords.getX(); x--) {
            ShipSize++;
            ourPieces.emplace_back(x, OriginalCoords.getY(), this);
        }
        break;

    case 'R':
        for (int x = OriginalCoords.getX(); x <= FinalCoords.getX(); x++) {
            ShipSize++;
            ourPieces.emplace_back(x, OriginalCoords.getY(), this);
        }
        break;
    }
}

// ----------------------------- Ship Methods ----------------------------

char Ship::GetDirection() {
    if (OriginalCoords.getX() == FinalCoords.getX()) {
        return (OriginalCoords.getY() < FinalCoords.getY()) ? 'D' : 'U';
    }
    else if (OriginalCoords.getY() == FinalCoords.getY()) {
        return (OriginalCoords.getX() < FinalCoords.getX()) ? 'R' : 'L';
    }
    return 'D';
}

void Ship::IncreaseHitCount() {
    HitCount++;
}

bool Ship::CheckIfSunk() {
    return HitCount == ShipSize;
}

bool Ship::CheckIfHit(int X, int Y) {
    bool hit = false;

    if (OriginalCoords.getX() == X || OriginalCoords.getY() == Y)
    {
        for (int i = 0; i < ShipSize; i++)
        {
            ShipPiece& currentPiece = ourPieces[i];

            if (currentPiece.getX() == X && currentPiece.getY() == Y)
            {
                hit = true;
                currentPiece.SetHit();
            }
        }
    }

    return hit;
}
void Ship::RemoveShipInUI() {
    char direction = GetDirection();

    switch (direction) {
    case 'U':
        for (int y = OriginalCoords.getY(); y >= FinalCoords.getY(); y--)
            parentUI->HighlightCell(y + 1, OriginalCoords.getX() + 1, 'E');
        break;
    case 'D':
        for (int y = OriginalCoords.getY(); y <= FinalCoords.getY(); y++)
            parentUI->HighlightCell(y + 1, OriginalCoords.getX() + 1, 'E');
        break;
    case 'L':
        for (int x = OriginalCoords.getX(); x >= FinalCoords.getX(); x--)
            parentUI->HighlightCell(OriginalCoords.getY() + 1, x + 1, 'E');
        break;
    case 'R':
        for (int x = OriginalCoords.getX(); x <= FinalCoords.getX(); x++)
            parentUI->HighlightCell(OriginalCoords.getY() + 1, x + 1, 'E');
        break;
    }
}


