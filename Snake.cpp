#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>  // For sleep()
#include <termios.h>  // For terminal input handling (Linux)
#include <stdio.h>  // For getchar()

#define MAXSNAKESIZE 100
#define MAXFRAMEX 119
#define MAXFRAMEY 29

using namespace std;

// Cross-platform way to move the cursor
void gotoxy(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}

class Point {
private:
    int x;
    int y;
public:
    Point() {
        x = y = 10;
    }
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
    void SetPoint(int x, int y) {
        this->x = x;
        this->y = y;
    }
    int GetX() {
        return x;
    }
    int GetY() {
        return y;
    }
    void MoveUp() {
        y--;
        if (y < 0) {
            y = MAXFRAMEY;
        }
    }
    void MoveDown() {
        y++;
        if (y > MAXFRAMEY) {
            y = 0;
        }
    }
    void MoveLeft() {
        x--;
        if (x < 0) {
            x = MAXFRAMEX;
        }
    }
    void MoveRight() {
        x++;
        if (x > MAXFRAMEX) {
            x = 0;
        }
    }
    void Draw() {
        gotoxy(x, y);
        cout << "*";
    }
    void Erase() {
        gotoxy(x, y);
        cout << " ";
    }
    void CopyPos(Point* p) {
        p->x = x;
        p->y = y;
    }
    int IsEqual(Point* p) {
        return (x == p->x && y == p->y);
    }
    void Debug() {
        cout << "(" << x << "," << y << ") ";
    }
};

class Snake {
private:
    Point* cell[MAXSNAKESIZE];  // Array of points to represent snake
    int size;  // Current size of snake
    char dir;  // Current direction of snake
    Point fruit;
    int state;
    int started;

public:
    Snake() {
        size = 1;  // Default size
        cell[0] = new Point(20, 20);  // 20,20 is default position
        for (int i = 1; i < MAXSNAKESIZE; i++) {
            cell[i] = NULL;
        }
        fruit.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
        state = 0;
    }

    void AddCell(int x, int y) {
        cell[size++] = new Point(x, y);
    }

    void TurnUp() {
        if (dir != 's') {
            dir = 'w';
        }
    }
    void TurnDown() {
        if (dir != 'w') {
            dir = 's';
        }
    }
    void TurnLeft() {
        if (dir != 'd') {
            dir = 'a';
        }
    }
    void TurnRight() {
        if (dir != 'a') {
            dir = 'd';
        }
    }

    void Welcomescreen() {
        cout << "Welcome to the snake game!";
    }

    void Move() {
        // Clear screen
        cout << "\033[H\033[J";  // ANSI escape code to clear screen

        // Making snake body follow its head
        for (int i = size - 1; i > 0; i--) {
            cell[i - 1]->CopyPos(cell[i]);
        }

        // Turning snake's head
        switch (dir) {
        case 'w':
            cell[0]->MoveUp();
            break;
        case 's':
            cell[0]->MoveDown();
            break;
        case 'a':
            cell[0]->MoveLeft();
            break;
        case 'd':
            cell[0]->MoveRight();
            break;
        }

        if (SelfCollision()) {
            state = 0;
        }

        // Collision with fruit
        if (fruit.GetX() == cell[0]->GetX() && fruit.GetY() == cell[0]->GetY()) {
            AddCell(0, 0);
            fruit.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
        }

        for (int i = 0; i < size; i++) {
            cell[i]->Draw();
        }
        fruit.Draw();

        usleep(50000);  // Sleep for 50ms
    }

    int SelfCollision() {
        for (int i = 1; i < size; i++) {
            if (cell[0]->IsEqual(cell[i]))
                return 1;
        }
        return 0;
    }

    void Debug() {
        for (int i = 0; i < size; i++) {
            cell[i]->Debug();
        }
    }
};

// Function for non-blocking keyboard input on Linux
char getKey() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // Disable buffering and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int main() {
    srand((unsigned)time(NULL));  // Random number generation

    // Testing snake
    Snake snake;
    char op = 'l';
    do {
        if (op = getKey()) {
            switch (op) {
            case 'w':
            case 'W':
                snake.TurnUp();
                break;
            case 's':
            case 'S':
                snake.TurnDown();
                break;
            case 'a':
            case 'A':
                snake.TurnLeft();
                break;
            case 'd':
            case 'D':
                snake.TurnRight();
                break;
            }
        }
        snake.Move();
    } while (op != 'i');

    return 0;
}
