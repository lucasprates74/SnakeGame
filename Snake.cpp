#include <iostream>
#include <conio.h>
#include <thread>
#include <Windows.h>
#include <stdlib.h>
using namespace std;
/* In this program the snake is represented as an array containing pairs of coordinates. The zeroeth coordinate 
is the tail of snake, while the last coordinate is the head of the snake. */

class coords {
    // represents a tuple of numbers //
    public:
        int row;
        int col;

        coords(int x=0, int y=0) {
            row = x;
            col = y;
        }

        bool operator== (coords &other) {
            return (row == other.row) && (col == other.col); 
            }
        
        bool in(coords arr[], int N) {
            /* Check if coords are in arr up to the N-1th entry*/
            for (int i = 0; i < N; ++i) {
                coords entry = arr[i];
                if (row == entry.row && col == entry.col) {
                    return true;
                }
            }
            return false;
        }
};

// Initialize constant to keep track of size of grid
const int GridSize = 10;

// Initialize array to contain coordinates of each part of snake and a set of coords for apple
coords snake[GridSize*GridSize];
coords apple;

// Initialize integers to keep track of the snake's length and the game's score
int SnakeLen = 3; 
int score = 0; 

bool GameLost = false; // Initialize a boolean for the game's lose conditions

// establish control parameters for game
string PlayerInput;
bool CommandGiven;
string direction = "r"; // indicates the direction the snake is currently moving (r, l, u, or d)

void ShiftN (coords arr[], int N, coords Replacement) {
    /*Delete the zeroth entry in an array and shift the entries 1 through N-1 back one. Replace entry
    N with Replacement*/
    for (int i = 0; i < N - 1; ++i) {
        arr[i] = arr[i + 1];
    }
    arr[N - 1] = Replacement;
}

void PrintGrid() {
    int grid[GridSize][GridSize] = { };
    grid[apple.row][apple.col] = 1;

    for (int i=0; i < SnakeLen - 1; ++i) {
        coords seg = snake[i];
        grid[seg.row][seg.col] = 2;
    }
    coords seg = snake[SnakeLen - 1];
    grid[seg.row][seg.col] = 3;

    for (int RowNum = 0; RowNum < GridSize; RowNum++) {
            for (int ColNum = 0; ColNum < GridSize; ColNum++) {
                int num = grid[RowNum][ColNum];
                if (num == 3) {cout << "H    ";}
                else if (num == 2) {cout << "B    ";}
                else if (num == 1) {cout << "A    ";}
                else {cout << "0    ";}
                }
            cout << "\n";
            }
}


void Introduction() {
    PrintGrid();

    cout << "\nWelcome to Snake! Control the snake with w, a, s, d. Press any key to begin!";
    getch();
}

void GetInput() {
    PlayerInput = getch();
    CommandGiven = true;
}


void MoveApple() {
    /* Writes a pair of random coordinates into apple.*/

    coords loc;
    int attempt = 1; // makes sure we use a new seed if the loop runs again

    do {
    srand(time(NULL) * attempt);
    loc.row = rand() % GridSize;
    srand(time(NULL) * 2 * attempt);
    loc.col = rand() % GridSize;
    
    ++attempt;
    }
    while (loc.in(snake, SnakeLen)); // while condition to make sure the snake is not overwritten by an apple

    apple = loc;
}

void MoveSnake(coords loc) {
    /* Checks the coordinates the snake's head is trying to move into for an apple and moves the snake
    appropriately. */
    if (loc == apple) {
        snake[SnakeLen] = loc; // replace the apple with the snake head
        ++SnakeLen;
        ++score;
        // add code to put apple at random position on grid
        MoveApple();
    } 
    else {
        // Move the snake according to new directions
        ShiftN(snake, SnakeLen, loc);
    }
}

int RunRound(string MyInput = "") {
    // This function directs the snake based on player input

    // Find the snake's head
    coords HeadCurr = snake[SnakeLen - 1];

    // Find where the player wants to move the head according to user input
    coords HeadNext;
    if (direction == "r" || direction == "l") { 
        if (MyInput == "w") {
            HeadNext.row = HeadCurr.row - 1;
            HeadNext.col = HeadCurr.col;
            direction = "u";
        } 
        else if (MyInput == "s") {
            HeadNext.row = HeadCurr.row + 1;
            HeadNext.col = HeadCurr.col;
            direction = "d";
        }
        else {
            if (direction == "r") {
                HeadNext.row = HeadCurr.row;
                HeadNext.col = HeadCurr.col + 1;
            }
            else {
                HeadNext.row = HeadCurr.row;
                HeadNext.col = HeadCurr.col - 1;
            }
        }
    } 
    else { 
        if (MyInput == "d") {
            HeadNext.row = HeadCurr.row;
            HeadNext.col = HeadCurr.col + 1;
            direction = "r";
        } 
        else if (MyInput == "a") {
            HeadNext.row = HeadCurr.row;
            HeadNext.col = HeadCurr.col - 1;
            direction = "l";
        }
        else {
            if (direction == "u") {
                HeadNext.row = HeadCurr.row - 1;
                HeadNext.col = HeadCurr.col;
            }
            else {
                HeadNext.row = HeadCurr.row + 1;
                HeadNext.col = HeadCurr.col;
            }
        }
    }
    
    // Check if the game is over
    if (HeadNext.row == GridSize || HeadNext.row == -1 || HeadNext.col == GridSize || HeadNext.col == -1) 
    {GameLost = true; return -1;}

    if (HeadNext.in(snake, SnakeLen)) {GameLost = true; return -1;}

    // Move the snake
    MoveSnake(HeadNext);
    return 0;
}

int main () {
    // set initial position of snake
    coords tail(3, 3);
    snake[0] = tail;

    coords mid(3, 4);
    snake[1] = mid;
    
    coords head(3, 5);
    snake[2] = head;

    // set initial apple position
    apple.row = 5;
    apple.col = 7;

    Introduction();
    while (!GameLost) {
        CommandGiven = false;

        thread thr(GetInput); 

        Sleep(400); // Gives user a time set time to make an input

        thr.detach(); thr.~thread(); 

        if (CommandGiven) {RunRound(PlayerInput);} else {RunRound();}

        system("CLS"); // clears screen
        
        PrintGrid();
    }
    
    cout << "Game over! You ate " << score << " apples!";
    getch();
    return 0;
}