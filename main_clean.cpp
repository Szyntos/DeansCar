#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <unordered_set>


enum Direction {
    PLUS = 1,
    MINUS = 2,
    BOTH = 3,
    ZERO = 4
};

enum CellType {
    CAR = 1,
    DEANSCAR = 2,
    EMPTY = 3,
    BARRIER = 4,
    EXIT = 5
};

class Cell {
public:
    CellType cellType;
    bool isVertical;
    Direction isNeighbouring;
    std::array<int, 4> northEastSouthWest = {};
    int x;
    int y;
    char c = '.';
    int nthPiece = 0;
    int carLength = 0;
    int carID = -1;

    Cell() : cellType(EMPTY), isVertical(false), isNeighbouring(ZERO),
             northEastSouthWest({0, 0, 0, 0}), x(0), y(0) {
    }
    // Constructor definition
    Cell(int xCoord, int yCoord)
            : cellType(EMPTY), isVertical(false), isNeighbouring(ZERO),
              northEastSouthWest({0, 0, 0, 0}), x(xCoord), y(yCoord) {
    }
};



class Car {
public:
    int ID;
    std::array<int, 4> northEastSouthWest = {};
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

    bool canMove(){
        for (int i = 0; i < 4; ++i) {
            if (northEastSouthWest[i] > 0){
                return true;
            }
        }
        return false;
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




class Move {
public:
    int x;
    int y;
    int n;
    Direction dir;
    int carID;
    char letter;
};


class Grid {
public:
    int width;
    int height;
    int carCount;
    std::vector<std::vector<Cell>> cells;
    std::vector<Car> cars;
    std::vector<Car> verticalCars;
    std::vector<Car> horizontalCars;
    int exitX = 0;
    int exitY = 0;
    int movesLimit = 0;

    Grid(){
        width = 0;
        height = 0;
        carCount = 0;
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

    bool isWon(){
        return cells[exitY][exitX].cellType != EXIT;
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
        if (x < 0 || x >= width || y < 0 || y >= height){
            return;
        }
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



     std::vector<unsigned int> toIntegersBaseFour() const {
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
        int toFind = 0;
        std::vector<int> verticalCarIdOnEachColumn(width, 0);
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
                        toFind = 0;
                        for (int z = 0; z < verticalCars.size(); z++){
                            if (verticalCars[z].x == j){
                                if (toFind == verticalCarIdOnEachColumn[j]){
                                    verticalCarIndex = z;
                                    verticalCarIdOnEachColumn[j]++;
                                    break;
                                }
                                toFind++;
                            }
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
                        break;
                    case 3: grid[i][j] = '#'; break;
                }
//                for (const auto& row : grid) {
//                    for (char cell : row) {
//                        std::cout << cell << " ";
//                    }
//                    std::cout << std::endl; // Move to the next line after printing each row
//                }
//                std::cout << "\n\n";

                currentBitIndex += 2; // Increment by 2 because each cell uses 2 bits
            }
        }
//        for (const auto& row : grid) {
//            for (char cell : row) {
//                std::cout << cell << " ";
//            }
//            std::cout << std::endl; // Move to the next line after printing each row
//        }

        std::string result = std::to_string(grid[0].size()) + " " + std::to_string(grid.size()) + " " + std::to_string(movesLimit);

        result += "\n";
        for (const auto& row : grid) {
            for (char cell : row) {
                result.push_back(cell);
            }
            result.push_back('\n');
        }
//        std::cout <<result;
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
                            possibleMoves.push_back(Move{selectedCar.x, selectedCar.y, j, MINUS, selectedCar.ID, 'U'});
                            break;
                        case 1:
                            possibleMoves.push_back(Move{selectedCar.x+selectedCar.length-1, selectedCar.y, j, PLUS, selectedCar.ID, 'R'});
                            break;
                        case 2:
                            possibleMoves.push_back(Move{selectedCar.x, selectedCar.y+selectedCar.length-1, j, PLUS, selectedCar.ID, 'D'});
                            break;
                        case 3:
                            possibleMoves.push_back(Move{selectedCar.x, selectedCar.y, j, MINUS, selectedCar.ID, 'L'});
                            break;
                        default:
                            break;
                    }
                }
            }

        }
        return possibleMoves;
    }

    CellType charToCellType(char c) {
        switch (c) {
            case '.':
                return EMPTY;
            case '#':
                return BARRIER;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'x':
            case 'y':
            case 'z':
            case 'w':
                return CAR;
            case 'o':
                return DEANSCAR;
            default:
                return EMPTY; // Default case to handle unexpected characters
        }
    }
    void parseInputToGrid(const std::string& input) {
        std::istringstream iss(input);
        int W, H, N;
        iss >> W >> H >> N;

        width = W;
        height = H;
        movesLimit = N;

        cells.clear(); // Clear existing cells
        cells.resize(height, std::vector<Cell>(width)); // Create new cells with default Cell objects

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                cells[i][j] = Cell(j, i);
            }
        }

        std::string line;
        std::getline(iss, line);
        int carsCount = 1;
        for (int i = 0; i < H; ++i) {
            std::getline(iss, line);
            for (int j = 0; j < W; ++j) {
                char c = line[j];
                if (c == 'x' || c == 'a'){
                    carsCount++;
                }
                cells[i][j].cellType = charToCellType(c);
                if (c == 'x' || c == 'y' || c == 'z' || c == 'w') {
                    cells[i][j].isVertical = true;
                }
                cells[i][j].c = c;
                if (c == '.' && (i == 0 || i == H - 1 || j == 0 || j == W - 1)){
                    cells[i][j].cellType = EXIT;
                    exitX = j;
                    exitY = i;
                }
            }
        }
        carCount = carsCount;
        cars.clear(); // Clear existing cars
        cars.resize(carCount);
        int kk = 0;
        bool deansCarInitialized = false;
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W; ++j) {
                Cell currentCell = cells[i][j];

                switch (currentCell.c) {

                    case 'a':
                        cells[i][j].nthPiece = 0;
                        cells[i][j].isNeighbouring = PLUS;
                        cars[kk] = Car(kk, j, i, false, 1);
                        cells[i][j].carID = kk;
                        kk++;
                        break;
                    case 'b':
                        cells[i][j].nthPiece = 1;
                        if (cells[i][j+1].c == 'c'){
                            cells[i][j].isNeighbouring = BOTH;
                        }else {

                            cells[i][j].isNeighbouring = MINUS;
                            cells[i][j-1].carLength = 2;
                            cells[i][j].carLength = 2;
                        }
                        cells[i][j].carID = cells[i][j-1].carID;
                        cars[cells[i][j].carID].length = cells[i][j].carLength;
                        break;
                    case 'c':
                        cells[i][j].nthPiece = 2;
                        if (cells[i][j+1].c == 'd'){
                            cells[i][j].isNeighbouring = BOTH;
                        }else {
                            cells[i][j].isNeighbouring = MINUS;
                            cells[i][j-2].carLength = 3;
                            cells[i][j-1].carLength = 3;
                            cells[i][j].carLength = 3;
                        }
                        cells[i][j].carID = cells[i][j-2].carID;
                        cells[i][j-1].carID = cells[i][j-2].carID;
                        cars[cells[i][j].carID].length = cells[i][j].carLength;
                        break;
                    case 'd':
                        cells[i][j].nthPiece = 3;
                        cells[i][j].isNeighbouring = MINUS;
                        cells[i][j-3].carLength = 4;
                        cells[i][j-2].carLength = 4;
                        cells[i][j-1].carLength = 4;
                        cells[i][j].carLength = 4;
                        cells[i][j].carID = cells[i][j-3].carID;
                        cells[i][j-1].carID = cells[i][j-3].carID;
                        cells[i][j-2].carID = cells[i][j-3].carID;
                        cars[cells[i][j].carID].length = cells[i][j].carLength;
                        break;
                    case 'x':
                        cells[i][j].nthPiece = 0;
                        cells[i][j].isNeighbouring = PLUS;
                        cars[kk] = Car(kk, j, i, true, 1);
                        cells[i][j].carID = kk;
                        kk++;
                        break;
                    case 'y':
                        cells[i][j].nthPiece = 1;
                        if (cells[i+1][j].c == 'z'){
                            cells[i][j].isNeighbouring = BOTH;
                        }else {
                            cells[i][j].isNeighbouring = MINUS;
                            cells[i-1][j].carLength = 2;
                            cells[i][j].carLength = 2;
                        }
                        cells[i][j].carID = cells[i-1][j].carID;
                        cars[cells[i][j].carID].length = cells[i][j].carLength;
                        break;
                    case 'z':
                        cells[i][j].nthPiece = 2;
                        if (cells[i+1][j].c == 'w'){
                            cells[i][j].isNeighbouring = BOTH;
                        }else {
                            cells[i][j].isNeighbouring = MINUS;
                            cells[i-2][j].carLength = 3;
                            cells[i-1][j].carLength = 3;
                            cells[i][j].carLength = 3;
                        }
                        cells[i][j].carID = cells[i-2][j].carID;
                        cells[i-1][j].carID = cells[i-2][j].carID;
                        cars[cells[i][j].carID].length = cells[i][j].carLength;
                        break;
                    case 'w':
                        cells[i][j].nthPiece = 3;
                        cells[i][j].isNeighbouring = MINUS;
                        cells[i-3][j].carLength = 4;
                        cells[i-2][j].carLength = 4;
                        cells[i-1][j].carLength = 4;
                        cells[i][j].carLength = 4;
                        cells[i][j].carID = cells[i-3][j].carID;
                        cells[i-1][j].carID = cells[i-3][j].carID;
                        cells[i-2][j].carID = cells[i-3][j].carID;
                        cars[cells[i][j].carID].length = cells[i][j].carLength;
                        break;
                    case 'o':

                        if (cells[i+1][j].c == 'o' && cells[i-1][j].c == 'o' ||
                            cells[i][j+1].c == 'o' && cells[i][j-1].c == 'o' ){
                            cells[i][j].isNeighbouring = BOTH;
                        }else if (cells[i+1][j].c == 'o' || cells[i][j+1].c == 'o'){
                            cells[i][j].isNeighbouring = PLUS;
                        } else{
                            cells[i][j].isNeighbouring = MINUS;
                        }

                        if (cells[i-1][j].c != 'o' && cells[i+1][j].c != 'o' && cells[i][j-1].c != 'o'){
                            if (!deansCarInitialized){
                                cars[kk] = Car(kk, j, i, false, 1);
                                cars[kk].type = DEANSCAR;
                                cells[i][j].carID = kk;
                                kk++;
                                deansCarInitialized = true;
                            }
                            cells[i][j].isVertical = false;
                            int k = 0;
                            while (cells[i][j + k].c == 'o'){
                                cells[i][j + k].nthPiece = k;
                                cells[i][j + k].isVertical = false;
                                k++;
                            }
                            int length = k;
                            cars[kk-1].length = length;
                            k--;
                            while (cells[i][j+k].c == 'o'){
                                cells[i][j+k].carLength = length;
                                cells[i][j+k].carID = cells[i][j].carID;
                                k--;
                            }

                        } else if (cells[i-1][j].c != 'o' && cells[i][j+1].c != 'o' && cells[i][j-1].c != 'o'){
                            if (!deansCarInitialized){
                                cars[kk] = Car(kk, j, i, true, 1);
                                cars[kk].type = DEANSCAR;
                                cells[i][j].carID = kk;
                                kk++;
                                deansCarInitialized = true;
                            }
                            cells[i][j].isVertical = true;
                            int k = 0;
                            while (cells[i+k][j].c == 'o'){
                                cells[i+k][j].nthPiece = k;
                                cells[i+k][j].isVertical = true;
                                k++;
                            }
                            int length = k;
                            cars[kk-1].length = length;
                            k--;
                            while (cells[i+k][j].c == 'o'){
                                cells[i+k][j].carLength = length;
                                cells[i+k][j].carID = cells[i][j].carID;
                                k--;
                            }

                        }

                        break;

                }
            }

        }
        splitCars();
        updateMoves();
    }



};


struct Node {
    int x = 0;
    int y = 0;
    // Node position
    int value = 0;
    float gCost = 0;
    float hCost = 0;
    float fCost = 0;
    Node* parent = nullptr;
    Grid* originalGrid = nullptr;
    Move prevMove = {0, 0, 0, BOTH, -1};

    std::vector<unsigned int> hash;

    Node()= default;

    Node(Grid* oGrid, Move move){
        originalGrid = oGrid;
        prevMove = move;
        hash = originalGrid->toIntegersBaseFour();

    };

    bool operator==(const Node& other) const {
        return hash == other.hash;
    }

    void recomputeGrid(){
        originalGrid->parseInputToGrid(originalGrid->getGridFromHashBaseFour(hash));
    }

    std::vector<Move> getNeighbours() {
        std::vector<Move> moves = originalGrid->getPossibleMoves();
        return moves;
    }// Parent node in the path
    // Calculate the F cost of a node
    void calculateFCost() {
        fCost = gCost + hCost;
    }

    bool isWon(){
        return originalGrid->isWon();
    }
};

struct NodeHasher {
    std::size_t operator()(const Node* node) const {
        std::size_t seed = 0;
        for (unsigned int i : node->hash) {
            seed ^= std::hash<unsigned int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
struct NodeEqual {
    bool operator()(const Node* lhs, const Node* rhs) const {
        return lhs->hash == rhs->hash;
    }
};


class AStar {
public:
    std::vector<Node*> findPath(Node* startNode, Node* goalNode) {
        openList.push_back(startNode);

        while (!openList.empty()) {
            Node* currentNode = getLowestFCostNode(openList);
            if (currentNode == goalNode) {
                return retracePath(startNode, goalNode);
            }

            openList.erase(std::remove(openList.begin(), openList.end(), currentNode), openList.end());
            closedList.push_back(currentNode);

            for (Node* neighbor : getNeighbors(currentNode)) {
                if (std::find(closedList.begin(), closedList.end(), neighbor) != closedList.end())
                    continue;

                float newGCost = currentNode->gCost + getDistance(currentNode, neighbor);
                if (newGCost < neighbor->gCost || std::find(openList.begin(), openList.end(), neighbor) == openList.end()) {
                    neighbor->gCost = newGCost;
                    neighbor->hCost = getDistance(neighbor, goalNode);
                    neighbor->calculateFCost();
                    neighbor->parent = currentNode;

                    if (std::find(openList.begin(), openList.end(), neighbor) == openList.end()) {
                        openList.push_back(neighbor);
                    }
                }
            }
        }

        return {}; // Return empty path if no path is found
    }
    bool dfs(Node* node, std::vector<Move>& path, std::unordered_set<Node*, NodeHasher, NodeEqual>& visited) {
        if (visited.find(node) != visited.end()) return false;

        if (path.size() > node->originalGrid->movesLimit){
            return false;
        }
//        std::cout << node->originalGrid->getGridFromHashBaseFour(node->hash) << "aa\n\n";

        // Mark this node as visited
        visited.insert(node);
        // Add the current node to the path
        path.push_back(node->prevMove);

        // Check if the current node is the target node
        if (node->isWon()) {
            return true; // Return true to stop the search
        }
        int neighbourCount = 0;
        Node neighbourNode;
        int prevCarID = node->prevMove.carID;
        // Recursively search in unvisited neighbours
        for (Move neighbour : node->getNeighbours()) {

            if (neighbour.carID == prevCarID &&( (neighbour.dir == MINUS && node->prevMove.dir == PLUS) || (neighbour.dir == PLUS && node->prevMove.dir == MINUS))){
                continue;
            }
            if (neighbourCount == 0){
                neighbourCount++;
                node->originalGrid->moveCar(neighbour.x, neighbour.y, neighbour.n, neighbour.dir);
                neighbourNode = Node(node->originalGrid, neighbour);
            }
            else {
                node->recomputeGrid();
                node->originalGrid->moveCar(neighbour.x, neighbour.y, neighbour.n, neighbour.dir);
                neighbourNode = Node(node->originalGrid, neighbour);
            }
            if (dfs(&neighbourNode, path, visited)) {
                return true; // If target is found in a subtree, return true to propagate the success back up
            }

        }

        // Backtrack: If not found in this path, remove the current node from the path before returning
        path.pop_back();
        return false;
    }

    void movesListToOutput(std::vector<Move>& moves){
        std::cout << moves.size()-1 << "\n";
        int i = 0;
        for (Move move: moves) {
            if (i == 0){
                i++;
                continue;
            }
            if (i == moves.size()-1){
                std::cout << move.x << " " << move.y << " " << move.letter << " " << move.n + 1 << "\n";
            }else{
                std::cout << move.x << " " << move.y << " " << move.letter << " " << move.n << "\n";
            }
            i++;
        }
    }


private:
    std::vector<Node*> openList;
    std::vector<Node*> closedList;

    // Function to get the node with the lowest F cost
    Node* getLowestFCostNode(const std::vector<Node*>& list) {
        Node* lowest = list[0];
        for (Node* node : list) {
            if (node->fCost < lowest->fCost) {
                lowest = node;
            }
        }
        return lowest;
    }

    // Function to retrace the path from the goal node back to the start node
    std::vector<Node*> retracePath(Node* startNode, Node* endNode) {
        std::vector<Node*> path;
        Node* currentNode = endNode;

        while (currentNode != startNode) {
            path.push_back(currentNode);
            currentNode = currentNode->parent;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    // Placeholder for a function to get a node's neighbors
    std::vector<Node*> getNeighbors(Node* node) {
        // Implement according to your grid/environment
        std::vector<Node*> neighbors;
        // Add neighbors to the list
        return neighbors;
    }

    // Function to calculate the distance between two nodes
    float getDistance(Node* nodeA, Node* nodeB) {
        int dx = std::abs(nodeA->x - nodeB->x);
        int dy = std::abs(nodeA->y - nodeB->y);
        return std::sqrt(dx * dx + dy * dy);
    }
};
class Solver {
public:
    Grid* grid = nullptr;
    AStar aStar;
    Node startNode;

    Solver(){
        startNode = Node();
    }
    explicit Solver(Grid* grid): grid(grid){
        startNode = Node(grid, {0, 0, 0, PLUS, -1});
    }
    std::vector<Move> solveDFS(){
        std::vector<Move> path;
        std::unordered_set<Node*, NodeHasher, NodeEqual> visited;
        bool found = aStar.dfs(&startNode, path, visited);
        return path;
    }
};
Grid grid;

int main() {
    std::string line;
    std::stringstream input;
    std::getline(std::cin, line);
    input << line << "\n";

    int rows, cols, extraInfo;
    std::istringstream(line) >> cols >> rows >> extraInfo; // Adjust according to your input format.

    for(int i = 0; i < rows; ++i) {
        std::getline(std::cin, line);
        input << line << "\n";
    }
    std::vector<Move> moves;
    grid.parseInputToGrid(input.str());
    Solver solver = Solver(&grid);
    moves = solver.solveDFS();

    solver.aStar.movesListToOutput(moves);
    return 0;
}
