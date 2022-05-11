#ifndef BOMB_H
#define BOMB_H

#include <vector>
#include <memory>
#include <thread>
#include <iostream>
#include "snake.h"
#include "object.h"
#include "game.h"
#include "enumerate.h"


class Snake;
class Game;

class Bomb {
  public:
    Bomb(float x, float y, Snake* snake) : x(x), y(y), snake(snake) {
      objectType = ObjectType::bomb;
      std::cout << "Bomb is dropped" << std::endl;
    }
    ~Bomb() { std::cout << "The bomb is exploded" << std::endl; }
    float x;
    float y;
    bool isExploded = false;
    bool isExploding = false;
    ObjectType objectType;
    Snake* snake;
    Game* gamePtr;
    
    void TriggerBomb();
    void Explode();
    void Destroy();
    void DestroyObject(Game* gamePtr);
    void DestroySingleDirectionObstacle(int deltaX, int deltaY, Game* gamePtr);
    void DestroySnake(std::shared_ptr<Snake> snakePtr);
};

#endif