// genenerates a random depth first search maze
// outputs to console

#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>

const int WIDTH = 21;
const int HEIGHT = 21;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Cell {                                   // each cell in maze
    int x, y;
    bool visited;
    bool walls[4]; // UP, DOWN, LEFT, RIGHT     // array of walls (present/not present)
};

class Maze {
public:
    Maze(int width, int height);
    void generate();
    void display() const;

private:
    int width, height;
    std::vector<std::vector<Cell>> grid;        // 2D vector representing maze grid
    std::stack<Cell*> cellStack;                // stack used for backtracking during maze generation
    Cell* currentCell;
    int totalCells;
    int visitedCells;

    void initGrid();
    Cell* getNeighbor(Cell* cell);
    void removeWall(Cell* current, Cell* next);
};

Maze::Maze(int width, int height)
    : width(width), height(height), visitedCells(0) {  // colon is passed to all member variables as they are passed their respective constructors
    initGrid();
}

void Maze::initGrid() {
    grid.resize(height, std::vector<Cell>(width));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = { x, y, false, { true, true, true, true } };   // set all walls to true and mark as unvisited
        }
    }
    currentCell = &grid[0][0];
    currentCell->visited = true;                                        
    totalCells = width * height;
    visitedCells = 1;
}

Cell* Maze::getNeighbor(Cell* cell) {
    std::vector<Cell*> neighbors;

    int x = cell->x;
    int y = cell->y;

    if (y > 0 && !grid[y - 1][x].visited) neighbors.push_back(&grid[y - 1][x]);     // UP
    if (y < height - 1 && !grid[y + 1][x].visited) neighbors.push_back(&grid[y + 1][x]); // DOWN
    if (x > 0 && !grid[y][x - 1].visited) neighbors.push_back(&grid[y][x - 1]);     // LEFT
    if (x < width - 1 && !grid[y][x + 1].visited) neighbors.push_back(&grid[y][x + 1]); // RIGHT

    if (neighbors.empty()) return nullptr;

    int randomIndex = std::rand() % neighbors.size();
    return neighbors[randomIndex];
}

void Maze::removeWall(Cell* current, Cell* next) {
    int dx = current->x - next->x;
    int dy = current->y - next->y;

    if (dx == 1) {
        current->walls[LEFT] = false;
        next->walls[RIGHT] = false;
    }
    else if (dx == -1) {
        current->walls[RIGHT] = false;
        next->walls[LEFT] = false;
    }
    else if (dy == 1) {
        current->walls[UP] = false;
        next->walls[DOWN] = false;
    }
    else if (dy == -1) {
        current->walls[DOWN] = false;
        next->walls[UP] = false;
    }
}

void Maze::generate() {
    std::srand(std::time(nullptr));                     // generates random maze
    while (visitedCells < totalCells) {
        Cell* nextCell = getNeighbor(currentCell);
        if (nextCell) {
            nextCell->visited = true;
            cellStack.push(currentCell);
            removeWall(currentCell, nextCell);
            currentCell = nextCell;
            ++visitedCells;
        }
        else if (!cellStack.empty()) {
            currentCell = cellStack.top();
            cellStack.pop();
        }
    }
}

void Maze::display() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << (grid[y][x].walls[UP] ? "+---" : "+   ");
        }
        std::cout << "+" << std::endl;
        for (int x = 0; x < width; ++x) {
            std::cout << (grid[y][x].walls[LEFT] ? "|   " : "    ");
        }
        std::cout << "|" << std::endl;
    }
    for (int x = 0; x < width; ++x) {
        std::cout << "+---";
    }
    std::cout << "+" << std::endl;
}

int main() {
    Maze maze(WIDTH, HEIGHT);
    maze.generate();
    maze.display();
    return 0;
}
