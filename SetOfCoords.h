#ifndef SETOFCOORDS_H
#define SETOFCOORDS_H
class SetOfCoords {
public:
    Coordinate<int> start;
    Coordinate<int> end;

    SetOfCoords() {
        start.setX(-1); start.setY(-1);
        end.setX(-1); end.setY(-1);
    }

    void reset() {
        start.setX(-1); start.setY(-1);
        end.setX(-1); end.setY(-1);
    }

    char getDirection() {
        if (start.getX() == end.getX()) return (start.getY() < end.getY()) ? 'D' : 'U';
        if (start.getY() == end.getY()) return (start.getX() < end.getX()) ? 'R' : 'L';
        return 'D';
    }
};

#endif // SETOFCOORDS_H
