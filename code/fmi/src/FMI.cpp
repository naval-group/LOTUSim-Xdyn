/*
 * FMI.cpp
 *
 *  Created on: May 12, 2015
 *      Author: cady
 */

#include "FMI.hpp"

#define MODEL_IDENTIFIER Simulator
extern "C"
{
    #include "fmiModelFunctions.h"
    #include "fmiModelTypes.h"
}

const char* fmiGetModelTypesPlatform()
{
    return fmiModelTypesPlatform;
}

const char* fmiGetVersion()
{
    return fmiVersion;
}

