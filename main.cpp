//Takes string file and parses and stores each value. 
//Prints output and changes 1 to mountains based on enum class.

#include <algorithm>  // for sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;
using std::abs;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

//Parses each line of string including characters and integers 
vector<State> ParseLine(string v){

    istringstream my_stream(v);

    char c;
    int n;
  
  	vector <State> row;

    while(my_stream >> n >> c && c == ','){
        if (n == 0){
            row.push_back(State::kEmpty);
        } else {
            row.push_back(State::kObstacle);
        }
    }
  	return row;
}

vector<vector<State>> ReadBoardFile(string path) {
    ifstream myfile (path);
    
    //Declares an empty board variable here with
    vector<vector<State>> board {};
        
    if (myfile) {
        string line;
        while (getline(myfile, line)) {
        //Calls ParseLine for each line and push the results of ParseLine to the back of the board.
        vector<State> row = ParseLine(line);
        board.push_back(row);
        }
    }
    //Return the board variable.
    return board;
}

//Compares F values of two cells
bool Compare (const vector<int> a, const vector<int> b){

    //Calls vector nodes from {x, y, g, h}
    //f = g + h(x,y)
    int f1 = a[2] + a[3]; //f1 = g1 + h1
    int f2 = b[2] + b[3]; //f2 = g2 + h2

    //returns the boolean of this expression
    return f1 > f2;
}

//Sort the two-dimensional vector of ints in descending order.
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

//Heuristic function that calculates the Manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}

// Checks that the cell is on the grid and not an obstacle (i.e. equals kEmpty).
bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {

//checks to see if x and y are greater than 0 and fall somewhere within the grid; hence grid.size()
  bool on_grid_x = (x >= 0 && x < grid.size());
  bool on_grid_y = (y >= 0 && y < grid[0].size());
  if (on_grid_x && on_grid_y)
    return grid[x][y] == State::kEmpty;
  return false;
}

//Add a node to the open list and mark it as open. 
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid) {
  
  // Add node to open vector, and mark grid cell as closed.
  openlist.push_back(vector<int>{x, y, g, h});
  grid[x][y] = State::kClosed;
}

//Expand current nodes's neighbors and add them to the open list.

void ExpandNeighbors (const vector<int> &current, int goal[2], vector<vector<int>> &openList, vector<vector<State>> &grid){
    
    //Retrieves current node's data
    int x = current[0];
    int y = current[1];
    int g = current[2];

    //Loops through current node's potential neighbors.
    for (int i = 0; i < 4; i++){

        //sets neighbor to value based on delta
        int x2 = x + delta[i][0];
        int y2 = y + delta[i][1];

        //Checks that the potential neighbor's x2 and y2 values are on the grid and not closed
        if (CheckValidCell(x2, y2, grid)){

            //Increment g value, compute h value, and adds neighbor to open list.
            int g2 = g + 1;
            int h2 = Heuristic (x2, y2, goal[0], goal[1]);
            AddToOpen (x2, y2, g2, h2, openList, grid);
        }

    }
}

//Implementation of A* search algorithm
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]) {

    // Create the vector of open nodes.
    vector<vector<int>> open {};
    int x1 = init[0];
    int y1 = init[1];
    int x2 = goal[0];
    int y2 = goal[1];
    int g = 0;
    int h = Heuristic(x1, y1, x2, y2);

    AddToOpen (x1, y1, g, h, open, grid);

    //while open vector is non empty {
    while (open.size() >0){

        //Sorts the open list using CellSort, and gets the current node.
        CellSort(&open);

        //returns last element in open nodes

        auto current = open.back();

        //removes element from back of nodes 
        open.pop_back();

        //Gets the x and y values from the current node
        x1 = current[0];
        y1 = current[1];

        //Set grid[x][y] to kPath
        grid[x1][y1] = State::kPath;

        //Checks if you've reached the goal. If so, return grid.
        if (x1 == x2 && y1 == y2){
            grid[init[0]][init[1]] = State::kStart;
            grid[x2][y2] = State::kFinish;
            return grid;
        }
        
        // If we're not done, expand search to current node's neighbors
        ExpandNeighbors(current, goal, open, grid);
    }
  
    // We've run out of new nodes to explore and haven't found a path.
    cout << "No path found!" << "\n";

    return std::vector<vector<State>> {};
}

string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    case State::kPath: return "🚗   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
    default: return "0   "; 
  }
}

//prints board 
void PrintBoard (const vector<vector<State>> board){
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
}

int main() {

  //Declares "init" and "goal" arrays with values {0, 0} and {4, 5} respectively.
  int init[2]{0,0};
  int goal[2]{4,5};

  auto board = ReadBoardFile("1.board");

  //Calls Search with "board", "init", and "goal". Stores the results in the variable "solution".
  auto solution = Search(board, init, goal);

  PrintBoard(solution);
  
}