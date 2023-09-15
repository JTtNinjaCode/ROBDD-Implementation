#ifndef FUNCTION_H
#define FUNCTION_H

#include "Node.h"
#include "Function.h"
#include "InputInfo.h"

struct Function {
    std::string label;
    std::shared_ptr<Node>root = nullptr;
    const InputInfo &inputInfo;

    Function(const std::string &label, const InputInfo &inputInfo, std::shared_ptr<Node>root = nullptr):
        label(label),
        inputInfo(inputInfo),
        root(root)
    {}
};

#endif