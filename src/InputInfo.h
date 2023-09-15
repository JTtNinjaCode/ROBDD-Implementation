#ifndef INPUTINFO_H
#define INPUTINFO_H
#include <iostream>
#include <string>
#include <vector>

struct InputInfo {
    unsigned int inputCount;
    unsigned int outputCount;
    unsigned int termsCount;
    std::vector<std::string> inputNames;
    std::vector<std::string> outputNames;
    std::vector<bool> termResults;
};

std::istream& operator>>(std::istream& in, InputInfo& inputInfo);
#endif