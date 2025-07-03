//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef NODE_COMMUNICATION_ROUTING_CONVERGECAST_CONVERGECASTROUTING_H_
#define NODE_COMMUNICATION_ROUTING_CONVERGECAST_CONVERGECASTROUTING_H_

#include "../VirtualRouting.h"
#include "./ConvergecastPacket_m.h"
#include "helpStructures/RoundTimerService.h"
#include "node/application/simpleAggregation/SimpleAggregation.h"
#include "node/communication/mac/VirtualMac.h"

#include <vector>
#include <string>

using namespace std;

struct CHInfo
{
    int src;
    double rssi;
};

struct NeighborInfo
{
    int id;
    double energy;
};

enum RoundTimers{
    START_ROUND,   //for all
    START_MAINALG, //for sink
    END_MAINAlG,   //for sink
    POST_ALG_SETUP,//for all
    SEND_DATA_UAV, //to UAV
    DEBUG_TRAP_    //not recommended
};

struct NetworkConfig
{
    NetworkConfig() {};
    NetworkConfig(int numNodes, int numUAVs) {
        clus_id.resize(numNodes, -1);
        trajectories.resize(numUAVs);
    };
    //deep cloning of this Object
//    NetworkConfig(NetworkConfig& original){
//        this->A=original.A;
//        this->clus_id=original.clus_id;
//        this->nextHop=original.nextHop;
//        this->trajectories=original.trajectories;
//    }

    void save(vector<int> A_, vector<int> clus_id_, vector<int> nextHop_, vector<vector<int>> trajectories_) {
        A = A_;
        clus_id = clus_id_;
        nextHop = nextHop_;
        trajectories = trajectories_;
    }

    void save(NetworkConfig config) {
        A = config.A;
        clus_id = config.clus_id;
        nextHop = config.nextHop;
        trajectories = config.trajectories;
        childrenList = config.childrenList;
    }

    void clear() {
        A.clear();
        int numNodes = clus_id.size();
        int numUAVs = trajectories.size();
        clus_id.clear();
        trajectories.clear();
        clus_id.resize(numNodes, -1);
        trajectories.resize(numUAVs);
    }

    bool isCH(int nodeId){
        for(int i=0; i<clus_id.size(); i++){
            if(nodeId == clus_id[i]) return true;
        }
        return false;
    }
    vector<int> getChildren_(int nodeId){
        vector<int> children;
        for (int i = 0; i < nextHop.size(); ++i) {
            if (nextHop[i] == nodeId) {
                children.push_back(i); // Add node i if its parent is nodeId
            }
        }
        return children;
    }

    vector<int> getChildren(int nodeId){
        return childrenList[nodeId];
    }

    //TODO: This is only "size" as if every nodes is equal!
    //      There must be another way to make this adaptable to hetero traffic
    int getSubtreeSize(int nodeId){
        int res=1;
        if(childrenList[nodeId].size()==0) return res;
        else {
            for(int childId: childrenList[nodeId]){
                res+=getSubtreeSize(childId);
            }
        }
        return res;
    }

    vector<int> A;
    vector<int> clus_id;
    vector<int> nextHop;
    vector<vector<int>> trajectories;
    vector<std::vector<int>> childrenList;
};

class ConvergecastRouting: public VirtualRouting {

public:
    static vector<double> *dCompare;
    static vector<double> dataPacketSizeAt;
    static bool initialized;

    static bool routingTreeReady;
    static bool bypassRoutingBuffer;

    static NetworkConfig config;
    static unordered_set<int> uncollectedSet;
    static vector<int> collectedSet;

    static vector<bool> lastRoundCollectedNode;
    static vector<int> lastRoundCollectedNodeId;
    static vector<int> lastRoundCollectedChildren; //count

    /*--- The .ned file's parameters ---*/
    static double totalConsumed;
    static double maxConsumed;
    static vector<double> energyConsumeds;
    static vector<double> rxSizes;

    static int totalCollected;
//    static bool allowIntraClusterMultiPath;

    double sensibility;
    double aggrConsumption;

    vector<NeighborRecord> neighborTable;
    vector<int> powers;
    Point selfLocation;

    static int sinkId;
    bool isSink;
    double roundLength;
    int roundNumber;

    vector<int> A;
    vector<int> clusId;
    vector<int> nextHop;
    ////////////////////CONVERGECAST/////////////////////////////////////
    vector<vector<int>> trajectories;
    double maxUAVFlyingDistance;
    double offsetSendToUAV;

    //all of these are per-round
    //    int metricsInboundPktsTotal_round; //only successful packets
    //    int metricsOutboundPktsTotal_round; //only sent to MAC packets
    //    int metricsGeneratedPktsTotal_round; //Application generated packets
    //    int metricsForwardedPktsTotal_round; //forwarded packets

    //    int metricsMismatchCHPktsTotal_round; //non-forwarded, discarded packets

    //TODO: move this into the formal Castalia output way
    int metricsOverhearing_round; //overheard packets not for this node, we capture it anyways

    //////CH metrics: packets arrived at CH (mini-sink), indiscriminate of origins
    //    int metricsPktsAtThisCH;

    //    double metricsCurrentRoundDatarate;
    //    double metricsNodeEnergyConsumation;
    //

    //these are End-Of-Sim metrics
    //    double metricsAvgEnergyConsumation;
    //    double metricsLowestRmnEnergy;

    SimpleAggregation* appModule;
    VirtualMac* macModule;

    vector<int> getNeighborList();
    double getNeighborRange(){
        return this->neighborRange;
    };

 protected:
    string debugString;

    //TODO: throw this into a formal Castalia output
    //TODO: add this to other algorithms, this lives in the base GAMBAC class

    chrono::duration<double> timeElapse_round;

    double maxLengthRatio;
    int numPacketReceived;
    int endCount;

    int numNodes;
    int numUAVs;

    //TODO: neighborRange lives here, for GAMBAC family of algorithm
    double neighborRange;

    virtual void protocolInitialize(){};
    virtual void initialize(int);
    int numInitStages() const {
        return 3;
    };

    void timerFiredCallback(int);
    void startRoundCallback(){};
    virtual void mainAlg(){
        throw cRuntimeError("Main algorithm handler not implemented");
    };

    virtual void startup(){
        setTimer(START_ROUND, 0.1);
    };

    //packets handling logics
    void fromApplicationLayer(cPacket *, const char *);
    void fromMacLayer(cPacket *, int, double, double);

    void forwardPacketToNextHop(cPacket *, cPacket *);
    void toRoutingLayerDirect(cPacket *, int);

    int getRandomNumber(int from, int to);

    bool reached(Point location);

    double txEnergy(double, double);
    double rxEnergy(double);
    double calculateCHEnergy(int, double);
    ResourceManager* getResMgrModule(int);
    double estimateMaxWeight(double);

    Point location(int id);
    double distance(int u, int v) {
        if (u == v) return 0;
        return G::distance(location(u), location(v));
    }

    double calculateRxSize(int i);
    double calculatePathLength(vector<int>);
    map<int, vector<int>> kMeansClustering(vector<int> nodes, int k);

    void sendDataToUAV(cPacket*);


    //TODO: all of these are metrics-log-level,
    // convert them into Castalia outputs instead of traces
    // it optimize how we collect the metrics after each simulation runs

    //logging per-round
    void endOfAlgoLogging();
    void singleRoundTopoLog();
    void calculateLastRoundNetCoverage();
    double lastRoundNetCoverage;

    void singleRoundMetricLog(std::string, std::string);
    void singleRoundMetricLog(std::string, int);
    void singleRoundMetricLog(std::string, double);
    void singleRoundMetricLog(std::string, vector<int>);
};

#endif /* NODE_COMMUNICATION_ROUTING_CONVERGECAST_CONVERGECASTROUTING_H_ */
