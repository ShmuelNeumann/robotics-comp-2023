#include <queue>
#include <iostream>
#include <array>

#define inf ((float)(_HUGE_ENUF * _HUGE_ENUF))
using namespace std;

class Maze {
    public:
        float maze[6][6] = {};
        array<array<bool, 4>, 6> noWall = {true};
        int currentposition[2] = {5,5}; // Set in bottom right corner
        array<bool, 4> availablewalls = {}; // Front, Right, Back, Left (needs to be referenced)
        queue<int> orientation; // 1 - East, 2 - South, 3 - West, 4 - North
        Maze() {orientation.push(1); orientation.push(2); orientation.push(3); orientation.push(4);}
        
/*
representation is a relative fixed matrix
{0, 0}  ------------------------------ 
        |                            |
        |     X                      |
        |                            |
        |                            |
        |                            |
        |                            |
        |                            |
        |                            |
        |                          0 |
        ------------------------------ {6, 6}
*/
    void aggregate() {

    }

    void map_maze(bool walls[4]) {
        // Update current map with current cell information
        // Colour is 1 - White, 2 - Black, 3 - Green, 4 - Blue, 5 - Purple, 6 - Yellow, 7 - Red
        queue<int> wallDetected;

        for (int i = 0; i < 4; i++) {
            wallDetected.push(walls[i]);
        }

        // Reorder elements relative to orientation
        if (orientation.front() != 1) {
            for (int i = 0; i < 5 - orientation.front(); i++) {
                wallDetected.push(wallDetected.front());
                wallDetected.pop();
            }
        }

        availablewalls[1] = !wallDetected.front(); wallDetected.pop();
        availablewalls[2] = !wallDetected.front(); wallDetected.pop();
        availablewalls[3] = !wallDetected.front(); wallDetected.pop();
        availablewalls[4] = !wallDetected.front(); wallDetected.pop();

        bool wallfound;
        for (int i = 0; i < 4; i++) {
            if (!availablewalls[i]) {
                wallfound = true;
            }
        }

        noWall[currentposition[0], currentposition[1]] = availablewalls;
    }

    void move() {
        // Move in a direction by 1 cell
        switch (orientation.front()) {
            case 1:
                currentposition[0] += 2;
                break;

            case 2:
                currentposition[1] += 2;
                break;

            case 3:
                currentposition[0] -= 2;
                break;

            case 4:
                currentposition[1] -= 2;
                break;
        }
    }

    void turn_left() {
        for (int i = 0; i < 3; i++) {
            orientation.push(orientation.front());
            orientation.pop();
        }
    }

    void turn_right() {
        orientation.push(orientation.front());
        orientation.pop();
    }

    void print_maze() {
        for (auto &row : maze)
        {
            for (auto &column : row)
            {
                cout << column << " ";
            }
            cout << endl;
        }
    }
};