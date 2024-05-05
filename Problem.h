#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

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
    int h_n;
};

struct CompareNode {
    bool operator() (SolutionNode const& n1, SolutionNode const& n2) {
        return (n1.g_n + n1.h_n) + (n2.g_n + n2.h_n);
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
                    
                    // TODO

                    break;
                case AED: // A* with Euclidean Distance
                    
                    // TODO

                    break;
                default: // Just apply UCS by default
                    newNode.h_n = 0;
                    break;
            }
        }

        // Operators
        // These will check validity of operations and only add valid nodes to the provided frontier
        void blankLeft(SolutionNode& currNode, std::priority_queue<SolutionNode, std::vector<SolutionNode>, CompareNode>& frontier, int blankIndex) {
            if ((blankIndex + 1) % probColumns != 1) { // Check if blank is already on left edge
                // Create new node
                SolutionNode newNode;
                newNode.state = currNode.state;
                newNode.parent = &currNode;

                // Alter new node's state to reflect changes
                std::iter_swap(newNode.state.begin() + blankIndex, newNode.state.begin() + (blankIndex - 1));
            
                // Calculate new node's heuristic
                calcHeuristic(newNode);

                // Push new node to frontier
                frontier.push(newNode);
            }
        }

        void blankRight(SolutionNode& currNode, std::priority_queue<SolutionNode, std::vector<SolutionNode>, CompareNode>& frontier, int blankIndex) {
            if ((blankIndex + 1) % probColumns != 0) { // Check if blank is already on right edge
                // Create new node
                SolutionNode newNode;
                newNode.state = currNode.state;
                newNode.parent = &currNode;

                // Alter new node's state to reflect changes
                std::iter_swap(newNode.state.begin() + blankIndex, newNode.state.begin() + (blankIndex + 1));

                // Calculate new node's heuristic
                calcHeuristic(newNode);

                // Push new node to frontier
                frontier.push(newNode);
            }
        }

        void blankUp(SolutionNode& currNode, std::priority_queue<SolutionNode, std::vector<SolutionNode>, CompareNode>& frontier, int blankIndex) {
            if ((blankIndex + 1) > probColumns) { // Check if blank is already on top edge
                // Create new node
                SolutionNode newNode;
                newNode.state = currNode.state;
                newNode.parent = &currNode;

                // Alter new node's state to reflect changes
                std::iter_swap(newNode.state.begin() + blankIndex, newNode.state.begin() + (blankIndex - probColumns));

                // Calculate new node's heuristic
                calcHeuristic(newNode);

                // Push new node to frontier
                frontier.push(newNode);
            }
        }

        void blankDown(SolutionNode& currNode, std::priority_queue<SolutionNode, std::vector<SolutionNode>, CompareNode>& frontier, int blankIndex) {
            if ((blankIndex + 1) <= (probColumns * (probRows - 1))) { // Check if blank is already on bottom edge
                // Create new node
                SolutionNode newNode;
                newNode.state = currNode.state;
                newNode.parent = &currNode;

                // Alter new node's state to reflect changes
                std::iter_swap(newNode.state.begin() + blankIndex, newNode.state.begin() + (blankIndex + probColumns));

                // Calculate new node's heuristic
                calcHeuristic(newNode);

                // Push new node to frontier
                frontier.push(newNode);
            }
        }

    public:
        // Constructor
        Problem(std::vector<int> initialState, algorithm heuristic) { 
            this->initialState = initialState; 
            this->heuristic = heuristic; 

            // Fill goal state with sequential values starting with 1 and ending with 0
            int totalSpace = probColumns * probRows;
            goalState.reserve(totalSpace);
            std::iota(goalState.begin(), goalState.end()-1, 1);
            goalState[goalState.size()-1] = 0;
        }

        // Accessors
        std::vector<int> getInitialState() { return initialState; }
        std::vector<int> getGoalState() { return goalState; }

        // Expansion Function
        // To be called from driver code
        void expandNode(SolutionNode& currNode, std::priority_queue<SolutionNode, std::vector<SolutionNode>, CompareNode>& frontier) {
            // Find index of blank space
            int blankIndex = 0;
            for (int i = 0; i < currNode.state.size(); ++i) {
                if (currNode.state[i] == 0) { blankIndex = i; }
            }

            blankLeft(currNode, frontier, blankIndex);
            blankRight(currNode, frontier, blankIndex);
            blankUp(currNode, frontier, blankIndex);
            blankDown(currNode, frontier, blankIndex);
        }
};
