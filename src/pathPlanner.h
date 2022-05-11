#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include <iostream>
#include <vector>
#include <memory>
#include "game.h"

class Game;

struct Node {
  public:
    Node(int x, int y) : x(x), y(y) {};
    Node(int x, int y, int g_value, int h_value, std::shared_ptr<Node> parentNodePtr) : x(x), y(y), g_value(g_value), h_value(h_value), parentNodePtr(parentNodePtr) {};
    bool operator!=(const Node& node) {
      return (this->x != node.x) || (this->y != node.y);
    }
    int x;
    int y;
    int g_value = 0;
    int h_value = 0;
    std::shared_ptr<Node> parentNodePtr = nullptr;
    bool isVisited = false;
};

class PathPlanner {
  public:
    PathPlanner(std::size_t grid_width, std::size_t grid_height);
    ~PathPlanner();
    std::vector<std::shared_ptr<Node>> CalcPath(int startX, int startY, int targetX, int targetY);
    void SetGamePtr(Game* gamePtr);
  private:
    static bool Compare(std::shared_ptr<Node> nodePtr1, std::shared_ptr<Node> nodePtr2);
    std::vector<Node> pathList;
    std::vector<std::shared_ptr<Node>> openList;
    std::vector<std::vector<Node>> nodeMap;
    std::vector<std::shared_ptr<Node>> CalcEndNodes(Node targetNode);
    std::vector<std::shared_ptr<Node>> CalcOptimumPath(std::shared_ptr<Node> startNodePtr, std::shared_ptr<Node> endNodePtr);
    bool CheckObstruction(std::shared_ptr<Node> checkNodePtr);
    void AddNeighborNode(std::shared_ptr<Node> currentNodePtr, std::shared_ptr<Node> endNodePtr);
    void AddNeighborNodeSingleDirection(std::shared_ptr<Node> currentNodePtr, std::shared_ptr<Node> endNodePtr, int deltaX, int deltaY);
    std::vector<std::shared_ptr<Node>> ConstructOptimumPath(std::shared_ptr<Node> currentNodePtr);
    std::shared_ptr<Node> NextNode();
    Game* gamePtr; 
    std::size_t grid_width;
    std::size_t grid_height;
};

#endif