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
        startNode = Node(grid, {}, {0, 0, 0, PLUS});
    }

    std::vector<Move> solveDFS(int maxPathLength){
        std::vector<Move> path;
        std::unordered_set<Node*> visited;
        aStar.dfs(&startNode, path, visited, maxPathLength);
        return path;
    }

};


#endif //DEANSCAR_SOLVER_H
