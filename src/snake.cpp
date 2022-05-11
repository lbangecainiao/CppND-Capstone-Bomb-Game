#include <cmath>
#include <iostream>
#include <thread>
#include <algorithm>
#include "snake.h"
#include "enumerate.h"

Snake::~Snake() {
  std::for_each(_bombsThread.begin(), _bombsThread.end(), [](std::thread& t) {
    t.join();
  });
}

void Snake::SetGamePtr(Game* gamePtr) {
  this->gamePtr = gamePtr;
}

Game* Snake::GetGamePtr() {
  return this->gamePtr; 
}

void Snake::Update() {
  Direction direction = Direction::kStop;
  if (msgQueue.size() > 0) {
      direction = msgQueue.back();
      msgQueue.pop_back();
  }
  UpdateHead(direction);
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};
}

void Snake::DropBomb() {
  if ((alive) && (bombNum > 0)) {
    std::cout << "Bomb num before drop is" << bombNum << std::endl;
    std::shared_ptr<Bomb> b = std::make_shared<Bomb>(head_x, head_y, this);
    int bombX = static_cast<int>(head_x);
    int bombY = static_cast<int>(head_y);
    if (this->gamePtr->objectMap[bombX][bombY]->objectType != ObjectType::bomb) {
      std::cout << "dropped bomb x is" << bombX << std::endl;
      std::cout << "dropped bomb y is" << bombY << std::endl;
      this->gamePtr->objectMap[bombX][bombY]->objectType = ObjectType::bomb;
      std::cout << "object type in drop bomb is" << this->gamePtr->objectMap[bombX][bombY]->objectType << std::endl;
      bombs.emplace_back(b);
      std::cout << "Bomb num is" << bombs.size() << std::endl;
      _bombsThread.emplace_back(std::thread(&Bomb::TriggerBomb, b));

      bombNum -= 1;
      std::cout << "Bomb num after drop is" << bombNum << std::endl;
    }
  }
}

void Snake::RecoverBomb() {
     bombNum += 1;
}

bool Snake::CheckObstracle(float targetX, float targetY, float originX, float originY) {
  int xIndexTarget = static_cast<int>(targetX);
  int yIndexTarget = static_cast<int>(targetY);
  int xIndexOrigin = static_cast<int>(originX);
  int yIndexOrigin = static_cast<int>(originY);
  
  std::cout << "xIndexTarget is" << xIndexTarget << std::endl;
  std::cout << "yIndexTarget is" << yIndexTarget << std::endl;
  std::cout << "object type in check obstacle is" << this->gamePtr->objectMap[xIndexTarget][yIndexTarget]->objectType << std::endl;
    
  if (this->gamePtr->objectMap[xIndexTarget][yIndexTarget]->objectType == ObjectType::empty) {
    std::cout << "no obstacle blocked" << std::endl;
    gamePtr->MoveObjectPtr(xIndexTarget, yIndexTarget, xIndexOrigin, yIndexOrigin, player);
    return false;
  }
  std::cout << "snake is blocked by obstacle" << std::endl;
  return true;
}

void Snake::UpdateHead(Direction direction) {
  int targetHeadX;
  int targetHeadY;
  switch (direction) {
    case Direction::kUp:
      targetHeadX = static_cast<int>(head_x - speed);
      targetHeadX = (targetHeadX < 0) ? (targetHeadX + grid_height) : targetHeadX;
      if (!(CheckObstracle(targetHeadX, head_y, head_x, head_y))) {
        head_x -= speed;
      }
      direction = Direction::kStop;
      break;

    case Direction::kDown:
      targetHeadX = static_cast<int>(head_x + speed);
      targetHeadX %= grid_height;
      if (!(CheckObstracle(targetHeadX, head_y, head_x, head_y))) {
        head_x += speed;
      }
      direction = Direction::kStop;
      break;

    case Direction::kLeft:
      targetHeadY = static_cast<int>(head_y - speed);
      targetHeadY = (targetHeadY < 0) ? (targetHeadY + grid_width) : targetHeadY;
      if (!(CheckObstracle(head_x, targetHeadY, head_x, head_y))) {
        head_y -= speed;
      }
      direction = Direction::kStop;
      break;

    case Direction::kRight:
      targetHeadY = static_cast<int>(head_y + speed);
      targetHeadY %= grid_width;
      if (!(CheckObstracle(head_x, targetHeadY, head_x, head_y))) {
        head_y += speed;
      }
      direction = Direction::kStop;
      break;
    case Direction::kStop:
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
  
}

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}