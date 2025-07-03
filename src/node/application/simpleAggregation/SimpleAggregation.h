/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Dimosthenis Pediaditakis                 *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *  
 ****************************************************************************/

#ifndef _SIMPLEAGGREGATION_H_
#define _SIMPLEAGGREGATION_H_

#include "node/application/VirtualApplication.h"

using namespace std;

enum SimpleAggregationTimers {
	REQUEST_SAMPLE = 1,
	SEND_AGGREGATED_VALUE = 2,
	RESERVED_TIMER = 3
};

class SimpleAggregation: public VirtualApplication {
protected:
    int aggregatedPktSize; //in bytes, this holds the data size, not all the data

	double aggregatedValue;
	double waitingTimeForLowerLevelData;
	double lastSensedValue;
	double sampleInterval;
	double sendInterval;
	int totalPackets;
	bool loggedInit = false;
	bool dataSamplingDisabled; //when this is true, sampling is never done to save CPU

// protected:
	void initialize();
	void startup();
	void timerFiredCallback(int);
	void handleSensorReading(SensorReadingMessage *);
	void handleNeworkControlMessage(cMessage *);

	void sendData();

	void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
	void fromNetworkLayer(ApplicationPacket *);
	void finishSpecific();
 public:

    double datarate; //datarate in kbps
    int currentDataPktSize;

	void printOutRoundResults(int);
};

#endif				// _SIMPLEAGGREGATION_APPLICATIONMODULE_H_
