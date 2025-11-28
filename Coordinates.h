#ifndef COORDINATE_H
#define COORDINATE_H

template<typename T>
class Coordinate {
public:
    Coordinate(T x = 0, T y = 0) : x(x), y(y) {}

    // Getters
    T getX() const { return x; }
    T getY() const { return y; }

    // Setters
    void setX(T newX) { x = newX; }
    void setY(T newY) { y = newY; }

    // Increment / Decrement
    void incrementX() { ++x; }
    void incrementY() { ++y; }
    void decrementX() { --x; }
    void decrementY() { --y; }

private:
    T x;
    T y;
};

#endif // COORDINATE_H
