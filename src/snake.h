#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <queue>
#include <thread>
#include <iostream>
#include <memory>
#include "SDL.h"
#include "bomb.h"
#include "object.h"
#include "game.h"
#include "enumerate.h"

class Game;
class Bomb;

class Snake {
 public:
  Snake(int grid_width, int grid_height, Player player)
      : grid_width(grid_width),
        grid_height(grid_height),
        player(player) {
          objectType = ObjectType::snake;
        }   
  Snake(Snake &&) = default;
  ~Snake();

  std::shared_ptr<Snake> snakeSharedPtr;
  Game* gamePtr;
  float speed{1.0f};
  int size{1};
  int explodeLength = 4;
  bool alive{true};
  float head_x;
  float head_y;
  Player player;
  std::vector<SDL_Point> body;
  std::vector<std::shared_ptr<Bomb>> bombs;
  int bombNum = 6;
  std::vector<std::thread> _bombsThread;
  ObjectType objectType;
  std::vector<Direction> msgQueue;
  int grid_width;
  int grid_height;
  
  void SetGamePtr(Game* gamePtr);
  Game* GetGamePtr();
  void Update();
  void DropBomb();
  void RecoverBomb(); 
  bool SnakeCell(int x, int y);
  bool CheckObstracle(float targetX, float targetY, float originX, float originY);

 private:
  void UpdateHead(Direction direction);
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  bool growing{false};
};

#endif