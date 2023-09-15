#include "ROBDD.h"
#include "Node.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

static void build(std::shared_ptr<Node> node, unsigned int depth, unsigned int termValue, const InputInfo &inputInfo);
static void fillNodes(std::shared_ptr<Node> root, std::vector<std::shared_ptr<Node>> &nodes);
static void fillEdges(std::shared_ptr<Node> node, std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<Node>> &edgeSet);

Function buildBDD(const InputInfo &inputInfo){
    std::shared_ptr<Node> root = std::make_shared<Node>(inputInfo.inputNames[0], 0, 0);
    Function function(inputInfo.outputNames[0], inputInfo, root); // assume only one output
    build(root, 0, 0, inputInfo);

    return function;
}

void reduceFunction(Function &function){
    std::vector<std::shared_ptr<Node>> nodes;
    fillNodes(function.root, nodes);

    std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<Node>> edgeSet;
    fillEdges(function.root, edgeSet);

    for(int i = function.inputInfo.inputCount; i > 0; i--){
        std::unordered_set<std::shared_ptr<Node>, Node::PointerHash, Node::PointerEqual> set;
        for (int j = 0; j < (1 << i); j++) {
            auto currentNode = nodes.back();
            nodes.pop_back();
            // condition 1
            if (currentNode->left == currentNode->right && currentNode->left != nullptr) { // left and right must be or not be nullptr at same time
                auto parent = edgeSet[currentNode];
                if (parent->left == currentNode){
                    parent->left = currentNode->left;
                }
                if (parent->right == currentNode){
                    parent->right = currentNode->right;
                }
                continue;
            }
            // condition 2
            auto primeNode = set.find(currentNode);
            if (primeNode == set.end()) {
                set.insert(currentNode);
            } else {
                auto parent = edgeSet[currentNode];
                if (parent->left == currentNode){
                    parent->left = *primeNode;
                }
                if (parent->right == currentNode) {
                    parent->right = *primeNode;
                }
            }
        }
    }

    if (function.root->left == function.root->right) { // edge case: root
        function.root = function.root->left; // left and right must be or not be nullptr at same time
    }
}

void build(std::shared_ptr<Node> node, unsigned int depth, unsigned int termValue, const InputInfo &inputInfo){
    unsigned int newDepth = depth + 1;
    unsigned int leftTermValue = termValue * 2 + 1;
    unsigned int rightTermValue = termValue * 2 + 2;
    if (depth < inputInfo.inputCount - 1) {
        node->left = std::make_shared<Node>(inputInfo.inputNames[newDepth], leftTermValue, newDepth);
        build(node->left, newDepth, leftTermValue, inputInfo);
        node->right = std::make_shared<Node>(inputInfo.inputNames[newDepth], rightTermValue, newDepth);
        build(node->right, newDepth, rightTermValue, inputInfo);
    } else {
        unsigned int offset = (1 << inputInfo.inputCount) - 1;
        if (inputInfo.termResults[leftTermValue - offset]) {
            node->left = std::make_shared<Node>("1", leftTermValue, newDepth, "box");
        } else {
            node->left = std::make_shared<Node>("0", leftTermValue, newDepth, "box");
        }
        if (inputInfo.termResults[rightTermValue - offset]) {
            node->right = std::make_shared<Node>("1", rightTermValue, newDepth, "box");
        } else {
            node->right = std::make_shared<Node>("0", rightTermValue, newDepth, "box");
        }
    }
}

static void fillNodes(std::shared_ptr<Node> root, std::vector<std::shared_ptr<Node>> &nodes){
    nodes.push_back(root);
    unsigned int index = 0;
    auto currentNode = *nodes.begin();
    while (currentNode->left != nullptr){ // left and right must be or not be nullptr at same time
        nodes.push_back(nodes[index]->left);
        nodes.push_back(nodes[index]->right);
        currentNode = nodes[++index];
    }
}

void fillEdges(std::shared_ptr<Node> node, std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<Node>> &edgeSet) {
    if (node->left == nullptr) // left and right must be or not be nullptr at same time
        return;
    edgeSet[node->left] = node;
    fillEdges(node->left, edgeSet);
    edgeSet[node->right] = node;
    fillEdges(node->right, edgeSet);
}