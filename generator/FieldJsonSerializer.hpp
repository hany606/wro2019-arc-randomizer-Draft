//
// Created by xappm on 03.01.19.
//

#ifndef GREENHOUSEGENERATOR_FIELDJSONSERIALIZER_HPP
#define GREENHOUSEGENERATOR_FIELDJSONSERIALIZER_HPP

#include "util.hpp"
#include "Field.hpp"
#include <sstream>



class FieldJsonSerializer {
public:
    static std::string serialize(const Field& f, const Field& fake1, const Field& fake2, const Field& fake3,  const std::list<Point>& path) {
        std::stringstream ss;
        ss << "{\n";
        ss << collectionToString<Point, Point>("parkingZone",
                                      f.parkingZone.location.points.begin(),
                                      f.parkingZone.location.points.end(),
                                      [](Point p) { return p; });
        ss << ",\n";
        ss << "\"parkingZoneDirection\": " << f.parkingZone.direction << ",\n";

        ss << collectionToString<Box, Rect>("boxes",
                                            f.boxes.begin(),
                                            f.boxes.end(),
                                            [](Box b) { return b.location; });
        ss << ",\n";

        ss << collectionToString<Box, Rect>("fakeBoxes1",
                                            fake1.boxes.begin(),
                                            fake1.boxes.end(),
                                            [](Box b) { return b.location; });
        ss << ",\n";

        ss << collectionToString<Box, Rect>("fakeBoxes2",
                                            fake2.boxes.begin(),
                                            fake2.boxes.end(),
                                            [](Box b) { return b.location; });
        ss << ",\n";

        ss << collectionToString<Box, Rect>("fakeBoxes3",
                                            fake3.boxes.begin(),
                                            fake3.boxes.end(),
                                            [](Box b) { return b.location; });
        ss << ",\n";


        ss << collectionToString<Box, std::string>("boxColors",
                                          f.boxes.begin(),
                                          f.boxes.end(),
                                          [](Box b) { return "\""+colorString(b.ownColor)+"\""; });
        ss << ",\n";
        // cubes are four small boxes on top of each big box
        ss << collectionToString<Box, std::string>("cubeColors",
                                          f.boxes.begin(),
                                          f.boxes.end(),
                                          [](Box b) { return "\""+colorString(b.targetColor)+"\""; });
        ss << ",\n";

        ss << collectionToString<Point, Point> ("path", path.begin(), path.end(), [](Point p) { return p; });
        ss << "}";

        return ss.str();
    }



private:

    template<typename In, typename Out, typename It = const std::iterator<std::forward_iterator_tag, In>>
    static std::string collectionToString(const std::string& name, It begin, It end, std::function<Out(In)> f) {
        std::stringstream ss;

        auto it = begin;
        ss << "\"" << name << "\": [\n";
        while (it != end) {
            ss << "\t" << f(*it);
            it++;
            if(it != end) {
                ss << ",\n";
            }
        }
        ss << "]";
        return ss.str();
    }


};


#endif //GREENHOUSEGENERATOR_FIELDJSONSERIALIZER_HPP
