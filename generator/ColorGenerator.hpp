//
// Created by xappm on 14.01.19.
//

#ifndef GREENHOUSEGENERATOR_COLORGENERATOR_HPP
#define GREENHOUSEGENERATOR_COLORGENERATOR_HPP

#include "Field.hpp"


/**
 * Distributes colors between provided boxes.
 * Each box has its own color and color of cubes atop the box
 * A robot will start searching for blue box, and then will visit target box of blue one and target box of target box of the blue one O.o
 * Cube colors are picked so that three target boxes are as far from each other as possible
 */
class ColorGenerator {
public:
    ColorGenerator(std::list<Box>& boxes):
       boxes {boxes} {

    }

    void generate() {
        std::vector<Color> colors = {Color::Blue, Color::Green, Color::Orange, Color::Red, Color::Yellow};

        auto it = boxes.begin();
        for (size_t i = 0; i < 5; ++i) {
            it->ownColor = colors.at(i);
            it++;
        }

        generateTargetColors();
    }

private:

    void generateTargetColors() {
        auto rand = std::default_random_engine();
        std::vector<Color> colors = {Color::Green, Color::Orange, Color::Red, Color::Yellow};
        std::shuffle(colors.begin(), colors.end(), rand);

        auto& blueBox = getBlueBox();
        blueBox.targetColor = getFarthestBoxFrom(blueBox).ownColor;
        auto& nextBox = getNextBox();
        nextBox.targetColor = getFarthestBoxFrom(nextBox, blueBox).ownColor;

        std::replace(colors.begin(), colors.end(), blueBox.targetColor, Color::Blue);
        colors.erase(std::remove(colors.begin(), colors.end(), nextBox.targetColor));

        for(auto& box: boxes) {
            if(box.ownColor != Color::Blue && box.ownColor != nextBox.ownColor) {
                auto it = std::find_if(colors.begin(), colors.end(), [&box](auto c) { return box.ownColor != c; });
                if(it != colors.end()) {
                    box.targetColor = *it;
                    colors.erase(it);
                } else {
                    box.targetColor = colors.front();
                    std::swap(box.targetColor, nextBox.targetColor);
                }
            }
        }
    }

    Box& getBlueBox() const {
        return *std::find_if(boxes.begin(), boxes.end(), [](auto b) { return b.ownColor == Color::Blue; });
    }

    Box& getNextBox() const {
        return *std::find_if(boxes.begin(), boxes.end(),
                             [this](auto b) { return b.ownColor == getBlueBox().targetColor; });
    }

    Box& getFarthestBoxFrom(const Box& box) const {
        return *std::max_element(boxes.begin(), boxes.end(), [&box](auto& currMax, auto& currBox) {
            auto loc = box.location.center();
            return currBox.location.center().distance(loc) > currMax.location.center().distance(loc);
        });
    }

    Box& getFarthestBoxFrom(const Box& box1, const Box& box2) const {
        float maxDist {};
        Box* currMax {};

        auto loc1 = box1.location.center();
        auto loc2 = box2.location.center();

        for(auto& currBox: boxes) {
            if(currBox.ownColor == box1.ownColor || currBox.ownColor == box2.ownColor)
                continue;

            float dist1toCurrBox = currBox.location.center().distance(loc1);
            float dist2toCurrBox = currBox.location.center().distance(loc2);

            if(dist1toCurrBox + dist2toCurrBox >= maxDist) {
                maxDist = dist1toCurrBox + dist2toCurrBox;
                currMax = &currBox;
            }
            return *currMax;
        }
    }

    std::list<Box>& boxes;
};


#endif //GREENHOUSEGENERATOR_COLORGENERATOR_HPP
