//
// Created by xappm on 16.12.18.
//

#ifndef GREENHOUSEGENERATOR_UTIL_HPP
#define GREENHOUSEGENERATOR_UTIL_HPP

#include <iostream>
#include <array>
#include <functional>
#include <cmath>
#include "vector.hpp"


using std::max;

static int sign(int i) {
    if(i > 0) {
        return 1;
    } else if (i == 0) {
        return 0;
    } else {
        return -1;
    }
}

static int min4(int n1, int n2, int n3, int n4) {
    if (n1 <= n2 && n1 <= n3 && n1 <= n4) {
        return n1;
    } else if (n2 <= n3 && n2 <= n4) {
        return n2;
    } else if (n3 <= n4) {
        return n3;
    } else {
        return n4;
    }
}




enum class Color {
    Blue = 0x10069F,
    Red = 0xD22630,
    Green = 0x44D62C,
    Orange = 0xFD5E00,
    Yellow = 0xFDE900,
    Black = 0
};

static std::string colorString(Color c) {
    switch(c) {
        case Color::Yellow:
            return "Yellow";

        case Color::Red:
            return "Red";

        case Color::Orange:
            return "Orange";

        case Color::Green:
            return "Green";

        case Color::Blue:
            return "Blue";

        case Color::Black:
            return "Black";

        default:
            throw std::runtime_error("Invalid color");
    }
}


static const int FREE_ZONE_SIZE = 460;
struct Point {

    Point operator+(const Point& other) const {
        return {x + other.x, y + other.y};
    }

    Point operator-(const Point& other) const {
        return {x - other.x, y - other.y};
    }

    int operator*(const Point& other) const {
        return x * other.y  - y * other.x;
    }

    Point operator/(int other) const {
        return {x / other, y / other};
    }

    double distance(Point p2) const {
        return sqrt(pow(x - p2.x, 2) + pow(y - p2.y, 2));
    }

    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point& p) const {
        return x != p.x || y != p.y;
    }

    int x, y;
};

namespace std
{
    template<> struct hash<Point>
    {
        size_t operator()(const Point& s) const noexcept
        {
            size_t const h1 ( std::hash<int>{}(s.x) );
            size_t const h2 ( std::hash<int>{}(s.y) );
            return h1 ^ (h2 << 1);
        }
    };
}



struct Shape {
    virtual bool contains(Point p) const = 0;
    virtual bool overlaps(const Shape& other) const = 0;
};


struct Rect: public Shape {

    Rect(int top, int left, int bottom, int right):
            top {top}, left {left}, bottom {bottom}, right {right} {
        if(top >= bottom) throw std::invalid_argument("Rect top must be less than bottom");
        if(left >= right) throw std::invalid_argument("Rect left must be less than right");
    }

    bool contains(Point p) const override {
        return (p.x < right) && (p.x > left) && (p.y > top) && (p.y < bottom);
    }

    bool overlaps(const Shape& other_shape) const override {
        const Rect& other = dynamic_cast<const Rect&>(other_shape);
        return  contains(other.rightBottom()) || contains(other.leftTop()) ||
                contains(other.rightTop()) || contains(other.leftBottom()) ||
                (other.left == left && other.right == right) ||
                (other.bottom == bottom && other.top == top);
    }

    Point leftBottom() const {
        return Point {left, bottom};
    }

    Point rightTop() const {
        return Point {right, top};
    }

    Point center() const {
        return Point {left + (right - left)/2, top + (bottom - top) / 2};
    }

    Point leftTop() const {
        return Point {left, top};
    }

    Point rightBottom() const {
        return Point {right, bottom};
    }

    Rect shrinked(int amount) {
        return Rect (top + amount, left + amount, bottom - amount, right - amount);
    }

    int left;
    int top;
    int right;
    int bottom;
};



static std::ostream& operator<<(std::ostream& os, Point p) {
    os << "{ \"x\": " << p.x << ", \"y\": " << p.y << " }";
    return os;
}

static std::ostream& operator<<(std::ostream& os, Rect rect) {
    os << "{\"top\": " << rect.top << ", \"left\": " << rect.left << ", \"bott\": " << rect.bottom << ", \"right\": " << rect.right << " }";
    return os;
}


struct Quad: public Shape {
    std::array<Point, 4> points;

    Quad():
        points {Point {0, 0}, {0, 0}, {0, 0}, {0, 0}} {
    }

    Quad(const std::array<Point, 4>& points):
        points{points} {
    }

    bool contains(Point p) const override {
        Point edgeVertices[4];
        for (int i = 0; i < 3; ++i) {
            edgeVertices[i] = points[i] - points[i + 1];
        }
        edgeVertices[3] = points[3] - points[0];

        Point diffVertices[4];
        for (int i = 0; i < 4; ++i) {
            diffVertices[i] = p - points[i];
        }

        int sign = edgeVertices[0] * diffVertices[0];
        for (int j = 1; j < 3; ++j) {
            if((edgeVertices[j] * diffVertices[j] < 0 && sign > 0) ||
               (edgeVertices[j] * diffVertices[j] > 0 && sign < 0)) {
                return false;
            }
        }

        return true;

    }

    // would work if rect fits into quad, otherwise may give the wrong answer
    bool overlaps(const Shape& other_shape) const override {
        auto rect = dynamic_cast<const Rect&>(other_shape);
        for(auto& p: points) {
            if(rect.contains(p)) {
                return true;
            }
        }

        return contains(rect.leftTop()) || contains(rect.rightTop()) ||
               contains(rect.leftBottom()) || contains(rect.rightBottom());
    }

};



struct Circle: public Shape {
    Circle(Point center, int radius):
        center {center},
        radius {radius} {
    }

    bool contains(Point p) const override {
        return p.distance(center) <= radius;
    }

    bool overlaps(const Shape& other) const override {
        try {
            auto circle = dynamic_cast<const Circle&>(other);
            return center.distance(circle.center) < (radius + circle.radius);

        } catch(std::bad_cast&) {
            ;
        }
        try {
            auto rect = dynamic_cast<const Rect&>(other);
            return rect.contains(center) ||
                intersectsSegment(rect.rightBottom(), rect.rightTop()) ||
                intersectsSegment(rect.leftBottom(), rect.leftTop()) ||
                intersectsSegment(rect.leftTop(), rect.rightTop()) ||
                intersectsSegment(rect.leftBottom(), rect.rightBottom());

        } catch(std::bad_cast&) {
            ;
        }
        try {
            auto quad = dynamic_cast<const Quad&>(other);
            return quad.contains(center) ||
                    intersectsSegment(quad.points[0], quad.points[1]) ||
                    intersectsSegment(quad.points[1], quad.points[2]) ||
                    intersectsSegment(quad.points[2], quad.points[3]) ||
                    intersectsSegment(quad.points[3], quad.points[0]);


        } catch(std::bad_cast&) {
            throw std::invalid_argument("This type of shape is unsupported by this method");
        }
    }


    bool intersectsSegment(Point p1, Point p2) const {
        Vector2f32 start (p1.x, p1.y);
        Vector2f32 end (p2.x, p2.y);

        Vector2f32 tmp(end.x() - start.x(), end.y() - start.y()); 		// holds polygon line vector
        Vector2f32 tmp1(center.x - start.x(), center.y - start.y()); 	// holds start to circle center vector
        float l = tmp.getLength();					// the euclidean length of line :(w * w + h * h) squared
        float u = tmp1 * tmp.normalized();				// the dot product for these 2 vectors
        Vector2f32 tmp2;
        if (u <= 0) {                                           // circle center is closest to start
            tmp2.set(start.x(), start.y());                  // set point to start point
        } else if (u >= l) {                                    // circle is closest to end
            tmp2.set(end.x(), end.y());                      // set point to end point
        } else {                                                // circle is in between
            Vector2f32 tmp3(tmp * u);                           // scale the normal by the dot product to get position on border
            tmp2.set(tmp3.x() + start.x(), tmp3.y() + start.y());// set point to position on line closest to circle center
        }
        // check if circle radius is longer than the line from our position on line to circle center (true for collisions)
        float x = center.x - tmp2.x();
        float y = center.y - tmp2.y();
        return x * x + y * y <= radius * radius;
    }

    Point center;
    int radius;
};

struct ComplexShape: public Shape {
    ComplexShape(const std::function<bool(Point)>& contains, const std::function<bool(const Shape&)>& overlaps) {
        contains_callback = contains;
        overlaps_callback = overlaps;
    }

    bool contains(Point p) const override {
        return contains_callback(p);
    }


    bool overlaps(const Shape& other) const override {
        return overlaps_callback(other);
    }


private:
    std::function<bool(Point)> contains_callback;
    std::function<bool(const Shape&)> overlaps_callback;
};



#endif //GREENHOUSEGENERATOR_UTIL_HPP
