#include <queue>
#include <iostream>
#include <array>

enum Direction {
    UP, DOWN, RIGHT, LEFT;

}

class Cell {
    public:
        
        Cell (int x, int y) {
            this.x = x;
            this.y = y;
        }

        int x;
        int y;

        bool upWall, downWall, rightWall, leftWall = false;

}

class Maze {
    public:
        Cell cellMap[6,6];
        
        array<array<bool, 4>, 6> noWall = {true};
}

class Robot {
    public:
        int x = 5;
        int y = 5;

        Direction direc = UP;
        
}




// 0,0 top left
//
//[(0,0),(1,0),(2,0),(3,0),(4,0)]
//[(0,1),(1,1),(2,1),(3,1),(4,1)]

// start = 5,5
// end: 1,1

//6x 6 grid