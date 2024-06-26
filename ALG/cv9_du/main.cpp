using namespace std;
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <ostream>

//8 puzzle game
//playing field is represented by vector<int> where 0 represents empty field

int N = 3;//size of the playing field, original is 3x3
//for whatever reason doesnt work on anything bigger than 3

vector<vector<int>> GetNextStates(const vector<int> playingField){
    int emptyIndex;//index of 0
    for (int i = 0; i < playingField.size(); i++)//getting index of 0
    {
        if(playingField[i] == 0){
            emptyIndex = i;
            break;
        }
    }

    vector<int> directionsColumn = {1, -1, 0, 0};//directions of columns, left, right
    vector<int> directionsRow = {0, 0, 1, -1};//directions of rows, up, down
    vector<vector<int>> nextStates;//vector of next states

    for (int i = 0; i < 4; i++)//for 4 directions
    {
        //convert index to column, row for esier manipulation
        int columnState = (emptyIndex % N) + directionsColumn[i];
        int rowState = (emptyIndex / N) + directionsRow[i];

        if (columnState >= 0 && columnState < N && rowState >=0 && rowState < N)//check if column, row is not out of bounds
        {
            vector<int> temp = playingField;
            int currIndex = (rowState * N) + columnState;//convert column, row back to index
            swap(temp[emptyIndex], temp[currIndex]);//swap old and new indexes
            nextStates.push_back(temp);//add to next states
        }
        
    }
    return nextStates;
}

//https://www.programmingalgorithms.com/algorithm/sdbm-hash/cpp/
unsigned int SDBMHash(const vector<int> vec) {

    string temp = "";
    for (int i : vec) temp += to_string(i);

	unsigned int hash = 0;

	for (int i = 0; i < temp.length(); i++)
	{
		hash = (temp[i]) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}


bool IsWinning(const vector<int> check){//checks if vector is winning by comparing itself with its sorted version
    vector<int> temp = check;
    sort(temp.begin(), temp.end());
    return check == temp;
}

bool IsPuzzleSolvable(const vector<int>& playingField, int& iterations) {//DFS search
    stack<vector<int>> stack;
    unordered_set<unsigned int> visited;

    stack.push(playingField);
    visited.insert(SDBMHash(playingField));

    while (!stack.empty()) {
        vector<int> currentState = stack.top();
        stack.pop();

        iterations++;
        if (IsWinning(currentState)) {//check if current playing field is sorted
            return true;
        }

        vector<vector<int>> nextStates = GetNextStates(currentState);
        for (const vector<int>& nextState : nextStates) {
            unsigned int temp = SDBMHash(nextState);
            if (visited.find(temp) == visited.end()) {
                stack.push(nextState);
                visited.insert(temp);
            }
        }
    }
    return false;
}

int main(){
    int iter = 0;
    vector<int> temp = {1,0,2,3,4,5,6,7,8};
    if (IsPuzzleSolvable(temp, iter))
    {
        cout << "puzzle is solvable" << endl;
    }
    else{
        cout << "puzzle has no solution" << endl;
    }
    cout << "number of iterations: " << iter << endl;
}
