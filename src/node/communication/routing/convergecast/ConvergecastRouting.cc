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

#include "ConvergecastRouting.h"

bool ConvergecastRouting::initialized = false;

// this is called after mainAlg
bool ConvergecastRouting::routingTreeReady=false;
bool ConvergecastRouting::bypassRoutingBuffer=true;

int ConvergecastRouting::sinkId;
int ConvergecastRouting::totalCollected;

vector<bool> ConvergecastRouting::lastRoundCollectedNode;
vector<int> ConvergecastRouting::lastRoundCollectedChildren;
vector<int> ConvergecastRouting::lastRoundCollectedNodeId;

double ConvergecastRouting::totalConsumed;
double ConvergecastRouting::maxConsumed;

vector<double> ConvergecastRouting::dataPacketSizeAt;
vector<double>* ConvergecastRouting::dCompare;
vector<double> ConvergecastRouting::energyConsumeds;
vector<double> ConvergecastRouting::rxSizes;

unordered_set<int> ConvergecastRouting::uncollectedSet;
vector<int> ConvergecastRouting::collectedSet;
NetworkConfig ConvergecastRouting::config;

Define_Module(ConvergecastRouting);

void ConvergecastRouting::initialize(int stage)
{
  if (stage==0) {
    initialized = true;
    srand(time(NULL)); // this seeds the rand function,
    VirtualRouting::initialize(0);
    //so that runs will be different although same config
  }
  if (stage==1){
    VirtualRouting::initialize(1);
    //legacy Virtual Routing vars
    numNodes = getParentModule()->getParentModule()->getParentModule()->par("numNodes");

    //only for algos where there are UAVs-assistance
    if(hasPar("maxUAVFlyingDistance"))
        maxUAVFlyingDistance = par("maxUAVFlyingDistance");

    if(hasPar("numUAVs"))
        numUAVs = getParentModule()->getParentModule()->getParentModule()->par("numUAVs");

    //enhanced vars: for timed rounds and centralized routing
    if(hasPar("isSink")){
        isSink = par("isSink");
        sinkId = self;
    }
    roundNumber=0;
    if(hasPar("roundLength"))
        roundLength = par("roundLength");
    else roundLength=50; //must have var

    //packet and tracing within round (presumably) //TODO: Use the new declareOutput() for this
    numPacketReceived = 0;
    metricsOverhearing_round=0;

    if (isSink) {
        sinkId = self;
        energyConsumeds.resize(numNodes);
        rxSizes.resize(numNodes);
        lastRoundCollectedNode.resize(numNodes, false);
        lastRoundCollectedChildren.resize(numNodes,0);
    }

    //UAV-related variables parsing
    offsetSendToUAV = par("offsetSendToUAV");

    //TODO: remove this feature, as it is related to the app Layer
    //only when more comfortable
    if (dataPacketSizeAt.empty()) dataPacketSizeAt.resize(numNodes);

    if(hasPar("dataPacketSize"))
        dataPacketSizeAt[self] = par("dataPacketSize");

    if(hasPar("dataScaleFactor")){
        int dataScaleFactor = par("dataScaleFactor");
        dataPacketSizeAt[self] = dataPacketSizeAt[self]*4*dataScaleFactor;
        dataPacketSizeAt[self] = 8;
    }
  }
  if (stage==2){
      WirelessChannel* tempWc = check_and_cast<WirelessChannel*>(getParentModule()->getParentModule()->getParentModule()->getSubmodule("wirelessChannel"));
      neighborRange = tempWc->transmissionRange-5;

      GlobalLocationService::setRange(tempWc->transmissionRange);
      GlobalLocationService::initialize(getParentModule()->getParentModule()->getParentModule());
      neighborTable = GlobalLocationService::getNeighborTable(self);

      if(neighborTable.size()==0) {
          routingTrace() << "ERROR: Infeasible logic: Isolated Node";
          throw cRuntimeError("Infeasible logic: Isolated Node");
      }

      //initialize trafic-related variables
      appModule = check_and_cast<SimpleAggregation*>(getParentModule()->getParentModule()->getSubmodule("Application"));
      macModule = check_and_cast<VirtualMac*>(getParentModule()->getSubmodule("MAC"));

      dataPacketSizeAt[self] = appModule->currentDataPktSize;

      // This runs at the latest stage, the 2nd stage
      protocolInitialize();
  }
}

vector<int> ConvergecastRouting::getNeighborList(){
    vector<int> res;
    for(int nbId: GlobalLocationService::getNeighborIndex(this->self)){
        res.push_back(nbId);
    }
    return res;
};

void ConvergecastRouting::fromApplicationLayer(cPacket *pkt, const char *destination)
{
    //clean up for Sink since this node doesn't send out APP data
    if(isSink or !routingTreeReady){
        cancelAndDelete(pkt);
        return;
    } else if(config.nextHop[self]==-1){
        lastRoundCollectedNode[self]=true;
        lastRoundCollectedNodeId.push_back(self);
        cancelAndDelete(pkt);
        return;
    }
    if(bypassRoutingBuffer) {
        if(config.nextHop.size()<=self){
            routingTrace() << "WARN: net config strange, algo failure, stop operation";
            cancelAndDelete(pkt);
            return;
        } else if(config.nextHop[self]==-1 and config.clus_id[self]==-1){
            cancelAndDelete(pkt);
            return;
        }
        else { //send to MAC directly

            ConvergecastPacket *netPacket = new ConvergecastPacket("Convergecast packet to parent", NETWORK_LAYER_PACKET);
            netPacket->setSource(self);
            netPacket->setSourceCH(config.clus_id[self]);
            netPacket->setDestination(config.nextHop[self]);
            netPacket->setSourceRound(roundNumber);
            netPacket->setByteLength(netDataFrameOverhead);
            netPacket->setConvergecastPacketKind(CC_ROUTING_DATA_PACKET);
            encapsulatePacket(netPacket, pkt);

            VirtualRouting::toMacLayer(netPacket, config.nextHop[self]);

            return;
        }
    }
    if(routingTreeReady){ //safeguard for no parents node
        if(config.clus_id.size()==0){
            routingTrace() << "WARN: net config blank, algo failure and no configurations";
            cancelAndDelete(pkt);
            return;
        }

        if(config.nextHop[self]==-1){
            cancelAndDelete(pkt);
            return;
        }
    }

    //ATTENTION: destination IS NOT USED here, this is a aggregating protocol,
    // it is assumed that  all pkts are routed already (nextHop ready), and the how is created
    // by the routing layer save to intermediate buffer until MAC-ready
}

void ConvergecastRouting::fromMacLayer(cPacket *pkt, int macAddress, double rssi, double lqi){

//    mac_trace() << "GAMBAC::fromMacLayer";

    ConvergecastPacket *netPacket = dynamic_cast <ConvergecastPacket*>(pkt);

    if (!netPacket) {
        routingTrace() << "unknown blank packet received";
        return; //turning away every packets that is not a GAMBACPacket type from MAC layer
    } else {


        switch (netPacket->getConvergecastPacketKind()) {

            case CC_ROUTING_DATA_PACKET:{
                int pktSourceCH = netPacket->getSourceCH();
                int pktSourceRound = netPacket->getSourceRound();

                if(pktSourceRound!=roundNumber){
                    applicationTrace() << "WARN: Past-PKT-collected";
                    break;
                } else if(self != netPacket->getDestination()){
                    applicationTrace() << "Wrong Next Hop, this should not be possible";
                    metricsOverhearing_round++;
                    //pkt not for me. Routing wise
                    break;
                }
                //for packets destined to this CH
                else {
                    if (self==pktSourceCH) { //data arrived

                        lastRoundCollectedNode[netPacket->getSource()]=true;
                        lastRoundCollectedNodeId.push_back(netPacket->getSource());
                        sendDataToUAV(pkt);
                    //TODO: this is not extend-able, fix it on the next re-factor
                    //for packets on its way to destined CH
                    } else {
                    //forwarding case
                        if(config.clus_id[self] == pktSourceCH) {
                            cPacket* dataPkt = decapsulatePacket(pkt);
                            forwardPacketToNextHop(pkt, dataPkt);
                        }
                    }
                }
                break;
            }
            case CC_ROUTING_CONTROL_PACKET: {
                routingTrace() << "Unimplemented behaviour";
                break;
            }
            case CC_ROUTING_JOIN_PACKET: {
                routingTrace() << "Unimplemented behaviour";
                break;
            }
            case CC_ROUTING_ADV_PACKET: {
                routingTrace() << "Unimplemented behaviour";
                break;
            }
            default: {
                throw cRuntimeError("Unknown GAMBAC PACKET TYPE, proceed to crash, please debug");
            }
        }
    }
}

void ConvergecastRouting::forwardPacketToNextHop(cPacket * pkt, cPacket * appPkt){

    if(isSink or !routingTreeReady){
        cancelAndDelete(pkt);
        return;
    } else if(config.nextHop[self]==-1){
        cancelAndDelete(pkt);
        return;
    }
    if(bypassRoutingBuffer) {
        applicationTrace() <<"Forwarded Packet";
        if(config.nextHop.size()<=self){
            routingTrace() << "WARN: net config strange, algo failure, stop operation";
            cancelAndDelete(pkt);
            return;
        } else if(config.nextHop[self]==-1 and config.clus_id[self]==-1){
//            routing_trace() << "INFO: packet already at ClusterHead, no routing needed";
            cancelAndDelete(pkt);//            netPacket->setDestinationAddress(config.nextHop[self]));

            return;
        }
        else { //send to MAC directly

            ConvergecastPacket *netPacket = new ConvergecastPacket("BypassRouting packet", NETWORK_LAYER_PACKET);

            ConvergecastPacket *fwdPacket = check_and_cast<ConvergecastPacket*>(pkt);

            netPacket->setByteLength(netDataFrameOverhead);
            encapsulatePacket(netPacket, appPkt);

            netPacket->setSourceCH(config.clus_id[self]);
            netPacket->setDestination(config.nextHop[self]);
            netPacket->setConvergecastPacketKind(CC_ROUTING_DATA_PACKET);
            netPacket->setSourceRound(fwdPacket->getSourceRound());
            netPacket->setSource(fwdPacket->getSource());

            VirtualRouting::toMacLayer(netPacket, config.nextHop[self]);

            return;
        }
    }
    if(routingTreeReady){ //safeguard for no parents node
        if(config.clus_id.size()==0){
            routingTrace() << "WARN: net config blank, algo failure and no configurations";
            cancelAndDelete(pkt);
            return;
        }

        if(config.nextHop[self]==-1){
            cancelAndDelete(pkt);
            return;
        }
    }

    //ATTENTION: destination IS NOT USED here, this is a aggregating protocol,
    // it is assumed that  all pkts are routed already (nextHop ready), and the how is created
    // by the routing layer save to intermediate buffer until MAC-ready
//    bufferPacket(pkt);

}


// DEPRECATED, USE THE ORIGINAL TOMACLAYER() INSTEAD
// A function to send packets directly to Routing of NodeID, providing a destination NodeIndex
void ConvergecastRouting::toRoutingLayerDirect(cPacket * pkt, int destination)
{
    RoutingPacket *netPacket = check_and_cast <RoutingPacket*>(pkt);

    //Simulate energy consumption---------
    double byteLength = netPacket->getByteLength();
    Point nextHopLocation = GlobalLocationService::getLocation(destination);
    double distance = G::distance(selfLocation, nextHopLocation);
    resMgrModule->consumeEnergy(txEnergy(byteLength, distance));
    // --------------

    netPacket->getNetMacInfoExchange().nextHop = destination;
    if (destination == -1) {
        throw cRuntimeError("Undefined Routing BypassWC broadcast?");
        //TODO: this is not sending it directly!!!, fix it
        //note: this is the bypassWC broadcast function
        //of the Virtual Routing class
        send(netPacket, "toMacModule");
    } else {
        cModule* desModule = getParentModule()->getParentModule()->getParentModule()->getSubmodule("node", destination)
            ->getSubmodule("Communication")->getSubmodule("Routing");
        sendDirect(netPacket, desModule, "fromDirect");
    }
}

bool compareByDistance(int lhs, int rhs) {
    return (ConvergecastRouting::dCompare->at(lhs) > ConvergecastRouting::dCompare->at(rhs));
}

bool ConvergecastRouting::reached(Point location) {
  for (auto n: neighborTable) {
    if (G::distance(n.location, location) < G::distance(selfLocation, location)) {
      return false;
    }
  }

  return G::distance(selfLocation, location) < RADIO_RANGE;
}

void ConvergecastRouting::timerFiredCallback(int index)
{
    switch (index) {

        //trigger for Rounds to start
        case START_ROUND:{

            if (roundNumber == 0) {
                resMgrModule->resetBattery();
                if (isSink)
                    routingTrace() << "Energy\tE_min\ti_min\tlevel\tconfig.clus_id[i_min]\tcalculateRxSize(i_min)\tamountData[i_min]\tE_total/numNodes\ttotalConsumed\tmaxConsumed\tconfig.A.size()\tdevE\tavgConsumed\tobjective value\tE0_min\ttime_elapse\ttotalCollected\tmaxLengthRatio";
            }

            if (isSink) {
                //log round coverages
                calculateLastRoundNetCoverage();
                singleRoundMetricLog(std::string("LastRound_NetCov"), lastRoundNetCoverage);
                singleRoundMetricLog(std::string("LastRound_NetCov_Nids"), lastRoundCollectedNodeId);
                lastRoundCollectedNode.clear();
                lastRoundCollectedNode.resize(numNodes, false);
                lastRoundNetCoverage=0;
                lastRoundCollectedNodeId.clear();

                //log round metrics for MAC

                applicationTrace() << "START_ROUND with roundNumber "<<roundNumber;

                //time to run the algorithm is not accounted for here, sending END_ROUND super quick
                // Time goes like this:
                // ...|Start]          |End]              [End Round duration|Start]...
                setTimer(START_MAINALG, 0.5);
                setTimer(END_MAINAlG, 1);
                setTimer(DEBUG_TRAP_, 2);
            } else {
                //abitrary data sending after an offset of 1/10th round length since round start
                double dataSendingOffset = roundLength/5;
            }
            setTimer(POST_ALG_SETUP, 1.5);

            if(roundNumber==0) {}
            else {
                macModule->logAndResetPktBreakdownForRound(roundNumber);
                macModule->overheardPacketCount_round=0;
            }

            //logic for all nodes: loop new round

            roundNumber++;
            startRoundCallback();
            setTimer(START_ROUND, roundLength);

            //refresh trafic-related variables
            dataPacketSizeAt[self]= appModule->currentDataPktSize;

            break;
        }
        case SEND_DATA_UAV:{
            throw cRuntimeError("Unimplemented behaviour");
            break;
        }
        case START_MAINALG:{
            if(!isSink) throw cRuntimeError("Invoked on non-sink");
            uncollectedSet.clear();
            collectedSet.clear();

            totalConsumed  = 0;
            maxConsumed    = 0;
            totalCollected = 0;

            auto start = std::chrono::steady_clock::now();
            mainAlg();
            config.childrenList.resize(numNodes);
            auto end = std::chrono::steady_clock::now();
            timeElapse_round = end - start;

            break;
        }
        case END_MAINAlG:{
            if(!isSink) throw cRuntimeError("Invoked on non-sink");
            routingTreeReady=true;
            if(config.clus_id.size()==0){
                routingTrace() << "WARN:CRITICAL: net config blank, algo failure and no configurations";
                routingTreeReady=false;
            }
            break;
        }
        case POST_ALG_SETUP: {
            config.childrenList[self] = config.getChildren_(self);
            endOfAlgoLogging();
            break;
        }
        case DEBUG_TRAP_: {
            if(isSink){
//                config;
//                throw cRuntimeError("Check out children in config");
            } else {
                throw cRuntimeError("Non sink is not allowed to call this handler");
            }
            break;
        }
    }
}

//formalized logging for singular metrics
void ConvergecastRouting::singleRoundTopoLog(){
    metricsTrace() << "#round_topos#"
    << roundNumber << "#" << self //identifiers: time and place
    << "#" << config.nextHop[self] << "#" << config.clus_id[self]; //topology for this round
}

void ConvergecastRouting::singleRoundMetricLog(string name, string value){
    metricsTrace() << "#round_metrics#"
    << roundNumber << "#" << self //identifiers: time and place
    << "#" << name << "#" << value; //topology for this round
}

void ConvergecastRouting::singleRoundMetricLog(string name, vector<int> intList){
    stringstream ss;
    for( int i : intList) {
        ss << "=" << i;
    };
    metricsTrace() << "#round_metrics#"
    << roundNumber << "#" << self //identifiers: time and place
    << "#" << name << "#" << ss.str(); //int List for this round
}

void ConvergecastRouting::singleRoundMetricLog(string name, int value){
    metricsTrace() << "#round_metrics#"
    << roundNumber << "#" << self //identifiers: time and place
    << "#" << name << "#" << value; //topology for this round
}

void ConvergecastRouting::singleRoundMetricLog(std::string name, double value){
    metricsTrace() << "#round_metrics#"
    << roundNumber << "#" << self //identifiers: time and place
    << "#" << name << "#" << value; //topology for this round
}

void ConvergecastRouting::calculateLastRoundNetCoverage(){
    int clt = 0;
    for(int i = 0; i<numNodes; i++){
        clt+= lastRoundCollectedNode[i]?1:0;
    }
    lastRoundNetCoverage = (double) clt/numNodes;
//    if(lastRoundNetCoverage>=0.7) throw cRuntimeError("Gucci");
}


void ConvergecastRouting::endOfAlgoLogging(){
    if(isSink){
        routingTrace() << "#endofalgologs";
        stringstream ss_chIds;
        for(int i=0; i<config.A.size(); i++){
            ss_chIds << config.A[i] << ' ';
        }
//        routing_trace() << "#CHID_list " << ss_chIds.str();
        applicationTrace() << "#CHIDCount " << config.A.size();
    }
    //log the round topology
    singleRoundTopoLog();
    double remainingEnergy = resMgrModule->remainingEnergy;
//    singleRoundMetricLog(std::string("Remaining_Energy"), remainingEnergy);

}

int ConvergecastRouting::getRandomNumber(int from, int to) {
    int result = intrand(to - from + 1) + from;
    return result;
}

// This is undesirable but acceptable, this aids the direct sending functions and burning functions
// we keep this for better simulation experience
double ConvergecastRouting::txEnergy(double byteLength, double distance) {
    //some esoteric math calculation, unsure if it's synced with the sending mechanism. It might be
    return 1 * (50e-9 * byteLength * 8 + 10e-12 * byteLength * 8 * distance * distance);
}

double ConvergecastRouting::rxEnergy(double byteLength) {
    return 1 * (50e-9 * byteLength * 8);
}

double ConvergecastRouting::calculateCHEnergy(int u, double byteLength) {
    return getResMgrModule(u)->getRemainingEnergy() - rxEnergy(byteLength) - txEnergy(byteLength + dataPacketSizeAt[u], distanceToUAV);
}

ResourceManager* ConvergecastRouting::getResMgrModule(int id) {
    if (isSink) return check_and_cast <ResourceManager*>(getParentModule()->getParentModule()
        ->getParentModule()->getSubmodule("node", id)->getSubmodule("ResourceManager"));
    return nullptr;
}

void ConvergecastRouting::sendDataToUAV(cPacket* dataPacket){
    //This ONLY burn through the needed energy to send data to UAV
    //This doesn't actually send anything
    ConvergecastPacket* cvcPkt = check_and_cast<ConvergecastPacket*>(dataPacket);
    double byteLength = cvcPkt->getByteLength();

    // TODO: check the legitimacy of the energy burning mechanism,
    // make sure it's synced with the sending mechanism
    double burningEnergy = txEnergy(byteLength, distanceToUAV);
    resMgrModule->consumeEnergy(burningEnergy);
}

Point ConvergecastRouting::location(int id) {
    return GlobalLocationService::getLocation(id);
}

double ConvergecastRouting::calculatePathLength(vector<int> path) {
    if (path.empty()) return 0;
    double pathLength = 0;
    for (int i=0; i<path.size()-1; i++){
        pathLength += G::distance(GlobalLocationService::getLocation(path[i]),
            GlobalLocationService::getLocation(path[i+1]));
    }
    return pathLength;
}

//void ConvergecastRouting::sendData(cPacket * pkt) {
//
//    RoutingPacket *dataPacket = check_and_cast <RoutingPacket*>(pkt);
//    double byteLength = dataPacket->getByteLength();
//    int src = dataPacket->getSource();
//
//    if (config.nextHop[self] == -1) {
//
//        //this immediately simulate the data sending to UAV,
//        //it is not aggregated and the data sending happens when this CH node receive data
//        //undesired behaviour
//        //TODONE: Change this into the new queue-based and size aggregation method
//        //If possible, move this into the APP Layer, for now.
//        //Since at the app layer does the aggregation happens
//        resMgrModule->consumeEnergy(txEnergy(byteLength, distanceToUAV));
//        energyConsumeds[self] += txEnergy(byteLength, distanceToUAV);
//        totalCollected += byteLength;
//
//        //book keeping behaviours, these MUST happen every time send data to UAV
//        uncollectedSet.erase(src);
//        collectedSet.push_back(src);
//
//        if (energyConsumeds[self] > maxConsumed) maxConsumed = energyConsumeds[self];
//    } else {
//        double distanceToNextHop = G::distance(selfLocation, GlobalLocationService::getLocation(config.nextHop[self]));
//        resMgrModule->consumeEnergy(txEnergy(byteLength, distanceToNextHop));
//        totalConsumed += txEnergy(byteLength, distanceToNextHop);
//        energyConsumeds[self] += txEnergy(byteLength, distanceToNextHop);
//        cModule* nextModule = getParentModule()->getParentModule()->getParentModule()->getSubmodule("node", config.nextHop[self])
//            ->getSubmodule("Communication")->getSubmodule("Routing");
//            // for (int i=0; i<10; i++) trace() << "toMacModule";
//        sendDirect(dataPacket, nextModule, "fromDirect");
//    }
//}

//double ConvergecastRouting::calculateRxSize(int curnode) {
//    double rxSize = 0;
//    queue<int> q;
//    q.push(curnode);
//    while (!q.empty()) {
//        int u = q.front();
//        q.pop();
//
//        for (int i=0; i<numNodes; i++) {
//            if (config.nextHop[i] == u) {
//                rxSize += dataPacketSizeAt[i];
//                q.push(i);
//            }
//        }
//    }
//    return rxSize;
//}

//void ConvergecastRouting::logConfig() {
//    auto nextHop = config.nextHop;
//    auto clus_id = config.clus_id;
//    auto trajectories = config.trajectories;
//
//    int N = numNodes;
//    for (int i=0; i<N; i++) {
//        auto P = GlobalLocationService::getLocation(i);
//        CastaliaModule::trace2() << roundNumber << "\tPOINT\tblue\t" << P.x() << "\t" << P.y() << "\t" << dataPacketSizeAt[i] << "\t" << clus_id[i] << "\t" << i << "\t" << nextHop[i];
//    }
//    for (int i=0; i<N; i++) {
//        if (clus_id[i] != -1) {
//            auto P = GlobalLocationService::getLocation(i);
//            auto Q = GlobalLocationService::getLocation(nextHop[i]);
//            CastaliaModule::trace2() << roundNumber << "\tLINE\tgreen\t" << P.x() << "\t" << P.y() << "\t" << Q.x() << "\t" << Q.y();
//        }
//    }
//    for (int k=0; k<trajectories.size(); k++) {
//        for (int i=0; i<trajectories[k].size()-1; i++){
//            auto P = GlobalLocationService::getLocation(trajectories[k][i]);
//            auto Q = GlobalLocationService::getLocation(trajectories[k][i+1]);
//            CastaliaModule::trace2() << roundNumber << "\tLINE\tred\t" << P.x() << "\t" << P.y() << "\t" << Q.x() << "\t" << Q.y();
//        }
//    }
//}
