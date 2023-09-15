#ifndef OUTPUTINFO_H
#define OUTPUTINFO_H
#include <ostream>
#include <vector>
#include "Function.h"

struct OutputInfo{
    std::vector<std::string> depthOutputs;
    std::vector<std::string> nodeOutputs;
    std::vector<std::string> edgeOutputs;

    OutputInfo(const Function &function);
private:
    void bfs(std::shared_ptr<Node> root, std::vector<std::vector<int>> &levelTable);
};

std::ostream &operator<<(std::ostream &out, const OutputInfo &outputInfo);
#endif