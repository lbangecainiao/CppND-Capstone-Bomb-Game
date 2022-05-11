cmake_minimum_required(VERSION 3.11.3)
# set(CMAKE_CXX_STANDARD 17)
project(SDL2Test)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

find_package(OpenCV 4.1 REQUIRED)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_definitions(-std=c++17)

# set(CXX_FLAGS "-Wall")




set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src)

add_executable(BombGame src/main.cpp src/game.cpp src/controller.cpp src/renderer.cpp src/snake.cpp src/bomb.cpp src/object.cpp src/pathPlanner.cpp)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(BombGame ${SDL2_LIBRARIES})
