#include "OutputInfo.h"
#include <vector>
#include <queue>
#include <list>
#include <unordered_set>

OutputInfo::OutputInfo(const Function &function) {
    std::shared_ptr<Node> root = function.root;
    std::vector<std::vector<int>> levelTable(function.inputInfo.inputCount + 1);
    bfs(root, levelTable);
    // Levels Output
    for (auto level: levelTable) {
        std::string levelInfo;
        levelInfo += "{rank=same";
        for (auto id: level) {
            levelInfo += ' ' + std::to_string(id);
        }
        levelInfo += "}";
        depthOutputs.push_back(levelInfo);
    }
}

std::ostream &operator<<(std::ostream &out, const OutputInfo &outputInfo) {
    out << "digraph ROBDD {\n";
    for (auto str: outputInfo.depthOutputs) {
        out << '\t' << str << '\n';
    }
    out <<'\n';
    for (auto str: outputInfo.nodeOutputs) {
        out << '\t' << str << '\n';
    }
    out <<'\n';
    for (auto str: outputInfo.edgeOutputs) {
        out << '\t' << str << '\n';
    }
    out << "}";
    return out;
}

void OutputInfo::bfs(std::shared_ptr<Node> root, std::vector<std::vector<int>> &levelTable) {
    std::unordered_set<std::shared_ptr<Node> , Node::PointerHash> nodeSet;
    std::queue<std::shared_ptr<Node> , std::list<std::shared_ptr<Node> >> nodeQueue;
    nodeQueue.push(root);
    do {
        auto currentNode = nodeQueue.front();

        // Levels Output
        levelTable[currentNode->level].push_back(currentNode->id);

        // nodeQueue Output
        std::string nodeInfo;
        nodeInfo += std::to_string(currentNode->id) + ' ';
        nodeInfo += "[";
        nodeInfo +="label=\"" + currentNode->label + '\"';
        if (!currentNode->shape.empty()) {
            nodeInfo += ", shape=" + currentNode->shape;
        }
        nodeInfo += "];";
        nodeOutputs.push_back(nodeInfo);

        // Edges Output
        if (currentNode->left) {
            std::string edgeInfo;
            edgeInfo += std::to_string(currentNode->id) + " -> " + std::to_string(currentNode->left->id) + ' ';
            edgeInfo += "[";
            edgeInfo += "label=\"0\"";
            edgeInfo += ", style=dotted";
            edgeInfo += "];";
            edgeOutputs.push_back(edgeInfo);
            if (nodeSet.count(currentNode->left) == 0) {
                nodeSet.insert(currentNode->left);
                nodeQueue.push(currentNode->left);
            }
        }
        if (currentNode->right) {
            std::string edgeInfo;
            edgeInfo += std::to_string(currentNode->id) + " -> " + std::to_string(currentNode->right->id) + ' ';
            edgeInfo += "[";
            edgeInfo += "label=\"1\"";
            edgeInfo += ", style=solid";
            edgeInfo += "];";
            edgeOutputs.push_back(edgeInfo);
            if (nodeSet.count(currentNode->right) == 0) {
                nodeSet.insert(currentNode->right);
                nodeQueue.push(currentNode->right);
            }
        }
        nodeQueue.pop();
    } while(!nodeQueue.empty());
}