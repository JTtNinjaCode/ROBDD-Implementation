
#include <sstream>
#include <cassert>
#include <stdexcept>
#include "InputInfo.h"

static void writeResult(std::vector<bool> &results, const std::string &term, const std::string &value, unsigned int index, unsigned int termValue);

std::istream& operator>>(std::istream& in, InputInfo& inputInfo) {
    while(true){
        std::string token;
        in >> token;
        if (token == ".i"){
            std::string value;
            in >> value;
            inputInfo.inputCount = std::stoi(value);
        } else if (token == ".o") {
            std::string value;
            in >> value;
            inputInfo.outputCount = std::stoi(value);
        } else if (token == ".ilb") {
            for (size_t i = 0; i < inputInfo.inputCount; i++) {
                std::string value;
                in >> value;
                inputInfo.inputNames.push_back(value);
            }
        } else if (token == ".ob") {
            for (size_t i = 0; i < inputInfo.outputCount; i++) {
                std::string value;
                in >> value;
                inputInfo.outputNames.push_back(value);
            }
        } else if (token == ".p") {
            std::string value;
            in >> value;
            inputInfo.termsCount = std::stoul(value);
            inputInfo.termResults.resize(1 << inputInfo.inputCount);
            for (size_t i = 0; i < inputInfo.termsCount; i++) {
                std::string value[2];
                in >> value[0] >> value[1];
                writeResult(inputInfo.termResults, value[0], value[1], 0, 0);
            }
        } else if (token == ".e"){
            return in;
        } else {
            throw std::runtime_error("[Runtime Error]Unknown Input\'s Token:" + token);
        }
    }
    return in;
}

static void writeResult(std::vector<bool> &results, const std::string &term, const std::string &value, unsigned int termIndex, unsigned int resultIndex) {
    if (termIndex < term.length()){
        char token = term[termIndex];
        if (token == '1') {
            writeResult(results, term, value,  termIndex + 1, (resultIndex << 1) + 1);
        } else if (token == '0') {
            writeResult(results, term, value,  termIndex + 1, (resultIndex << 1) + 0);
        } else if (token == '-') {
            std::string newTerm = term;
            newTerm[termIndex] = '0';
            writeResult(results, term, value,  termIndex + 1, (resultIndex << 1) + 1);
            newTerm[termIndex] = '1';
            writeResult(results, term, value,  termIndex + 1, (resultIndex << 1) + 0);
        } else {
            throw std::runtime_error("[Runtime Error]Unknown Term\'s Token:" + token);
        }
    } else {
        results[resultIndex] = std::stoi(value);
    }
}
