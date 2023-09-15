#include <fstream>
#include "InputInfo.h"
#include "OutputInfo.h"
#include "Function.h"
#include "ROBDD.h"

int main(int argc, char **argv){
    std::ifstream ifs(argv[1]);
    InputInfo inputInfo;
    ifs >> inputInfo;

    Function function = buildBDD(inputInfo);
    reduceFunction(function);
    OutputInfo outputInfo(function);
    std::ofstream ofs(argv[2]);
    ofs << outputInfo;
}