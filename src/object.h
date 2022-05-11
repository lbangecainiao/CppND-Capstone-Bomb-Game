#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include "enumerate.h"
#include "snake.h"

class Snake;

class Object {
  public:
    Object();
    Object(ObjectType);
    ObjectType objectType;
    std::shared_ptr<Snake> snakePtr;
};

#endif
