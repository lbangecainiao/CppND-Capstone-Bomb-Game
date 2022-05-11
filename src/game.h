#ifndef GAME_H
#define GAME_H

#include <random>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <algorithm>
#include <memory>
#include <string>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "object.h"
#include "snake.h"
#include "enumerate.h"
#include "pathPlanner.h"

class Controller;
class Renderer;
class Object;
class PathPlanner;
class Node;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller &&controller, Renderer &renderer,
           std::size_t target_frame_duration);
  void MoveObjectPtr(int xIndexTarget, int yIndexTarget, int xIndexOrigin, int yIndexOrigin, Player player);
  int GetScore() const;
  int GetSize() const;
  std::vector<std::vector<std::shared_ptr<Object>>> objectMap;
  std::shared_ptr<Snake> snake1Ptr;
  std::shared_ptr<Snake> snake2Ptr;
  bool running;

 private:
  void MoveSnake();
  void PathPlan();
  void ChooseGameMode();
  bool CheckPossibleDirection(int deltaX, int deltaY, Direction direction, std::vector<Direction>& listDangerDirection);
  bool CheckExplodeRange(int targetX, int targetY);
  bool CheckExplodeRangeSingleDirection(int x, int y, int deltaX, int deltaY);
  void GenerateObstacle(std::size_t grid_width, std::size_t grid_height);
  void GenerateRandomSnakePosition(std::shared_ptr<Snake> snakePtr);
  SDL_Point food;
  
  std::size_t grid_width;
  std::size_t grid_height;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  
  std::vector<std::future<void>> _inputHandler;
  GameMode gameMode;
  std::shared_ptr<PathPlanner> pathPlannerPtr;

  int score{0};
  int pathStep;
  int lastHeadX = 0;
  int lastHeadY = 0;
  std::vector<std::shared_ptr<Node>> path;

  void Update(Game* gamePtr);
};

#endif