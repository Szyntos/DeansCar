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
    // Node position
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
    }

    std::vector<Node*> getNeighboursAsNodes() {
        std::vector<Move> moves = originalGrid->getPossibleMoves();
        std::vector<Node*> neighbours;
        for (Move move: moves) {
            Node node = Node();
            originalGrid->moveCar(move.x, move.y, move.n, move.dir);
            node.hash = originalGrid->toIntegersBaseFour();
            neighbours.push_back(&node);
        }
        return neighbours;
    }
    // Parent node in the path
    // Calculate the F cost of a node
    void calculateFCost() {
        fCost = gCost + hCost;
    }

    bool isWon(){
        return originalGrid->isWon();
    }
};

struct VectorHash {
    size_t operator()(const std::vector<unsigned int>& v) const {
        std::hash<unsigned int> hasher;
        size_t seed = 0;
        for (unsigned int i : v) {
            // Combine the hash of the current element with the running hash
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

// Define a custom equality comparison for vectors of unsigned int
struct VectorEqual {
    bool operator()(const std::vector<unsigned int>& a, const std::vector<unsigned int>& b) const {
        return a == b;
    }
};

using VisitedSet = std::unordered_set<std::vector<unsigned int>, VectorHash, VectorEqual>;

class AStar {
public:
    std::vector<Move> findPath(Node* startNode, Node* goalNode) {
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
    bool dfs(Node* node, std::vector<Move>& path, VisitedSet visited) {
        if (visited.find(node->hash) != visited.end())  return false;

        if (path.size() > node->originalGrid->movesLimit){
            return false;
        }
//        std::cout << node->originalGrid->getGridFromHashBaseFour(node->hash) << "aa\n\n";

        // Mark this node as visited
        visited.insert(node->hash);
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

    static bool isVisited(const std::vector<unsigned int>& hash, const std::vector<std::vector<unsigned int>>& visited) {
        for (const auto& i : visited) {
            if (hash.size() != i.size()) {
                continue; // Skip to the next iteration if sizes don't match
            }
            bool allElementsMatch = true; // Assume all elements match initially
            for (size_t j = 0; j < i.size(); ++j) {
                if (hash[j] != i[j]) {
                    allElementsMatch = false; // Found a mismatch
                    break; // No need to check further elements
                }
            }
            if (allElementsMatch) {
                return true; // Found a matching vector
            }
        }
        return false; // No match found
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
    std::vector<Move> retracePath(Node* startNode, Node* endNode) {
        std::vector<Move> path;
        Node* currentNode = endNode;

        while (currentNode != startNode) {
            path.push_back(currentNode->prevMove);
            currentNode = currentNode->parent;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    // Placeholder for a function to get a node's neighbors
    std::vector<Node *> getNeighbors(Node* node) {
        return node->getNeighboursAsNodes();
    }

    // Function to calculate the distance between two nodes
    float getDistance(Node* nodeA, Node* nodeB) {

    }
};

#endif //DEANSCAR_ASTAR_H
