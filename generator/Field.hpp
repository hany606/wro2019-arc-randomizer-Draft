//
// Created by xappm on 16.12.18.
//

#ifndef GREENHOUSEGENERATOR_FIELD_HPP
#define GREENHOUSEGENERATOR_FIELD_HPP

#include <iostream>
#include <list>
#include <cstring>
#include <random>
#include "util.hpp"
#include "vector.hpp"


struct Box {
    Box(Point leftTop):
        location {Rect {leftTop.y, leftTop.x, leftTop.y + SIZE, leftTop.x + SIZE}} {
    }

    static const uint16_t SIZE = 230;

    Rect location;
    Color ownColor = Color::Black;
    Color targetColor = Color::Black;
};


/*
 =====
 ^   ^
 p0  p1
     |
     |
 =====
 ^   ^
 p2  p3
 */

struct ParkingZone {

    static const int SIZE = 345;

    ParkingZone(Point p0, Point p1_shift):
        location {},
        direction {p1_shift} {

        Vector2f32 v2 {float(p1_shift.x), float(p1_shift.y)};
        v2.normalize() *= ParkingZone::SIZE;
        v2 += Vector2f32(p0.x, p0.y);

        Point p1 {
            (int) v2.x(),
            (int) v2.y()
        };
        int xv1 = -p0.x + p1.x;
        int yv1 = -p0.y + p1.y;

        Point p2 {
                -yv1 + p1.x,
                xv1 + p1.y
        };
        int xv2 = - p1.x + p2.x;
        int yv2 = - p1.y + p2.y;

        Point p3 {
                -yv2 + p2.x,
                xv2 + p2.y
        };

        location = Quad ({p0, p1, p2, p3});
    }



    Quad getFieldOfSight() const {
        Quad fieldOfSight = location;

        Point sideWallVector {location.points[0].x - location.points[1].x,
                              location.points[0].y - location.points[1].y};
        sideWallVector.x *= 30;
        sideWallVector.y *= 30;

        fieldOfSight.points[0] = fieldOfSight.points[1] + sideWallVector;
        fieldOfSight.points[1] = location.points[0];
        fieldOfSight.points[2] = location.points[3];
        fieldOfSight.points[3] = fieldOfSight.points[3] + sideWallVector;

        return fieldOfSight;

    }

    Quad location;
    Point direction;
};



struct Field {

    Field(ParkingZone zone):
        parkingZone {std::move(zone)} {
    }

    static const uint16_t SIZE = 2300;
    static const uint16_t CELL_SIZE = 115;

    static Rect FieldRect() {
        return Rect {0, 0, SIZE, SIZE};
    }

    std::list<Box> boxes;
    ParkingZone parkingZone;
};



#endif //GREENHOUSEGENERATOR_FIELD_HPP
