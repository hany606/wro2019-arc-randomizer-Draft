//
// Created by xappm on 16.12.18.
//

#include "../util.hpp"
#include <cassert>


int main() {
    Quad q ({Point {-2, -1}, Point {-5, 2}, Point {3, 1}, Point {5, -3}});
    assert(q.contains({0, 0}));
    assert(q.contains({5, -3}));
    assert(q.contains({3, 1}));
    assert(q.contains({2, 1}));
    assert(q.contains({-5, 2}));
    assert(q.contains({-2, -1}));
    assert(!q.contains({-2, -2}));

    assert(q.overlaps(Rect {1, 2, 10, 10}));

    Point p {0, 0};
    assert(p.distance({100, 0}) == 100);

    bool yes = Circle {p, 400}.overlaps(q);
    bool no = ! Circle {Point {100, 100}, 40}.overlaps(q);
    assert(yes);
    assert(no);
}
