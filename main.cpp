#include "Problem.h"
#include <iostream>
using namespace std;

SolutionNode* graph_search(Problem pr, int& maxNodes, int& numExpanded);

int main() {

    int maxNodes = 0;
    int numExpanded = 0;
    std::vector<int> initialState { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
    algorithm selectedAlgorithm = UCS;
    
    // Construct new Problem object using provided input
    Problem pr(initialState, selectedAlgorithm);

    SolutionNode* solution = graph_search(pr, maxNodes, numExpanded);
    
    if (solution) {
        cout << "Success" << endl;
    }
    else {
        cout << "Failure" << endl;
    }

    return 0;
}

SolutionNode* graph_search(Problem pr, int& maxNodes, int& numExpanded) {
    // Set up frontier and explored sets
    std::priority_queue<SolutionNode, std::vector<SolutionNode>, SolutionNode> frontier;
    std::set<SolutionNode, SolutionNode> frontierRecord; // Used for determining if nodes already exist in frontier
    std::set<SolutionNode, SolutionNode> explored;

    // Create pointer to return with
    SolutionNode* solution = nullptr;

    // Create temporary pointer to maintain reference to node
    // while between frontier and explored sets
    SolutionNode temp;

    // Create initial state node
    SolutionNode rootNode;
    rootNode.state = pr.getInitialState();
    rootNode.g_n = 0;
    rootNode.h_n = 0;

    // Push initial state node to frontier
    frontier.push(rootNode);

    while (1) {
        if (frontier.empty()) { break; } // Break out of loop if frontier is empty

        // Check number of nodes in frontier, update count if applicable
        if (frontier.size() > maxNodes) { maxNodes = frontier.size(); }

        // Store reference to node in temp pointer
        temp = frontier.top();

        // Pop node off of frontier
        frontier.pop();

        // Check if state of node matches goal, return solution if it does
        if (temp.state == pr.getGoalState()) {
            solution = &temp;
            break;
        }

        // Add node to explored set
        explored.insert(temp);

        // Expand node
        cout << "Expanding State with g(n) = " << temp.g_n << " and h(n) = " << temp.h_n << endl;
        for (int i = 0; i < temp.state.size();) {
            for (int j = 0; j < pr.getColumnCount(); ++j) {
                cout << temp.state[i] << " ";
                ++i;
            }
            cout << endl;
        }
        ++numExpanded;
        pr.expandNode(temp, frontier, frontierRecord, explored);
    }

    // If this returns a nullptr, the search has failed
    return solution;
}
