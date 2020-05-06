//Takes string file and parses and stores each value. 
//Prints output and changes 1 to mountains based on enum class.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using std::cout;
using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;

enum class State {kEmpty, kObstacle};

//TODO: Add ParseLine function here
vector<State> ParseLine(string v){

    istringstream my_stream(v);

    char c;
    int n;
  
  	vector <State> row;

    while(my_stream >> n >> c){
        if (n == 0){
            row.push_back(State::kEmpty);
        } else {
            row.push_back(State::kObstacle);
        }
    }
  	return row;
}

// TODO: Change the return type of ReadBoardFile.
vector<vector<State>> ReadBoardFile(string path) {
    ifstream myfile (path);
    
    // TODO: Declare an empty board variable here with
    vector<vector<State>> board;
        
    if (myfile) {
        string line;
        while (getline(myfile, line)) {
        // TODO: Replace the "cout" code with a call to ParseLine for each line and push the results of ParseLine to the back of the board.
        vector<State> row = ParseLine(line);
        board.push_back(row);
        }
    }
    // TODO: Return the board variable.
    return board;
}

// TODO: Create the CellString function here,
// using the following return strings:
// "⛰️   "
// "0   "

string CellString (State cell){

    if (cell == State::kObstacle){
        return "⛰️   ";
    } else {
        return "0   ";
    }
}

// TODO: Add PrintBoard function here.
void PrintBoard (const vector<vector<State>> board){
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
}

int main() {
  // TODO: Call the ReadBoardFile function here.
  auto board = ReadBoardFile("1.board");
  // Leave the following line commented out.
  PrintBoard(board);
}