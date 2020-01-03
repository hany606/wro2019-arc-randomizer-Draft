//
// Created by xappm on 17.12.18.
//

#include <queue>
#include <set>
#include <unordered_set>
#include <map>
#include <iterator>
#include "FieldGenerator.hpp"
#include "ColorGenerator.hpp"


std::default_random_engine FieldGenerator::rand(time(NULL));


ParkingZone FieldGenerator::generateParkingZone() {
    ParkingZone zone = randParkingZone();
    while(minDistToWall(zone.location.points[3]) < 460) {
        zone = randParkingZone();
    }
    return zone;
}

Field FieldGenerator::generate(ParkingZone baseZone) {

    freePoints.reserve(19*19);
    for(int i = 0; i <= 18; i++) {
        for (int j = 0; j <= 18; ++j) {
            freePoints.emplace_back(Point {i * 115, j * 115});
        }
    }
    std::sort(freePoints.begin(), freePoints.end(), [](auto p1, auto p2) {
        return p1.distance(Field::FieldRect().center()) > p2.distance(Field::FieldRect().center());
    });

    ParkingZone zone = baseZone;

    f = new Field(zone);

    updateFreePoints();

    int count = 0;
    std::binomial_distribution<> dist(freePoints.size() - 1, 0.7);
    while(count < 5) {
        auto p = std::begin(freePoints) + dist(rand);
        if(tryPutBox(Box(*p))) {
            count++;
        } else {
            freePoints.erase(p);
        }
        if(freePoints.empty()) {
            throw std::runtime_error("Generation failed: Couldn't arrange the boxes");
        }
        dist = std::binomial_distribution<>(freePoints.size() - 1, 0.5);
    }

    ColorGenerator c(f->boxes);
    c.generate();

    if (!isPassable()) {
        throw std::runtime_error("Generation failed: Field is not passable");
    }

    Field f_copy = *f;
    delete f;

    return f_copy;
}


std::list<Point> FieldGenerator::getPath() const {
    return validPath;
}



// zone that should be free of field objects
Rect FieldGenerator::getFreeZone(const Box& b) {
    int min = min4(b.location.left, b.location.top,
                   Field::SIZE - b.location.right, Field::SIZE - b.location.bottom);

    Rect freeZone(0, 0, 1, 1);
    if(min == b.location.left) {
        freeZone.left = b.location.right;
        freeZone.top = b.location.center().y - FREE_ZONE_SIZE / 2;

    } else if(min == b.location.top) {
        freeZone.left = b.location.center().x - FREE_ZONE_SIZE / 2;
        freeZone.top = b.location.bottom;

    } else if(min == Field::SIZE - b.location.right) {
        freeZone.left = b.location.left - FREE_ZONE_SIZE;
        freeZone.top = b.location.center().y - FREE_ZONE_SIZE / 2;

    } else if(min == Field::SIZE - b.location.bottom) {
        freeZone.left = b.location.center().x - FREE_ZONE_SIZE / 2;
        freeZone.top = b.location.top - FREE_ZONE_SIZE;
    } else {
        std::cerr << "Min failed\n";
        exit(1);
    }
    freeZone.right = freeZone.left + FREE_ZONE_SIZE;
    freeZone.bottom = freeZone.top + FREE_ZONE_SIZE;

    if(freeZone.right > Field::SIZE) freeZone.right = Field::SIZE - 1;
    if(freeZone.bottom > Field::SIZE) freeZone.bottom = Field::SIZE - 1;
    if(freeZone.left < 0) freeZone.left = 0;
    if(freeZone.top < 0) freeZone.top = 0;

    return freeZone;
}



bool FieldGenerator::isAroundParkingZone(const Box& box) const {
    auto loc = f->parkingZone.location;
    auto contains = [loc](Point point) -> bool {
        return point.distance(loc.points[0]) < 460 ||
               point.distance(loc.points[3]) < 460;
        };


    return contains(box.location.leftTop()) || contains(box.location.rightBottom()) ||
           contains(box.location.leftBottom()) || contains(box.location.rightTop());
}



bool FieldGenerator::isPassable() const {
    if (f == nullptr || f->boxes.size() != 5) {
        throw std::runtime_error("Passability Check Error: Boxes generation stage is incomplete!");
    }

    Box& blueBox = *std::find_if(f->boxes.begin(), f->boxes.end(),
            [](auto& box) { return box.ownColor == Color::Blue; });

    auto& nextBox = *std::find_if(f->boxes.begin(), f->boxes.end(),
            [&blueBox](auto b) { return b.ownColor == blueBox.targetColor; });

    auto& lastBox = *std::find_if(f->boxes.begin(), f->boxes.end(),
            [&nextBox](auto b) { return b.ownColor == nextBox.targetColor; });

    Rect start = getFreeZone(lastBox);
    Point robotCenter = start.center();


    auto res = tryReach(robotCenter,
                        [&nextBox](const Point& pos) {
                            return pos == getFreeZone(nextBox).center();
                        });
    bool nextBoxIsReachable = res.has_value();
    if(nextBoxIsReachable) {
        validPath.insert(validPath.end(), res.value().begin(), res.value().end());
        res = tryReach(getFreeZone(nextBox).center(),
                            [&blueBox](const Point& pos) {
                                return pos == getFreeZone(blueBox).center();
                            });
        bool blueBoxIsReachable = res.has_value();
        if (blueBoxIsReachable) {
            validPath.insert(validPath.end(), res.value().begin(), res.value().end());

            res = tryReach(getFreeZone(blueBox).center(),
                                   [this](const Point &pos) -> bool {
                                       auto loc = f->parkingZone.location;
                                       auto p0 = loc.points[0], p1 = loc.points[3];
                                       return Circle(pos, Field::CELL_SIZE * 2).contains((p1 - p0) / 2 + p0);
                                   });
            bool parkingZoneIsReachable = res.has_value();
            if(parkingZoneIsReachable) {
                validPath.insert(validPath.end(), res.value().begin(), res.value().end());
            } else {
                validPath.clear();
            }
            return parkingZoneIsReachable;
        }
    }

    return false;
}



std::optional<std::list<Point>> FieldGenerator::tryReach(const Point &start,
                                                         const std::function<bool(const Point &)>& hasArrived) const {
    std::queue<Point> wave;
    wave.push(start);
    std::unordered_set<Point> visited;
    std::unordered_map<Point, Point> previousOf;

    visited.insert(wave.front());
    while(!wave.empty()) {
        Point currCenter = wave.front();
        wave.pop();

        for(int dx = -Field::CELL_SIZE; dx <= Field::CELL_SIZE; dx += Field::CELL_SIZE) {
            for(int dy = -Field::CELL_SIZE; dy <= Field::CELL_SIZE; dy += Field::CELL_SIZE) {

                Point newCenter = currCenter + Point {dx, dy};
                Circle robotCircle {newCenter, Field::CELL_SIZE * 2 - 1};

                if((dy != 0 || dx != 0) && (visited.find(newCenter) == visited.end())
                   && Field::FieldRect().shrinked(Field::CELL_SIZE * 2).contains(newCenter)) {
                    if (hasArrived(newCenter)) {
                        previousOf.insert({newCenter, currCenter});

                        std::list<Point> path;
                        path.push_front(newCenter);

                        while(path.front() != start) {
                            path.push_front(previousOf.at(path.front()));

                        }

                        return path;

                    } else if (!collidesWithAnyObject(robotCircle)) {
                        previousOf.insert({newCenter, currCenter});
                        wave.push(newCenter);
                        visited.insert(newCenter);
                    }
                }
            }
        }
    }
    return {};
}



bool FieldGenerator::collidesWithAnyObject(Circle c) const {
    auto& pts = f->parkingZone.location.points;
    Circle approxLocation ((pts[2] - pts[0])/2 + pts[0], ((pts[0] - pts[2]) / 2).distance({0, 0}));
    if (c.overlaps(approxLocation)) {
        return true;
    }
    for(auto& box: f->boxes) {
        if (c.overlaps(box.location)) {
            return true;
        }
    }
    return false;
}



int FieldGenerator::minDistToWall(const Box& b) const {
    return min4(b.location.left, b.location.top,
            Field::SIZE - b.location.bottom, Field::SIZE - b.location.right);
}



// return true and updates free points if a box is put successfully
// otherwise returns false
bool FieldGenerator::tryPutBox(Box box) {
    if(!isBoxPositionValid(box)) {
        return false;
    }

    for (Box& putBox: f->boxes) {
        if(getFreeZone(box).overlaps(putBox.location) ||
           getFreeZone(putBox).overlaps(box.location) ||
           box.location.overlaps(putBox.location)) {
            return false;
        }

    }
    if(isAroundParkingZone(box) ||
       f->parkingZone.location.overlaps(getFreeZone(box)) ||
       f->parkingZone.getFieldOfSight().overlaps(box.location)) {
        return false;
    }
    f->boxes.push_back(box);

    return true;
}



bool FieldGenerator::isBoxPositionValid(const Box& b) const {
    int dists[] = {b.location.left, b.location.top,
                   Field::SIZE - b.location.bottom, Field::SIZE - b.location.right};
    for (int i = 0; i < 4; ++i) {
        for (int j = i+1; j < 4; ++j) {
            if(abs(dists[i] - dists[j]) < 230) {
                return false;
            }
        }
    }
    return true;
}



void FieldGenerator::updateFreePoints() {
    auto& zone = f->parkingZone.location;

    std::remove_if(freePoints.begin(), freePoints.end(), [&zone](auto p) {
        return p.distance(zone.points[0]) < 460 || p.distance(zone.points[3]) < 460;
    });
}



ParkingZone FieldGenerator::randParkingZone() {
    Point p1 = randPoint(460);

    // shift from position parallel to the upper wall
    Vector2f32 secondPointShifts[] = {
            {3, 0},
            {3, 1},
            {2, 1},
            {2, 2},
            {1, 2},
            {1, 3},
            {0, 3}
    };
    std::uniform_int_distribution dist(0, 5);
    Vector2f32 shift = secondPointShifts[dist(rand)];

    shift *= Field::CELL_SIZE;
    shift.setY(shift.y() * randSign());
    shift.setX(shift.x() * randSign());

    ParkingZone zone(p1, Point {(int)shift.x(), (int)shift.y()});

    return zone;
}



Point FieldGenerator::randPoint(int margin) {
    std::uniform_int_distribution dist(margin, Field::SIZE - margin);

    // make coordinates lie on the grid
    int x = dist(rand);
    x /= Field::CELL_SIZE;
    x *= Field::CELL_SIZE;
    int y = dist(rand);
    y /= Field::CELL_SIZE;
    y *= Field::CELL_SIZE;

    return {x, y};
}



int FieldGenerator::randSign() {
    if(std::uniform_int_distribution(0, 1)(rand) == 0) {
        return -1;
    } else {
        return 1;
    }
}
