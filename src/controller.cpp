#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::SetControllerGameMode(GameMode gameMode) {
  this->gameMode = gameMode;
  std::cout << "controller mode is" << gameMode << std::endl;
}

void Controller::ChangeDirection(std::shared_ptr<Snake> snakePtr, Direction input) const {
  snakePtr->msgQueue.emplace_back(input);  
  return;
}

void Controller::DropBomb(std::shared_ptr<Snake> snakePtr) const {
  snakePtr->DropBomb();
  return;
}

void Controller::HandleInput(bool &&running, std::shared_ptr<Snake> snake1Ptr, std::shared_ptr<Snake> snake2Ptr,     GameMode gameMode) {
  std::cout << "gameMode in controller is" << gameMode << std::endl;

  while (running) {
    SDL_Event e;
    int wCounter = 0;
    int sCounter = 0;
    int aCounter = 0;
    int dCounter = 0;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
        snake1Ptr->gamePtr->running = false;
      }
      
      const Uint8* state = SDL_GetKeyboardState(NULL);
      if (state[SDL_SCANCODE_W]) {
        if (wCounter == 1) {
          ChangeDirection(snake1Ptr, Direction::kUp);
          wCounter = 0;
        }
        wCounter++;
      }
      
      if (state[SDL_SCANCODE_S]) {
        if (sCounter == 1) {
          ChangeDirection(snake1Ptr, Direction::kDown);
          sCounter = 0;
        }
        sCounter++;
      }
      
      if (state[SDL_SCANCODE_A]) {
        if (aCounter == 1) {
          ChangeDirection(snake1Ptr, Direction::kLeft);
          aCounter = 0;
        }
        aCounter++;
      }
      
      if (state[SDL_SCANCODE_D]) {
        if (dCounter == 1) {
          ChangeDirection(snake1Ptr, Direction::kRight);
          dCounter = 0;
        }
        dCounter++;
      }
      
      if (state[SDL_SCANCODE_SPACE]) {
        DropBomb(snake1Ptr);
      }
      if (gameMode == GameMode::doublePlayer) {
        if (state[SDL_SCANCODE_UP]) {
          ChangeDirection(snake2Ptr, Direction::kUp);
          std::cout << "press up" << std::endl;
        }

        if (state[SDL_SCANCODE_DOWN]) {
          ChangeDirection(snake2Ptr, Direction::kDown);
          std::cout << "press right" << std::endl;
        }

        if (state[SDL_SCANCODE_LEFT]) {
          ChangeDirection(snake2Ptr, Direction::kLeft);
          std::cout << "press right" << std::endl;
        }

        if (state[SDL_SCANCODE_RIGHT]) {
          ChangeDirection(snake2Ptr, Direction::kRight);
          std::cout << "press right" << std::endl;
        }

        if (state[SDL_SCANCODE_L]) {
          DropBomb(snake2Ptr);
        }
      }
    }
  }
  std::cout << "handle input exits" << std::endl;
}