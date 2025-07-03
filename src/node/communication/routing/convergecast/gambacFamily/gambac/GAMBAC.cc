#include "GAMBAC.h"

Define_Module(GAMBAC);


void GAMBAC::protocolInitialize()
{
    /*---getParentModule The .ned file's parameters ---*/

    //fail-safe in case we have Isolated Nodes
    if(neighborTable.size()==0) {
        throw cRuntimeError("Isolated Node, may result in algorithm breaking! Consider modify the Path-loss variables");
    }
    smplAggAppModule = check_and_cast<SimpleAggregation*>(getParentModule()->getParentModule()->getSubmodule("Application"));

    // Unused variables
    applicationID = par("applicationID").stringValue();
    gamma = par("gamma");
    maxCHperUAV = par("maxCHperUAV");


    // Network setting collected from Network's NED
    numUAVs = getParentModule()->getParentModule()->getParentModule()->par("numUAVs");
    if(numUAVs<=0) throw cRuntimeError("Faulty logic! Check if you set numUAV to be a positive value ?");

    clusterVolume_optimal = -1;
    clusterVolume_start = 0;
    countSuccess = 0; //TODO: rename
    countAdjusment = 0; //TODO: rename
    debugRecruitProcess = par("debugRecruitProcess"); // This looks like a logging flag

    //THIS IS PAYLOAD PER ROUND (which is the calculated bytes per roundlength the system create)

    // initialize the data volume at each nodes, this refers to the app Module variable
    // TODO: check if the update mechanism is suffice
    if(smplAggAppModule->currentDataPktSize <0)
        throw cRuntimeError("currentDataPktSize variable at SimpleAgg app module negative! Check if it is set/updated");
    else dataPacketSizeAt[self]= smplAggAppModule->currentDataPktSize;

    // since startup is ran after init, we can count on this to
    if (isSink) GAMBACinit();

    // Send EVERY NODES into a round-based time loop
    setTimer(START_ROUND, 1);
}


void GAMBAC::GAMBACinit() {
    /*- GAMBAC-specific init logic */

    N = numNodes;

    // in GB alpha (base algo)
    epsilon = par("epsilon");


    // This is done at the lower Class
    // neighborTable = GlobalLocationService::getNeighborTable(self);

    // Unused, TODO: do we need this?
    selfLocation = GlobalLocationService::getLocation(self);

    //for grabbing WC transmission range!
    //throw cRuntimeError("debug trap");
    //neighborRange = tempWc->transmissionRange;

    connectivityGraph.init(numNodes, self, neighborRange);

    trajectories.resize(numUAVs);
    distanceToCH.resize(N, 0.);

    // All net topo building vars here
    clusId.resize(N, -1);
    nextHop.resize(N, -1);
    isCH.resize(N, false);
    clusterMembers.resize(N);
    w_max.resize(N); //TODO: rename
    remainingEnergy_tmp = vector<double>(numNodes, 0); //TODO: rename
}

void GAMBAC::startRoundCallback(){
    if(smplAggAppModule->currentDataPktSize <0)
        throw cRuntimeError("currentDataPktSize variable at SimpleAgg app module negative! Check if it is set/updated");
    else dataPacketSizeAt[self]= smplAggAppModule->currentDataPktSize;
}



// The main-algo procedure, this is nothing more than a hook function for the START_MAINALG callback signal
// This is only ran at Sink Routing Object
void GAMBAC::mainAlg() {

    for (int u=0; u<numNodes; u++) remainingEnergy_tmp[u] = getResMgrModule(u)->getRemainingEnergy();
    config.A.clear(); //TODO: rename config, rename A, what the hell is A?

    routingTrace() << "mainAlg";

    dataVolume_total = 0;
    for (int i=0; i<numNodes; i++) dataVolume_total += dataPacketSizeAt[i];

    //============== This is Binary Search for the Cluster Volume. But Why??? Arent we suppose to search for the optimal CH count?
    clusterVolume_start = dataVolume_total/numUAVs/2;
    clusterVolume_end = 1000;
    maxClusterVolume = (clusterVolume_start+clusterVolume_end)/2;

    trace() << "Cluster Volume MAX: "<< maxClusterVolume;
    //============== This is very important to the running of system

    bool lengthViolated;
    bool isSaved = false;
    int retry = 0;
    double UAVUtilizationRatio = 1;
    double W_saved;
    double maxLen = 0;

    do {
        // TODO: make a list of used algo vars, see if they are properly reseted?
        reset();

        int loop_count = selectCHsAndBuildDFT(maxClusterVolume);
        if (A == config.A) { //TODO: this looks dangerously strange, what is it doing ? And is == even implemented for this struct ???
            //validate trajs
            for(int i=0;i<config.trajectories.size();i++){
                if(!isCH[config.trajectories[i][config.trajectories[i].size()-1]]){
                    config.trajectories[i].pop_back();
                }
                if(config.trajectories[i][config.trajectories[i].size()-1]!=sinkId){
                    config.trajectories[i].push_back(sinkId);
                }
            }
            trajectories = config.trajectories;
        }
        else buildTrajectories();

        //validate trajs
        for(int i=0;i<trajectories.size();i++){
            if(!isCH[trajectories[i][trajectories[i].size()-1]]){
                trajectories[i].pop_back();
            }
            if(trajectories[i][trajectories[i].size()-1]!=sinkId){
                trajectories[i].push_back(sinkId);
            }
        }

        //validate trajs
        for(int i=0;i<config.trajectories.size();i++){
            if(!isCH[config.trajectories[i][config.trajectories[i].size()-1]]){
                config.trajectories[i].pop_back();
            }
            if(config.trajectories[i][config.trajectories[i].size()-1]!=sinkId){
                config.trajectories[i].push_back(sinkId);
            }
        }

        while (trajectories.size() < numUAVs) {
            trajectories.push_back(vector<int>());
        }

        lengthViolated = false;
        maxLen = 0;

        // Verify trajectories length
        for (int t=0; t<numUAVs; t++) {
            double len = calculatePathLength(trajectories[t]);
            if (len > maxUAVFlyingDistance) {
                lengthViolated = true;
            };
            if (len > maxLen)
                maxLen = len;
        }
        UAVUtilizationRatio = maxLen / maxUAVFlyingDistance;

        routingTrace() << "maxLen " << maxLen << " ratioToLmax " << UAVUtilizationRatio;

        if (lengthViolated) {
            // Increase W
            retry += 1;
            if (retry >= 5) {
                clusterVolume_end = maxClusterVolume;
                maxClusterVolume = maxClusterVolume*UAVUtilizationRatio;
                retry = 0;
            }
            else continue;
        } else {
            if (A.size() > config.A.size() && loop_count <= 50){
                config.save(A, clusId, nextHop, trajectories);
                isSaved = true;
                clusterVolume_start = maxClusterVolume;
                maxClusterVolume = maxClusterVolume*UAVUtilizationRatio;
            }
            else {
                retry += 1;
                if (retry >= 5) {
                    if (!isSaved) {
                        config.save(A, clusId, nextHop, trajectories);
                        isSaved = true;
                    }
                    clusterVolume_start = maxClusterVolume;
                    maxClusterVolume = maxClusterVolume*UAVUtilizationRatio;
                    retry = 0;
                }
                continue;
            }

            retry = 0;
        }

        if (clusterVolume_start - maxClusterVolume <= 10000) break;


        if ((clusterVolume_start-clusterVolume_end <= 10000) && !isSaved) {
            clusterVolume_start = dataVolume_total/numUAVs/2;
            clusterVolume_end = 1000;
            clusterVolume_optimal = -1;
        }

    } while (clusterVolume_start-clusterVolume_end>10000);


    A = config.A;
    stringstream ssA;
    for(int l : A) {
        ssA << l << " ";
    }
    routingTrace() << "CH list: " << ssA.str();

    for(int i=0; i<config.trajectories.size(); i++){
        stringstream trajectory_tmp;
        for(int j=0; j<(config.trajectories[i].size()-1); j++){
            trajectory_tmp << config.trajectories[i][j] << "=";
        }
        trajectory_tmp << config.trajectories[i][config.trajectories[i].size()];
        metricsTrace() << "#round_trajectory#" << roundNumber << "#" << i << "#" << trajectory_tmp.str();
    }

    ///////////////////////////////////////////////////////////

    routingTrace() << "END mainAlg";
//    if(!inittializedRPL) inittializedRPL=true;
    if(!routingTreeReady) routingTreeReady=true;

}


void GAMBAC::reset() {

    trajectories = vector<vector<int>>(numUAVs);

    A.clear();
    distanceToCH = vector<double>(N, 0.);
    clusId = vector<int>(N, -1);
    nextHop = vector<int>(N, -1);
    isCH = vector<bool>(N, false);
    clusterMembers = vector<list<int>>(N);
    w_max = vector<double>(N, 0);
}

vector<int> GAMBAC::randomFromSet(vector<int> Candidates, double selectedNumber) {
    int nodeW = Candidates.size();
    if (nodeW > 0) {
        if (nodeW < selectedNumber) {
            for (int node : Candidates) {
                isCH[node] = true; //update ALL candidates as CHs
            }
            return Candidates;
        } else {
            double prob_net = selectedNumber / nodeW;
            vector<int> selected_candidates;
            for (int node : Candidates) {
                if (isCH[node]) continue;
                double random_float = uniform(0,1);
                if (random_float < prob_net) {
                    selected_candidates.push_back(node);
                    isCH[node] = true;
                }
            }
            if (selected_candidates.empty()) return randomFromSet(Candidates, selectedNumber);
            else {
                return selected_candidates;
            }
        }
    }
    return Candidates;
}

void GAMBAC::growBalls(vector<int> CHSet){

    for (int u : connectivityGraph.getNodesExceptSink()){
        if (isCH[u]){
            distanceToCH[u] = 0;
        }
        else {
            distanceToCH[u] = DBL_MAX;
        }
        clusId[u] = -1;
        nextHop[u] = -1;
        clusterMembers[u].clear(); //refresh, re-do from none
    }

    // rebuild the clusters from the ground up
    for (int u : CHSet) {
        isCH[u] = true; //JUL28 modification
        distanceToCH[u] = 0;
        clusterMembers[u].clear();
        clusId[u] = u;
    }

    dCompare =  &distanceToCH;

    // a shortest to longest distance to CH prio queue
    priority_queue<int,vector<int>, decltype(&compareByDistance)> closestToCHFirst_queue(compareByDistance);
    for (int l : CHSet) {
        closestToCHFirst_queue.push(l); //obviously as CH are the closest nodes to CHs
    }
    unordered_set<int> removedSet;

    while (!closestToCHFirst_queue.empty()) {

        int u = closestToCHFirst_queue.top();
        closestToCHFirst_queue.pop();

        //fail safe
        if (removedSet.find(u) != removedSet.end()) continue;
        else removedSet.insert(u);

        if (!isCH[u]) //if u is not CH and u is "considering" a CH,
                                       //cement that in clusterMembers[CH]
            if(clusId[u]==-1) throw cRuntimeError("Error logic!");
            clusterMembers[clusId[u]].push_back(u);

        for (int v : connectivityGraph.getAdjExceptSink(u)) {
            //Grab ALL of u's adj, regardless if they has a "considering" CH or not

            //fail safe
            if ((removedSet.find(v) != removedSet.end())) continue;

            double distanceToNewCH = distanceToCH[u] + connectivityGraph.getLength(u,v);
            //the only "new" CH to consider is the CH of u !

            if (distanceToNewCH - distanceToCH[v] < -EPSILON) {
                //if assigning v to a new CH is better, distance-wise then do it!
                    distanceToCH[v] = distanceToNewCH;
                    clusId[v] = clusId[u]; //add v to u's Cluster
                    nextHop[v] = u;
                    closestToCHFirst_queue.push(v);
                    //this is actually reseting v's position in queue, not adding new v
                }
           }
    }
    int totalSize = 0; //TODO: ???
    computeBallWeight();
}

int GAMBAC::growBallsV2(vector<int> CHSet){

    int netCoverage=0;

    for (int u : connectivityGraph.getNodesExceptSink()){
        if (isCH[u]){
            distanceToCH[u] = 0;
        }
        else {
            distanceToCH[u] = DBL_MAX;
        }
        clusId[u] = -1;
        nextHop[u] = -1;
        clusterMembers[u].clear();
    }

    // rebuild the clusters from the ground up
    for (int u : CHSet) {
        distanceToCH[u] = 0;
        clusterMembers[u].clear();
        clusId[u] = u;
    }

    dCompare =  &distanceToCH;
    map<int, double> ballWeight_tmp;
    for(int u : CHSet){
        ballWeight_tmp.insert({u, 0.0});
    }

    // a shortest to longest distance to CH prio queue
    priority_queue<int,vector<int>, decltype(&compareByDistance)> queue(compareByDistance);
    for (int l : CHSet) {
        queue.push(l);
    }
    unordered_set<int> removedSet;

    while (!queue.empty()) {
        int u = queue.top();
        queue.pop();

        //fail safe
        if (removedSet.find(u) != removedSet.end()) continue;
        removedSet.insert(u);

        //add new node to a cluster
        if (!isCH[u])
            clusterMembers[clusId[u]].push_back(u);
            ballWeight_tmp.find(clusId[u])->second+=dataPacketSizeAt[u];
            netCoverage++;


        for (int v : connectivityGraph.getAdjExceptSink(u)) {

            //fail safe
            if ((removedSet.find(v) != removedSet.end())) continue;
            double alt = distanceToCH[u] + connectivityGraph.getLength(u,v);

//            if (alt - distanceToCH[v] < -EPSILON){
//            auto it = mp.find(2);
            if (
                    alt - distanceToCH[v] < -EPSILON
                    && dataPacketSizeAt[v]+ ballWeight_tmp.find(clusId[u])->second <= maxClusterVolume
               )
                {
                    distanceToCH[v] = alt;
                    clusId[v] = clusId[u];
                    nextHop[v] = u;
                    queue.push(v);
                }
           }
    }
    int totalSize = 0;
    computeBallWeight();
    return netCoverage+1;
}


vector<int> GAMBAC::getOuterOversizePart(){
    outerSet.clear();
    innerSet.clear();
    vector<int> returnlist;
    for (int CH : A){
        double weight = ballWeight.find(CH)->second;

        if (weight > w_max[CH]) {
            weight = 0;
            dCompare =  &distanceToCH;
            priority_queue<int,vector<int>, decltype(&compareByDistance)> queue(compareByDistance);
            for (int u : clusterMembers[CH]) queue.push(u);
            while (weight < w_max[CH]){
                int u = queue.top();
                queue.pop();
//                if (debugRecruitProcess) for (int i=0; i<1; i++) routing_trace() << "CH " << CH << " inner " << u << " distanceToCH " << distanceToCH[u] << " weight " << weight;
                weight += dataPacketSizeAt[u];
                if (weight >= w_max[CH]) break;
                innerSet.push_back(u);
            }
            while (!queue.empty()) {
                int u = queue.top();
//                if (debugRecruitProcess) for (int i=0; i<1; i++) routing_trace() << "CH " << CH << " fringe " << u << " distanceToCH " << distanceToCH[u];
                queue.pop();
                outerSet.push_back(u);
            }
        }
    }
    return returnlist.empty() ? outerSet : returnlist;
}

vector<int> GAMBAC::samplingCH(vector<int> Candidates, double b) {

    int maxTrial = 3;
    vector<int> selected_candidates;
    while (!Candidates.empty()){
        int nextLandmark = Candidates[rand() % Candidates.size()];

        if (isCH[nextLandmark]) {
            Candidates.erase(std::remove(Candidates.begin(), Candidates.end(), nextLandmark), Candidates.end());
            continue;
        }

        vector<int> intersectBall;
        double newBallSize = 0;
        double intersectBallSize = 0;

        vector<double> d(N, DBL_MAX);
        d[nextLandmark] = 0;
        dCompare =  &d;
        priority_queue<int,vector<int>, decltype(&compareByDistance)> queue(compareByDistance);
        queue.push(nextLandmark);
        unordered_set<int> removedSet;
        if(queue.empty()) continue;
        while (!queue.empty()){
            int v = queue.top();
            queue.pop();
            if (removedSet.find(v) != removedSet.end()) continue;
            removedSet.insert(v);
            newBallSize += dataPacketSizeAt[v];
            if (newBallSize > w_max[nextLandmark]) break;
            if (std::find(Candidates.begin(), Candidates.end(), v) != Candidates.end()){
                Candidates.erase(std::remove(Candidates.begin(), Candidates.end(), v), Candidates.end());
                intersectBall.push_back(v);
                intersectBallSize += dataPacketSizeAt[v];
            }
            for (int w : connectivityGraph.getAdjExceptSink(v)){
                if ((w < N) && (removedSet.find(w) == removedSet.end())){
                    double alt = d[v] + connectivityGraph.getLength(v,w);
                    if (alt < d[w]){
                        d[w] = alt;
                        queue.push(w);
                    }
                }
            }
        }
        if ((maxTrial == 0) || (intersectBallSize>maxClusterVolume/4)){//selected_candidates.empty() &&
            selected_candidates.push_back(nextLandmark);
            maxTrial = 3;
        }
        else{
            for (int v : intersectBall) Candidates.push_back(v);
            if (maxTrial>0){
                maxTrial--;
                continue;
            }
            else break;
        }
    }
    return selected_candidates;
}

void GAMBAC::computeBallWeight(){

    maxBallWeight = 0;
    minBallWeight = INT64_MAX;

    double tempBallWeight[A.size()];
    for (int i=0; i<A.size(); i++) tempBallWeight[i] = 0;
    int size = 0;
    for (int i=0; i<A.size(); i++){
        size += clusterMembers[A[i]].size();
        for (int u : clusterMembers[A[i]]) {
            tempBallWeight[i] += dataPacketSizeAt[u];
        }
    }
    ballWeight.clear();
    for (int i=0; i<A.size(); i++){
        ballWeight.insert(pair<int,double>(A[i], tempBallWeight[i]));
    }
}

void GAMBAC::recruitNewCHs(){

    vector<int> Candidates = getOuterOversizePart();
    vector<int> newCHs = samplingCH(Candidates, 0);
    if (!newCHs.empty()) A.insert(A.end(), newCHs.begin(), newCHs.end());
    for (int u : newCHs) {
        isCH[u] = true;
    }
}

int GAMBAC::selectCHsAndBuildDFT(double maxClusterWeight){
    E_min = DBL_MAX;
    E_max = 0;

    // traverse the nodes set for E_min and E_mac
    for (int i=0; i<numNodes; i++) {
        if (i == sinkId) continue;
        if (remainingEnergy_tmp[i] > E_max) E_max = remainingEnergy_tmp[i];
        if (remainingEnergy_tmp[i] < E_min) E_min = remainingEnergy_tmp[i];
    }

    for (int i=0; i<N; i++) {
        if (i == sinkId) continue;
        w_max[i] = maxClusterWeight;// set every single Node Cluster max Weight as maxClusterWeight
    }

    int k_max = (int) ((1 + epsilon) * dataVolume_total / maxClusterWeight); // max CHs this can cram into

    int count = 0;
    vector<int> allSNs;
    for (int i=0; i<N; i++) allSNs.push_back(i);
    while (A.empty()) {
        A = randomFromSet(allSNs, k_max/2); // the initial Candidates available, here is 1/2 randomed candidates
    }
    routingTrace() << "initial size " << A.size();
    growBalls(A);
    bool isBalance;
    bool fullCoverage;
    do {
        int Asize = A.size();
        recruitNewCHs();
        growBalls(A);

        isBalance = true; // assume that every Cluster is balanced interm of weights
        double W_new = min(maxClusterWeight, (1 + epsilon) * dataVolume_total / A.size());
        double maxWeight = 0;
        double totalWeight = 0;
        for (auto pair : ballWeight) {
            if (maxWeight < pair.second) {
                maxWeight = pair.second;
            }
        }
        if (maxWeight > W_new) isBalance = false;

        if (A.size() > k_max) {
            vector<double> tempBallWeight(N, 0);
            for (auto pair : ballWeight) {
                tempBallWeight[pair.first] = pair.second;
            }
            dCompare =  &tempBallWeight;
            priority_queue<int,vector<int>, decltype(&compareByDistance)> Q(compareByDistance);
            for (int l : A) Q.push(l);
            if(A.size() <= k_max) throw cRuntimeError("False!");
            while (A.size() > k_max){
                routingTrace() << "Maximal CHs count reached, deleting Clusters";
                double WeightTotal = 0;
                for (int l : A) {
                    if(tempBallWeight[l]==0) WeightTotal += 100000;
                    else WeightTotal += 1./tempBallWeight[l];
                }
                double f = (double)rand() / RAND_MAX * WeightTotal;
                int u=-1;
                int curr_l;
                for (int l : A) { //traverse in the set of CHs
                    f -= 1./tempBallWeight[l]; //ball weight with root at CH l
                    curr_l = l;
                    if (f <= 0) {
                        u = l; //assign deletion target as current CH
                        break;
                    }
                }

                if(u<0) throw cRuntimeError("Deleting CH error: Negative Node Index");
                A.erase(std::remove(A.begin(), A.end(), u), A.end());
                isCH[u] = false;
            }
        }

        // grow Balls AGAIN after we trimmed down the excessive Clusters
//        if(growBallsV2(A)==N) fullCoverage=true;
        growBalls(A);
        // assume is every new Balls is balance
        isBalance = true;
        W_new = min(maxClusterWeight, (1 + epsilon) * dataVolume_total / A.size());
        //reset the Weight variable, since now A is maximal

        maxWeight = 0;
        totalWeight = 0;
        for (auto pair : ballWeight) {
            if (maxWeight < pair.second) {
                maxWeight = pair.second;
            }
            if (w_max[pair.first] < pair.second) {
                isBalance = false;
            }
        }
        if (count++ > 20) {
            routingTrace() << "Reached timeout, isBalance: " << isBalance;
            break;
        }

        countAdjusment++;
        nloop = count;
    } while (!isBalance);
//    } while (!isBalance or !fullCoverage);

//    if(fullCoverage) throw cRuntimeError("trap");

    double maxWeight = 0;
    for (auto pair : ballWeight) {
        if (pair.second > maxWeight) maxWeight = pair.second;
    }

    clusId[self] = -1;
    for (int l : A) clusId[l] = -1;

    trace() << "W " << maxClusterWeight;
    trace() << "k_max " << k_max;
    return count;
}

//void GAMBAC::buildTrajectories(){
//    buildTrajectories(false);
//}
//
//void GAMBAC::buildTrajectories(bool isBreak){
//    for (int i=0; i<numUAVs; i++) trajectories[i].clear();
//    if (A.size() < numUAVs) {
//        for (int i=0; i<A.size(); i++) {
//            trajectories[i].push_back(self);
//            trajectories[i].push_back(A[i]);
//            trajectories[i].push_back(self);
//        }
//    }
//    else {
//        trajectories = Vrptw::call(*this, sinkId, A, numUAVs, maxUAVFlyingDistance);
////        auto initialTrajectories = buildInitialTrajectories();
//        //        trajectories = Vrptw::call(*this, sinkId, A, numUAVs, maxUAVFlyingDistance, isBreak, initialTrajectories);
////        trajectories = Vrptw::call(*this, sinkId, A, numUAVs, maxUAVFlyingDistance, initialTrajectories);
//
//        //validate trajs
//        for(int i=0;i<trajectories.size();i++){
//            if(!isCH[trajectories[i][trajectories[i].size()-1]]){
//                trajectories[i].pop_back();
//            }
//            if(trajectories[i][trajectories[i].size()-1]!=sinkId){
//                trajectories[i].push_back(sinkId);
//            }
//        }//        //validate trajs
//        for(int i=0;i<trajectories.size();i++){
//            if(!isCH[trajectories[i][trajectories[i].size()-1]]){
//                trajectories[i].pop_back();
//            }
//            if(trajectories[i][trajectories[i].size()-1]!=sinkId){
//                trajectories[i].push_back(sinkId);
//            }
//        }

//
//        while (trajectories.size() < numUAVs) {
//            trajectories.push_back(vector<int>());
//        }
//    }
//
//    double maxLength = 0;
//    unordered_set<int> CHs;
//    CHs.insert(A.begin(), A.end());
//    for (int k=0; k<numUAVs; k++) {
//        double length = calculatePathLength(trajectories[k]);
//        if (length > maxLength) maxLength = length;
//        trace() << "trajectory " << k << " length " << length;
//        for(int i : trajectories[k]) {
//            CHs.erase(i);
//        }
//    }
//    trace() << "maxLength " << maxLength;
////    for (int i : CHs) trace() << i << " is unvisited!!!";
//
//    int blank_trajs_count = 0;
//    for (int k=0; k<trajectories.size(); k++) {
//        if (trajectories[k].empty()) {
//            blank_trajs_count++;
//            trajectories[k].push_back(sinkId);
//            trajectories[k].push_back(sinkId);
//        }
//    }
//    if(blank_trajs_count==trajectories.size())
//        throw cRuntimeError("All blank trajectories, false algorithm");
//};

void GAMBAC::buildTrajectories(){
    for (int i=0; i<numUAVs; i++) trajectories[i].clear();
    if (A.size() < numUAVs) {
        for (int i=0; i<A.size(); i++) {
            trajectories[i].push_back(self);
            trajectories[i].push_back(A[i]);
            trajectories[i].push_back(self);
        }
    }
    else {
        for (int i=0; i<10; i++) trace() << "buildTrajectories";

        auto start = high_resolution_clock::now();
        trajectories =  Vrptw::call(*this, sinkId, A, numUAVs, maxUAVFlyingDistance);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        trace() << "Check Vrptw " << A.size() << " " << duration.count();
        // trajectories = callTspga(*this, sinkId, A, numUAVs, L_max);
    }

    //validate trajs
    for(int i=0;i<trajectories.size();i++){
        if(!isCH[trajectories[i][trajectories[i].size()-1]]){
            trajectories[i].pop_back();
        }
        if(trajectories[i][trajectories[i].size()-1]!=sinkId){
            trajectories[i].push_back(sinkId);
        }
    }

    while (trajectories.size() < numUAVs) {
        trajectories.push_back(vector<int>());
    }

    double maxLength = 0;
    unordered_set<int> CHs;
    CHs.insert(A.begin(), A.end());
    for (int k=0; k<numUAVs; k++) {
        double length = calculatePathLength(trajectories[k]);
        if (length > maxLength) maxLength = length;
        trace() << "trajectory " << k << " length " << length;
        stringstream ss;
        for(int i : trajectories[k]) {
            ss << i << " ";
            CHs.erase(i);
        }
        trace() << ss.str();
    }
    trace() << "maxLength " << maxLength;
    for (int i : CHs) trace() << i << " is unvisited!!!";

    for (int k=0; k<trajectories.size(); k++) {
        if (trajectories[k].empty()) {
            trajectories[k].push_back(sinkId);
            trajectories[k].push_back(sinkId);
        }
    }
};

vector<vector<int>> GAMBAC::buildInitialTrajectories(){
    vector<int> A_tmp = A;
    vector<int> TSP_tour = TSP(A_tmp);
    vector<vector<int>> new_trajectories;

//    routing_trace() << "TSPLength = " << calculatePathLength(TSP_tour);
//    stringstream ss_tsp;
//    for (int l : TSP_tour) {
//        ss_tsp << l << " ";
//    }
//    routing_trace() << "TSP tour: " << ss_tsp.str();

    double minMaxLength = DBL_MAX;
    for (int offset=0; offset<TSP_tour.size(); offset++) {
        double L_max_last = maxUAVFlyingDistance;
        double maxLengthLast;
        double maxLength = DBL_MAX;
        vector<vector<int>> tmp_trajectories;
        do {
            maxLengthLast = maxLength;
            maxLength = 0;
            tmp_trajectories = vector<vector<int>>(numUAVs);
            for (auto &trajectory : tmp_trajectories) trajectory.push_back(sinkId);
            int cur_trajectory_id = 0;
            double curLength = 0;
            double totalLength = 0;
            for (int i=0; i<TSP_tour.size(); i++) {
                int node = TSP_tour[(i+offset)%TSP_tour.size()];
                vector<int> &cur_trajectory = tmp_trajectories[cur_trajectory_id];
                int lastNode = cur_trajectory[cur_trajectory.size()-1];
                double curLengthLast = curLength;
                curLength += distance(lastNode, node) + distance(node, sinkId) - distance(lastNode, sinkId);
                if (curLength > L_max_last) {
                    if (cur_trajectory_id < numUAVs-1) {
                        cur_trajectory.push_back(sinkId);
                        totalLength += curLengthLast;
                        cur_trajectory_id++;
                        vector<int> &next_trajectory = tmp_trajectories[cur_trajectory_id];
                        next_trajectory.push_back(node);
                        curLength = distance(sinkId, node)*2;
                        if (curLength > maxLength) maxLength = curLength;
                    }
                    else {
                        cur_trajectory.push_back(node);
                        if (curLength > maxLength) maxLength = curLength;
                    }
                }
                else {
                    cur_trajectory.push_back(node);
                    if (curLength > maxLength) maxLength = curLength;
                }
            }
            tmp_trajectories[cur_trajectory_id].push_back(sinkId);
            totalLength += curLength;
            L_max_last = totalLength / numUAVs;
            if (maxLength < minMaxLength) {
                minMaxLength = maxLength;
                new_trajectories = tmp_trajectories;
            }
        } while (maxLength < maxLengthLast);
    }

    routingTrace() << "minMaxLength = " << minMaxLength;

//    int jj = 1;
//    for (auto trajectory : new_trajectories) {
//        stringstream ss_tra;
//        for (int u : trajectory) ss_tra << u << " ";
//        routing_trace() << "Tour " << jj << ": " << ss_tra.str() << " - Length: " << calculatePathLength(trajectory);
//        jj++;
//    }

    vector<vector<int>> tours(numUAVs);

    vector<int> A_new;
    int matchingId = 1;
    for (int i=0; i<numUAVs; i++) {
        for (int u : new_trajectories[i]) {
            if (u != sinkId) {
                A_new.push_back(u);
                tours[i].push_back(matchingId);
                matchingId++;
            }
        }
    }
    A = A_new;
    return tours;
}

vector<int> GAMBAC::TSP(vector<int> AA) {
    vector<int> TSP_tour;
    vector<Point> sites;
    for (int l : AA) {
        Point p = location(l);
        p.id_ = l;
        sites.push_back(p);
    }

    while (!sites.empty()) {
        vector<Point> convexHull = G::convexHull(sites);
        if (convexHull.empty()) {
            convexHull = sites;
        }
        for (Point p : convexHull) {
            sites.erase(std::remove(sites.begin(), sites.end(), p), sites.end());
        }
        if (TSP_tour.empty()) {
            for (Point p : convexHull) {
                TSP_tour.push_back(p.id_);
            }
        }
        else {
            while (!convexHull.empty()) {
                double minIncreased = DBL_MAX;
                int i_min = -1;
                int pos = -1;
                for (int i=0; i<convexHull.size(); i++) {
                    Point p = convexHull[i];
                    for (int j=0; j<TSP_tour.size(); j++) {
                        Point A = location(TSP_tour[j]);
                        Point B = location(TSP_tour[(j+1)%TSP_tour.size()]);
                        double lengthIncreased = G::distance(A, p) + G::distance(p, B) - G::distance(A, B);
                        if (lengthIncreased < minIncreased) {
                            minIncreased = lengthIncreased;
                            i_min = i;
                            pos = (j+1)%TSP_tour.size();
                        }
                    }
                }

                Point insertedNode = convexHull[i_min];
                convexHull.erase(convexHull.begin() + i_min);
                TSP_tour.insert(TSP_tour.begin() + pos, insertedNode.id_);
            }
        }
    }
    return TSP_tour;
}



////****************************************************************************
////*  Copyright (c) Federal University of Para, brazil - 2011                 *
////*  Developed at the Research Group on Computer Network and Multimedia      *
////*  Communication (GERCOM)                                  *
////*  All rights reserved                                         *
////*                                                                          *
////*  Permission to use, copy, modify, and distribute this protocol and its   *
////*  documentation for any purpose, without fee, and without written         *
////*  agreement is hereby granted, provided that the above copyright notice,  *
////*  and the author appear in all copies of this protocol.                   *
////*                                                                          *
////*  Module:   LEACH Clustering Protocol for Castalia Simulator              *
////*  Version:  0.2                                                           *
////*  Author(s): Adonias Pires <adonias@ufpa.br>                              *
////*             Claudio Silva <claudio.silva@itec.ufpa.br>                   *
////****************************************************************************/
//
//#include "node/communication/routing/cluster/gambac/GAMBAC.h"
//#include "node/application/ApplicationPacket_m.h"
//#include "node/communication/routing/RoutingPacket_m.h"
//#include <omnetpp.h>
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////START///////////////////
//bool GAMBAC::initialized = false;
//bool GAMBAC::scheduled = false;
//
//int GAMBAC::sinkId;
//NetworkConfig GAMBAC::config;
//vector<double> GAMBAC::amountData;
//vector<double>* GAMBAC::dCompare;
//vector<double> GAMBAC::energyConsumeds;
//vector<double> GAMBAC::rxSizes;
//int GAMBAC::totalCollected;
//unordered_set<int> GAMBAC::uncollectedSet;
//vector<int> GAMBAC::collectedSet;
//
//
//using namespace std::chrono;
//
/////////////////////////////////////////////////////////////////////////////////////////////////ENDDDD///////////////////
//
//
//Define_Module(GAMBAC);
//
//void GAMBAC::initialize(){
//    //////////////////////////////////////////////////////////////////////////////////////////////////START////////////////
//      GlobalLocationService::initialize(getParentModule()->getParentModule()->getParentModule());
//      if (!initialized) {
//        initialized = true;
//        srand(time(NULL));
//      }
//
//        roundLength = par("roundLength");
//        L_max = par("maxUAVFlyingDistance");
//        numPacketReceived = 0;
//        numPacketSent = 0;
//    //////////////////////////////////////////////////////////////////////////////////////////////////ENDDD////////////////
//        VirtualRouting::initialize();
//    ///////////////////////////////////////////////////////////////////////////////?START?////////////////////////////////
//        neighborTable = GlobalLocationService::getNeighborTable(self);
//        selfLocation = GlobalLocationService::getLocation(self);
//        // create the routing level address using self
//        stringstream out; out << self;  selfAddress = out.str();
//
//        isSink = par("isSink");
//
//        numNodes = getParentModule()->getParentModule()->getParentModule()->par("numNodes");
//        numUAVs = getParentModule()->getParentModule()->getParentModule()->par("numUAVs");
//
//        roundNumber=0;
//
//        // setup vector variables for the algorithm
//        if (amountData.empty()) amountData.resize(numNodes);
//
//        //-- Class Variables for data rate
//        amountData[self] = par("dataPacketSize");
//        int dataScaleFactor = par("dataScaleFactor");
//        //-- Finalized is the scaled-datarate: 4 times dataPktSize * scaleFactor
//        amountData[self] = amountData[self]*4*dataScaleFactor;
//
//        roundLengthGuard = par("roundLengthGuard");
//
//        if (isSink) {
//            sinkId = self;
//            energyConsumeds.resize(numNodes);
//            rxSizes.resize(numNodes);
//        }
//
//        declareOutput("Setup enery");
//        declareOutput("Setup time");
//        declareOutput("Collect energy");
//        declareOutput("Collect time");
//        declareOutput("End energy");
//        declareOutput("End time");
//        declareOutput("Round energy");
//        declareOutput("preList size");
//    ///////////////////////////////////////////////////////////////////////////////?ENDDD?////////////////////////////////
//}
//
//void GAMBAC::printOutRoundResults(){
//    metrics_trace() << "GAMBAC::printOutRoundResults";
//    radioModule->printRoundStats(roundNumber);
//    radioModule->resetRoundStats();
//}
//
//// this runs after initialization. Init func is for reading vars and setting vars, this is for everytime node wakes up
//void GAMBAC::startup()
//{
//    application_trace()<<"startup";
//    /*--- The .ned file's parameters ---*/
//    applicationID = par("applicationID").stringValue();
//
//    neighborRange = par("neighborRange");
//    //algo related variables
//    gamma = par("gamma");
//
//    //verysus var, investigate lifetime!//
//    maxCHperUAV = par("maxCHperUAV");
//    //////////////////////////////////////
//
//    //algo-related vars for optimizing
//    W_opt = -1;
//    W_start = 0;
//    countSuccess = 0;
//    countAdjusment = 0;
//    epsilon = par("epsilon"); //how much is leisured at max data can a CH be in charge of
//
//    debugRecruitProcess = par("debugRecruitProcess");
//
//    if (isSink) {
//        GAMBACinit(); //init for the algo if this is sink
//        application_trace() << "Operating under paramset: ";
//        application_trace() << "neighborRange " << neighborRange;
//        application_trace() << "epsilon " << epsilon;
//        application_trace() << "gamma " << gamma;
//        application_trace() << "maxCHperUAV " << maxCHperUAV;
//    }
//
//    //starting round with NO WARM UP
//    setTimer(START_ROUND, 0);
//}
//
//void GAMBAC::fromApplicationLayer(cPacket *pkt, const char *destination)
//{
//    //ATTENTION: destination IS NOT USED here,
//    //since this is a aggregating protocol, it is assumed that
//    //all pkts are routed already (nextHop ready), and the how is created by the routing layer
//    application_trace() << "GAMBAC::fromApplicationLayer";
//    if(!isSink)
//    {
//        routing_trace() << "GAMBAC dataPacket fromApplicationLayer, buffering...";
//        //save to intermediate buffer until MAC-ready
//        bufferPacket(pkt);
//    } else delete pkt;
//}
//
//void GAMBAC::finishSpecific(){
//    application_trace() << "GAMBAC::finishSpecific";
//}
//
//void GAMBAC::fromMacLayer(cPacket *pkt, int macAddress, double rssi, double lqi){
//
//    application_trace() << "GAMBAC::fromMacLayer";
//    RoutingPacket *netPacket = dynamic_cast <RoutingPacket*>(pkt);
//
//    if (!netPacket)
//        return; //turning away every packets that is not a RoutingPacket type from MAC layer
//    else {
//        cPacket* dataPkt = decapsulatePacket(pkt);
//        toApplicationLayer(dataPkt);
//    }
//}
//
//void GAMBAC::timerFiredCallback(int index)
//{
//    switch (index) {
//
//        case START_ROUND:{
//
//            if(isSink)
//                for (int i=0; i<3; i++)
//                    routing_trace() << "START_ROUND number " << roundNumber;
//
//            if (roundNumber == 0) {
//                //first round setting ups
//                resMgrModule->resetBattery();
//                if (isSink)
//                    routing_trace() << "Energy\tE_min\ti_min\tlevel\tconfig.clus_id[i_min]\tcalculateRxSize(i_min)\tweights[i_min]\tE_total/numNodes\ttotalConsumed\tmaxConsumed\tmaxTxSize\tconfig.A.size()\tdevE\tavgConsumed\tobjective value\tE0_min\ttime_elapse\ttotalCollected\tmaxLengthRatio";
//            }
//
//            // Sink logic for routing algorithm
//            if (isSink) {
//
//                routing_trace() << "starting round main Algo at Sink";
//                setTimer(START_MAINALG, 1); //
//
//                routing_trace() << "setting round end signal at Sink for " << this->roundLength << " time unit";
//                setTimer(END_ROUND, this->roundLength - this->roundLengthGuard);
//                //TODO_: change these roundLength* vars into class variables, as they should be
//
//                E_min = DBL_MAX;
//                for (int i=0; i<numNodes; i++) {
//                    if (i == self) continue;
//                    double E_i = getResMgrModule(i)->getRemainingEnergy();
//                    if (E_i < E_min) {
//                        E_min = E_i;
//                    }
//                }
//                E0_min = E_min;
//                //This is intended, the END_ROUND signal only applies to Sink
//                //TODO_: very counter-intuitive, change this timer's name to be better
//                setTimer(END_ROUND, this->roundLength - this->roundLengthGuard);
//            } else {
//                //For non-Sink Nodes:
//                //start sending data from 2nd round, to send
//                //the previous round collected data-packets, after exactly 1 second of the system
//                if(roundNumber!=0) setTimer(SEND_DATA, 1);
//            }
//
//            //couple of event timers to set this as a closed loop
//            roundNumber++;
//            setTimer(START_ROUND, roundLength); //   ------|     ROUND     | GUARD |     ROUND     | GUARD |----->time
//
//            if(isSink) routing_trace() << "Setting round start signal every node after " << this->roundLength << " seconds";
//
//            break;
//        }
//        //special timer, only Sink gets
//        case START_MAINALG:{
//
//            if(!isSink) throw cRuntimeError("Wrong algo signal event set at non-sink node.");
//            //timed algorithm running
//            auto start = std::chrono::steady_clock::now();
//            totalCollected = 0;
//            mainAlg();
//            auto end = std::chrono::steady_clock::now();
//            time_elapse = end - start;
//            break;
//        }
//
//        case SEND_DATA:{
//            routing_trace() << "Self-generated SEND_DATA signal, proceed to send everything buffered in the Routing queue";
//            if(!scheduled or isSink) {
//                routing_trace() << "Is not scheduled or is at Sink, faulty post-algorithmic logic";
//                break;
//            }
//            processBufferedPacket();
//            break;
//        }
//
//        case END_ROUND:{
//            routing_trace() << "Sink END_ROUND signal, proceed to do clean-ups, there is nothing to be done here";
//            //TODO_: this seem like a legacy of logging remaining energy,
//            //we currently do not have a use for this, consider deprecate and delete code
//
//            int i_min; //Index of Node with minimal energy right now (after round)
//            E_min = DBL_MAX;
//            double E_total = 0; //Entire network accumulated energy amount
//
//            for (int i=0; i<numNodes; i++) {
//                if (i == self) continue;
//                double E_i = getResMgrModule(i)->getRemainingEnergy();
//                if (E_i < E_min) {
//                    E_min = E_i;
//                    i_min = i;
//                }
//                E_total += E_i;
//            }
//            routing_trace() << "Round " << roundNumber << " found e_min " << E_min << " at node " << i_min;
//            routing_trace() << "Round " << roundNumber << " total energy entire network " << E_total;
//
//            int node_order = 0; //node rank, from bottom up
//            int current = i_min;//start from the node with minimum energy amount
//            while (config.clus_id[current] != -1) { //traverse up to the CH, collect the step count to CH
//                node_order++;
//                current = config.nextHop[current];
//            }
//
//            double E_avg = E_total/(numNodes-1); //omit the Sink
//            double devE = 0;
//            for (int i=0; i<numNodes; i++) {
//                if (i == self) continue;
//                double E_i = getResMgrModule(i)->getRemainingEnergy();
//                devE += (E_i - E_avg) * (E_i - E_avg);
//            }
//            devE = devE/(numNodes-1);
//            devE = sqrt(devE);
//            double avgConsumed = 0;
//            for (int i=0; i<numNodes; i++) {
//                if (i == self) continue;
//                avgConsumed += energyConsumeds[i];
//            }
//            avgConsumed = avgConsumed/(N-1);
//            double objVal = -E_min/E0_min/2 + 1000*avgConsumed/1.8937;
//            routing_trace() << "E_min " << E_min;
//            break;
//        }
//    }
//}
//
////TODO_: change this function's name since this mechanism only applies to non-sink nodes
//void GAMBAC::processBufferedPacket()
//{
//    application_trace()<<"processBufferedPacket";
//    if(isSink) throw cRuntimeError("Sending packet signal");
//    metrics_trace() << "Routing TXBuffer holding " << this->TXBuffer.size() << " packets, dumping everything to MAC layer.";
//
//    //safe guard for empty buffer
//    if (this->TXBuffer.size()==0) return;
//
//    //dumping contents to MAC layer buffer
//    do {
//        //THE right way of sending any APP pkt:
//        //encapsulate data with a routing packet containing routing info
//        //  BypassRoutingPacket *netPacket = new BypassRoutingPacket("BypassRouting packet", NETWORK_LAYER_PACKET);
//        //  netPacket->setSource(SELF_NETWORK_ADDRESS);
//        //  netPacket->setDestination(atoi(destination));
//        //  encapsulatePacket(netPacket, pkt);
//        //  toMacLayer(netPacket, resolveNetworkAddress(destination));
//        cPacket* dataPkt = check_and_cast <cPacket*>(TXBuffer.front());
//        GAMBACPacket* netPkt = new GAMBACPacket("Data Sample", NETWORK_LAYER_PACKET);
//        netPkt->setByteLength(dataPacketSize);
//        netPkt->setSource(self);
//
//        //always send to next Hop in the routing tree
//        netPkt->setDestination(config.nextHop[self]);
//
//        //package this packet
//        encapsulatePacket(netPkt, dataPkt);
//
//        VirtualRouting::toMacLayer(netPkt, config.nextHop[self]);
//        TXBuffer.pop();
//    } while (!TXBuffer.empty());
//
//}
//
//void GAMBAC::mainAlg() {
//    application_trace()<<"GAMBAC starts at RUN_MAINALG timer event";
//
//    //set up node energies
//    for (int u=0; u<numNodes; u++) nodes_energy[u] = getResMgrModule(u)->getRemainingEnergy();
//
//    //TODO_:URGENT: decode and rename A
//    config.A.clear();
//
//    routing_trace() << "START mainAlg";
//
//    //Weight is related to datarate, infact, W IS DATA AMOUNT each round
//    //TODO_:URGENT: change this amountData into data packet in queue sum, for exact simulation
//    W_total = 0;
//    for (int i=0; i<numNodes; i++) W_total += amountData[i];
//    W_start = W_total/numUAVs/2;
//    W_end = W_start+1000; //1000 Joules search range is a ball-park estimation
//
//    W = (W_start+W_end)/2;
//
//    bool lengthViolated;
//    bool isSaved = false;
//    int retry = 0;
//    double ratioToLmax = 1;
//    double maxLen = 0;
//
//    do {
//        reset();
//
//        routing_trace() << " W_start " << W_start << "W " << W << " W_end " << W_end;
//        routing_trace() << "selectCHsAndBuildDFT";
//        int loop_count = selectCHsAndBuildDFT(W);
//        routing_trace() << "with loop_count: " << loop_count;
//        routing_trace() << "buildTrajectories " << A.size();
//        if (A == config.A) {
//            trajectories = config.trajectories;
//        }
//        else buildTrajectories(true);
//        lengthViolated = false;
//        maxLen = 0;
//
//        // verify trajectories length of built clusters
//        for (int t=0; t<numUAVs; t++) {
//            double len = calculatePathLength(trajectories[t]);
//            if (len > L_max) {
//                lengthViolated = true;
//                routing_trace() << "GAMBAC Algorithm logic failure with traj len " << len;
//            };
//            if (len > maxLen) maxLen = len;
//        }
//        ratioToLmax = pow(maxLen / L_max, 2);
//        routing_trace() << "maxLen " << maxLen << " ratioToLmax " << ratioToLmax;
//
//        if (lengthViolated) {
//            // Increase W for each cluster to reduce CHs count
//            retry += 1;
//            if (retry >= 5) {
//                routing_trace() << "Max retry of 5 reached, reset W_end to current W to narrow binary search";
//                //reset Binary Search of cluster weight
//                W_end = W;
//                W = W*ratioToLmax;
//                retry = 0;
//            }
//            else continue;
//        } else {
//            routing_trace() << "No violated length, algorithm result satisfied UAV travel capacity!";
//            if (A.size() > config.A.size() && loop_count <= 100){
//                routing_trace() << "save config";
//                config.save(A, clus_id, nextHop, trajectories);
//                isSaved = true;
//                W_start = W;
//                W = W*ratioToLmax;
//            }
//            else {
//                retry += 1;
//                if (retry >= 5) {
//                    if (!isSaved) {
//                        config.save(A, clus_id, nextHop, trajectories);
//                        isSaved = true;
//                    }
//                    W_start = W;
//                    W = W*ratioToLmax;
//                    retry = 0;
//                }
//                continue;
//            }
//
//            retry = 0;
//        }
//
//        if (W_start - W <= 10000) break; //what is this
//
//        if ((W_start-W_end <= 10000) && !isSaved) {
//            W_start = W_total/numUAVs/2;
//            W_end = 1000;
//            W_opt = -1;
//        }
//
//    } while (W_start-W_end>10000);
//
//    //saving current config nomaterwhat
//    if (!isSaved) {
//        config.save(A, clus_id, nextHop, trajectories);
//        isSaved = true;
//    }
//
//    A = config.A;
//    ///////////////////////////////////////////////////////////
//
//    double maxLength = 0;
//    for (auto T : config.trajectories) {
//        double length = calculatePathLength(T);
//        if (length > maxLength) maxLength = length;
//
//        stringstream ss;
//        for(int l : T) {
//            ss << l << " ";
//        }
//        routing_trace() << ss.str();
//    }
//    routing_trace() << "maxLength " << maxLength;
//    maxLengthRatio = maxLength / L_max;
//
//    //TODO_: printout round clustering result:
//    metrics_trace() << "Routing clustering results for ROUNDNUMBER" << this->roundNumber << " below: ";
//
//    for(int i=0; i<numNodes; i++){
//        routing_trace() << "RouteInfoForNode" << i << config.getRoute(i);
//    }
//
//    if(!scheduled) scheduled=true;
//    metrics_trace() << "END mainAlg";
//}
//
//void GAMBAC::GAMBACinit() {
//
//    for (int i=0; i<10; i++) trace() << "GAMBACinit";
//
//    N = numNodes;
//    graph.init(numNodes, self, d0);
//    trajectories.resize(numUAVs);
//
//    d2CH.resize(N, 0.); //distance to appointed CH, is inf if no appointed CH
//    clus_id.resize(N, -1); //ID of appointed CHs, indexed by NodeId
//    nextHop.resize(N, -1); //ID of nextHops, indexed by NodeId
//    isCH.resize(N, false); //self-expl, indexed by NodeId
//    representSet.resize(N); // ??
//    w_max.resize(N); // ??
//    nodes_energy = vector<double>(numNodes, 0); //current Energy
//
//    //TODO_legacy_code: all of these vars are outside and discrete, why is it not in "config"?
//}
//
//void GAMBAC::reset() {
//
//    application_trace()<<"GAMBAC reseting";
//    trajectories = vector<vector<int>>(numUAVs);
//
//    A.clear();
//    distanceToCH = vector<double>(N, 0);
//    clus_id = vector<int>(N, -1);
//    nextHop = vector<int>(N, -1);
//    isCH = vector<bool>(N, false);
//    clusterMembers = vector<list<int>>(N);
//    w_max = vector<double>(N, 0);
//
////    int
////    if(roundNumber==3){
////        int a=0;
////    }
//}
//
//vector<int> GAMBAC::randomFromSet(vector<int> Candidates, double s) {
//    application_trace()<<"randomFromSet";
//
//    int nodeW = Candidates.size();
//    if (nodeW > 0) {
//        if (nodeW < s) {
//            for (int node : Candidates) {
//                isCH[node] = true;
//            }
//            return Candidates;
//        } else {
//            double prob_net = s / nodeW;
//            vector<int> selected_candidates;
//            for (int node : Candidates) {
//                if (isCH[node]) continue;
//                double random_float = uniform(0,1);
//                if (random_float < prob_net) {
//                    selected_candidates.push_back(node);
//                    isCH[node] = true;
//                }
//            }
//            if (selected_candidates.empty()) return randomFromSet(Candidates, s);
//            else {
//                return selected_candidates;
//            }
//        }
//    }
//
//    stringstream ss;
//    for(int i=0; i<Candidates.size(); i++) ss << " " << Candidates[i];
//    routing_trace() << "Picked candidates: "<<ss.str();
//
//    return Candidates;
//}
//
//void GAMBAC::growBalls(vector<int> CHSet){
//    routing_trace() <<"Grow Clustered Balls from a set of CHs";
//
//    for (int u : graph.getNodesExceptSink()){
//        if (isCH[u]){
//            distanceToCH[u] = 0;
//        }
//        else {
//            distanceToCH[u] = DBL_MAX;
//        }
//        clus_id[u] = -1;
//        nextHop[u] = -1;
//        clusterMembers[u].clear();
//    }
//
//    for (int u : CHSet) {
//        distanceToCH[u] = 0;
//        clusterMembers[u].clear();
//        clus_id[u] = u;
//    }
//
//    dCompare =  &distanceToCH;
//    priority_queue<int,vector<int>, decltype(&Comparebydistance)> queue(Comparebydistance);
//    for (int l : CHSet) {
//        queue.push(l);
//    }
//    unordered_set<int> removedSet;
//
//    while (!queue.empty()) {
//        int u = queue.top();
//        queue.pop();
//        if (removedSet.find(u) != removedSet.end()) continue;
//        removedSet.insert(u);
//        if (!isCH[u]) clusterMembers[clus_id[u]].push_back(u);
//        for (int v : graph.getAdjExceptSink(u)) {
//            if ((removedSet.find(v) != removedSet.end())) continue;
//            double alt = distanceToCH[u] + graph.getLength(u,v);
//
//            if (alt - distanceToCH[v] < -EPSILON){
//                distanceToCH[v] = alt;
//                clus_id[v] = clus_id[u];
//                nextHop[v] = u;
//                queue.push(v);
//            }
//        }
//    }
////    int totalSize = 0;
////
//    routing_trace()<<"growBalls ends";
////
////    routing_trace()<<"vector clus_id size :" <<clus_id.size();
////    routing_trace()<<"NumNodes:" << numNodes;
//
////
////    for (int u=0; u<numNodes; u++) {
////        if (u ==sinkId) continue;
////        if (clus_id[u] == -1) {
////            stringstream ss;
////            for(int l : A) {
////                ss << l << " ";
////            }
////        }
////    }
//    computeBallWeight();
//}
//
//vector<int> GAMBAC::getOuterOversizePart(){
//    application_trace() <<"getOuterOversizePart";
//
//    outerSet.clear();
//    innerSet.clear();
//    vector<int> returnlist;
//    for (int CH : A){
//        double weight = ballWeight.find(CH)->second;
//
//        if (weight > w_max[CH]) {
//            weight = 0;
//            dCompare =  &distanceToCH;
//            priority_queue<int,vector<int>, decltype(&Comparebydistance)> queue(Comparebydistance);
//            for (int u : clusterMembers[CH]) queue.push(u);
//            while (weight < w_max[CH]){
//                int u = queue.top();
//                queue.pop();
////                if (debugRecruitProcess) for (int i=0; i<1; i++) routing_trace() << "CH " << CH << " inner " << u << " distanceToCH " << distanceToCH[u] << " weight " << weight;
//                weight += amountData[u];
//                if (weight >= w_max[CH]) break;
//                innerSet.push_back(u);
//            }
//            while (!queue.empty()) {
//                int u = queue.top();
////                if (debugRecruitProcess) for (int i=0; i<1; i++) routing_trace() << "CH " << CH << " fringe " << u << " distanceToCH " << distanceToCH[u];
//                queue.pop();
//                outerSet.push_back(u);
//            }
//        }
//    }
//    return returnlist.empty() ? outerSet : returnlist;
//}
//
//vector<int> GAMBAC::samplingCH(vector<int> Candidates, double b) {
//    application_trace()<<"samplingCH";
//
//    for (int i=0; i<0; i++) routing_trace() << "samplingCH";
//    int maxTrial = 3;
//    vector<int> selected_candidates;
//    while (!Candidates.empty()){
//        int nextLandmark = Candidates[rand() % Candidates.size()];
//
//        if (isCH[nextLandmark]) {
//            Candidates.erase(std::remove(Candidates.begin(), Candidates.end(), nextLandmark), Candidates.end());
//            continue;
//        }
//
//        vector<int> intersectBall;
//        double newBallSize = 0;
//        double intersectBallSize = 0;
//
//        vector<double> d(N, DBL_MAX);
//        d[nextLandmark] = 0;
//        dCompare =  &d;
//        priority_queue<int,vector<int>, decltype(&Comparebydistance)> queue(Comparebydistance);
//        queue.push(nextLandmark);
//        unordered_set<int> removedSet;
//        while (!queue.empty()){
//            int v = queue.top();
//            queue.pop();
//            if (removedSet.find(v) != removedSet.end()) continue;
//            removedSet.insert(v);
//            newBallSize += amountData[v];
//            if (newBallSize > w_max[nextLandmark]) break;
//            if (std::find(Candidates.begin(), Candidates.end(), v) != Candidates.end()){
//                Candidates.erase(std::remove(Candidates.begin(), Candidates.end(), v), Candidates.end());
//                intersectBall.push_back(v);
//                intersectBallSize += amountData[v];
//            }
//            for (int w : graph.getAdjExceptSink(v)){
//                if ((w < N) && (removedSet.find(w) == removedSet.end())){
//                    double alt = d[v] + graph.getLength(v,w);
//                    if (alt < d[w]){
//                        d[w] = alt;
//                        queue.push(w);
//                    }
//                }
//            }
//        }
//        if ((maxTrial == 0) || (intersectBallSize>W/4)){//selected_candidates.empty() &&
//            selected_candidates.push_back(nextLandmark);
//            maxTrial = 3;
//        }
//        else{
//            for (int v : intersectBall) Candidates.push_back(v);
//            if (maxTrial>0){
//                maxTrial--;
//                continue;
//            }
//            else break;
//        }
//    }
//    return selected_candidates;
//}
//
//void GAMBAC::computeBallWeight(){
//    routing_trace()<<"computeBallWeight";
//
//    maxBallWeight = 0;
//    minBallWeight = INT64_MAX;
//
//    double tempBallWeight[A.size()];
//    for (int i=0; i<A.size(); i++) tempBallWeight[i] = 0;
//    int size = 0;
//    for (int i=0; i<A.size(); i++){
//        size += clusterMembers[A[i]].size();
//        for (int u : clusterMembers[A[i]]) {
//            tempBallWeight[i] += amountData[u];
//        }
//    }
//    ballWeight.clear();
//    for (int i=0; i<A.size(); i++){
//        ballWeight.insert(pair<int,double>(A[i], tempBallWeight[i]));
//    }
//
//    routing_trace()<<"computeBallWeight ends";
//
//}
//
//void GAMBAC::recruitNewCHs(){
//    application_trace()<<"recruitNewCHs";
//
//    for (int i=0; i<0; i++) routing_trace() << "recruitNewCHs";;
//    vector<int> Candidates = getOuterOversizePart();
//    // routing_trace() << "Candidates size " << Candidates.size();
//    vector<int> newCHs = samplingCH(Candidates, 0);
//    if (!newCHs.empty()) A.insert(A.end(), newCHs.begin(), newCHs.end());
//    for (int u : newCHs) {
//        isCH[u] = true;
//    }
//}
//
//int GAMBAC::selectCHsAndBuildDFT(double W){
//    application_trace()<<"selectCHsAndBuildDFT___";
//
//    E_min = DBL_MAX;
//    E_max = 0;
//
//    //traverse the list to find
//    //network-wide E(nergy) min and max values
//    for (int i=0; i<numNodes; i++) {
//        if (i == sinkId) continue;
//        if (nodes_energy[i] > E_max) E_max = nodes_energy[i];
//        if (nodes_energy[i] < E_min) E_min = nodes_energy[i];
//    }
//
//    for (int i=0; i<N; i++) {
//        if (i == sinkId) continue;
//        w_max[i] = W;
//    }
//
//    //specify the maximal number of clusters,
//    //using the maximal data volume per-cluster: W
//    //this value is calculated using a Binary Search
//    //between (Wmax and Wmin of )
//    int k_max = (int) ((1 + epsilon) * W_total / W);
//
//    int count = 0;
//    vector<int> allSNs;
//    for (int i=0; i<N; i++) allSNs.push_back(i);
//    while (A.empty()) {
//        A = randomFromSet(allSNs, 8);
//    }
//    routing_trace() << "initial size " << A.size();
//    growBalls(A);
//    bool isBalance;
//    do {
//        int Asize = A.size();
//        recruitNewCHs();
//        growBalls(A);
//
//        isBalance = true;
//        double W_new = min(W, (1 + epsilon) * W_total / A.size());
//        double maxWeight = 0;
//        double totalWeight = 0;
//        for (auto pair : ballWeight) {
//            if (maxWeight < pair.second) {
//                maxWeight = pair.second;
//            }
//        }
//        if (maxWeight > W_new) isBalance = false;
//
//        if (A.size() > k_max) {
//            vector<double> tempBallWeight(N, 0);
//            for (auto pair : ballWeight) {
//                tempBallWeight[pair.first] = pair.second;
//            }
//            dCompare =  &tempBallWeight;
//            priority_queue<int,vector<int>, decltype(&Comparebydistance)> Q(Comparebydistance);
//            for (int l : A) Q.push(l);
//            while (A.size() > k_max){
//                double WeightTotal = 0;
//                for (int l : A) {
//                    WeightTotal += 1./tempBallWeight[l];
//                }
//                double f = (double)rand() / RAND_MAX * WeightTotal;
//                int u;
//                bool assigned = false;
//                vector<double> fss;
//                for (int l : A) {
//                    assigned = false;
//                    f -= 1./tempBallWeight[l];
//                    fss.push_back(f);
//                    if (f <= 0) {
//                        u = l;
//                        break;
//                    } else continue;
//                }
//                A.erase(std::remove(A.begin(), A.end(), u), A.end());
//                if(u<0){
//                    throw cRuntimeError("indexing failure, logic false at GAMBAC.cc line 645");
//                }
//                isCH[u] = false;
//            }
//        }
//        growBalls(A);
//
//        isBalance = true;
//        W_new = min(W, (1 + epsilon) * W_total / A.size());
//        maxWeight = 0;
//        totalWeight = 0;
//        for (auto pair : ballWeight) {
//            if (maxWeight < pair.second) {
//                maxWeight = pair.second;
//            }
//            if (w_max[pair.first] < pair.second) {
//                isBalance = false;
//            }
//        }
//        routing_trace() << "loop " << count << " epsilon " << maxWeight/W_total*A.size() - 1;
//
//        if (count++ > 50) {
//            break;
//        }
//
//        countAdjusment++;
//        nloop = count;
//    } while (!isBalance);
//
//    double maxWeight = 0;
//    for (auto pair : ballWeight) {
//        if (pair.second > maxWeight) maxWeight = pair.second;
//    }
//    routing_trace() << "epsilon " << maxWeight/W_total*A.size() - 1 << " count " << count;
//    clus_id[self] = -1;
//    for (int l : A) clus_id[l] = -1;
//    for (int i=0; i<1; i++)routing_trace() << "A size " << A.size();
//    routing_trace() << "W " << W;
//    routing_trace() << "k_max " << k_max;
//    return count;
//}
//
//void GAMBAC::buildTrajectories(){
//    buildTrajectories(true);
//}
//
//void GAMBAC::buildTrajectories(bool isBreak){
//    for (int i=0; i<numUAVs; i++)
//        trajectories[i].clear();
//    if (A.size() < numUAVs) {
//        for (int i=0; i<A.size(); i++) {
//            trajectories[i].push_back(self);
//            trajectories[i].push_back(A[i]);
//            trajectories[i].push_back(self);
//        }
//    }
//    else {
//        auto start = high_resolution_clock::now();
//        trajectories = Vrptw::call(*this, sinkId, A, numUAVs, L_max);
//        auto stop = high_resolution_clock::now();
//        auto duration = duration_cast<microseconds>(stop - start);
//
//        routing_trace() << "Check Vrptw " << A.size() << " " << duration.count();
//        // trajectories = callTspga(*this, sinkId, A, numUAVs, L_max);
//        while (trajectories.size() < numUAVs) {
//            trajectories.push_back(vector<int>());
//        }
//    }
//
//    double maxLength = 0;
//    unordered_set<int> CHs;
//    CHs.insert(A.begin(), A.end());
//    for (int k=0; k<numUAVs; k++) {
//        double length = calculatePathLength(trajectories[k]);
//        if (length > maxLength) maxLength = length;
//       routing_trace() << "trajectory " << k << " length " << length;
//        stringstream ss;
//        for(int i : trajectories[k]) {
//            ss << i << " ";
//            CHs.erase(i);
//        }
//       routing_trace() << ss.str();
//    }
//
//   routing_trace() << "maxLength " << maxLength;
//    for (int i : CHs)
//        routing_trace() << i << " is unvisited!!!";
//
//    for (int k=0; k<trajectories.size(); k++) {
//        if (trajectories[k].empty()) {
//            trajectories[k].push_back(sinkId);
//            trajectories[k].push_back(sinkId);
//        }
//    }
//};
//
//vector<vector<int>> GAMBAC::buildInitialTrajectories(){
//    application_trace()<<"buildInitialTrajectories";
//    vector<int> A_tmp = A;
//    vector<int> TSP_tour = TSP(A_tmp);
//    vector<vector<int>> new_trajectories;
//
//    routing_trace() << "TSPLength = " << calculatePathLength(TSP_tour);
//    stringstream ss_tsp;
//    for (int l : TSP_tour) {
//        ss_tsp << l << " ";
//    }
//    routing_trace() << "TSP tour: " << ss_tsp.str();
//    double minMaxLength = DBL_MAX;
//    for (int offset=0; offset<TSP_tour.size(); offset++) {
//        double L_max_last = L_max;
//        double maxLengthLast;
//        double maxLength = DBL_MAX;
//        vector<vector<int>> tmp_trajectories;
//        do {
//            maxLengthLast = maxLength;
//            maxLength = 0;
//            tmp_trajectories = vector<vector<int>>(numUAVs);
//            for (auto &trajectory : tmp_trajectories) trajectory.push_back(sinkId);
//            int cur_trajectory_id = 0;
//            double curLength = 0;
//            double totalLength = 0;
//            for (int i=0; i<TSP_tour.size(); i++) {
//                int node = TSP_tour[(i+offset)%TSP_tour.size()];
//                vector<int> &cur_trajectory = tmp_trajectories[cur_trajectory_id];
//                int lastNode = cur_trajectory[cur_trajectory.size()-1];
//                double curLengthLast = curLength;
//                curLength += distance(lastNode, node) + distance(node, sinkId) - distance(lastNode, sinkId);
//                if (curLength > L_max_last) {
//                    if (cur_trajectory_id < numUAVs-1) {
//                        cur_trajectory.push_back(sinkId);
//                        totalLength += curLengthLast;
//                        cur_trajectory_id++;
//                        vector<int> &next_trajectory = tmp_trajectories[cur_trajectory_id];
//                        next_trajectory.push_back(node);
//                        curLength = distance(sinkId, node)*2;
//                        if (curLength > maxLength) maxLength = curLength;
//                    }
//                    else {
//                        cur_trajectory.push_back(node);
//                        if (curLength > maxLength) maxLength = curLength;
//                    }
//                }
//                else {
//                    cur_trajectory.push_back(node);
//                    if (curLength > maxLength) maxLength = curLength;
//                }
//            }
//            tmp_trajectories[cur_trajectory_id].push_back(sinkId);
//            totalLength += curLength;
//            L_max_last = totalLength / numUAVs;
//            if (maxLength < minMaxLength) {
//                minMaxLength = maxLength;
//                new_trajectories = tmp_trajectories;
//            }
//        } while (maxLength < maxLengthLast);
//    }
//
//    routing_trace() << "minMaxLength = " << minMaxLength;
//
//    int jj = 1;
//    for (auto trajectory : new_trajectories) {
//        stringstream ss_tra;
//        for (int u : trajectory) ss_tra << u << " ";
//        routing_trace() << "Tour " << jj << ": " << ss_tra.str() << " - Length: " << calculatePathLength(trajectory);
//        jj++;
//    }
//    stringstream ss_tras;
//    ss_tras << "Round ="<< roundNumber <<" = TRAJS_HERE ";
//    for (auto traj: new_trajectories){
//        for (int u : traj)
//            ss_tras << u << ",";
//        ss_tras << ";";
//    }
//    routing_trace() << ss_tras.str();
//
//    vector<vector<int>> tours(numUAVs);
//    vector<int> A_new;
//    int matchingId = 1;
//    for (int i=0; i<numUAVs; i++) {
//        for (int u : new_trajectories[i]) {
//            if (u != sinkId) {
//                A_new.push_back(u);
//                tours[i].push_back(matchingId);
//                matchingId++;
//            }
//        }
//    }
//    A = A_new;
//    return tours;
//}
//
//vector<int> GAMBAC::TSP(vector<int> AA) {
//    application_trace()<<"TSP";
//
//    vector<int> TSP_tour;
//    vector<Point> sites;
//    for (int l : AA) {
//        Point p = location(l);
//        p.id_ = l;
//        sites.push_back(p);
//    }
//
//    while (!sites.empty()) {
//        vector<Point> convexHull = G::convexHull(sites);
//        if (convexHull.empty()) {
//            convexHull = sites;
//        }
//        for (Point p : convexHull) {
//            sites.erase(std::remove(sites.begin(), sites.end(), p), sites.end());
//        }
//        if (TSP_tour.empty()) {
//            for (Point p : convexHull) {
//                TSP_tour.push_back(p.id_);
//            }
//        }
//        else {
//            while (!convexHull.empty()) {
//                double minIncreased = DBL_MAX;
//                int i_min = -1;
//                int pos = -1;
//                for (int i=0; i<convexHull.size(); i++) {
//                    Point p = convexHull[i];
//                    for (int j=0; j<TSP_tour.size(); j++) {
//                        Point A = location(TSP_tour[j]);
//                        Point B = location(TSP_tour[(j+1)%TSP_tour.size()]);
//                        double lengthIncreased = G::distance(A, p) + G::distance(p, B) - G::distance(A, B);
//                        if (lengthIncreased < minIncreased) {
//                            minIncreased = lengthIncreased;
//                            i_min = i;
//                            pos = (j+1)%TSP_tour.size();
//                        }
//                    }
//                }
//
//                Point insertedNode = convexHull[i_min];
//                convexHull.erase(convexHull.begin() + i_min);
//                TSP_tour.insert(TSP_tour.begin() + pos, insertedNode.id_);
//            }
//        }
//    }
//    return TSP_tour;
//}
//
//double GAMBAC::txEnergy(double byteLength, double distance) {
//    return 1 * (50e-9 * byteLength * 8 + 10e-12 * byteLength * 8 * distance * distance);
//}
//
//double GAMBAC::rxEnergy(double byteLength) {
//    return 1 * (50e-9 * byteLength * 8);
//}
//
//double GAMBAC::calculateCHEnergy(int u, double byteLength) {
//    return getResMgrModule(u)->getRemainingEnergy() - rxEnergy(byteLength) - txEnergy(byteLength + amountData[u], D2UAV);
//}
//
//double GAMBAC::calculateRxSize(int curnode) {
//    double rxSize = 0;
//    queue<int> q;
//    q.push(curnode);
//    while (!q.empty()) {
//        int u = q.front();
//        q.pop();
//
//        for (int i=0; i<numNodes; i++) {
//            if (config.nextHop[i] == u) {
//                rxSize += amountData[i];
//                q.push(i);
//            }
//        }
//    }
//    return rxSize;
//}
//
//Point GAMBAC::location(int id) {
//    return GlobalLocationService::getLocation(id);
//}
//
//double GAMBAC::calculatePathLength(vector<int> path) {
//    if (path.empty()) return 0;
//    double pathLength = 0;
//    for (int i=0; i<path.size()-1; i++){
//        pathLength += G::distance(GlobalLocationService::getLocation(path[i]),
//            GlobalLocationService::getLocation(path[i+1]));
//    }
//    return pathLength;
//}
//
//bool Comparebydistance(int lhs, int rhs) {
//    return (GAMBAC::dCompare->at(lhs) > GAMBAC::dCompare->at(rhs));
//}
//
////void GAMBAC::logConfig() {
////    auto nextHop = config.nextHop;
////    auto clus_id = config.clus_id;
////    auto trajectories = config.trajectories;
////
////    int N = numNodes;
////    for (int i=0; i<N; i++) {
////        auto P = GlobalLocationService::getLocation(i);
////        CastaliaModule::trace2() << roundNumber << "\tPOINT\tblue\t" << P.x() << "\t" << P.y() << "\t" << amountData[i] << "\t" << clus_id[i] << "\t" << i << "\t" << nextHop[i];
////    }
////    for (int i=0; i<N; i++) {
////        if (clus_id[i] != -1) {
////            auto P = GlobalLocationService::getLocation(i);
////            auto Q = GlobalLocationService::getLocation(nextHop[i]);
////            CastaliaModule::trace2() << roundNumber << "\tLINE\tgreen\t" << P.x() << "\t" << P.y() << "\t" << Q.x() << "\t" << Q.y();
////        }
////    }
////    for (int k=0; k<trajectories.size(); k++) {
////        for (int i=0; i<trajectories[k].size()-1; i++){
////            auto P = GlobalLocationService::getLocation(trajectories[k][i]);
////            auto Q = GlobalLocationService::getLocation(trajectories[k][i+1]);
////            CastaliaModule::trace2() << roundNumber << "\tLINE\tred\t" << P.x() << "\t" << P.y() << "\t" << Q.x() << "\t" << Q.y();
////        }
////    }
////}
////
//
//ResourceManager* GAMBAC::getResMgrModule(int id) {
//    if (isSink) return check_and_cast <ResourceManager*>(getParentModule()->getParentModule()
//        ->getParentModule()->getSubmodule("node", id)->getSubmodule("ResourceManager"));
//    return nullptr;
//}
//
