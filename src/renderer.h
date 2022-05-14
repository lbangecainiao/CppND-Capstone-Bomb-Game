#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include <string>
#include "SDL.h"
#include "snake.h"
#include "game.h"

class Game;

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height, std::shared_ptr<Game> gamePtr);
  ~Renderer();

  void Render(std::shared_ptr<Snake> snake1Ptr, std::shared_ptr<Snake> snake2Ptr);
  void RenderSinglePlayer(std::shared_ptr<Snake> snake);
  void RenderExplosionSingleDirection(int bombX, int bombY, int deltaX, int deltaY, int explodeLength);
  void RenderObstacle();
  void UpdateWindowTitle(std::string gameInfo, int fps);
  std::shared_ptr<Game> gamePtr;

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif