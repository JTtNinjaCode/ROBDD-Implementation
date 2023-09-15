#ifndef ROBDD_H
#define ROBDD_H

#include "InputInfo.h"
#include "Function.h"

Function buildBDD(const InputInfo &inputInfo);
void reduceFunction(Function &function);

#endif