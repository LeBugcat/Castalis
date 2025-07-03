//==================================================================
// File			: main.cpp
// Author		: Rebecca Sagalyn
// Date			: Aug 25, 2013
// Description	: Driver for tsp.h
//==================================================================
#include <iostream>
#include <climits>
#include "helpStructures/TSP/tsp.h"
#include "helpStructures/TSP/usage.h"
#include "helpStructures/TSP/twoOpt.h"
#include "helpStructures/TSP/MyThread.h"		// thread wrapper class
// The length was annoying me.
#define CPS CLOCKS_PER_SEC

#define NUM_THREADS 1

vector<int> mainTSP(vector<int> landmarks);
