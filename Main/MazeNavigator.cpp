// #include <queue>
// #include <iostream>
// #include <array>


//ultrasonic setup
#include <NewPing.h> 
#define FORWARD_TRIGGER_PIN A4
#define FORWARD_ECHO_PIN A5
#define MAX_DISTANCE 200
NewPing forwardSonar(FORWARD_TRIGGER_PIN, FORWARD_ECHO_PIN, MAX_DISTANCE);

#define RIGHT_TRIGGER_PIN  A3
#define RIGHT_ECHO_PIN     A2
NewPing rightSonar(RIGHT_TRIGGER_PIN, RIGHT_ECHO_PIN, MAX_DISTANCE);

#define LEFT_TRIGGER_PIN  2
#define LEFT_ECHO_PIN     4
NewPing leftSonar(LEFT_TRIGGER_PIN, LEFT_ECHO_PIN, MAX_DISTANCE);


//motor setup
#include "MotorControl.h"


using namespace std;

enum Direction {UP, DOWN, RIGHT, LEFT};
enum Colour {UNKNOWN, BW, RED, OTHER};
enum WallType{MAYBEWALL, NOWALL, YESWALL};
//queue<string> cur_dir;

class Cell {
    public:
        
        Cell () {}
        int x;
        int y;

        int distance;

        Colour colour = UNKNOWN;

        WallType upWall, downWall, rightWall, leftWall = MAYBEWALL;
    
};

class Maze {
    public:
        Maze () {}
        Cell startCell;
        Cell endCell;

        Cell cellMap[6][6];

};

class Robot {
    public:
        Robot () {}

        int x = 5;
        int y = 5;

        Direction direc = UP;
};

Robot robot;
Maze maze;

// pre initialised to avoid stupid errors
void initMaze();
void PerformNextTurn(Direction currentDirection);
void RemoveColourWalls(Colour toRemove);
void SetMazeStartAndEnd(int startX, int startY, int endX, int endY);
void NavigateMaze(int startX, int startY, int endX, int endY, bool reset);
void TurnAnticlockwise();
void TurnClockwise();
bool ScanForWall();
void MoveForward();
Colour ReadColour();
void UpdateDistances();
void UpdateCellDist(int distance, int x, int y);
Cell GetCell(Cell cell, Direction direction);
Direction GetClockwise(Direction direction);
Direction GetAnticlockwise(Direction direction);
void SetWall(Direction direction, int x, int y);
WallType GetWall(int x, int y, Direction direction);
Colour get_colour();


// globals

void initMaze() {
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            Cell cell;
            cell.x = i;
            cell.y = j;

            maze.cellMap[i][j] = cell;
            
            if (i == 0) {   
                SetWall(LEFT, cell.x, cell.y);
            } if (i == 5) {
                SetWall(RIGHT, cell.x, cell.y);
            } if (j == 0) {
                SetWall(UP, cell.x, cell.y);
            } if (j == 5) {
                SetWall(DOWN, cell.x, cell.y);
            }

        }
    }    
};

void PerformNextTurn(Direction currentDirection) {
    //cout << "-------------------------------\n";

    //cout << "Performing Next turn:\n";

    UpdateDistances();

    int thisDist = maze.cellMap[robot.x][robot.y].distance;

    int temp = GetCell(maze.cellMap[robot.x][robot.y], currentDirection).distance;

    //cout << "Current location is: x = " << robot.x << ", y = " << robot.y << "\n";

    //cout << "Currently Facing " << cur_dir.front() << "\n";

    //cout << "distance of target: " << temp << "\n";

    if (GetWall(robot.x, robot.y, robot.direc) != YESWALL && GetCell(maze.cellMap[robot.x][robot.y], currentDirection).distance == thisDist-1) {
        //cout << "Decided to move forward\n";
        return;
    }
    else if (GetWall(robot.x, robot.y, GetClockwise(robot.direc)) != YESWALL && GetCell(maze.cellMap[robot.x][robot.y], GetClockwise(currentDirection)).distance == thisDist-1) {
        //cout << "Decided to turn clockwise\n";
        robot.direc = GetClockwise(currentDirection);
        TurnClockwise();
    }
    else if (GetWall(robot.x, robot.y, GetAnticlockwise(robot.direc)) != YESWALL && GetCell(maze.cellMap[robot.x][robot.y], GetAnticlockwise(currentDirection)).distance == thisDist-1) {
        //cout << "Decided to turn anticlockwise\n";

        robot.direc = GetAnticlockwise(currentDirection);
        TurnAnticlockwise();
    }
    else {
        //cout << "Decided to turn clockwise twice\n";

        robot.direc = GetClockwise(GetClockwise(currentDirection));

        TurnClockwise();
        TurnClockwise();

    }

}

void RemoveColourWalls(Colour toRemove) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (maze.cellMap[i][j].colour == OTHER) {
                if (i > 0) {
                    maze.cellMap[i][j].leftWall = MAYBEWALL;
                    maze.cellMap[i-1][j].rightWall = MAYBEWALL;
                }
                if (i < 5) {
                    maze.cellMap[i][j].rightWall = MAYBEWALL;
                    maze.cellMap[i+1][j].leftWall = MAYBEWALL;
                }
                if (j > 0) {
                    maze.cellMap[i][j].upWall = MAYBEWALL;
                    maze.cellMap[i][j-1].downWall = MAYBEWALL;
                }
                if (j < 5) {
                    maze.cellMap[i][j].downWall = MAYBEWALL;
                    maze.cellMap[i][j+1].upWall = MAYBEWALL;
                }
            }
        }
    }
}

void SetMazeStartAndEnd(int startX, int startY, int endX, int endY) {
    maze.startCell = maze.cellMap[startX][startY];


    maze.endCell = maze.cellMap[endX][endY];

}

void Scan() {

    if (rightSonar.ping_cm() < 20) {
        SetWall(GetClockwise(robot.direc), robot.x, robot.y);
    }
    if (leftSonar.ping_cm() < 20) {
        SetWall(GetAnticlockwise(robot.direc), robot.x, robot.y);
    }
    if (forwardSonar.ping_cm() < 20) {
        SetWall(robot.direc, robot.x, robot.y);

    }

}


void NavigateMaze(int startX, int startY, int endX, int endY, bool reset, bool setRobotToStart) {
    if (reset) {
        initMaze();
    }

    if (setRobotToStart) {
        robot.x = startX;
        robot.y = startY;
    }
    SetMazeStartAndEnd(startX, startY, endX, endY);

    while(robot.x != maze.endCell.x || robot.y != maze.endCell.y) {

        maze.cellMap[robot.x][robot.y].colour = get_colour();
    
        Scan();
        PerformNextTurn(robot.direc);
        
        int x = GetCell(maze.cellMap[robot.x][robot.y], robot.direc).x;
        int y = GetCell(maze.cellMap[robot.x][robot.y], robot.direc).y;

        robot.x = x;
        robot.y = y;
        MoveForward();


    }
}

void TurnAnticlockwise() {
    // something like this, needs calibration
    SetMotor(-0.5, 0.5);
    delay(500);
    SetMotor(0, 0);
    delay(10);


    //cur_dir.push(cur_dir.front()); cur_dir.pop(); cur_dir.push(cur_dir.front()); cur_dir.pop(); cur_dir.push(cur_dir.front()); cur_dir.pop();
    //cout << "Turning anticlockwise\n";
    //cout << "Now Facing " << cur_dir.front() << "\n";
}
void TurnClockwise() {
    // something like this, needs calibration
    SetMotor(0.5, -0.5);
    delay(500);
    SetMotor(0, 0);
    delay(10);

    //  cur_dir.push(cur_dir.front()); cur_dir.pop();
    //cout << "Turning clockwise\n";
    //cout << "Now Facing " << cur_dir.front() << "\n";
}

void MoveForward() {
    // something like this, needs calibration
    SetMotor(1, 1);
    delay(500);
    SetMotor(0, 0);
    delay(10);


    //cout << "current location is: x = " << robot.x << ", y = " << robot.y << "\n";
    //cout << "Actually moving forward\n";

}


Colour ReadColour() {
    return RED;
}

void UpdateDistances () {
    // clear all distances
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            maze.cellMap[i][j].distance = 100;
        }
    }

    // call the recursive flood fill function
    UpdateCellDist(0, maze.endCell.x, maze.endCell.y);

    // for (int i = 0; i < 6; i++) {
    //     for (int j = 0; j < 6; j++) {
    //         cout << maze.cellMap[i][j].distance << " ";
    //     }
    //     cout << "\n";
    // }

}

void UpdateCellDist(int distance, int x, int y) {
    // cout << "UpdateCellDist() Called\n";
    maze.cellMap[x][y].distance = distance;
    
    if (y > 0 && maze.cellMap[x][y].upWall != YESWALL && maze.cellMap[x][y-1].distance > distance + 1) {
        UpdateCellDist(distance + 1, x, y-1);
    }
    if (y < 5 && maze.cellMap[x][y].downWall != YESWALL && maze.cellMap[x][y+1].distance > distance + 1) {
        UpdateCellDist(distance + 1, x, y+1);
    }
    if (x < 5 && maze.cellMap[x][y].rightWall != YESWALL && maze.cellMap[x+1][y].distance > distance + 1) {
        UpdateCellDist(distance + 1, x + 1, y);
    }
    if (x > 0 && maze.cellMap[x][y].leftWall != YESWALL && maze.cellMap[x-1][y].distance > distance + 1) {
        UpdateCellDist(distance + 1, x - 1, y);
    } 
}

// Does not account for running off array edges
Cell GetCell(Cell cell, Direction direction) {
    if (direction == UP) {return maze.cellMap[cell.x][cell.y - 1];}
    if (direction == DOWN) {return maze.cellMap[cell.x][cell.y + 1];}
    if (direction == RIGHT) {return maze.cellMap[cell.x + 1][cell.y];}
    else {return maze.cellMap[cell.x - 1][cell.y];}
}

Direction GetClockwise(Direction direction) {
    // returns the clockwise of this direction
    if (direction == UP) {return RIGHT;}
    if (direction == DOWN) {return LEFT;}
    if (direction == RIGHT) {return DOWN;}
    else {return UP;}
}
Direction GetAnticlockwise(Direction direction) {
    // returns the clockwise of this direction
    if (direction == UP) {return LEFT;}
    if (direction == DOWN) {return RIGHT;}
    if (direction == RIGHT) {return UP;}
    else {return DOWN;}
}

void SetWall(Direction direction, int x, int y) {
    if (direction == UP) {
        maze.cellMap[x][y].upWall = YESWALL;

        if (y != 0) {
            maze.cellMap[x][y-1].downWall = YESWALL;
        }
    }
    else if (direction == DOWN) {
        maze.cellMap[x][y].downWall = YESWALL;

        if (y != 5) {
            maze.cellMap[x][y+1].upWall = YESWALL;
        }
    }
    else if (direction == RIGHT) {
        maze.cellMap[x][y].rightWall = YESWALL;

        if (x != 5) {
            maze.cellMap[x+1][y].leftWall = YESWALL;
        }
    }
    else if (direction == LEFT) {
        maze.cellMap[x][y].leftWall = YESWALL;

        if (x != 0) {
            maze.cellMap[x-1][y].rightWall = YESWALL;
        }
    }
}

WallType GetWall(int x, int y, Direction direction) {
    if (direction == UP) {return maze.cellMap[x][y].upWall;}
    if (direction == DOWN) {return maze.cellMap[x][y].downWall;}
    if (direction == RIGHT) {return maze.cellMap[x][y].rightWall;}
    else{return maze.cellMap[x][y].leftWall;}
}


Colour get_colour() {
  // Setting red filtered photodiodes to be read
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  
  // Reading the output frequency
  int red = pulseIn(2, LOW);

  delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);

  // Reading the output frequency
  int green = pulseIn(2, LOW);

  delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(2,LOW);
  digitalWrite(3,HIGH);

  // Reading the output frequency
  int blue = pulseIn(10, LOW);

  delay(100);

  Serial.print("R: "); 
  Serial.print(red); 
  Serial.print("G: "); 
  Serial.print(green); 
  Serial.print("B: "); 
  Serial.print(blue); 
  Serial.println("");

  // =============================== convert to colour ===============================
  if (red >= 40 && red <= 85 && green >= 150 && green <=210 && blue >= 105 && blue <= 150) {
    return RED;
  }
  else if (red >= 95 && red <= 130 && green >= 150 && green <=210 && blue >= 80 && blue <= 140) {
    return OTHER;
  }
  else if (red >= 120 && red <= 190 && green >= 100 && green <=210 && blue >= 60 && blue <= 120) {
    return OTHER;
  }
  else if (red >= 65 && red <= 105 && green >= 60 && green <=110 && blue >= 90 && blue <= 140) {
    return OTHER;
  }
  else if (red >= 40 && red <= 80 && green >= 65 && green <=120 && blue >= 95 && blue <= 150) {
    return OTHER;
  }
  else if (red >= 25 && red <= 70 && green >= 30 && green <=80 && blue >= 25 && blue <= 65) {
    return BW;
  }
  else {
    return BW;
  }
}



// int main(int argc, char const *argv[])
// {
//     //cur_dir.push("Up"); cur_dir.push("Right"); cur_dir.push("Down"); cur_dir.push("Left");
//     NavigateMaze(5,5,1,1,true, true);
// }




// lastDistance
// check current cell dist > lastdist + 1
// set dist = last dist + 1;
// for eahc cell that doenst have a wall, and dist < last dist + 1:


// start from dest
// if it hits the start cell dist 
// follow the number 


// get firsection
// check wall
// if wall == true?
//  update!
//  recalculate
// else:
//  move:
//  recalculate / find next lowest?

// 0,0 top left
//
//[(0,0),(1,0),(2,0),(3,0),(4,0)]
//[(0,1),(1,1),(2,1),(3,1),(4,1)]

// start = 5,5
// end: 1,1

//6x 6 grid
