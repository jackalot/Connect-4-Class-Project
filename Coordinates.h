// Coordinate.h
#ifndef COORDINATE_H
#define COORDINATE_H

class Coordinate {
public:
    Coordinate(int x = 0, int y = 0) : x(x), y(y) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }

private:
    int x;
    int y;
};

#endif // COORDINATE_H
