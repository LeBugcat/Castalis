/*******************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                            *
 *  Developed at the ATP lab, Networked Systems research theme                 *
 *  Author(s): Athanassios Boulis, Dimosthenis Pediaditakis, Yuriy Tselishchev *
 *  This file is distributed under the terms in the attached LICENSE file.     *
 *  If you do not find this file, copies can be found by writing to:           *
 *                                                                             *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia                *
 *      Attention:  License Inquiry.                                           *
 *                                                                             *  
 *******************************************************************************/

#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <map>
#include "helpStructures/CastaliaModule.h"
#include "node/resourceManager/ResourceManagerMessage_m.h"

using namespace std;

enum ResoruceManagerTimers {
	PERIODIC_ENERGY_CALCULATION = 1,
};

class ResourceManager: public CastaliaModule {

public:
    double remainingEnergy;
    double baselineNodePower;
    double currentNodePower;

private:
	// parameters and variables
	/*--- The .ned file's parameters ---*/
	double sigmaCPUClockDrift;
	double cpuClockDrift;
	double initialEnergy;
	double ramSize;
//	double baselineNodePower;
//	double currentNodePower;
	simtime_t timeOfLastCalculation;
	double periodicEnergyCalculationInterval;
	double deathTimestampMs;

	/*--- Custom class para
	 * meters ---*/
	double totalRamData;

	map<int,double> storedPowerConsumptions;

	cMessage *energyMsg;
	bool disabled;

 protected:
	virtual void initialize();
	virtual void handleMessage(cMessage * msg);
	virtual void finishSpecific();
	void calculateEnergySpent();

 public:
	double getCPUClockDrift(void);
	void consumeEnergy(double amount);
	double getRemainingEnergy();
	double getSpentEnergy(void);
	double estimateLifetime();
	void destroyNode(void);
	int RamStore(int numBytes);
	void RamFree(int numBytes);
	double getInitialEnergy();
	void resetBattery();
	double getDeath();
};

#endif				// _RESOURCEGENERICMANAGER_H_
