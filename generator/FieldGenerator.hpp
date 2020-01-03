//
// Created by xappm on 17.12.18.
//

#ifndef GREENHOUSEGENERATOR_FIELDGENERATOR_HPP
#define GREENHOUSEGENERATOR_FIELDGENERATOR_HPP

#include "Field.hpp"
#include "vector.hpp"


class FieldGenerator {
public:

    Field generate(ParkingZone baseZone);
    ParkingZone generateParkingZone();

    std::list<Point> getPath() const;

    // zone that should be free of field objects
    static Rect getFreeZone(const Box& b);

    bool isAroundParkingZone(const Box& box) const;

private:

    int minDistToWall(const Box& b) const;

    bool tryPutBox(Box box);

    void updateFreePoints();
    bool isPassable() const;
    bool collidesWithAnyObject(Circle c) const;
    bool isBoxPositionValid(const Box& b) const;

    static ParkingZone randParkingZone();
    static Point randPoint(int margin);
    static int randSign();

    Field* f;
    static std::default_random_engine rand;

    std::vector<Point> freePoints;
    mutable std::list<Point> validPath; // from last box to parking zone;

    std::optional<std::list<Point>> tryReach(const Point &start, const std::function<bool(const Point&)>& hasArrived) const;
};


#endif //GREENHOUSEGENERATOR_FIELDGENERATOR_HPP
