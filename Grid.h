//
// Created by szynt on 21.03.2024.
//

#ifndef DEANSCAR_GRID_H
#define DEANSCAR_GRID_H


#include "Cell.h"
#include "Car.h"
#include <vector>
#include <iostream>


class Grid {
public:
    int width;
    int height;
    int carCount;
    std::vector<std::vector<Cell>> cells;
    std::vector<Car> cars;
    bool win = false;

    Grid(){
        width = 0;
        height = 0;
        carCount = 0;
    }

    Grid(int w, int h, int cars) : width(w), height(h), cells(height, std::vector<Cell>(width)),
                                    carCount(cars), cars(cars) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                cells[i][j] = Cell(j, i);
            }
        }
    }

    void updateMoves(){
        int k = 0;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (cells[i][j].cellType == EMPTY || cells[i][j].cellType == EXIT){
                    updateCell(i, j);
                }
            }
        }
        for (int i = height-1; i > 0; --i) {
            for (int j = width-1; j > 0; --j) {
                if (cells[i][j].cellType == EMPTY || cells[i][j].cellType == EXIT){
//                    if (j == 7){
//                        std::cout << " " << i << " " << j<< "\n";
//                    }
                    updateCell(i, j);

                }
            }
        }
    }

    void partialUpdateMoves(int x, int y, bool isVertical, Direction dir, bool canBounce){
//        std::cout << "update" <<  std::endl;
        partialUpdateCell(y, x, isVertical);
        if (isVertical){
            int i = 1;
            while (y + i < cells.size()  && cells[y+i][x].cellType == EMPTY){
                partialUpdateCell(y+i, x, isVertical);
                i += 1;
            }
            if (canBounce && y + i < cells.size()){
                partialUpdateMoves(x, y+i, isVertical, PLUS, false);
            }

            i = 1;
            while (y-i >= 0 && cells[y-i][x].cellType == EMPTY){
                partialUpdateCell(y-i, x, isVertical);
                i += 1;
            }
            if (canBounce && y-i >= 0){
                partialUpdateMoves( x, y-i, isVertical, PLUS, false);
            }
        } else {
            int i = 1;
            while (x + i < cells[0].size() && cells[y][x+i].cellType == EMPTY){
                partialUpdateCell(y, x+i, isVertical);
                i += 1;
            }
            if (canBounce && x + i < cells[0].size()){
                partialUpdateMoves(x+i, y, isVertical, PLUS, false);
            }
            i = 1;
            while (x - i >= 0 && cells[y][x-i].cellType == EMPTY){
                partialUpdateCell(y, x-i, isVertical);
                i += 1;
            }
            if (canBounce && x - i >= 0){
                partialUpdateMoves(x-i, y, isVertical, PLUS, false);
            }
        }
    }

    void updateCell(int i, int j){
        partialUpdateCell(i, j, true);
        partialUpdateCell(i, j, false);
    }

    void partialUpdateCell(int i, int j, bool isVertical){
        if (j == 7 || j == 6){
            int k = 6;

        }
        if (cells[i][j].cellType == BARRIER){
            return;
        }
        if (isVertical){
            if (i + 1 <= cells.size() && ((cells[i+1][j].isVertical && (cells[i+1][j].cellType == CAR || cells[i+1][j].cellType == DEANSCAR)) || cells[i+1][j].cellType == EMPTY || cells[i+1][j].cellType == EXIT)){
                cells[i+1][j].northEastSouthWest[0] = 1 + cells[i][j].northEastSouthWest[0];
                if (cells[i+1][j].carID != -1){
                    updateCarNESW(cells[i+1][j].carID);
//                    cars[cells[i+1][j].carID].northEastSouthWest[0] = 1 + cells[i][j].northEastSouthWest[0];
                }
            }
            if (i - 1 >= 0 && ((cells[i-1][j].isVertical && (cells[i-1][j].cellType == CAR || cells[i-1][j].cellType == DEANSCAR)) || cells[i-1][j].cellType == EMPTY || cells[i-1][j].cellType == EXIT)){
                cells[i-1][j].northEastSouthWest[2] = 1 + cells[i][j].northEastSouthWest[2];
                if (cells[i-1][j].carID != -1){
                    updateCarNESW(cells[i-1][j].carID);
//                    cars[cells[i-1][j].carID].northEastSouthWest[2] = 1 + cells[i][j].northEastSouthWest[2];
                }
            }
        } else {
            if (j - 1 >= 0 && ((!cells[i][j-1].isVertical && (cells[i][j-1].cellType == CAR || cells[i][j-1].cellType == DEANSCAR)) || cells[i][j-1].cellType == EMPTY || cells[i][j-1].cellType == EXIT)){
                cells[i][j-1].northEastSouthWest[1] = 1 + cells[i][j].northEastSouthWest[1];
                if (cells[i][j-1].carID != -1){
                    updateCarNESW(cells[i][j-1].carID);
//                    cars[cells[i][j-1].carID].northEastSouthWest[1] = 1 + cells[i][j].northEastSouthWest[1];
                }
            }
            if (j + 1  <= cells[0].size() && ((!cells[i][j+1].isVertical && (cells[i][j+1].cellType == CAR || cells[i][j+1].cellType == DEANSCAR)) || cells[i][j+1].cellType == EMPTY || cells[i][j+1].cellType == EXIT)){
                cells[i][j+1].northEastSouthWest[3] = 1 + cells[i][j].northEastSouthWest[3];
                if (cells[i][j+1].carID != -1){
                    updateCarNESW(cells[i][j+1].carID);
//                    cars[cells[i][j+1].carID].northEastSouthWest[3] = 1 + cells[i][j].northEastSouthWest[3];
                }
            }
        }
        if (cells[i][j].cellType == CAR || cells[i][j].cellType == DEANSCAR){

            if (j + 1 < cells[0].size()){
                cells[i][j+1].northEastSouthWest[3] = 0;
                if (cells[i][j+1].carID != -1 && cells[i][j+1].isNeighbouring != BOTH){
                    updateCarNESW(cells[i][j+1].carID);
                }
            }
            if (j - 1 >= 0){
                cells[i][j-1].northEastSouthWest[1] = 0;
                if (cells[i][j-1].carID != -1 && cells[i][j-1].isNeighbouring != BOTH){
                    updateCarNESW(cells[i][j-1].carID);
                }
            }
            if (i - 1 >= 0){
                cells[i-1][j].northEastSouthWest[2] = 0;
                if (cells[i-1][j].carID != -1 && cells[i-1][j].isNeighbouring != BOTH){
                    updateCarNESW(cells[i-1][j].carID);
                }
            }
            if (i + 1 < cells.size()){
                cells[i+1][j].northEastSouthWest[0] = 0;
                if (cells[i+1][j].carID != -1 && cells[i+1][j].isNeighbouring != BOTH){
                    updateCarNESW(cells[i+1][j].carID);
                }
            }
        }
    }

    void updateCarNESW(int carID){
        if (cars[carID].isVertical){
            cars[carID].northEastSouthWest[0] = cells[cars[carID].y][cars[carID].x].northEastSouthWest[0];
            cars[carID].northEastSouthWest[2] = cells[cars[carID].y+cars[carID].length-1][cars[carID].x].northEastSouthWest[2];
        }
        else{
            cars[carID].northEastSouthWest[3] = cells[cars[carID].y][cars[carID].x].northEastSouthWest[3];
            cars[carID].northEastSouthWest[1] = cells[cars[carID].y][cars[carID].x+cars[carID].length-1].northEastSouthWest[1];
        }
    }

    void moveCar(int x, int y, int n, Direction dir){
        if (!(cells[y][x].cellType == CAR || cells[y][x].cellType == DEANSCAR)){
            return;
        }
//        std::cout << "x " << x << " y " << y << " n " << n << " dir " << dir << "\n";
//        std::cout << cars[cells[y][x].carID].getInfo() << "\n";
//        std::cout << cells[y][x].getInfo() << "\n";


        CellType originalCellType = cells[y][x].cellType;
        bool originalIsVertical = cells[y][x].isVertical;
        int originalX = x;
        int originalY = y;
        int originalCarLength = cells[y][x].carLength;
        if (cells[y][x].isVertical){
            switch (dir) {
                case PLUS:
                    if (cells[y][x].northEastSouthWest[2] < n){
                        std::cout << "Cannot " << x << " " << y << " " << n << " Dir " << dir << "\n";
                        return;
                    }
                    cars[cells[y][x].carID].move(dir, n);
                    y -= cells[y][x].carLength - (cells[y][x].nthPiece + 1);
                    for (int offset = 0; offset < originalCarLength; ++offset) {
                        if (cells[y - offset + n][x].cellType == EXIT){
                            win = true;
                        }
                        cells[y - offset + n][x].cellType = cells[y - offset][x].cellType;
                        cells[y - offset + n][x].isVertical = cells[y - offset][x].isVertical;
                        cells[y - offset + n][x].isNeighbouring = cells[y - offset][x].isNeighbouring;
                        cells[y - offset + n][x].c = cells[y - offset][x].c;
                        cells[y - offset + n][x].nthPiece = cells[y - offset][x].nthPiece;
                        cells[y - offset + n][x].carLength = cells[y - offset][x].carLength;
                        cells[y - offset + n][x].carID = cells[y - offset][x].carID;
                        cells[y - offset + n][x].northEastSouthWest = {0, 0, 0, 0};

                        cells[y - offset][x].cellType = EMPTY;
                        cells[y - offset][x].isVertical = false;
                        cells[y - offset][x].isNeighbouring = ZERO;
                        cells[y - offset][x].c = '.';
                        cells[y - offset][x].nthPiece = 0;
                        cells[y - offset][x].carLength = 0;
                        cells[y - offset][x].carID = -1;
                        cells[y - offset][x].northEastSouthWest = {0, 0, 0, 0};

                    }
                    break;
                case MINUS:
                    if (cells[y][x].northEastSouthWest[0] < n){
                        std::cout << "Cannot " << x << " " << y << " " << n << " Dir " << dir << "\n";

                        return;
                    }
                    cars[cells[y][x].carID].move(dir, n);
                    y += cells[y][x].carLength - (cells[y][x].nthPiece)-cells[y][x].carLength;
                    for (int offset = 0; offset < originalCarLength; ++offset) {
                        if (cells[y + offset - n][x].cellType == EXIT){
                            win = true;
                        }
                        cells[y + offset - n][x].cellType = cells[y + offset][x].cellType;
                        cells[y + offset - n][x].isVertical = cells[y + offset][x].isVertical;
                        cells[y + offset - n][x].isNeighbouring = cells[y + offset][x].isNeighbouring;
                        cells[y + offset - n][x].c = cells[y + offset][x].c;
                        cells[y + offset - n][x].nthPiece = cells[y + offset][x].nthPiece;
                        cells[y + offset - n][x].carLength = cells[y + offset][x].carLength;
                        cells[y + offset - n][x].carID = cells[y + offset][x].carID;
                        cells[y + offset - n][x].northEastSouthWest = {0, 0, 0, 0};

                        cells[y + offset][x].cellType = EMPTY;
                        cells[y + offset][x].isVertical = false;
                        cells[y + offset][x].isNeighbouring = ZERO;
                        cells[y + offset][x].c = '.';
                        cells[y + offset][x].nthPiece = 0;
                        cells[y + offset][x].carLength = 0;
                        cells[y + offset][x].carID = -1;
                        cells[y + offset][x].northEastSouthWest = {0, 0, 0, 0};

                    }
                    break;
                default:
                    break;
            }
        } else{

            switch (dir) {
                case PLUS:
                    if (cells[y][x].northEastSouthWest[1] < n){
                        std::cout << "Cannot " << x << " " << y << " " << n << " Dir " << dir << "\n";

                        return;
                    }
                    cars[cells[y][x].carID].move(dir, n);
                    x -= cells[y][x].carLength - (cells[y][x].nthPiece + 1);
                    for (int offset = 0; offset < originalCarLength; ++offset) {
                        if (cells[y][x - offset + n].cellType == EXIT){
                            win = true;
                        }
                        cells[y][x - offset + n].cellType = cells[y][x - offset].cellType;
                        cells[y][x - offset + n].isVertical = cells[y][x - offset].isVertical;
                        cells[y][x - offset + n].isNeighbouring = cells[y][x - offset].isNeighbouring;
                        cells[y][x - offset + n].c = cells[y][x - offset].c;
                        cells[y][x - offset + n].nthPiece = cells[y][x - offset].nthPiece;
                        cells[y][x - offset + n].carLength = cells[y][x - offset].carLength;
                        cells[y][x - offset + n].carID = cells[y][x - offset].carID;
                        cells[y][x - offset + n].northEastSouthWest = {0, 0, 0, 0};

                        cells[y][x - offset].cellType = EMPTY;
                        cells[y][x - offset].isVertical = false;
                        cells[y][x - offset].isNeighbouring = ZERO;
                        cells[y][x - offset].c = '.';
                        cells[y][x - offset].nthPiece = 0;
                        cells[y][x - offset].carLength = 0;
                        cells[y][x - offset].carID = -1;
                        cells[y][x - offset].northEastSouthWest = {0, 0, 0, 0};

                    }
                    break;
                case MINUS:
                    if (cells[y][x].northEastSouthWest[3] < n){
                        std::cout << "Cannot " << x << " " << y << " " << n << " Dir " << dir << "\n";

                        return;
                    }
                    cars[cells[y][x].carID].move(dir, n);
                    x += cells[y][x].carLength - (cells[y][x].nthPiece) - cells[y][x].carLength;
                    for (int offset = 0; offset < originalCarLength; ++offset) {
                        if (cells[y][x + offset - n].cellType == EXIT){
                            win = true;
                        }
                        cells[y][x + offset - n].cellType = cells[y][x + offset].cellType;
                        cells[y][x + offset - n].isVertical = cells[y][x + offset].isVertical;
                        cells[y][x + offset - n].isNeighbouring = cells[y][x + offset].isNeighbouring;
                        cells[y][x + offset - n].c = cells[y][x + offset].c;
                        cells[y][x + offset - n].nthPiece = cells[y][x + offset].nthPiece;
                        cells[y][x + offset - n].carLength = cells[y][x + offset].carLength;
                        cells[y][x + offset - n].carID = cells[y][x + offset].carID;
                        cells[y][x + offset - n].northEastSouthWest = {0, 0, 0, 0};

                        cells[y][x + offset].cellType = EMPTY;
                        cells[y][x + offset].isVertical = false;
                        cells[y][x + offset].isNeighbouring = ZERO;
                        cells[y][x + offset].c = '.';
                        cells[y][x + offset].nthPiece = 0;
                        cells[y][x + offset].carLength = 0;
                        cells[y][x + offset].carID = -1;
                        cells[y][x + offset].northEastSouthWest = {0, 0, 0, 0};

                    }
                    break;
                default:
                    break;
            }
        }
        if (originalIsVertical){
            if (dir == PLUS){
                for (int i = 0; i < originalCarLength; ++i) {
//                    std::cout << x << " " << y - i << std::endl;
                    partialUpdateMoves(x, y-i, false, dir, true);
                    partialUpdateMoves(x, y-i+n, false, dir, true);
                }

                partialUpdateMoves(x, y + n, true, dir, true);
                partialUpdateMoves(x, y-originalCarLength+1 + n, true, dir, true);
            } else {
                for (int i = 0; i < originalCarLength; ++i) {
                    partialUpdateMoves(x, y+i, false, dir, true);
                    partialUpdateMoves(x, y+i-n, false, dir, true);
                }
                partialUpdateMoves(x, y - n, true, dir, true);
                partialUpdateMoves(x, y+originalCarLength-1 - n, true, dir, true);
            }

        }
        else {
            if (dir == PLUS){
                for (int i = 0; i < originalCarLength; ++i) {
                    partialUpdateMoves(x-i, y, true, dir, true);
                    partialUpdateMoves(x-i+n, y, true, dir, true);
                }
                partialUpdateMoves(x + n, y, false, dir, true);
                partialUpdateMoves(x-originalCarLength+1 + n, y, false, dir, true);
            } else {
                for (int i = 0; i < originalCarLength; ++i) {
                    partialUpdateMoves(x+i, y, true, dir, true);
                    partialUpdateMoves(x+i-n, y, true, dir, true);
                }
                partialUpdateMoves(x - n, y, false, dir, true);
                partialUpdateMoves(x+originalCarLength-1 - n, y, false, dir, true);

            }

        }
    }
};

#endif //DEANSCAR_GRID_H
