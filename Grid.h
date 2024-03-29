//
// Created by szynt on 21.03.2024.
//

#ifndef DEANSCAR_GRID_H
#define DEANSCAR_GRID_H


#include "Cell.h"
#include "Car.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <algorithm>



class Grid {
public:
    int width;
    int height;
    int carCount;
    std::vector<std::vector<Cell>> cells;
    std::vector<Car> cars;
    std::vector<Car> verticalCars;
    std::vector<Car> horizontalCars;


    bool win = false;

    Grid(){
        width = 0;
        height = 0;
        carCount = 0;
    }

    Grid(int w, int h, int carCount) : width(w), height(h), cells(height, std::vector<Cell>(width)),
                                    carCount(carCount), cars(carCount) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                cells[i][j] = Cell(j, i);
            }
        }

    }

    void splitCars(){
        for (int i = 0; i < carCount; i++) {
            if (cars[i].isVertical) {
                verticalCars.push_back(cars[i]);
            } else {
                horizontalCars.push_back(cars[i]);
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


    [[nodiscard]] std::vector<unsigned int> toIntegers() const {
        std::vector<unsigned int> result;
        const int bitsInInt = sizeof(unsigned int) * 8; // Total bits in an unsigned int

        // Total number of bits required to represent the grid
        int totalBits = width * height;
        // Total number of unsigned ints required to represent the grid
        int totalInts = (totalBits + bitsInInt - 1) / bitsInInt;

        unsigned int currentInt = 0; // Current integer being constructed
        int bitPos = 0; // Current bit position in the currentInt

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                // Set the bit in currentInt if the cell is full
                if (cells[i][j].cellType != EMPTY && cells[i][j].cellType != EXIT) {
                    currentInt |= (1U << bitPos);
                }
                bitPos++;

                // If the currentInt is full or we are at the last cell
                if (bitPos == bitsInInt || (i == height - 1 && j == width - 1)) {
                    // Save the currentInt and reset for the next
                    result.push_back(currentInt);
                    currentInt = 0; // Reset currentInt
                    bitPos = 0; // Reset bit position
                }
            }
        }

        return result;
    }
    void printGridFromHash(const std::vector<unsigned int>& integerHash, int width, int height) {
        int bitsInInt = sizeof(unsigned int) * 8;
        int totalBits = width * height;

        // Keep track of the current bit position across all integers
        int currentBitIndex = 0;

        for (int i = 0; i < height; ++i) {
            std::string row;
            for (int j = 0; j < width; ++j) {
                // Find which integer and bit position within that integer corresponds to this cell
                int intIndex = currentBitIndex / bitsInInt;
                int bitPos = currentBitIndex % bitsInInt;

                // Retrieve the correct integer from the hash
                unsigned int value = integerHash[intIndex];

                // Determine if the current cell is full ('#') or empty ('.') by checking the bit at bitPos
                if (value & (1U << bitPos)) {
                    row += '#';
                } else {
                    row += '.';
                }

                // Move to the next bit
                currentBitIndex++;
            }
            // Print the row
            std::cout << row << std::endl;
        }
    }

    [[nodiscard]] std::vector<unsigned int> toIntegersBaseFour() const {
        std::vector<unsigned int> result;
        const int bitsInInt = sizeof(unsigned int) * 8; // Total bits in an unsigned int

        // Each cell now needs 2 bits for its state (00, 01, 10, 11)
        int totalCells = width * height;
        // Calculate how many cells can be represented in one unsigned int
        int cellsPerInt = bitsInInt / 2;

        unsigned int currentInt = 0; // Current integer being constructed
        int cellCount = 0; // Current cell count within the currentInt
        int state;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                switch (cells[i][j].cellType) {
                    case EXIT:
                    case EMPTY:
                        state = 0;
                        break;
                    case BARRIER:
                        state = 3;
                        break;
                    case CAR:
                    case DEANSCAR:
                        if (cells[i][j].isVertical){
                            state = 2;
                        } else{
                            state = 1;
                        }
                        break;
                }
                // Ensure state is only 2 bits and shift it into the current position
                currentInt |= (state << (2 * cellCount));
                cellCount++;

                // If the currentInt is full of cells or we are at the last cell
                if (cellCount == cellsPerInt || (i == height - 1 && j == width - 1)) {
                    // Save the currentInt and reset for the next
                    result.push_back(currentInt);
                    currentInt = 0; // Reset currentInt
                    cellCount = 0; // Reset cell count
                }
            }
        }

        return result;
    }

    void printGridFromHashBaseFour(const std::vector<unsigned int>& integerHash) {
        int bitsInInt = sizeof(unsigned int) * 8;
        // Since each cell is represented by 2 bits, totalBits is twice the cell count
        int totalBits = width * height * 2;

        // Keep track of the current bit position across all integers
        int currentBitIndex = 0;

        for (int i = 0; i < height; ++i) {
            std::string row;
            for (int j = 0; j < width; ++j) {
                // Calculate which integer and bit position within that integer corresponds to this cell
                int intIndex = currentBitIndex / bitsInInt;
                int bitPos = (currentBitIndex % bitsInInt);

                // Retrieve the correct integer from the hash
                unsigned int value = integerHash[intIndex];

                // Extract the two bits for the current cell's state
                unsigned int cellState = (value >> bitPos) & 0b11; // Mask the two relevant bits

                // Map the cellState to its character representation
                switch (cellState) {
                    case 0:
                        row += '.';
                        break;
                    case 1:
                        row += 'a';
                        break;
                    case 2:
                        row += 'x';
                        break;
                    case 3:
                        row += '#';
                        break;
                }

                // Move to the next cell's bits
                currentBitIndex += 2; // Increment by 2 because each cell uses 2 bits
            }
            // Print the row
            std::cout << row << std::endl;
        }
    }

    std::string getGridFromHashBaseFour(const std::vector<unsigned int>& integerHash) {
        int bitsInInt = sizeof(unsigned int) * 8;
        // Since each cell is represented by 2 bits, totalBits is twice the cell count
        int totalBits = width * height * 2;

        // Initialize the 2D vector with the specified dimensions
        std::vector<std::vector<char>> grid(height, std::vector<char>(width));

        // Keep track of the current bit position across all integers
        int currentBitIndex = 0;
        int horizontalCarIndex = 0;
        int currentHorizontalLength = 0;
        int verticalCarIndex = 0;
        int currentVerticalLength = 0;
        bool isDean = false;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                // Calculate which integer and bit position within that integer corresponds to this cell
                int intIndex = currentBitIndex / bitsInInt;
                int bitPos = (currentBitIndex % bitsInInt);

                // Retrieve the correct integer from the hash
                unsigned int value = integerHash[intIndex];

                // Extract the two bits for the current cell's state
                unsigned int cellState = (value >> bitPos) & 0b11; // Mask the two relevant bits

                // Map the cellState to its character representation and assign to the grid
                switch (cellState) {
                    case 0: grid[i][j] = '.'; break;
                    case 1:
                        currentHorizontalLength++;
                        if (horizontalCars[horizontalCarIndex].type == DEANSCAR){
                            switch (currentHorizontalLength) {
                                case 1:
                                case 2:
                                case 3:
                                case 4:
                                    grid[i][j] = 'o';
                                    break;
                            }
                        } else{
                            switch (currentHorizontalLength) {
                                case 1:
                                    grid[i][j] = 'a';
                                    break;
                                case 2:
                                    grid[i][j] = 'b';
                                    break;
                                case 3:
                                    grid[i][j] = 'c';
                                    break;
                                case 4:
                                    grid[i][j] = 'd';
                                    break;
                            }
                        }

                        if (currentHorizontalLength >= horizontalCars[horizontalCarIndex].length){
                            currentHorizontalLength = 0;
                            horizontalCarIndex++;
                        }
                        break;
                    case 2:
                        if (grid[i][j] == 'x' || grid[i][j] == 'y' || grid[i][j] == 'z' || grid[i][j] == 'w'){
                            break;
                        }
                        for (int k = 0; k < verticalCars[verticalCarIndex].length; ++k) {
                            switch (k) {
                                case 0:
                                    grid[i+k][j] = 'x';
                                    break;
                                case 1:
                                    grid[i+k][j] = 'y';
                                    break;
                                case 2:
                                    grid[i+k][j] = 'z';
                                    break;
                                case 3:
                                    grid[i+k][j] = 'w';
                                    break;
                            }
                        }
                        verticalCarIndex++;
                        break;
                    case 3: grid[i][j] = '#'; break;
                }

                currentBitIndex += 2; // Increment by 2 because each cell uses 2 bits
            }
        }
//        for (const auto& row : grid) {
//            for (char cell : row) {
//                std::cout << cell << " ";
//            }
//            std::cout << std::endl; // Move to the next line after printing each row
//        }

        std::string result = std::to_string(grid[0].size()) + " " + std::to_string(grid.size()) + " " + std::to_string(carCount);

        result += "\n";
        for (const auto& row : grid) {
            for (char cell : row) {
                result.push_back(cell);
            }
            result.push_back('\n');
        }
        std::cout <<result;
        return result;
    }

    std::vector<Car> getCarsThatCanMove(){
        std::vector<Car> carsThatCanMove;
        std::copy_if(cars.begin(), cars.end(), std::back_inserter(carsThatCanMove), [](Car& car) {
            return car.canMove();
        });
        return carsThatCanMove;
    };

    std::vector<Move> getPossibleMoves(){
        std::vector<Move> possibleMoves;
        std::vector<Car> carsThatCanMove = getCarsThatCanMove();
        for (const auto & selectedCar : carsThatCanMove) {
            for (size_t moveIndex = 0; moveIndex < selectedCar.northEastSouthWest.size(); ++moveIndex) {
                for (int j = 1; j < selectedCar.northEastSouthWest[moveIndex] + 1; j++){
                    switch (moveIndex) {
                        case 0:
                            possibleMoves.push_back(Move{selectedCar.x, selectedCar.y, j, MINUS});
                            break;
                        case 1:
                            possibleMoves.push_back(Move{selectedCar.x+selectedCar.length-1, selectedCar.y, j, PLUS});
                            break;
                        case 2:
                            possibleMoves.push_back(Move{selectedCar.x, selectedCar.y+selectedCar.length-1, j, PLUS});
                            break;
                        case 3:
                            possibleMoves.push_back(Move{selectedCar.x, selectedCar.y, j, MINUS});
                            break;
                        default:
                            break;
                    }
                }
            }

        }
        return possibleMoves;
    }

};

#endif //DEANSCAR_GRID_H