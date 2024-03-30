//
// Created by Szymon on 28.03.2024.
//

#ifndef DEANSCAR_ASTAR_H
#define DEANSCAR_ASTAR_H
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include "Grid.h"

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
    Move prevMove = {};

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
    bool dfs(Node* node, std::vector<Move>& path, std::unordered_set<Node*, NodeHasher, NodeEqual>& visited, int maxPathLength) {
        if (visited.find(node) != visited.end()) return false;

        if (path.size() > maxPathLength){
            return false;
        }
//        std::cout << node->originalGrid->getGridFromHashBaseFour(node->hash) << "aa\n\n";

        // Mark this node as visited
        visited.insert(node);
        // Add the current node to the path
        path.push_back(node->prevMove);

        // Check if the current node is the target node
        if (node->isWon()) {
            std::cout << "AAAAAAAAAAAAAAAAAAAAA\n";
            return true; // Return true to stop the search
        }
        int neighbourCount = 0;
        Node neighbourNode;
        // Recursively search in unvisited neighbours
        for (Move neighbour : node->getNeighbours()) {

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
            if (dfs(&neighbourNode, path, visited, maxPathLength)) {
                return true; // If target is found in a subtree, return true to propagate the success back up
            }

        }

        // Backtrack: If not found in this path, remove the current node from the path before returning
        path.pop_back();
        return false;
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

#endif //DEANSCAR_ASTAR_H
