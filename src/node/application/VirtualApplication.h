/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef VIRTUALAPPLICATIONMODULE
#define VIRTUALAPPLICATIONMODULE

#include <sstream>
#include <string>
#include <omnetpp.h>
#include "node/application/ApplicationPacket_m.h"
#include "node/sensorManager/SensorManagerMessage_m.h"
#include "node/resourceManager/ResourceManager.h"
#include "helpStructures/DebugInfoWriter.h"
#include "node/communication/radio/Radio.h"
#include "node/mobilityManager/VirtualMobilityManager.h"
#include "helpStructures/CastaliaModule.h"
#include "helpStructures/TimerService.h"

#define SELF_NETWORK_ADDRESS selfAddress.c_str()

using namespace std;

class VirtualApplication: public CastaliaModule, public TimerService {

public:
    static vector<int> nodesDeathTimestamp;
    static vector<int> isAlive;
    string applicationID;

 protected:
	/*--- The .ned file's parameters ---*/
	int priority;
	int packetHeaderOverhead;
	int constantDataPayload;
	bool isSink;
	double latencyMax;
	int latencyBuckets;

	/*--- Custom class parameters ---*/
	int self;								// the node's ID
	string selfAddress;						// the node's routing level address
	ResourceManager *resMgrModule;			// a pointer to the Resource Manager module
	VirtualMobilityManager *mobilityModule;	// a pointer to the mobility Manager module
	Radio *radioModule;						// a pointer to the Radio module
	bool disabled;
	double cpuClockDrift;
    int inboundPacketCount;
    int outboundPacketCount;

	virtual void initialize();
	virtual void startup() {}
	virtual void handleMessage(cMessage * msg);
	virtual void finish();
	virtual void finishSpecific() {}

	void requestSensorReading(int index = 0);
	void toNetworkLayer(cMessage *);
	void toNetworkLayer(cPacket *, const char *);

	ApplicationPacket *createGenericDataPacket(double, unsigned int, int = -1);
	virtual void fromNetworkLayer(ApplicationPacket *, const char *, double, double) = 0;
	virtual void handleSensorReading(SensorReadingMessage *) {}
	virtual void handleNetworkControlMessage(cMessage *) {}
	virtual void handleMacControlMessage(cMessage *) {}
	virtual void handleRadioControlMessage(RadioControlMessage *) {}
    virtual void fromNetworkLayer(ApplicationPacket *) {};
 public:
    virtual void printOutRoundResults(int roundNumber){
        metricsTrace() << "Stuck at virtual APP";
    };
};

#endif
