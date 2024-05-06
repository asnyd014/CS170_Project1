#include "Problem.h"

using namespace std;

SolutionNode graph_search(Problem pr, int& maxNodes, int& numExpanded);

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
        selectedAlgorithm = UCS;
        break;
    case 2:
        selectedAlgorithm = AMT;
        break;
    case 3:
        selectedAlgorithm = AED;
        break;
    default:
        cerr << "Invalid choice. Exiting.\n";
        exit(1);
    }
}

int main() {
    vector<int> initialState;
    algorithm selectedAlgorithm;
    int maxNodes = 0;
    int numExpanded = 0;
    int solutionDepth = 0;

    getUserInput(initialState, selectedAlgorithm);
    
    // Construct new Problem object using provided input
    Problem pr(initialState, selectedAlgorithm);

    SolutionNode solution = graph_search(pr, maxNodes, numExpanded);
    
    if (solution.g_n >= 0) {
        cout << "Success" << endl;
        cout << "Number of expanded nodes: " << numExpanded << endl;
        cout << "Maximum number of nodes in frontier: " << maxNodes << endl;
        cout << "Depth of goal node: " << solution.g_n << endl;
    }
    else {
        cout << "Failure" << endl;
    }

    return 0;
}

SolutionNode graph_search(Problem pr, int& maxNodes, int& numExpanded) {
    bool firstNode = true;
    // Set up frontier and explored sets
    std::priority_queue<SolutionNode, std::vector<SolutionNode>, SolutionNode> frontier;
    std::set<SolutionNode, SolutionNode> frontierRecord; // Used for determining if nodes already exist in frontier
    std::set<SolutionNode, SolutionNode> explored;

    // Create node to indicate failure
    SolutionNode failNode;
    failNode.g_n = -1;

    // Create node to return with
    SolutionNode solution;

    // Create temporary pointer to maintain reference to node
    // while between frontier and explored sets
    SolutionNode temp;

    // Create initial state node
    SolutionNode rootNode;
    rootNode.state = pr.getInitialState();
    rootNode.g_n = 0;
    rootNode.h_n = 0;

    // Output information for initial node
    cout << "\nExpanding State" << endl;
    for (int i = 0; i < rootNode.state.size();) {
        for (int j = 0; j < pr.getColumnCount(); ++j) {
            cout << rootNode.state[i] << " ";
            ++i;
        }
        cout << endl;
    }
    cout << endl;

    // Push initial state node to frontier
    frontier.push(rootNode);

    while (1) {
        if (frontier.empty()) { solution = failNode; break; } // Break out of loop if frontier is empty

        // Check number of nodes in frontier, update count if applicable
        if (frontier.size() > maxNodes) { maxNodes = frontier.size(); }

        // Store reference to node in temp pointer
        temp = frontier.top();

        // Pop node off of frontier
        frontier.pop();

        // Check if state of node matches goal, return solution if it does
        if (temp.state == pr.getGoalState()) {
            solution = temp;
            break;
        }

        // Add node to explored set
        explored.insert(temp);

        if (!firstNode) { // Don't output on first node
            // Expand node
            cout << "Expanding State with g(n) = " << temp.g_n << " and h(n) = " << temp.h_n << endl;
            for (int i = 0; i < temp.state.size();) {
                for (int j = 0; j < pr.getColumnCount(); ++j) {
                    cout << temp.state[i] << " ";
                    ++i;
                }
                cout << endl;
            }
            cout << endl;
        }
        firstNode = false;    

        ++numExpanded;
        pr.expandNode(temp, frontier, frontierRecord, explored);
    }

    // If this returns a nullptr, the search has failed
    return solution;
}
