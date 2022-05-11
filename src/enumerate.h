#ifndef ENUMERATE_H
#define ENUMERATE_H

enum Direction { kUp = 1, kDown = 2, kLeft = 3, kRight = 4, kStop = 0 };
enum ObjectType { empty = 0, bomb = 1, obstacle = 2, snake = 3 };
enum Player { player1 = 0, player2 = 1};
enum GameMode { singlePlayer = 0, doublePlayer = 1};

#endif