//
// Created by xappm on 17.12.18.
//

#ifndef GREENHOUSEGENERATOR_FIELDPRINTER_HPP
#define GREENHOUSEGENERATOR_FIELDPRINTER_HPP

#include "FieldGenerator.hpp"



class FieldPrinter {
    void drawField(const Field& field) {
        char screen[20][20];
        memset(screen, '.', 20 * 20 * sizeof(char));

        for(const Box& box: field.boxes) {
            screen[box.location.top / 115][box.location.left / 115] = 'O';
            screen[box.location.bottom / 115 - 1][box.location.left / 115] = 'O';
            screen[box.location.top / 115][box.location.right / 115 - 1] = 'O';
            screen[box.location.bottom / 115 - 1][box.location.right / 115 - 1] = 'O';
            auto free = FieldGenerator::getFreeZone(box);
            for (int i = free.left; i < free.right; i += 115) {
                for (int j = free.top; j < free.bottom; j += 115) {
                    if(screen[j / 115][i / 115] != 'O')
                        screen[j / 115][i / 115] = 'x';
                }
            }
        }

        for (int i = 0; i < Field::SIZE / 115; i++) {
            std::cout << i*115 << " ";
            for (int j = 0; j < Field::SIZE / 115; j++) {
                std::cout << screen[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

};


#endif //GREENHOUSEGENERATOR_FIELDPRINTER_HPP
