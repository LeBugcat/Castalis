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

#include "GAMBACFPercent.h"

Define_Module(GAMBACFPercent);


// Gotta do this since this algo is traffic aware routing with updates to dataPacketSizeAt
void GAMBACFPercent::timerFiredCallback(int index)
{

	switch (index) {
		case START_MAINALG:{

		    dataPacketSizeAt[self]= smplAggAppModule->currentDataPktSize;

		    sinkId = 0;

//		    application_trace() << "GAMBACFPercent::timerFiredCallback case StartAlg";
			if(isSink) mainAlg();

			break;
		}
		default:{
			GAMBAC::timerFiredCallback(index);
		}
	}
}

void GAMBACFPercent::mainAlg() {
    dataPacketSizeAt[self]= smplAggAppModule->currentDataPktSize;

    for (int u=0; u<numNodes; u++) remainingEnergy_tmp[u] = getResMgrModule(u)->getRemainingEnergy();
    config.A.clear();

//    for (int i=0; i<10; i++) routing_trace() << "mainAlg3";
    E_min = DBL_MAX;
    E_max = 0;
    for (int i=0; i<numNodes; i++) {
        if (i == self) continue;
        double E_i = getResMgrModule(i)->getRemainingEnergy();
        if (E_i > E_max) E_max = E_i;
        if (E_i < E_min) E_min = E_i;
    }
    dataVolume_total = 0;
    for (int i=0; i<numNodes; i++) dataVolume_total += dataPacketSizeAt[i];
    double f_end = 100;
    double f_start = 0;
    double f = (f_end+f_start)/2;

    bool lengthViolated;
    bool isSaved = false;
    int count = 0;
    double ratioToLmax = 1;
    double maxLen = 0;
    vector<int> A_old_saved;
    do {
        reset();

//        routing_trace() << "f " << f << " f_start " << f_start << " f_end " << f_end;
//        routing_trace() << "selectCHsAndBuildDFT";
        int loop_count = selectCHsAndBuildDFT(f);
//        application_trace() << "selectCHsAndBuildDFT " << loop_count;
//        routing_trace() << "buildTrajectories " << A.size();
        lengthViolated = false;
        maxLen = 0;

        if (A == config.A) {
            trajectories = config.trajectories;
        }
        else buildTrajectories();

        for (int t=0; t<numUAVs; t++) {
            double len = calculatePathLength(trajectories[t]);
            if (len > maxUAVFlyingDistance) {
                lengthViolated = true;
            };
            if (len > maxLen) maxLen = len;
        }

        ratioToLmax = pow(maxLen / maxUAVFlyingDistance, 2);

        if (lengthViolated) {
            count += 1;
//            routing_trace() << "reduce num. clusters, max length = " << maxLen << ", ratioToLmax = " << ratioToLmax;
            if (count >= 1) {
                f_start = f;
                count = 0;

                f *= ratioToLmax;
                if (f >= f_end) break;
                continue;
            }
            else continue;
        } else {
            if (A.size() > config.A.size() && loop_count <= 50){
//                routing_trace() << "save config";
                config.save(A, clusId, nextHop, trajectories);
                isSaved = true;
                f_end = f;
            }
            else {
                count += 1;
                if (count >= 1) {
                    if (!isSaved) {
//                        routing_trace() << "save config";
                        config.save(A, clusId, nextHop, trajectories);
                        isSaved = true;
                    }
                    f_end = f;
                    count = 0;
                }
                else continue;
            }
            f_end = f;
//            routing_trace() << "increase num. clusters, max length = " << maxLen << ", ratioToLmax = " << ratioToLmax;
            count = 0;
        }

        f = (f_end + f_start) / 2;
        if (f >= f_end) break;

    } while (f_end-f_start>0.002);
    countSuccess++;

//    application_trace() << "Ending main with f=" << f;

    double maxLength = 0;
    for (auto T : config.trajectories) {
        double length = calculatePathLength(T);
        if (length > maxLength) maxLength = length;
        stringstream ss;
        for(int l : T) {
            ss << l << " ";
        }
        routingTrace() << ss.str();
    }
//    application_trace() << "maxLength " << maxLength;
//    CastaliaModule::trace2() << roundNumber << "\tMAXLENGTH\t" << maxLength;
    maxLengthRatio = maxLength / maxUAVFlyingDistance;

    routingTrace() << "END mainAlg";

}


int GAMBACFPercent::selectCHsAndBuildDFT(double f){

    double percent = f/100;
    double E1 = rxEnergy(1) + txEnergy(1,distanceToUAV);
    for (int i=0; i<N; i++) {
        if (i == sinkId) continue;
        if (distance(i, sinkId) > distanceToUAV/2) w_max[i] = 0;
        else w_max[i] = remainingEnergy_tmp[i];
    }

    double totalWeight = 0;

    vector<int> A_min;
    double min_cnsmptn = DBL_MAX;
    stringstream ss_csmpt;

    int retries_count = 0;

    for (int retries=0; retries<50; retries++) {
        retries_count= retries;
        A.clear();
        unordered_set<int> uncovered_nodes;
        for (int u=0; u<numNodes; u++) {
            if (u != sinkId) uncovered_nodes.insert(u);
        }
        while (!uncovered_nodes.empty()) {
            auto it = std::begin(uncovered_nodes);
            auto r = rand() % uncovered_nodes.size();
            std::advance(it,r);
            int new_CH = *it;

            // Grow Ball
            A.push_back(new_CH);

            growBallsPercent(A, percent);
            for (int i=0; i<N; i++) {
                if (i == sinkId) continue;
                if (clusId[i] != -1) uncovered_nodes.erase(i);
            }
        }

        double rxSizes[numNodes];
        for (int u=0; u<numNodes; u++) { rxSizes[u]=0; }
        for (int u=0; u<numNodes; u++) {
            if (u == sinkId) continue;
            int tmp = u;
            while (nextHop[tmp] != -1) {
                rxSizes[nextHop[tmp]] += dataPacketSizeAt[u];
                tmp = nextHop[tmp];
            }
        }
        double new_cnsmptn = 0;
        for (int i=0; i<numNodes; i++) {
            if (i == sinkId) continue;
            double rxSize = rxSizes[i];//

            new_cnsmptn += rxEnergy(rxSize);
            //TODO: fix this, packetSizeAt is now officially important
            double txSize = rxSize + dataPacketSizeAt[i];
            if (clusId[i] == i) {
                new_cnsmptn += txEnergy(txSize, distanceToUAV);
            } else {
                double d2nextHop = G::distance(GlobalLocationService::getLocation(i), GlobalLocationService::getLocation(nextHop[i]));
                new_cnsmptn += txEnergy(txSize, d2nextHop);
            }
        }
        if (new_cnsmptn < min_cnsmptn) {
            min_cnsmptn = new_cnsmptn;
            A_min = A;
            ss_csmpt << min_cnsmptn << " ";
        }
    }

//    routing_trace() << "min_cnsmptn " << ss_csmpt.str();
    A = A_min;
    stringstream ss_A;
    for (int l : A) ss_A << l << " ";
    routingTrace() << "A " << ss_A.str();
    growBallsPercent(A, percent);;

    clusId[self] = -1;
    for (int l : A) clusId[l] = -1;

    return retries_count;
}

void iota(vector<long unsigned int>::iterator first, vector<long unsigned int>::iterator last, int value)
{
    while (first != last)
    {
        *first++ = value;
        ++value;
    }
}

template <typename T>
vector<size_t> GAMBACFPercent::sort_indexes(const vector<T> &v) {
  vector<size_t> idx(v.size());
  iota(idx.begin(), idx.end(), 0);
  stable_sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});

  return idx;
}

void GAMBACFPercent::growBallsPercent(vector<int> CHSet, double percent){
    for (int u : connectivityGraph.getNodesExceptSink()){ //iterate through all nodes in network
        if (isCH[u]){
            distanceToCH[u] = 0;
        }
        else {
            distanceToCH[u] = DBL_MAX; //init: all node has no CH
        }
        clusId[u] = -1;  //init: all node has no CH
        nextHop[u] = -1;   //init: all node has no nextHop
        clusterMembers[u].clear(); //init: if u is CH, it has no members
    }
    //END initialize

    for (int u : CHSet) {
        distanceToCH[u] = 0;
        clusterMembers[u].clear();
        clusId[u] = u;
    }

    dCompare =  &distanceToCH;
    priority_queue<int,vector<int>, decltype(&compareByDistance)> queue(compareByDistance);
    for (int l : CHSet) {
        queue.push(l);
    }
    unordered_set<int> removedSet;
    vector<double> T(N, 0);
    vector<int> level1_node(N, -1);

    while (!queue.empty()) { //iterate through the candidates set
        int u = queue.top(); // candidate for CH
        queue.pop();
//        trace() << "u " << u << " d[u] " << distanceToCH[u];
        if (removedSet.find(u) != removedSet.end()) continue; // u already iterated thru
        removedSet.insert(u);

        for (int v : connectivityGraph.getAdjExceptSink(u)) {
            if ((removedSet.find(v) != removedSet.end())) continue;
            double alt = distanceToCH[u] + connectivityGraph.getLength(u,v);
            double T_new = T[clusId[u]] + dataPacketSizeAt[v]; // Weight at CH
            double T_level1 = 0;
            if (level1_node[u] != -1) T_level1 = T[level1_node[u]] + dataPacketSizeAt[v];

            if (alt - distanceToCH[v] < - epsilon && rxEnergy(T_new) + txEnergy(T_new,distanceToUAV) <= remainingEnergy_tmp[clusId[u]] * percent){
                T[clusId[u]] = T_new;
                if (clusId[v] != -1) T[clusId[v]] -= dataPacketSizeAt[v];
                distanceToCH[v] = alt;
                clusId[v] = clusId[u];
                nextHop[v] = u;
                queue.push(v);
            }
        }
    }
}


//void GAMBACFPercent::mainAlg() {
////    routing_trace() << "GAMBACFPercent::mainAlg";
//
//	for (int u=0; u<numNodes; u++) E_tmp[u] = getResMgrModule(u)->getRemainingEnergy();
//	config.A.clear();
//
//	for (int i=0; i<10; i++) application_trace() << "mainAlg3";
//	E_min = DBL_MAX;
//	E_max = 0;
//	for (int i=0; i<numNodes; i++) {
//		if (i == self) continue;
//		double E_i = getResMgrModule(i)->getRemainingEnergy();
//		if (E_i > E_max) E_max = E_i;
//		if (E_i < E_min) E_min = E_i;
//	}
//	W_total = 0;
//	for (int i=0; i<numNodes; i++) W_total += dataPacketSizeAt[i];
//	double f_end = 100;
//	double f_start = 0;
//	double f = (f_end+f_start)/2;
//
//	bool lengthViolated;
//	bool isSaved = false;
//	int count = 0;
//	double ratioToLmax = 1;
//	double maxLen = 0;
//	vector<int> A_old_saved;
//
//	do {
//		reset();
//
//		int loop_count = selectCHsAndBuildDFT(f);
//
//		lengthViolated = false;
//		maxLen = 0;
//
//		if (A == config.A) {
//			trajectories = config.trajectories;
//		}
//		else buildTrajectories(true);
//
//		for(int i=0;i<10;i++) metrics_trace()<< numUAVs << " " <<trajectories.size();
//
//		for (int t=0; t<numUAVs; t++) {
//			double len = calculatePathLength(trajectories[t]);
//			if (len > maxUAVFlyingDistance) {
//				lengthViolated = true;
//			};
//			if (len > maxLen) maxLen = len;
//		}
//
//		ratioToLmax = pow(maxLen / maxUAVFlyingDistance, 2);
//
//
//		if (lengthViolated) {
//			count += 1;
//
//			if (count >= 1) {
//				f_start = f;
//				count = 0;
//
//				f *= ratioToLmax;
//				if (f >= f_end) break;
//				continue;
//			}
//			else continue;
//		} else {
//			if (A.size() > config.A.size() && loop_count <= 50){
//				config.save(A, clus_id, nextHop, trajectories);
//				isSaved = true;
//				f_end = f;
//			}
//			else {
//				count += 1;
//				if (count >= 1) {
//					if (!isSaved) {
//						config.save(A, clus_id, nextHop, trajectories);
//						isSaved = true;
//					}
//					f_end = f;
//					count = 0;
//				}
//				else continue;
//			}
//			f_end = f;
//			count = 0;
//		}
//
//		f = (f_end + f_start) / 2;
//		if (f >= f_end) break;
//
//	} while (f_end-f_start>0.002);
//	countSuccess++;
//    routing_trace() << "END mainAlg1";
//
//	double maxLength = 0;
//	for (auto T : config.trajectories) {
//		double length = calculatePathLength(T);
//		if (length > maxLength) maxLength = length;
//		stringstream ss;
//		for(int l : T) {
//			ss << l << " ";
//		}
//		routing_trace() << ss.str();
//	}
//	//log this as a metric
//	routing_trace() << "maxLength " << maxLength;
//
//	maxLengthRatio = maxLength / maxUAVFlyingDistance;
//	routing_trace() << "END mainAlg2";
//
//	throw cRuntimeError("trappp");
//    if(!scheduled) scheduled = true;
//    if(!inittializedRPL) inittializedRPL = true;
//}
//
//
//
//int GAMBACFPercent::selectCHsAndBuildDFT(double f){
//
//	double percent = f/100;
//	double E1 = rxEnergy(1) + txEnergy(1,distanceToUAV);
//
//	for (int i=0; i<N; i++) {
//		if (i == sinkId) continue;
//		if (distance(i, sinkId) > maxUAVFlyingDistance/2) w_max[i] = 0;
//		else w_max[i] = E_tmp[i];
//	}
//
//	double totalWeight = 0;
//
//	vector<int> A_min;
//	double min_cnsmptn = DBL_MAX;
////	stringstream ss_csmpt;
//
//	int return_res = 0;
//
//	for (int retries=0; retries<50; retries++) {
//		A.clear();
//		return_res = retries;
//		unordered_set<int> uncovered_nodes;
//		for (int u=0; u<numNodes; u++) {
//			if (u != sinkId) uncovered_nodes.insert(u);
//		}
//
//		while (!uncovered_nodes.empty()) {
//			auto it = std::begin(uncovered_nodes);
//			auto r = rand() % uncovered_nodes.size();
//			std::advance(it,r);
//			int new_CH = *it;
//
//			// Grow Ball
//			A.push_back(new_CH);
//
//			growBallsPercent(A, percent, false);
//			for (int i=0; i<N; i++) {
//				if (i == sinkId) continue;
//				if (clus_id[i] != -1) uncovered_nodes.erase(i);
//			}
//		}
////	    application_trace() << "GAMBACFPercent::selectCHsAndBuildDFT:209 in loop " << retries;
//		double rxSizes[numNodes];
//		for (int u=0; u<numNodes; u++) { rxSizes[u]=0; }
//		for (int u=0; u<numNodes; u++) {
//			if (u == sinkId) continue;
//			int tmp = u;
//			while (nextHop[tmp] != -1) {
//				rxSizes[nextHop[tmp]] += dataPacketSizeAt[u];
//				tmp = nextHop[tmp];
//			}
//		}
////        application_trace() << "GAMBACFPercent::selectCHsAndBuildDFT:220 in loop " << retries;
//		double new_cnsmptn = 0;
//		for (int i=0; i<numNodes; i++) {
//			if (i == sinkId) continue;
//			double rxSize = rxSizes[i];//
//
//			new_cnsmptn += rxEnergy(rxSize);
//			double txSize = rxSize + dataPacketSizeAt[i];
//			if (clus_id[i] == i) {
//				new_cnsmptn += txEnergy(txSize, distanceToUAV);
//			} else {
//				double d2nextHop = G::distance(GlobalLocationService::getLocation(i), GlobalLocationService::getLocation(nextHop[i]));
//				new_cnsmptn += txEnergy(txSize, d2nextHop);
//			}
//		}
////        application_trace() << "GAMBACFPercent::selectCHsAndBuildDFT:235 in loop " << retries;
//		if (new_cnsmptn < min_cnsmptn) {
//			min_cnsmptn = new_cnsmptn;
//			A_min = A;
////			ss_csmpt << min_cnsmptn << " ";
//		}
////        application_trace() << "GAMBACFPercent::selectCHsAndBuildDFT:241 in loop " << retries;
//	}
//
//
////	application_trace() << "min_cnsmptn " << ss_csmpt.str();
//	A = A_min;
////	stringstream ss_A;
////	for (int l : A) ss_A << l << " ";
////	routing_trace() << "A " << ss_A.str();
////    application_trace() << "GAMBACFPercent::selectCHsAndBuildDFT:255_new";
////	growBallsPercent(A, percent, true);
//
//	clus_id[self] = -1;
//	for (int l : A) clus_id[l] = -1;
//
//	return return_res;
//}
//
//
//void iota(vector<long unsigned int>::iterator first, vector<long unsigned int>::iterator last, int value)
//{
//    while (first != last)
//    {
//        *first++ = value;
//        ++value;
//    }
//}
//
//template <typename T>
//vector<size_t> GAMBACFPercent::sort_indexes(const vector<T> &v) {
//  vector<size_t> idx(v.size());
//  iota(idx.begin(), idx.end(), 0);
//  stable_sort(idx.begin(), idx.end(),
//       [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});
//
//
//  return idx;
//}
//void printUOSet(std::unordered_set<int> const &s)
//{
//    std::copy(s.begin(),
//            s.end(),
//            std::ostream_iterator<int>(std::cout, " "));
//}
//
//void GAMBACFPercent::growBallsPercent(vector<int> CHSet, double percent, bool finals){
//
//    for (int u : graph.getNodesExceptSink()){
//		if (isCH[u]){
//			distanceToCH[u] = 0;
//		}
//		else {
//			distanceToCH[u] = DBL_MAX;
//		}
//		clus_id[u] = -1;
//		nextHop[u] = -1;
//		clusterMembers[u].clear();
//	}
//
//	for (int u : CHSet) {
//		distanceToCH[u] = 0;
//		clusterMembers[u].clear();
//		clus_id[u] = u;
//	}
//
//	dCompare =  &distanceToCH;
//	priority_queue<int,vector<int>, decltype(&compareByDistance)> queue(compareByDistance);
//
//	for (int l : CHSet) { queue.push(l); }
//
//	unordered_set<int> removedSet;
//
//	for (const auto& elem: removedSet) {
//	    application_trace()<<elem;
//	}
//
//	vector<double> T(N, 0);
//	vector<int> level1_node(N, -1);
//
//    int loop_c=0;
//
//	while (!queue.empty()) {
//	    loop_c++;
//
//		int u = queue.top();
//		queue.pop();
//
//		if(u>=distanceToCH.size())
//		    if(removedSet.find(u) != removedSet.end()) {
//		        continue;
//		    }
//
//		removedSet.insert(u);
//
//		for (int v : graph.getAdjExceptSink(u)) {
//
//			if ((removedSet.find(v) != removedSet.end())) {
//			    continue;
//			}
//
//			double alt = distanceToCH[u] + graph.getLength(u,v);
//			double T_new = T[clus_id[u]] + dataPacketSizeAt[v];
//			double T_level1 = 0;
//
//			if (level1_node[u] != -1) T_level1 = T[level1_node[u]] + dataPacketSizeAt[v];
//
//			if (alt - distanceToCH[v] < -EPSILON && rxEnergy(T_new) + txEnergy(T_new,distanceToUAV) <= E_tmp[clus_id[u]] * percent){
//
//				T[clus_id[u]] = T_new;
//				if (clus_id[v] != -1) T[clus_id[v]] -= dataPacketSizeAt[v];
//				distanceToCH[v] = alt;
//				clus_id[v] = clus_id[u];
//                nextHop[v] = u;
//				queue.push(v);
//
//			} else if(finals&&queue.empty())
//			    application_trace() << "GAMBACFPercent::growBallsPercent: Entered For Loo in While loop at 362, condition not met";
//		}
//	}
//
//    for (const auto& elem: removedSet) {
//        application_trace()<<elem;
//    }
//}
