#include "globals.h"

VM vm;

bool VERBOSE = false;

bool verbose()
{
    return VERBOSE;
}

void setVerbose(bool value)
{
    VERBOSE = value;
}