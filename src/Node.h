#ifndef NODE_H
#define NODE_H

#include <string>
#include <memory>
#include <iostream>

struct Node {
    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;
    std::string label;
    unsigned int id = 0;
    unsigned int level = 0;
    std::string shape;

    Node(const std::string &label, unsigned int id, unsigned int level, const std::string &shape = ""):
        label(label),
        id(id),
        level(level),
        shape(shape) {
    }
    
    bool operator==(const Node &node) const {
        if(label == node.label && left == node.left && right == node.right) {
            return true;
        }
        return false;
    }

    struct Hash {
        size_t operator()(const Node& node) const {
            int hashValue = std::hash<std::string>{}(node.label) + std::hash<std::shared_ptr<Node>>{}(node.left) + std::hash<std::shared_ptr<Node>>{}(node.right);
            return hashValue;
        }
    };

    struct PointerEqual
    {
        bool operator()(std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs ) const {
            return *lhs == *rhs;
        }
    };

    struct PointerHash
    {
        size_t operator()(std::shared_ptr<Node> node) const {
            return Node::Hash{}(*node);
        }
    };
};

#endif