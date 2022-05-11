#include <iostream>
#include "game.h"
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height) :
    snake1Ptr(std::make_shared<Snake>(grid_width, grid_height, Player::player1)),
    snake2Ptr(std::make_shared<Snake>(grid_width, grid_height, Player::player2)),
    engine(dev()),
    random_w(0, static_cast<int>(grid_width - 1)),
    random_h(0, static_cast<int>(grid_height - 1)),
    grid_width(grid_width),
    grid_height(grid_height),
    pathPlannerPtr(std::make_shared<PathPlanner>(grid_width, grid_height)) {
      // Choose game mode
      ChooseGameMode();
      
      // Initialize obstacle
      GenerateObstacle(grid_width, grid_height);
      
      // Initialize snake
      GenerateRandomSnakePosition(snake1Ptr);
      GenerateRandomSnakePosition(snake2Ptr);
    }      

Game::~Game() {}

void Game::ChooseGameMode() {
  while (true) {
    std::string gameMode;
    std::cout << "Please choose game mode, single player mode press 1, double player mode press 2" << std::endl;
    std::cin >> gameMode;
    if (gameMode == "1") {
      std::cout << "Start single player mode" << std::endl;
      this->gameMode = GameMode::singlePlayer;
      break;
    } else if (gameMode == "2") {
      std::cout << "Start double player mode" << std::endl;
      this->gameMode = GameMode::doublePlayer;
      break;
    }
  }
}

void Game::GenerateObstacle(std::size_t grid_width, std::size_t grid_height) {
  int i = 0;
  while (i < grid_height) {
    int j = 0;
    std::vector<std::shared_ptr<Object>> objectMapRow;
    while (j < grid_width) {
      objectMapRow.emplace_back(std::make_shared<Object>(ObjectType::empty));
      if ((i % 2) == 0) {
        objectMapRow.emplace_back(std::make_shared<Object>(ObjectType::obstacle));
        j++;
      }
      j++;
    }
    objectMap.emplace_back(objectMapRow);
    i++;
  }
}

void Game::GenerateRandomSnakePosition(std::shared_ptr<Snake> snakePtr) {
  while (true) {
    int x = random_w(engine);
    int y = random_h(engine);
    if (objectMap[x][y]->objectType == ObjectType::empty) {
      snakePtr->head_x = x;
      snakePtr->head_y = y;
      objectMap[x][y]->objectType == ObjectType::snake;
      objectMap[x][y]->snakePtr = snakePtr;
      break;
    }
  }
}

void Game::Run(Controller &&controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  snake1Ptr->SetGamePtr(this);
  snake2Ptr->SetGamePtr(this);
  pathPlannerPtr->SetGamePtr(this);
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  running = true;
  std::shared_ptr<Controller> controllerPtr(new Controller);
  _inputHandler.emplace_back(std::async(std::launch::async, &Controller::HandleInput, controllerPtr, std::move(running), snake1Ptr, snake2Ptr, gameMode));
  
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
        
    Update(this);
    renderer.Render(snake1Ptr, snake2Ptr);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  std::cout << "game exits" << std::endl;
}

void Game::MoveObjectPtr(int xIndexTarget, int yIndexTarget, int xIndexOrigin, int yIndexOrigin, Player player) {
  if (player == Player::player1) {
    objectMap[xIndexTarget][yIndexTarget]->snakePtr = snake1Ptr;
    objectMap[xIndexOrigin][yIndexOrigin]->snakePtr = nullptr;
    objectMap[xIndexTarget][yIndexTarget]->objectType = ObjectType::snake;
    if (objectMap[xIndexOrigin][yIndexOrigin]->objectType == ObjectType::snake) {
      objectMap[xIndexOrigin][yIndexOrigin] = std::make_shared<Object>(ObjectType::empty);
    }
  } else if (player == Player::player2) {
    objectMap[xIndexTarget][yIndexTarget]->snakePtr = snake2Ptr;
    objectMap[xIndexOrigin][yIndexOrigin]->snakePtr = nullptr;
    objectMap[xIndexTarget][yIndexTarget]->objectType = ObjectType::snake;
    if (objectMap[xIndexOrigin][yIndexOrigin]->objectType == ObjectType::snake) {
      objectMap[xIndexOrigin][yIndexOrigin] = std::make_shared<Object>(ObjectType::empty);
    }
  }
}

void Game::Update(Game* gamePtr) {
  if (snake1Ptr->alive) {
    snake1Ptr->Update();
  }
  if (this->gameMode == GameMode::singlePlayer) {
    if ((snake1Ptr->head_x != lastHeadX) || (snake1Ptr->head_y != lastHeadY)) {
      PathPlan();
      lastHeadX = snake1Ptr->head_x;
      lastHeadY = snake1Ptr->head_y;
    }
    MoveSnake();
  }
  if (snake2Ptr->alive) {
    snake2Ptr->Update();
  }
}

void Game::PathPlan() {
    pathStep = 1;
    path = pathPlannerPtr->CalcPath(snake2Ptr->head_x, snake2Ptr->head_y, snake1Ptr->head_x, snake1Ptr->head_y);
    for (auto nodePtr : path) {
      std::cout << "Node.x is" << nodePtr->x << std::endl;
      std::cout << "Node.y is" << nodePtr->y << std::endl;
    }
}

void Game::MoveSnake() {
  int headX = static_cast<int>(snake2Ptr->head_x);
  int headY = static_cast<int>(snake2Ptr->head_y);
  if(!CheckExplodeRange(headX, headY)) {
    if (pathStep == path.size()) {
      return;  
    }
    
    int deltaX = path[pathStep]->x - snake2Ptr->head_x;
    int deltaY = path[pathStep]->y - snake2Ptr->head_y;
    Direction direction;
    if ((deltaX == -1) && (deltaY == 0)) {
      direction = Direction::kUp; 
    }
    
    if ((deltaX == 1) && (deltaY == 0)) {
      direction = Direction::kDown; 
    }
    
    if ((deltaX == 0) && (deltaY == -1)) {
      direction = Direction::kLeft; 
    }
    
    if ((deltaX == 0) && (deltaY == 1)) {
      direction = Direction::kRight; 
    }
    snake2Ptr->msgQueue.emplace_back(direction);
    pathStep += 1;
    return;
  }
  
  int deltaX = -1;
  int deltaY = 0;
  std::vector<Direction> listDangerDirection;
  
  if (CheckPossibleDirection(0, -1, Direction::kLeft, listDangerDirection)) {
    return;
  }
  
  if (CheckPossibleDirection(0, 1, Direction::kRight, listDangerDirection)) {
    return;
  }
  
  if (CheckPossibleDirection(1, 0, Direction::kDown, listDangerDirection)) {
    return;
  }
  
  if (CheckPossibleDirection(-1, 0, Direction::kUp, listDangerDirection)) {
    return;
  }
  std::cout << "No possible direction" << std::endl;
  std::cout << "Danger direction is" << listDangerDirection.back() << std::endl;
  snake2Ptr->msgQueue.emplace_back(listDangerDirection.back());
  listDangerDirection.clear();
}

bool Game::CheckPossibleDirection(int deltaX, int deltaY, Direction direction, std::vector<Direction>& listDangerDirection) {
  int headX = static_cast<int>(snake2Ptr->head_x);
  int headY = static_cast<int>(snake2Ptr->head_y);
  
  bool isPossibleDirectionExist = false;
  int targetX = headX + deltaX;
  int targetY = headY + deltaY;
  targetX = (targetX < 0) ? (targetX + grid_width) : targetX;
  targetX = (targetX >= grid_width) ? (targetX - grid_width) : targetX;
  targetY = (targetY < 0) ? (targetY + grid_height) : targetY;
  targetY = (targetY >= grid_height) ? (targetY - grid_height) : targetY;
  
  if (objectMap[targetX][targetY]->objectType == ObjectType::empty) {
    bool isInBombRange = CheckExplodeRange(targetX, targetY);
    if (!(isInBombRange)) {
      snake2Ptr->msgQueue.emplace_back(direction);
      isPossibleDirectionExist = true;
    } else {
      std::cout << "Object type in danger direction is" << objectMap[targetX][targetY]->objectType << std::endl;
      listDangerDirection.emplace_back(direction);
    }
  }
  return isPossibleDirectionExist;
}

bool Game::CheckExplodeRange(int x, int y) {
  bool isInExplodeRange = false;
  if (CheckExplodeRangeSingleDirection(x, y, -1, 0)) {
    isInExplodeRange = true;  
  }
  
  if (CheckExplodeRangeSingleDirection(x, y, 1, 0)) {
    isInExplodeRange = true;
  }
  
  if (CheckExplodeRangeSingleDirection(x, y, 0, -1)) {
    isInExplodeRange = true;
  }
  
  if (CheckExplodeRangeSingleDirection(x, y, 0, 1)) {
    isInExplodeRange = true;
  }
  return isInExplodeRange;
}

bool Game::CheckExplodeRangeSingleDirection(int x, int y, int deltaX, int deltaY) {
  bool isInExplodeRange = false;
  int explodeLength = snake2Ptr->explodeLength;
  int length = 1;
  while (length <= explodeLength) {
    int targetX = x + (deltaX * length);
    int targetY = y + (deltaY * length);
    targetX = (targetX < 0) ? (targetX + grid_width) : targetX;
    targetX = (targetX >= grid_width) ? (targetX - grid_width) : targetX;
    targetY = (targetY < 0) ? (targetY + grid_height) : targetY;
    targetY = (targetY >= grid_height) ? (targetY - grid_height) : targetY;
    
    if (objectMap[targetX][targetY]->objectType == ObjectType::bomb) {
      isInExplodeRange = true;
      break;
    } else if (objectMap[targetX][targetY]->objectType == ObjectType::obstacle) {
      break;
    }
    length++;
  }
  return isInExplodeRange;
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake1Ptr->size; }