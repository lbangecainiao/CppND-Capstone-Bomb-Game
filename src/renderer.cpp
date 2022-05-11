#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height, std::shared_ptr<Game> gamePtr)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height),
      gamePtr(gamePtr) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(std::shared_ptr<Snake> snake1Ptr, std::shared_ptr<Snake> snake2Ptr) {
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);
  
  RenderObstacle();
  RenderSinglePlayer(snake1Ptr);
  RenderSinglePlayer(snake2Ptr);
  
  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::RenderObstacle() {
  SDL_Rect block;
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xCC);
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  int i = 0;
  while (i < grid_height) {
    int j = 0;
    while (j < grid_width) {
      if (gamePtr->objectMap[i][j]->objectType == ObjectType::obstacle) {
        block.y = i * block.w;
        block.x = j * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
      }
      j++;
    }
    i++;
  }
}

void Renderer::RenderSinglePlayer(std::shared_ptr<Snake> snakePtr) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Render snake's head
  block.y = static_cast<int>(snakePtr->head_x) * block.w;
  block.x = static_cast<int>(snakePtr->head_y) * block.h;
  if (snakePtr->alive) {
    if (snakePtr->player == Player::player1) {
      SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
    } else if (snakePtr->player == Player::player2) {
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x7A, 0xCC, 0xFF);
    }
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
  
  // Render bombs
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xA5, 0x00, 0xFF);
  int iter = 0;
  while (iter < snakePtr->bombs.size()) {
      std::shared_ptr<Bomb> bomb_ptr = snakePtr->bombs[iter];
      iter++;
      if (bomb_ptr->isExploded == true) {
        continue;
      }
      if (bomb_ptr->isExploding == true) {
        int length = 1;
        int bombX = static_cast<int>(bomb_ptr->x);
        int bombY = static_cast<int>(bomb_ptr->y);
        int explodeLength = snakePtr->explodeLength;
        RenderExplosionSingleDirection(bombX, bombY, 1, 0, explodeLength);
        RenderExplosionSingleDirection(bombX, bombY, -1, 0, explodeLength);
        RenderExplosionSingleDirection(bombX, bombY, 0, 1, explodeLength);
        RenderExplosionSingleDirection(bombX, bombY, 0, -1, explodeLength);
        continue;
      }    
          
    block.y = static_cast<int>(bomb_ptr->x) * block.w;
    block.x = static_cast<int>(bomb_ptr->y) * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }
}

void Renderer::RenderExplosionSingleDirection(int bombX, int bombY, int deltaX, int deltaY, int explodeLength) {
  SDL_Rect block;
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xA5, 0x00, 0xFF);
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  
  int length = 1;
  while (length <= explodeLength) {
    int targetX = bombX + (deltaX * length);
    int targetY = bombY + (deltaY * length);
    targetX = (targetX < 0) ? (targetX + grid_width) : targetX;
    targetX = (targetX >= grid_width) ? (targetX - grid_width) : targetX;
    targetY = (targetY < 0) ? (targetY + grid_height) : targetY;
    targetY = (targetY >= grid_height) ? (targetY - grid_height) : targetY;
    block.y = targetX * block.w;
    block.x = targetY * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
    if (gamePtr->objectMap[targetX][targetY]->objectType == ObjectType::obstacle) {
      break;
    }
    length++;
  }
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
