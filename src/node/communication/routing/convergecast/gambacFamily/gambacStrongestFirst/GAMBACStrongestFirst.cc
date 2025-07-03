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

#include "./GAMBACStrongestFirst.h"

Define_Module(GAMBACStrongestFirst);


void GAMBACStrongestFirst::timerFiredCallback(int index)
{
	switch (index) {

	    case START_MAINALG:{

	        uncollectedSet.clear();
			collectedSet.clear();
			for (int i=0; i<N; i++) if (i != sinkId) uncollectedSet.insert(i);
			totalConsumed = 0;
			maxConsumed = 0;
			energyConsumeds = vector<double>(N, 0);
			rxSizes = vector<double>(N, 0);
			auto start = std::chrono::steady_clock::now();
			totalCollected = 0;

			dataPacketSizeAt[self]= smplAggAppModule->currentDataPktSize;

			mainAlg();
//			logConfig();
			auto end = std::chrono::steady_clock::now();
			timeElapse_round = end - start;
			break;

	    }

	    default:{

	        GAMBAC::timerFiredCallback(index);

	    }
	}
}

void GAMBACStrongestFirst::mainAlg() {

    dataPacketSizeAt[self]= smplAggAppModule->currentDataPktSize;

	for (int u=0; u<numNodes; u++) remainingEnergy_tmp[u] = getResMgrModule(u)->getRemainingEnergy();
	config.A.clear();

	for (int i=0; i<10; i++) routingTrace() << "mainAlg3";
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

//		int loop_count = selectCHsAndBuildDFT(f);
		selectCHsAndBuildDFT(f);
		lengthViolated = false;
		maxLen = 0;

		if (A == config.A) {
			trajectories = config.trajectories;
		}
		else buildTrajectories();

		for (int t=0; t<numUAVs; t++) {
			double len = calculatePathLength(trajectories[t]);
			if (len >maxUAVFlyingDistance) {
				lengthViolated = true;
			};
			if (len > maxLen) maxLen = len;
		}

		ratioToLmax = pow(maxLen /maxUAVFlyingDistance, 2);


		if (lengthViolated) {
			count += 1;

			if (count >= 1) {
				f_start = f;
				count = 0;

				f *= ratioToLmax;
				if (f >= f_end) break;
				continue;
			}
			else continue;
		} else {

		    //early stopping at 50 max CHs
			if (A.size() > config.A.size()){

				config.save(A, clusId, nextHop, trajectories);
				isSaved = true;
				f_end = f;
			}
			else {
				count += 1;
				if (count >= 1) {
					if (!isSaved) {

						config.save(A, clusId, nextHop, trajectories);
						isSaved = true;
					}
					f_end = f;
					count = 0;
				}
				else continue;
			}
			f_end = f;
//			routing_trace() << "increase num. clusters, max length = " << maxLen << ", ratioToLmax = " << ratioToLmax;
			count = 0;
		}

		f = (f_end + f_start) / 2;
		if (f >= f_end) break;

	} while (f_end-f_start>0.002);
	countSuccess++;

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
//	routing_trace() << "maxLength " << maxLength;
//	CastaliaModule::trace2() << roundNumber << "\tMAXLENGTH\t" << maxLength;
	maxLengthRatio = maxLength /maxUAVFlyingDistance;
	routingTrace() << "END mainAlg";

}


void GAMBACStrongestFirst::selectCHsAndBuildDFT(double f){

	double percent = f/100;
	double E1 = rxEnergy(1) + txEnergy(1,distanceToUAV);
	for (int i=0; i<N; i++) {
		if (i == sinkId) continue;
		if (distance(i, sinkId) >maxUAVFlyingDistance/2) w_max[i] = 0;
		else w_max[i] = remainingEnergy_tmp[i];
	}

	auto sorted_idx = sort_indexes(w_max);
	vector<bool> isSelected(N, false);
	isSelected[sinkId] = true;
	double totalWeight = 0;
    int count = 0;

	for (int new_CH : sorted_idx) {
		if (isSelected[new_CH]) continue;

		// Grow Ball
		A.push_back(new_CH);
		growBallsPercent(A, percent);
		for (int i=0; i<N; i++) {
			if (i == sinkId) continue;
			if (clusId[i] != -1) isSelected[i] = true;
		}
	}
	clusId[self] = -1;
	for (int l : A) clusId[l] = -1;
}

void iota2(vector<long unsigned int>::iterator first, vector<long unsigned int>::iterator last, int value)
{
    while (first != last)
    {
        *first++ = value;
        ++value;
    }
}

template <typename T>
vector<size_t> GAMBACStrongestFirst::sort_indexes(const vector<T> &v) {
  vector<size_t> idx(v.size());
  iota2(idx.begin(), idx.end(), 0);
  stable_sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});

  return idx;
}

void GAMBACStrongestFirst::growBallsPercent(vector<int> CHSet, double perclus_id){
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

	while (!queue.empty()) {
		int u = queue.top();
		queue.pop();
//		trace() << "u " << u << " d[u] " << distanceToCH[u];
		if (removedSet.find(u) != removedSet.end()) continue;
		removedSet.insert(u);

		for (int v : connectivityGraph.getAdjExceptSink(u)) {
			if ((removedSet.find(v) != removedSet.end())) continue;
			double alt = distanceToCH[u] + connectivityGraph.getLength(u,v);
			double T_new = T[clusId[u]] + dataPacketSizeAt[v];
			double T_level1 = 0;
			if (level1_node[u] != -1) T_level1 = T[level1_node[u]] + dataPacketSizeAt[v];

			if (alt - distanceToCH[v] < -EPSILON && rxEnergy(T_new) + txEnergy(T_new,distanceToUAV) <= remainingEnergy_tmp[clusId[u]] * perclus_id){
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
