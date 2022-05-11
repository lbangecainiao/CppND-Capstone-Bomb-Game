#include <iostream>
#include <memory>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Controller controller;
  std::shared_ptr<Game> gamePtr = std::make_shared<Game>(kGridWidth, kGridHeight);
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight, gamePtr);
  gamePtr->Run(std::move(controller), renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " <<gamePtr->GetScore() << "\n";
  std::cout << "Size: " << gamePtr->GetSize() << "\n";
  return 0;
}