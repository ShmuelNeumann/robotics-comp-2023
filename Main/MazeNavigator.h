//# include "Arduino.h"

class Maze;
class Robot;
class Cell;

enum Direction;
enum Colour;
enum WallType;

void initMaze(Maze maze);
void PerformNextTurn(Direction currentDirection, Maze maze, Robot robot);
void RemoveColourWalls(Colour toRemove);
void SetMazeStartAndEnd(int startX, int startY, int endX, int endY, Maze maze);
void NavigateMaze(int startX, int startY, int endX, int endY, bool reset, Maze maze, Robot robot);
void TurnAnticlockwise(Robot robot);
void TurnClockwise(Robot robot);
bool ScanForWall();
void MoveForward(Robot robot);
Colour ReadColour();
void UpdateDistances (Maze maze);
void UpdateCellDist(int distance, int x, int y, Maze maze);
Cell GetCell(Cell cell, Direction direction, Maze maze);
Direction GetClockwise(Direction direction);
Direction GetAnticlockwise(Direction direction);
void SetWall(Direction direction, Cell cell, Maze maze);
WallType GetWall(int x, int y, Direction direction, Maze maze);