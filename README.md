# CPPND: Capstone Bomb game

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

## Project introduction

The game is generally a player vs player game. Each player controller their own block. The block can be moved toward four directions, up, down, left, right. And also, they can controller the block to drop a bomb. The bomb will explode two secend after been dropped and will destroy the obstacle or players within a certain range.

The map is initialized with obstacles and the positions of the players will be initialized randomly. The goal of the game is to use bomt to destroy the other player.

The player1 can move the block using w,a,s,d  key and drop the bomb using space key. The player2 can move the block using up, down, left, right key and drop the bomb using the l key.

Apart fromt the p vs p mode, the game also support single player mode. The player can control the block to move and drop bomb to destroy the computer, while the computer can also move to dodge the explosion of the bomb dropped by the player.


## File structures

Like the structure of other game projects, the file consists of the controller, the renderer and the class of the elements of the game.

The class snake describe the block controller by the player. It can be moved towards four directions, and can drop the bomb. 

The behavior of the bomb is described in the bomb class. It has the method destroy and explode, once the bomb is initialized, a thread will be created, and after two seconed, the method explode and destroy are called. The destroy method will check the obstacle in four directions within a certain range, if there is an obstacle existing in the range, it will destroy it.

The snake and bomb class both inherit from the object class. The the enumerate objectType will distinguish the snake, bomb, and obstacle.

The controller is also a thread working parallel with the main thread, so that it can monitor the state of the key board,and the two player can control their own block withour any delay.

The path planner class contains a path planner algorithm which will guide the computer to approach the player.

## Rubric checklists:

1.A variety of control structures are used in the project.
For example in game.cpp GenerateObstacle function a double while loop is used to generate the obstacle map.

2.The project accepts input from a user as part of the necessary operation of the program.
In controller.cpp in function HandleInput the user input are used to control the behavior of the block.

3.The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
The game elements are described with several different classes: bomb, snake, object, etc.

4.All class members that are set to argument values are initialized through member initialization lists.
The bomb, snake, game classes are all initialized with initialization lists.

5.The project uses multiple threads in the execution.
In snake.cpp function DropBomb, the drop is initialized as a thread without any interruptions with the snake thread.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
