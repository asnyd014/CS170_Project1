#include "Problem.h"
#include <iostream>
#include <vector>

using namespace std;

enum algorithm { UCS, AMT, AED };

void getUserInput(vector<int>& initialState, algorithm& selectedAlgorithm) {
    int choice, algorithmChoice;

    //prompt user for puzzle selection
    cout << "Welcome to the 862245168 8 puzzle solver.\n";
    cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle.\n";
    cin >> choice;

    //read the puzzle design based on the user choice
    if (choice == 1) {  //using default puzzle given in sample pdf
        initialState = { 1, 2, 3, 4, 8, 0, 7, 6, 5 };
    }
    else if (choice == 2) {  //user's own puzzle
        cout << "Enter your puzzle, use a zero to represent the blank\n";
        cout << "Enter the first row, use space or tabs between numbers: ";
        initialState.resize(9); 
        for (int i = 0; i < 3; ++i) {   //1st row
            cin >> initialState[i];
        }
        cout << "Enter the second row, use space or tabs between numbers: ";
        for (int i = 3; i < 6; ++i) {   //2nd row
            cin >> initialState[i];
        }
        cout << "Enter the third row, use space or tabs between numbers: ";
        for (int i = 6; i < 9; ++i) {   //3rd row
            cin >> initialState[i];
        }
    }
    else {  //invalid choice
        cerr << "Invalid choice. Exiting.\n";
        exit(1);
    }

    //this prompts user to select an algorithm selection
    cout << "Enter your choice of algorithm:\n";
    cout << "1. Uniform Cost Search\n";
    cout << "2. A* with Misplaced Tile heuristic.\n";
    cout << "3. A* with Euclidean distance heuristic.\n";
    
    cin >> algorithmChoice;
    switch (algorithmChoice) {
    case 1:
        selectedAlgorithm = algorithm::UCS;
        break;
    case 2:
        selectedAlgorithm = algorithm::AMT;
        break;
    case 3:
        selectedAlgorithm = algorithm::AED;
        break;
    default:
        cerr << "Invalid choice. Exiting.\n";
        exit(1);
    }
}

int main() {
    vector<int> initialState;
    algorithm selectedAlgorithm;

    getUserInput(initialState, selectedAlgorithm);  //get the user input
    //initialState: a vector that stores the order of the tiles on the puzzle board
    //selectedAlgorithm: stores the user's choice of the algorithm

    
    return 0;
}
