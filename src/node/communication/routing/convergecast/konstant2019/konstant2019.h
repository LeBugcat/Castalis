//****************************************************************************

//*  Copyright (c) Federal University of Para, brazil - 2011                 *
//*  Developed at the Research Group on Computer Network and Multimedia      *
//*  Communication (GERCOM)     				             *
//*  All rights reserved    				                     *
//*                                                                          *
//*  Permission to use, copy, modify, and distribute this protocol and its   *
//*  documentation for any purpose, without fee, and without written         *
//*  agreement is hereby granted, provided that the above copyright notice,  *
//*  and the author appear in all copies of this protocol.                   *
//*                                                                          *
//*  Module:   LEACH Clustering Protocol for Castalia Simulator              *
//*  Version:  0.2                                                           *
//*  Author(s): Adonias Pires <adonias@ufpa.br>                              *
//*             Claudio Silva <claudio.silva@itec.ufpa.br>                   *
//****************************************************************************/

#ifndef _konstant2019_H_
#define _konstant2019_H_

#include <queue>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <omnetpp.h>
#include <algorithm>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "mainTSP.h"
#include "lib/TSPGA/mainTspga.h"

#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/convergecast/ConvergecastRouting.h"
#include "node/application/VirtualApplication.h"
#include "node/communication/routing/convergecast/konstant2019/konstant2019Packet_m.h"
#include "node/application/ApplicationPacket_m.h"
#include "node/mobilityManager/noMobilityManager/NoMobilityManager.h"
#include "helpStructures/GeoMathHelper.h"
#include "helpStructures/GlobalLocationService.h"
#include "helpStructures/UDG.h"
// #include "mainVrptw.h"

using namespace std;

//struct CHInfo
//{
//	int src;
//	double rssi;
//};
//
//struct NeighborInfo
//{
//	int id;
//	double energy;
//};

class konstant2019 : public ConvergecastRouting {

private:
	
	string applicationID;	

	queue <cPacket *> tempTXBuffer;
	vector <konstant2019Packet> bufferAggregate;
	double dataPacketSize;
    int countBufferedPacketsForUAV;

	////////////////////////////////////////////////

	UDG graph;
    int N;
//	double neighborRange;

	stringstream ss0;
	
    vector<double> distanceToCH;
	vector<list<int>> clusterMembers;
    double maxBallWeight;
    double minBallWeight;
    unordered_set<int> rmSet;
    vector<bool> isCH;

	double E_min;

	vector<double> E0;
	vector<double> E_tmp;

//////////////////////////////////////////////////


protected:

	void startup();

	void konstantInit();
	void reset();
	void mainAlg();

	//ALL ALGO-RELATED for ROUTING PROTOCOL
	vector<vector<int>> partitionIntoSectors();
	vector<vector<int>> basicToursPlanning();
	void findEnergyEfficientSolution();
	pair<double,string> calculateConsumption(vector<vector<int>> tours);
	void clusterTreeBuilding(vector<int> A);
};

#endif			
