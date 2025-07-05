//****************************************************************************
//*  Copyright (c) Federal University of Para, brazil - 2011                 *
//*  Developed at the Research Group on Computer Network and Multimedia      *
//*  Communication (GERCOM)                                  *
//*  All rights reserved                                         *
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

#ifndef _GAMBAC_H_
#define _GAMBAC_H_

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

#include "mainTSP.h"
#include "lib/VRPTW/Vrptw.h"
#include "lib/TSPGA/mainTspga.h"

#include "./GAMBACPacket_m.h"
#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/convergecast/ConvergecastRouting.h"
#include "node/application/VirtualApplication.h"
#include "node/application/simpleAggregation/SimpleAggregation.h"
#include "node/application/simpleAggregation/HeterogeneousTraffic/HeterogeneousTraffic.h"
#include "wirelessChannel/defaultChannel/WirelessChannel.h"
#include "node/application/ApplicationPacket_m.h"
#include "node/mobilityManager/noMobilityManager/NoMobilityManager.h"
#include "helpStructures/GeoMathHelper.h"
#include "helpStructures/GlobalLocationService.h"
#include "helpStructures/UDG.h"


using namespace std;

class GAMBAC : public ConvergecastRouting {

public:

    ////////////////////vars for the ConvergeCast base class/////////////////
    string applicationID;
    SimpleAggregation* smplAggAppModule; //for infering operational data
    double dataPacketSize;
//    static bool scheduled;
    static bool bypassRoutingBuffer;
//    double neighborRange;
    //if set to true the routing MUST deliver the App packet
    //immediately to the MAC layer buffer
    ////////////////////vars for the Convergecast base class/////////////////

    queue <cPacket*> tempTXBuffer;
    vector <GAMBACPacket> bufferAggregate;
    vector<double> distanceToCH;
    ////////////////////////////////////////////////
//    void timerFiredCallback(int);

protected:

    UDG connectivityGraph;
    int N;
    int k0;

    int nloop;
    int countSuccess;
    int maxCHperUAV;
    int countBufferedPacketsForUAV;

    map<int, double> ballWeight;
    double maxThres;
    double epsilon;
    double Eavg = 0;
    vector<list<int>> clusterMembers;
    double maxBallWeight;
    double minBallWeight;
    unordered_set<int> rmSet;
    vector<list<int>> centList;
    vector<bool> isCH;
    vector<int> outerSet;
    vector<int> innerSet;
    int countAdjusment;
    bool debugRecruitProcess;

    double maxClusterVolume;
    double clusterVolume_optimal;
    double clusterVolume_start;
    double clusterVolume_end;
    vector<double> w_max;
    double E_opt;
    double E_max;
    double E_min;
    double E0_min;
    double w_min;
    double dataVolume_total;

    double gamma;
    vector<double> remainingEnergy_tmp;



//////////////////////////////////////////////////

    ///////////// LIFECYCLE AND TIMECYCLE
    // void startup(); //deprecating this since it runs everytime nodes wakes up
    void protocolInitialize();
    void processBufferedPacket();


//    void packageAndBuffer(cPacket*, cPacket*);


    /////////////THESE LOOKS LIKE ALGO-RELATED AND SHOULD BE AT LOWER LAYER
    void startRoundCallback();
    void reset();
    void mainAlg();//The algorithm loop
    void GAMBACinit(); // the initializer
    /////////////THESE LOOKS LIKE ALGO-RELATED AND SHOULD BE AT LOWER LAYER

    ///////////////////////////////////////////////////////////////

    vector<int> samplingCH(vector<int> Candidates, double b);
    vector<int> randomFromSet(vector<int> Candidates, double s);
    void recruitNewCHs();
    int selectCHsAndBuildDFT(double W);

    // The in-network routing loop
    void growBalls(vector<int> CHSet);
    int growBallsV2(vector<int> CHSet);
    double computeClusterWeight(int uNode);
    vector<int> getOuterOversizePart();
    void computeBallWeight();
    void updateNodeWeight();
    void updateCentList();

    // The outer UAV checking and CH count update loop
    void buildTrajectories();
//    void buildTrajectories(bool isBreak);
    vector<vector<int>> buildInitialTrajectories();
    vector<int> TSP(vector<int>);
    vector<int> sortedCHVector();
    double computeWeight(list<int>);
};

#endif
