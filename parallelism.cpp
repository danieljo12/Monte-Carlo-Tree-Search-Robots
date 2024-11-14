#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>

// Define a Node class for MCTS
class Node {
public:
    Node* parent;
    std::vector<Node*> children;
    int visits;
    double value;
    bool isFullyExpanded;

    Node(Node* parent = nullptr) 
        : parent(parent), visits(0), value(0.0), isFullyExpanded(false) {}

    // Add the method to add the child node
    void addChild(Node* child){
        children.push_back(child);
    }

    // In this method, we'll calculate the UCT
    double getUCTValue(double explorationConstant = 1.41){
        if (visits == 0){
            return std::numeric_limits<double>::infinity();
        }

        double exploitation = value / visits;
        double exploration = explorationConstant * std::sqrt(std::long(parent->visits) / visits);
        return exploitation + exploration;
    }
};

// MCTS definition

class MCTS{
public:
    Node* root;
    double explorationConstant;
    int maxIterations;

    MCTS(double explorationConstant = 1.41, int maxIterations = 1000)
        : explorationConstant(explorationConstant), maxIterations(maxIterations){
            root = new Node();
    }


    ~MCTS(){
        deleteTree(root);
    }

    void run(){
        for (int i=0; i<maxIterations; i++){
            Node* selectedNode = selectNode(root);
            double reward = simulate(selectedNode);
            backpropagate(selectedNode, reward);
        }
    }


private:
    // Selecting node using UCT formula
    Node* selectNode(Node* node){
        while(!node->children.empty()){
            node = *std::max_element(node->children.begin(), node->children.end(),
            [](Node* a, Node* b){
                return a->getUCTValue() < b->getUCTValue();
            });
        }

        return node;
    }


    // Simulate and return a reward
    double simulate(Node* node){
        // Implement domain-specific simulation logic here
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0,1);

        return dis(gen); // Random between 0 and 1
    }

    // Reward backpropagation
    void backpropagate(Node* node, double reward){
        while(node != nullptr){
            node->visits++;
            node->value += reward;
            node = node->parent;
        }
    }

    // Delete memory
    void deleteTree(Node* node){
        for(Node* child : node->children){
            deleteTree(child);
        }

        delete node;
    }
};


int main(){
    MCTS mcts;
    mcts.run();

    std::cout << "MCTS execution completed." << std::endl;
    return 0;
}