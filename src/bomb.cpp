#include "bomb.h"
#include <chrono>

void Bomb::TriggerBomb() {
   std::this_thread::sleep_for(std::chrono::milliseconds(2000));
   Explode();
}

void Bomb::Explode() {
  isExploding = true;
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  isExploded = true;
  Destroy();
  std::cout << "snake address in bomb is " << snake << std::endl;
  snake->RecoverBomb();
}

void Bomb::Destroy() {
  gamePtr = snake->GetGamePtr();
  DestroyObject(gamePtr);
}

void Bomb::DestroyObject(Game* gamePtr) {
  DestroySingleDirectionObstacle(1, 0, gamePtr);
  DestroySingleDirectionObstacle(-1, 0, gamePtr);
  DestroySingleDirectionObstacle(0, 1, gamePtr);
  DestroySingleDirectionObstacle(0, -1, gamePtr);
}

void Bomb::DestroySingleDirectionObstacle(int deltaX, int deltaY, Game* gamePtr) {
  int length = 0;
  bool isExplosionStop = false;
  while ((length < snake->explodeLength) && !(isExplosionStop)) {
    int checkX = static_cast<int>(x) + deltaX * length;
    int checkY = static_cast<int>(y) + deltaY * length;
    checkX = (checkX < 0) ? (checkX + snake->grid_width) : checkX;
    checkX = (checkX >= snake->grid_width) ? (checkX - snake->grid_width) : checkX;
    checkY = (checkY < 0) ? (checkY + snake->grid_height) : checkY;
    checkY = (checkY >= snake->grid_height) ? (checkY - snake->grid_height) : checkY;
    if (gamePtr->objectMap[checkX][checkY]->objectType == ObjectType::obstacle) {
      gamePtr->objectMap[checkX][checkY]->objectType = ObjectType::empty;
      std::cout << "Explosion stopped" << std::endl;
      isExplosionStop = true;
      break;
    } else if (gamePtr->objectMap[checkX][checkY]->snakePtr != nullptr) {
      std::cout << "Snake destroyed" << std::endl;
      gamePtr->objectMap[checkX][checkY]->snakePtr->alive = false;
      break;
    }
    length++;
  }
}
