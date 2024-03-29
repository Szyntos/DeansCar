//
// Created by szynt on 24.03.2024.
//

#ifndef DEANSCAR_CAR_H
#define DEANSCAR_CAR_H
#include <array>
#include "Cell.h"

class Car {
public:
    int ID;
    std::array<int, 4> northEastSouthWest = {0, 0, 0, 0};
    int x;
    int y;
    bool isVertical;
    int length;
    CellType type = CAR;
    Car(){
        ID = 0;
        x = 0;
        y = 0;
        isVertical = false;
        length = 0;
    }
    explicit Car(int id, int xPos, int yPos, bool vert, int len): ID(id), x(xPos), y(yPos), isVertical(vert), length(len){

    }
    [[nodiscard]] std::string getInfo() const {
        std::string info = "Car Info:\n";
        info += "ID: " + std::to_string(ID) + "\n";
        info += "NorthEastSouthWest: {" +
                std::to_string(northEastSouthWest[0]) + ", " +
                std::to_string(northEastSouthWest[1]) + ", " +
                std::to_string(northEastSouthWest[2]) + ", " +
                std::to_string(northEastSouthWest[3]) + "}\n";
        info += "Coordinates: (" + std::to_string(x) + ", " + std::to_string(y) + ")\n";
        info += "Is Vertical: " + std::to_string(isVertical) + "\n";
        info += "Length: " + std::to_string(length) + "\n";
        info += "CarType: " + std::to_string(type) + "\n";
        return info;
    }

    bool canMove(){
        for (int i = 0; i < 4; ++i) {
            if (northEastSouthWest[i] > 0){
                return true;
            }
        }
        return false;
    }

    void setNorth(int north){
        northEastSouthWest[0] = north;
    }
    void setEast(int east){
        northEastSouthWest[1] = east;
    }
    void setSouth(int south){
        northEastSouthWest[2] = south;
    }
    void setWest(int west){
        northEastSouthWest[3] = west;
    }
    void move(Direction dir, int n){
        switch (dir) {
            case PLUS:
                if (isVertical){
                    y += n;
                }else{
                    x += n;
                }
                break;
            case MINUS:
                if (isVertical){
                    y -= n;
                }else{
                    x -= n;
                }
                break;
            default:
                break;
        }

    }

};


#endif //DEANSCAR_CAR_H
