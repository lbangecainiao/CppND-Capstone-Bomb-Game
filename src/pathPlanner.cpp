#include "pathPlanner.h"
#include <algorithm>

PathPlanner::PathPlanner(std::size_t grid_width, std::size_t grid_height) : grid_width(grid_width), grid_height(grid_height) {
  int i = 0;
  while (i < grid_height) {
    int j = 0;
    std::vector<Node> nodeRow;
    while (j < grid_width) {
      nodeRow.emplace_back(Node(i, j));
      j++;
    }
    nodeMap.emplace_back(nodeRow);
    i++;
  }
  std::cout << "PathPlanner is constructed" << std::endl;
}

PathPlanner::~PathPlanner() {}

void PathPlanner::SetGamePtr(Game* gamePtr) {
  this->gamePtr = gamePtr;
}

std::vector<std::shared_ptr<Node>> PathPlanner::CalcPath(int startX, int startY, int targetX, int targetY) {
  std::vector<std::shared_ptr<Node>> pathList;
  std::shared_ptr<Node> startNodePtr = std::make_shared<Node>(startX, startY);
  Node targetNode = Node(targetX, targetY);
  std::vector<std::shared_ptr<Node>> endNodePtrs = CalcEndNodes(targetNode);
  
  // TODO: Compute the optimum endpoint with minimum cost
  for (auto endNodePtr : endNodePtrs) {
    pathList = CalcOptimumPath(startNodePtr, endNodePtr);
  }
  
  return pathList;
}

std::vector<std::shared_ptr<Node>> PathPlanner::CalcEndNodes(Node targetNode) {
  std::vector<std::shared_ptr<Node>> endNodePtrs;
  std::shared_ptr<Node> checkNodePtr = std::make_shared<Node>(targetNode.x + 1, targetNode.y);
  if (!CheckObstruction(checkNodePtr)) {
    endNodePtrs.emplace_back(checkNodePtr);
  }
  
  checkNodePtr = std::make_shared<Node>(targetNode.x - 1, targetNode.y);
  if (!CheckObstruction(checkNodePtr)) {
    endNodePtrs.emplace_back(checkNodePtr);
  }
  
  checkNodePtr = std::make_shared<Node>(targetNode.x, targetNode.y + 1);
  if (!CheckObstruction(checkNodePtr)) {
    endNodePtrs.emplace_back(checkNodePtr);
  }
  
  checkNodePtr = std::make_shared<Node>(targetNode.x, targetNode.y - 1);
  if (!CheckObstruction(checkNodePtr)) {
    endNodePtrs.emplace_back(checkNodePtr);
  }
  
  return endNodePtrs;
}

bool PathPlanner::CheckObstruction(std::shared_ptr<Node> checkNodePtr) {
  int targetX = checkNodePtr->x;
  int targetY = checkNodePtr->y;
  targetX = (targetX < 0) ? (targetX + grid_width) : targetX;
  targetX = (targetX >= grid_width) ? (targetX - grid_width) : targetX;
  targetY = (targetY < 0) ? (targetY + grid_height) : targetY;
  targetY = (targetY >= grid_height) ? (targetY - grid_height) : targetY;
  
  if (this->gamePtr->objectMap[targetX][targetY]->objectType == ObjectType::empty) {
    return false;
  }
  return true;
}
  
std::vector<std::shared_ptr<Node>> PathPlanner::CalcOptimumPath(std::shared_ptr<Node> startNodePtr, std::shared_ptr<Node> endNodePtr) {
  std::vector<std::shared_ptr<Node>> optimumPathList;
  optimumPathList.emplace_back(startNodePtr);
  openList.emplace_back(startNodePtr);
  nodeMap[startNodePtr->x][endNodePtr->y].isVisited = true;
  std::shared_ptr<Node> currentNodePtr = startNodePtr;
  while ((currentNodePtr->x != endNodePtr->x) && (currentNodePtr->y != endNodePtr->y)) { 
    AddNeighborNode(currentNodePtr, endNodePtr);
    currentNodePtr = NextNode();
  }
  optimumPathList = ConstructOptimumPath(currentNodePtr);
  return optimumPathList;
}

std::vector<std::shared_ptr<Node>> PathPlanner::ConstructOptimumPath(std::shared_ptr<Node> currentNodePtr) {
  std::vector<std::shared_ptr<Node>> optimumPathList;
  if (currentNodePtr->parentNodePtr != nullptr) {
    optimumPathList = ConstructOptimumPath(currentNodePtr->parentNodePtr);
  }
  optimumPathList.emplace_back(currentNodePtr);
  return optimumPathList;
}

void PathPlanner::AddNeighborNode(std::shared_ptr<Node> currentNodePtr, std::shared_ptr<Node> endNodePtr) {
  AddNeighborNodeSingleDirection(currentNodePtr, endNodePtr, -1, 0);
  AddNeighborNodeSingleDirection(currentNodePtr, endNodePtr, 1, 0);
  AddNeighborNodeSingleDirection(currentNodePtr, endNodePtr, 0, -1);
  AddNeighborNodeSingleDirection(currentNodePtr, endNodePtr, 0, 1);
}

void PathPlanner::AddNeighborNodeSingleDirection(std::shared_ptr<Node> currentNodePtr, std::shared_ptr<Node> endNodePtr, int deltaX, int deltaY) {
  int targetX = currentNodePtr->x + deltaX;
  int targetY = currentNodePtr->y + deltaY;
  targetX = (targetX < 0) ? (targetX + grid_width) : targetX;
  targetX = (targetX >= grid_width) ? (targetX - grid_width) : targetX;
  targetY = (targetY < 0) ? (targetY + grid_height) : targetY;
  targetY = (targetY >= grid_height) ? (targetY - grid_height) : targetY;
  
  int g_value = currentNodePtr->g_value + 1;
  int h_value = abs(endNodePtr->x - targetX) + abs(endNodePtr->y - targetY);
  std::shared_ptr<Node> checkNodeFormedPtr = std::make_shared<Node>(targetX, targetY, g_value, h_value, currentNodePtr);
  if ((!(nodeMap[targetX][targetY].isVisited)) && (!(CheckObstruction(checkNodeFormedPtr)))) {
    openList.emplace_back(checkNodeFormedPtr);
  }
}

bool PathPlanner::Compare(std::shared_ptr<Node> nodePtr1, std::shared_ptr<Node> nodePtr2) {
    return (nodePtr1->h_value + nodePtr1->g_value) > (nodePtr2->h_value + nodePtr2->g_value); 
}

std::shared_ptr<Node> PathPlanner::NextNode() {
  sort((&openList)->begin(), (&openList)->end(), Compare);
  std::shared_ptr<Node> nextNode = openList.back();
  openList.pop_back();
  return nextNode;
}

