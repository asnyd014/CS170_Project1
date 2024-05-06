#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cmath>

// To be passed in when constructing a Problem object. 
// UCS = Uniform Cost Search
// AMT = A* with Misplaced Tile
// AED = A* with Euclidean Distance
enum algorithm { UCS, AMT, AED };

// To be used for containing node states and determining solution path
// A node with a state matching the goal state (a goal node), can immediately refer to parents and output in some format
struct SolutionNode {
    std::vector<int> state;
    SolutionNode* parent = nullptr;
    int g_n;
    float h_n;

    bool operator() (const SolutionNode& n1, const SolutionNode& n2) const {
        return (n1.g_n + n1.h_n) > (n2.g_n + n2.h_n);
    }
    
    bool operator< (SolutionNode& n) const {
        return (this->g_n + this->h_n) > (n.g_n + n.h_n);
    }
};

class Problem {

        // 3 columns and 3 rows for normal 8 puzzle
        static const int probColumns = 3;
        static const int probRows = 3;

        std::vector<int> initialState;
        std::vector<int> goalState;
        algorithm heuristic;

    private:
        void calcHeuristic(SolutionNode& newNode) {
            // Set new node's g(n) to parent's g(n) + 1
            SolutionNode* parent = newNode.parent;
            newNode.g_n = (*parent).g_n + 1; 

            // Calculate h(n) based on algorithm chosen
            switch (this->heuristic) {
                case UCS: // Uniform Cost Search
                    newNode.h_n = 0;
                    break;
                case AMT: // A* with Misplaced Tile
                    // Check if values at each index of the current state and the goal state match
                    // If they do not, increment number of misplaced tiles by one
                    newNode.h_n = 0;
                    for (int i = 0; i < goalState.size(); ++i) {
                        if (newNode.state[i] != goalState[i]) { ++newNode.h_n; }
                    }
                    break;
                case AED: // A* with Euclidean Distance
                {    
                    int x1, x2, y1, y2 = 0;
                    int targetIndex = 0;
                    newNode.h_n = 0;

                    // Iterate through goal state, determine distance each value is from corresponding value in node state
                    for (int i = 0; i < goalState.size(); ++i) {
                        // Translate current goal state index into coordinates
                        x1 = i % probColumns;
                        y1 = (i / probColumns) * -1;

                        // Check for matching value in current state
                        for (int j = 0; j < goalState.size(); ++j) {
                            if (newNode.state[j] == goalState[i]) { targetIndex = j; break; }
                        }

                        if (i != targetIndex) {

                            // Translate current goal state index into coordinates
                            x1 = i % probColumns;
                            y1 = (i / probColumns) * -1;
                        
                            // Translate current node state index into coordinates
                            x2 = (targetIndex % probColumns);
                            y2 = (targetIndex / probColumns) * -1;

                            // Calculate distance and add to h(n)
                            newNode.h_n += sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
                        }
                    }
                    break;
                }
                default: // Just apply UCS by default
                    newNode.h_n = 0;
                    break;
            }
        }

        // Operators
        // These will check validity of operations and only add valid nodes to the provided frontier
        void blankLeft(SolutionNode& currNode, std::priority_queue<SolutionNode, std::vector<SolutionNode>, SolutionNode>& frontier, std::set<SolutionNode, SolutionNode>& frontierRecord, std::set<SolutionNode, SolutionNode>& explored, int blankIndex) {
            if ((blankIndex + 1) % probColumns != 1) { // Check if blank is already on left edge
                // Create new node
                SolutionNode newNode;
                newNode.state = currNode.state;
                newNode.parent = &currNode;

                // Alter new node's state to reflect changes
                std::iter_swap(newNode.state.begin() + blankIndex, newNode.state.begin() + (blankIndex - 1));
            
                // Calculate new node's heuristic
                calcHeuristic(newNode);

                // Check if node exists in frontierRecord or explored
                if (frontierRecord.count(newNode) == 0 || explored.count(newNode) == 0) {
                    // Push new node to frontier
                    frontier.push(newNode);
                }
            }
        }

        void blankRight(SolutionNode& currNode, std::priority_queue<SolutionNode, std::vector<SolutionNode>, SolutionNode>& frontier, std::set<SolutionNode, SolutionNode>& frontierRecord, std::set<SolutionNode, SolutionNode>& explored, int blankIndex) {
            if ((blankIndex + 1) % probColumns != 0) { // Check if blank is already on right edge
                // Create new node
                SolutionNode newNode;
                newNode.state = currNode.state;
                newNode.parent = &currNode;

                // Alter new node's state to reflect changes
                std::iter_swap(newNode.state.begin() + blankIndex, newNode.state.begin() + (blankIndex + 1));

                // Calculate new node's heuristic
                calcHeuristic(newNode);

                // Check if node exists in frontierRecord or explored
                if (frontierRecord.count(newNode) == 0 || explored.count(newNode) == 0) {    
                    // Push new node to frontier
                    frontier.push(newNode);
                }
            }
        }

        void blankUp(SolutionNode& currNode, std::priority_queue<SolutionNode, std::vector<SolutionNode>, SolutionNode>& frontier, std::set<SolutionNode, SolutionNode>& frontierRecord, std::set<SolutionNode, SolutionNode>& explored, int blankIndex) {
            if ((blankIndex + 1) > probColumns) { // Check if blank is already on top edge
                // Create new node
                SolutionNode newNode;
                newNode.state = currNode.state;
                newNode.parent = &currNode;

                // Alter new node's state to reflect changes
                std::iter_swap(newNode.state.begin() + blankIndex, newNode.state.begin() + (blankIndex - probColumns));

                // Calculate new node's heuristic
                calcHeuristic(newNode);

                // Check if node exists in frontierRecord or explored
                if (frontierRecord.count(newNode) == 0 || explored.count(newNode) == 0) {    
                    // Push new node to frontier
                    frontier.push(newNode);
                }
            }
        }

        void blankDown(SolutionNode& currNode, std::priority_queue<SolutionNode, std::vector<SolutionNode>, SolutionNode>& frontier, std::set<SolutionNode, SolutionNode>& frontierRecord, std::set<SolutionNode, SolutionNode>& explored, int blankIndex) {
            if ((blankIndex + 1) <= (probColumns * (probRows - 1))) { // Check if blank is already on bottom edge
                // Create new node
                SolutionNode newNode;
                newNode.state = currNode.state;
                newNode.parent = &currNode;

                // Alter new node's state to reflect changes
                std::iter_swap(newNode.state.begin() + blankIndex, newNode.state.begin() + (blankIndex + probColumns));

                // Calculate new node's heuristic
                calcHeuristic(newNode);

                // Check if node exists in frontierRecord or explored
                if (frontierRecord.count(newNode) == 0 || explored.count(newNode) == 0) {    
                    // Push new node to frontier
                    frontier.push(newNode);
                }
            }
        }

    public:
        // Constructor
        Problem(std::vector<int> initialState, algorithm heuristic) { 
            this->initialState = initialState; 
            this->heuristic = heuristic; 

            // Fill goal state with sequential values starting with 1 and ending with 0
            int totalSpace = probColumns * probRows;
            int inc = 1;
            for (int i = 0; i < totalSpace-1; ++i) {
                goalState.push_back(inc);
                ++inc;
            }
            goalState.push_back(0);
        }

        // Accessors
        int getColumnCount() { return probColumns; }
        int getRowCount() { return probRows; }
        std::vector<int> getInitialState() { return initialState; }
        std::vector<int> getGoalState() { return goalState; }

        // Expansion Function
        // To be called from driver code
        void expandNode(SolutionNode& currNode, std::priority_queue<SolutionNode, std::vector<SolutionNode>, SolutionNode>& frontier, std::set<SolutionNode, SolutionNode>& frontierRecord, std::set<SolutionNode, SolutionNode>& explored) {
            // Find index of blank space
            int blankIndex = 0;
            for (int i = 0; i < currNode.state.size(); ++i) {
                if (currNode.state[i] == 0) { blankIndex = i; }
            }

            blankLeft(currNode, frontier, frontierRecord, explored, blankIndex);
            blankRight(currNode, frontier, frontierRecord, explored, blankIndex);
            blankUp(currNode, frontier, frontierRecord, explored, blankIndex);
            blankDown(currNode, frontier, frontierRecord, explored, blankIndex);
        }
};
