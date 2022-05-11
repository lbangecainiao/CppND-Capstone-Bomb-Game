#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include "enumerate.h"

class Snake;

class Controller {
 public:
  void HandleInput(bool &&running, std::shared_ptr<Snake> snake1Ptr, std::shared_ptr<Snake> snake2Ptr, GameMode gameMode);
  void SetControllerGameMode(GameMode gameMode);

 private:
  GameMode gameMode;
  void ChangeDirection(std::shared_ptr<Snake> snakePtr, Direction input) const;
  void DropBomb(std::shared_ptr<Snake> snake) const;
  
};

#endif