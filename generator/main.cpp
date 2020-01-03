#include <iostream>
#include <fstream>
#include <chrono>
#include <iterator>
#include "Field.hpp"
#include "FieldGenerator.hpp"
#include "FieldJsonSerializer.hpp"


int main(int argc, char** argv) {

    FieldGenerator generator;

    int fails = 0;
    while (fails < 1000) {
        try {
            ParkingZone zone = generator.generateParkingZone();
            Field baseField  = generator.generate(zone);
            Field fakeField1 = generator.generate(zone);
            Field fakeField2 = generator.generate(zone);
            Field fakeField3 = generator.generate(zone);

            auto json = FieldJsonSerializer::serialize(baseField, fakeField1, fakeField2, fakeField3, generator.getPath());
            std::cout << json;

            if(argc > 1 && strcmp(argv[1], "log") == 0) {
                using namespace std::chrono;
                std::ofstream out("generator-log.txt", std::ios_base::app);
                auto t = system_clock::to_time_t(system_clock::now());
                out << std::ctime(&t) << "\n" << json << "\n\n===================================\n";
            }

            return 0;

        } catch (std::runtime_error& e) {
            std::cerr << e.what() << '\n';
            fails++;
        }
    }

    std::cerr << "Generation failed :(\nTry again!\n";

    return 1;
}



