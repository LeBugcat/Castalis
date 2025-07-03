/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Yuriy Tselishchev, Athanassios Boulis                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#ifndef _VIRTUALROUTING_H_
#define _VIRTUALROUTING_H_

#include <queue>
#include <vector>
#include <unordered_set>
#include <omnetpp.h>

#include "helpStructures/CastaliaModule.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "helpStructures/TimerService.h"
#include "CastaliaMessages.h"
#include "node/communication/radio/Radio.h"
#include "helpStructures/Util.h"
#include <sstream>
#include "wirelessChannel/defaultChannel/WirelessChannel.h"
#include "node/resourceManager/ResourceManager.h"
#include "node/communication/routing/RoutingPacket_m.h"
#include "node/application/ApplicationPacket_m.h"
#include "helpStructures/GlobalLocationService.h"
#include "helpStructures/GeoMathHelper.h"
#include <chrono>
#include <ctime>
using namespace std::chrono;

#define SELF_NETWORK_ADDRESS selfAddress.c_str()
#define ROUTE_DEST_DELIMITER "#"
#define PACKET_HISTORY_SIZE 5
#define RADIO_RANGE 100
#define distanceToUAV 20

using namespace std;

class VirtualRouting: public CastaliaModule, public TimerService {
 public:

    /*--- The .ned file's parameters ---*/
    double sensibility;
    double aggrConsumption;

    int maxNetFrameSize;        //in bytes
    int netDataFrameOverhead;   //in bytes
    int netBufferSize;          //in # of messages
    unsigned int currentSequenceNumber;

    /*--- Custom class parameters ---*/
    ResourceManager *resMgrModule;
    queue<int> controlPacketBuffer;

    queue< cPacket* > TXBuffer;
    vector< list< unsigned int> > pktHistory;

    double cpuClockDrift;
    bool disabled;

    Radio *radioModule;
    string selfAddress;
    int self;

    virtual void initialize(int);
    int numInitStages() const {
        return 3;
    };

    virtual void startup() { }
    virtual void handleMessage(cMessage * msg);
    virtual void finish();

    virtual void fromApplicationLayer(cPacket *, const char *) = 0;
    virtual void fromMacLayer(cPacket *, int, double, double) = 0;

    int bufferPacket(cPacket *);

    void toApplicationLayer(cMessage *);
    void toMacLayer(cMessage *);
    void toMacLayer(cPacket *, int);
    bool isNotDuplicatePacket(cPacket *);

    void encapsulatePacket(cPacket *, cPacket *);
    cPacket *decapsulatePacket(cPacket *);
    int resolveNetworkAddress(const char *);
    virtual void handleMacControlMessage(cMessage *);
    virtual void handleRadioControlMessage(cMessage *);

};

bool compareByDistance(int a, int b);

#endif              //_VIRTUALROUTING_H_
