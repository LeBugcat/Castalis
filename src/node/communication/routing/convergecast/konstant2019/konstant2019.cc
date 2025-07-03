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

#include "konstant2019.h"
#include <chrono>
#include <stdio.h>

Define_Module(konstant2019);

void konstant2019::startup()
{	
	/*--- The .ned file's parameters ---*/
	applicationID = par("applicationID").stringValue(); 	
	numUAVs = getParentModule()->getParentModule()->getParentModule()->par("numUAVs");
	dataPacketSize = par("dataPacketSize");
	if (isSink) konstantInit();
		
	setTimer(START_ROUND, 1);
}


void konstant2019::mainAlg() {
	for (int i=0; i<1; i++) routingTrace() << "mainAlg";
	reset();
	for (int u=0; u<numNodes; u++) E_tmp[u] = getResMgrModule(u)->getRemainingEnergy();
	findEnergyEfficientSolution();

//	throw cRuntimeError("debugtrap: check main alg variables");

    if(!routingTreeReady)
        routingTreeReady=true;

}


void konstant2019::konstantInit() {
	N = numNodes;

//    WirelessChannel* tempWc = check_and_cast<WirelessChannel*>(getParentModule()->getParentModule()->getParentModule()->getSubmodule("wirelessChannel"));

    neighborTable = GlobalLocationService::getNeighborTable(self);
    selfLocation = GlobalLocationService::getLocation(self);

    //for grabbing WC transmission range!
//    neighborRange = tempWc->transmissionRange;

	graph.init(numNodes, self, neighborRange);
	trajectories.resize(numUAVs);

	distanceToCH.resize(N, 0.);
	clusId.resize(N, -1);
	nextHop.resize(N, -1);
	isCH.resize(N, false);
	clusterMembers.resize(N);
	E_tmp = vector<double>(numNodes, 0);
	E0.resize(N);
	for (int u=0; u<N; u++) {
		E0[u] = getResMgrModule(u)->getInitialEnergy() + 0.04;
	}
}

void konstant2019::reset() {
	trajectories = vector<vector<int>>(numUAVs);

	A.clear();
	distanceToCH = vector<double>(N, 0.);
	clusId = vector<int>(N, -1);
	nextHop = vector<int>(N, -1);
	isCH = vector<bool>(N, false);
	clusterMembers = vector<list<int>>(N);
}

void konstant2019::clusterTreeBuilding(vector<int> CHSet){
	for (int u : graph.getNodesExceptSink()){
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
	vector<double> vals(numNodes, 0);
	vector<double> d(numNodes, DBL_MAX);
	for (int u : CHSet) {
		d[u] = 0;
		vals[u] = -DBL_MAX;
		clusterMembers[u].clear();
		clusId[u] = u;
		nextHop[u] = -1;
	}

	unordered_set<int> removedSet;

	dCompare =  &vals;
	priority_queue<int,vector<int>, decltype(&compareByDistance)> queue(compareByDistance);
	for (int l : CHSet) {
		queue.push(l);
	}
	
	removedSet.clear();
	int countLoop = 0;
	while (!queue.empty()) {
		int u = queue.top();
		if (countLoop++ > 1600000) trace() << u;
		queue.pop();
		if (removedSet.find(u) != removedSet.end()) continue;
		removedSet.insert(u);
		if (removedSet.size() == numNodes-1) break;
		if (nextHop[u] >= 0) d[u] = d[nextHop[u]] + graph.getLength(u, nextHop[u]);
		for (int v : graph.getAdjExceptSink(u)) {
			if ((removedSet.find(v) != removedSet.end())) continue;
			double alt = - E_tmp[u] / (d[u] + graph.getLength(u,v)) / (d[u] + graph.getLength(u,v));
			
			if (alt - vals[v] < -EPSILON){
				vals[v] = alt;
				clusId[v] = clusId[u];
				nextHop[v] = u;
				queue.push(v);
			}
		}
	}
}


vector<vector<int>> konstant2019::partitionIntoSectors() {

    vector<vector<int>> Sectors;
	for (int k=0; k<numUAVs; k++) Sectors.push_back(vector<int>());
	int i0 = rand() % numNodes;
	if (i0 == sinkId) i0 = (sinkId+1) % numNodes;
	Point sinkLocation = location(sinkId);

	auto P = GlobalLocationService::getLocation(i0);

	vector<double> PE (numNodes);
	double P_total = 0;
	for (int i=0; i<N; i++) {
		if (i == sinkId) continue;
		PE[i] = (E0[i] - getResMgrModule(i)->getRemainingEnergy()) / E0[i];
		P_total += PE[i];
	}
	vector<int> sortedList;
    for (int i = 0; i < numNodes; i++) {
		if (i == sinkId) continue;
        Angle angle_i = G::angle(sinkLocation, location(i0), sinkLocation, location(i));

		unsigned int j = 0;
        for (j = 0; j < sortedList.size(); j++) {
			Angle angle_j1 = G::angle(sinkLocation, location(i0), sinkLocation, location(sortedList[j]));
            if (angle_j1 > angle_i) {
                sortedList.insert(sortedList.begin() + j, i);
                break;
            }
        }
        if (sortedList.empty() || (j == sortedList.size())) {
            sortedList.push_back(i);
        }
    }
	int sectorId = 0;
	double sectorPerclus_id = P_total/numUAVs;
	for (int u : sortedList) {
		Sectors[sectorId].push_back(u);
		sectorPerclus_id -= PE[u];
		if (sectorPerclus_id < 0) {
			sectorId++;
			sectorPerclus_id = P_total/numUAVs;
		}
	}

	vector<vector<int>> RaySectors (numUAVs);
	for (int k=0; k<numUAVs; k++) {
		vector<int> sector = Sectors[k];
		int u0 = sector[0];
		Angle maxAngle = G::angle(sinkLocation, location(u0), sinkLocation, location(sector[sector.size()-1]));
		Angle ray2Angle = maxAngle/4;
		Angle ray4Angle = maxAngle*3/4;
		for (int u : sector) {
			Angle uAngle = G::angle(sinkLocation, location(u0), sinkLocation, location(u));
			if ((uAngle >= ray2Angle) && (uAngle <= ray4Angle)) {
				unsigned int j = 0;
				for (j = 0; j < RaySectors[k].size(); j++) {
					if (G::distance(location(RaySectors[k][j]), sinkLocation) > G::distance(location(u), sinkLocation)) {
						RaySectors[k].insert(RaySectors[k].begin() + j, u);
						break;
					}
				}

				if (RaySectors[k].empty() || (j == RaySectors[k].size())) {
					RaySectors[k].push_back(u);
				}
			}
		}
	}
	return RaySectors;
}

vector<vector<int>> konstant2019::basicToursPlanning() {
	vector<vector<int>> Sectors = partitionIntoSectors();
	
	vector<vector<int>> basicTours;
	for (int k=0; k<numUAVs; k++) {
		vector<Point> newTourPoint;
		vector<Point> innerPoint;
		innerPoint.push_back(location(sinkId));
		if (Sectors[k].size() >= 1) {
			innerPoint.push_back(location(Sectors[k][0]));
			newTourPoint = innerPoint;
			double per;
			if (Sectors[k].size() >= 2) {
				int i = 1;
				int n_min = 0;
				int n_max = Sectors[k].size()-1;
				int n_mid;
				do {
					n_mid = (n_min+n_max)/2;
					innerPoint.clear();
					innerPoint.push_back(location(sinkId));
					for (int j=0; j<=n_mid; j++) {
						innerPoint.push_back(location(Sectors[k][j]));
					}
					vector<Point> convexHull = G::convexHull(innerPoint);
					per = G::polygonPerimeter(convexHull);
					if (per <= maxUAVFlyingDistance) {
						newTourPoint = convexHull;
						n_min = n_mid+1;
					} else {
						n_max = n_mid-1;
					}
					i++;
				} while (n_max >= n_min);
			}
		}
		vector<int> newTour;
		for (Point p : newTourPoint) newTour.push_back(GlobalLocationService::getId(p));
		basicTours.push_back(newTour);
	}
//	for (int k=0; k<numUAVs; k++) {
////		trace() << "tour " << k << " size " << basicTours[k].size();
////		trace() << " length " << GlobalLocationService::convexHullPerimeter(basicTours[k]);
//		stringstream ss;
//		if (!basicTours[k].empty()) for(int l : basicTours[k]) {
//			ss << l << " ";
//		}
//		trace() << ss.str();
//	}
	return basicTours;
}

void konstant2019::findEnergyEfficientSolution() {
	vector<vector<int>> basicTours = basicToursPlanning();

	vector<vector<int>> finalTours = basicTours;

	string str0 = "";

	vector<int> pointsNotInTour;
	for (int i=0; i<numNodes; i++) pointsNotInTour.push_back(i);
	for (int k=0; k<numUAVs; k++) {
		for (int u : basicTours[k]) {
			if (find(pointsNotInTour.begin(), pointsNotInTour.end(), u) != pointsNotInTour.end())
				pointsNotInTour.erase(find(pointsNotInTour.begin(), pointsNotInTour.end(), u));
		}
	}

//	trace() << "findEnergyEfficientSolution 0";
	int retries = 1;
	int maxIter = 2000;
	stringstream ss_cnsmptn;
	stringstream ss_issaved;
	double cur_cnsmptn = DBL_MAX;//calculateConsumption(finalTours);
	string ss_rxsize_str;
	string ss_next_str;
	for (int i=0; i<retries; i++) {
		for (int j=0; j<maxIter; j++) {
//			trace() << "iter " << j << " retry " << i;
			// if (j%10 == 0) routing_trace() << "iter " << j;
			int rdm;
//			trace() << "findEnergyEfficientSolution 1";
			auto start = std::chrono::high_resolution_clock::now();
			do {
				int k = rand() % basicTours.size();
				if (basicTours[k].size() > 2) {
					int h = rand() % basicTours[k].size();
					if (basicTours[k][h] != sinkId) {
						pointsNotInTour.push_back(basicTours[k][h]);
						basicTours[k].erase(basicTours[k].begin() + h);
					}
				}
				rdm = rand() % 100;
//				trace() << "rdm " << rdm;
			} while (rdm < 75);
			auto finish = std::chrono::high_resolution_clock::now();
			auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
//			trace() << "TIME STAGE_1 " << microseconds.count() << " us";

//			trace() << "findEnergyEfficientSolution 2";
			bool feasible;
			
			start = std::chrono::high_resolution_clock::now();
			do {
				int index = rand() % pointsNotInTour.size();
				int u = pointsNotInTour[index];
				feasible = false;
				random_shuffle ( basicTours.begin(), basicTours.end() );
				for (int k=0; k<numUAVs; k++)  {
					double minPer = DBL_MAX;
					vector<int> minTour;
					if (basicTours[k].size() > 8) continue;

					for (int h=0; h<basicTours[k].size(); h++) {
						vector<int> newTour;
						vector<Point> newTourPoint;
						for (int o=0; o<=h; o++) {
							newTour.push_back(basicTours[k][o]);
							newTourPoint.push_back(location(basicTours[k][o]));
						}
						newTour.push_back(u);
						newTourPoint.push_back(location(u));
						for (int o=h+1; o<basicTours[k].size(); o++) {
							newTour.push_back(basicTours[k][o]);
							newTourPoint.push_back(location(basicTours[k][o]));
						}

						double per = G::polygonPerimeter(newTourPoint);
						if (per < minPer) {
							minPer = per;
							minTour = newTour;
						}
					}
					if (minPer <= maxUAVFlyingDistance) {
						basicTours[k] = minTour;
						pointsNotInTour.erase(find(pointsNotInTour.begin(), pointsNotInTour.end(), u));
						feasible = true;
						break;
					}
					if (feasible) break;
				}
			} while (feasible);
			finish = std::chrono::high_resolution_clock::now();
			microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);

			start = std::chrono::high_resolution_clock::now();
			auto p_result = calculateConsumption(basicTours);
			double new_cnsmptn = p_result.first;
			ss_cnsmptn << new_cnsmptn << " ";
			double delta = new_cnsmptn - cur_cnsmptn;
			// if (delta < 0 || exp(delta*j/maxIter) > ((double) rand() / RAND_MAX)) {
			if (cur_cnsmptn/j > new_cnsmptn/maxIter) {
				ss_issaved << 1 << " ";
				ss_rxsize_str = p_result.second;
				stringstream ss_next;
				for (int i=0; i<numNodes; i++) ss_next << nextHop[i] << " ";
				ss_next_str = ss_next.str();
				str0 = ss0.str();
				finalTours = basicTours;
				cur_cnsmptn = new_cnsmptn;
				config.save(A, clusId, nextHop, trajectories);

				//!!!trajectories log
//				stringstream ss;
//				for (auto trajectory: trajectories) {
//					for (int u: trajectory) {
//						ss << u << " ";
//					}
//				}
			}
			else ss_issaved << 0 << " ";
			finish = std::chrono::high_resolution_clock::now();
			microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
		}
	}
	trace() << "findEnergyEfficientSolution done";
}

pair<double, string> konstant2019::calculateConsumption(vector<vector<int>> tours) {

    A.clear();
	for (int i=0; i<numNodes; i++)  isCH[i] = false;
	vector<int> startIndex;
	for (int k=0; k<numUAVs; k++) {
		for (int i=0; i<tours[k].size(); i++) {
			int l = tours[k][i];
			if (l == sinkId) {
				startIndex.push_back(i);
			}
			else if (!isCH[l]) {
				isCH[l] = true;
				A.push_back(l);
			}
		}
	}
	
	
	auto start = std::chrono::high_resolution_clock::now();
	clusterTreeBuilding(A);

	auto finish = std::chrono::high_resolution_clock::now();
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);

	for (int l : A) clusId[l] = -1;
	for (int i=0; i<numNodes; i++) {
		if (i == sinkId) continue;
	}

	for (int k=0; k<numUAVs; k++) trajectories[k].clear();
	for (int k=0; k<numUAVs; k++) {
		for (int i=0; i<tours[k].size(); i++) {
			int trueIndex = (i+startIndex[k])%tours[k].size();
			trajectories[k].push_back(tours[k][trueIndex]);
		}
		trajectories[k].push_back(sinkId);
	}
	NetworkConfig oldConfig;
	oldConfig.save(config);
	config.save(A, clusId, nextHop, trajectories);

	start = std::chrono::high_resolution_clock::now();
	double rxSizes[numNodes];
	for (int u=0; u<numNodes; u++) { rxSizes[u]=0; }

	for (int u=0; u<numNodes; u++) {
		int tmp = u;
		while (config.nextHop[tmp] != -1) {
			rxSizes[config.nextHop[tmp]] += dataPacketSizeAt[u];
			tmp = config.nextHop[tmp];
		}
	}

	double Er_min = DBL_MAX;
	double Er_total = 0;
	double new_cnsmptn = 0;
	for (int i=0; i<numNodes; i++) {
		double Er_i = E_tmp[i];
		if (i == sinkId) continue;
		double rxSize = rxSizes[i];//calculateRxSize(i);
		new_cnsmptn += rxEnergy(rxSize);

		Er_i -= rxEnergy(rxSize);

		double txSize = rxSize + dataPacketSizeAt[i];
		if (config.clus_id[i] == -1) {
			new_cnsmptn += txEnergy(txSize, distanceToUAV);
			Er_i -= txEnergy(txSize, distanceToUAV);

		} else {
			double d2nextHop = G::distance(GlobalLocationService::getLocation(i), GlobalLocationService::getLocation(config.nextHop[i]));
			new_cnsmptn += txEnergy(txSize, d2nextHop);
			Er_i -= txEnergy(txSize, d2nextHop);

		}
		Er_total += Er_i;
		if (Er_i < Er_min) Er_min = Er_i;
	}

	finish = std::chrono::high_resolution_clock::now();
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);

	config.save(oldConfig);
	double alpha = 0.9;
	string r_str = "";//ss2.str();
	return pair<double,string>(new_cnsmptn, r_str);
}

