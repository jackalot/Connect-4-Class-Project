#ifndef SETOFCOORDS_H
#define SETOFCOORDS_H

#include "Coordinates.h"

class SetOfCoords {
public:
    Coordinate<int> start;
    Coordinate<int> end;
    bool firstPointPlaced = false;
    bool secondPointPlaced = false;

    SetOfCoords() {
        reset();
    }

    void reset() {
        start.setX(-1); start.setY(-1);
        end.setX(-1); end.setY(-1);
        firstPointPlaced = false;
        secondPointPlaced = false;
    }

    char getDirection() const {
        if (start.getX() == end.getX()) return (start.getY() < end.getY()) ? 'D' : 'U';
        if (start.getY() == end.getY()) return (start.getX() < end.getX()) ? 'R' : 'L';
        return 'D';
    }
};

#endif
