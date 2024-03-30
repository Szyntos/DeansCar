//
// Created by Szymon on 29.03.2024.
//

#ifndef DEANSCAR_MOVE_H
#define DEANSCAR_MOVE_H


#include "Direction.h"

class Move {
public:
    int x;
    int y;
    int n;
    Direction dir;
    int carID;
    char letter;
};


#endif //DEANSCAR_MOVE_H
