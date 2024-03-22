//
// Created by szynt on 21.03.2024.
//

#ifndef DEANSCAR_CELL_H
#define DEANSCAR_CELL_H

#include "Direction.h"
#include <array>
#include <string>
#include "CellType.h"



class Cell {
public:
    CellType cellType;
    bool isVertical;
    Direction isNeighbouring;
    std::array<int, 4> northEastSouthWest;
    int x;
    int y;
    char c = '.';
    int nthPiece = 0;
    int carLength = 0;

    Cell() : cellType(EMPTY), isVertical(false), isNeighbouring(ZERO),
             northEastSouthWest({0, 0, 0, 0}), x(0), y(0) {
    }
    // Constructor definition
    Cell(int xCoord, int yCoord)
            : cellType(EMPTY), isVertical(false), isNeighbouring(ZERO),
              northEastSouthWest({0, 0, 0, 0}), x(xCoord), y(yCoord) {
    }
    [[nodiscard]] std::string getInfo() const {
        std::string info = "Cell Info:\n";
        info += "Type: " + cellTypeToString(cellType) + "\n";
        info += "Is Vertical: " + std::to_string(isVertical) + "\n";
        info += "Neighbouring Direction: " + directionToString(isNeighbouring) + "\n";
        info += "NorthEastSouthWest: {" +
                std::to_string(northEastSouthWest[0]) + ", " +
                std::to_string(northEastSouthWest[1]) + ", " +
                std::to_string(northEastSouthWest[2]) + ", " +
                std::to_string(northEastSouthWest[3]) + "}\n";
        info += "Coordinates: (" + std::to_string(x) + ", " + std::to_string(y) + ")\n";
        info += "Character: '" + std::string(1, c) + "'\n";
        info += "nthPiece: '" + std::to_string(nthPiece) + "'\n";
        info += "carLength: '" + std::to_string(carLength) + "'\n";
        return info;
    }

private:
    // Helper functions to convert enums to strings
    static std::string cellTypeToString(CellType type) {
        switch (type) {
            case CAR: return "CAR";
            case DEANSCAR: return "DEAN'S CAR";
            case EMPTY: return "EMPTY";
            case BARRIER: return "BARRIER";
            case EXIT: return "EXIT";
            default: return "UNKNOWN";
        }
    }

    static std::string directionToString(Direction dir) {
        switch (dir) {
            case PLUS: return "PLUS";
            case MINUS: return "MINUS";
            case BOTH: return "BOTH";
            case ZERO: return "ZERO";
            default: return "UNKNOWN";
        }
    }
};


#endif //DEANSCAR_CELL_H
