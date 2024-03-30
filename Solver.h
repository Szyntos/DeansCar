//
// Created by Szymon on 28.03.2024.
//

#ifndef DEANSCAR_SOLVER_H
#define DEANSCAR_SOLVER_H


#include "Grid.h"
#include "AStar.h"

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
        if (found){
            for (const auto& move : path) {
                std::cout << "Move - x: " << move.x << ", y: " << move.y
                          << ", n: " << move.n << ", Direction: "
                          << directionToString(move.dir) << std::endl;
            }
        }

        return path;
    }
private:
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


#endif //DEANSCAR_SOLVER_H
