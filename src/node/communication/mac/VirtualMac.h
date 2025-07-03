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

#ifndef VIRTUALMACMODULE
#define VIRTUALMACMODULE

#define SELF_MAC_ADDRESS self

#include <queue>
#include <vector>
#include <omnetpp.h>

#include "node/communication/routing/RoutingPacket_m.h"
#include "MacPacket_m.h"
#include "node/communication/radio/RadioControlMessage_m.h"
#include "wirelessChannel/defaultChannel/WirelessChannel.h"


#include "node/resourceManager/ResourceManager.h"
#include "node/communication/radio/Radio.h"
#include "helpStructures/TimerService.h"
#include "helpStructures/CastaliaModule.h"
#include "helpStructures/GlobalLocationService.h"
#include "helpStructures/GeoMathHelper.h"

using namespace std;

class VirtualMac: public CastaliaModule, public TimerService {
 protected:
    int self;       // the node's ID
    bool disabled;
    int macBufferSize;
    int macMaxFrameSize;
    int macFrameOverhead;
    unsigned int currentSequenceNumber;

    int inboundPacketsCount;
    int outboundPacketsCount;

    //location variables
    string selfAddress;
    Point selfLocation;
    vector<int> neighborIndex;

    //a pointer to the object of the Radio Module (used for direct method calls)
    Radio *radioModule;

    //a pointer to the object of the Resource Manager Module (used for direct method calls)
    ResourceManager *resMgrModule;

    queue< cPacket* > TXBuffer;
    vector< unsigned int > pktHistory;

    void createAndSendRadioControlCommand(RadioControlCommand_type, double, const char *, BasicState_type);
    void initialize(int);
    int numInitStages() const {
        return 4; //after the routing
    }

    void toNetworkLayer(cMessage *);
    void toRadioLayer(cMessage *);
    void handleMessage(cMessage * msg);
    void finish();

    virtual void specificInitialize();
    virtual void startup() {}
    virtual int handleControlCommand(cMessage * msg);
    virtual int handleRadioControlMessage(cMessage * msg);
    //bypass lower networking layers and directly send to another node's Mac object
    virtual void toMacLayerDirect(cPacket* ,int);

    virtual void fromNetworkLayer(cPacket *, int) = 0;
    virtual void fromRadioLayer(cPacket *, double, double) = 0;
    virtual void fromMacLayerDirect(cPacket *){};
    virtual void finishSpecific() {}

    int bufferPacket(cPacket *);
    bool isNotDuplicatePacket(cPacket *);

    double txEnergy(double pktByteLength, double distance) {
        return 1 * (50e-9 * pktByteLength * 8 + 10e-12 * pktByteLength * 8 * distance * distance);
    }
    void encapsulatePacket(cPacket *, cPacket *);
    cPacket *decapsulatePacket(cPacket *);

    //for routing to call
 public:

    int overheardPacketCount_round;

    void logAndResetPktBreakdownForRound(int currentRoundNumber){
        singleRoundMetricLog("RxFailedInterference",radioModule->currentRoundStats.RxFailedInterference,currentRoundNumber);
        singleRoundMetricLog("transmissionsCount",radioModule->currentRoundStats.transmissions,currentRoundNumber);
        singleRoundMetricLog("RxReachedNoInterference",radioModule->currentRoundStats.RxReachedNoInterference,currentRoundNumber);
//        singleRoundMetricLog("CollisionToTransmissionRatio",radioModule->currentRoundStats.RxReachedNoInterference/radioModule->currentRoundStats.transmissions,currentRoundNumber);
        radioModule->resetRoundStats();
    };
    void singleRoundMetricLog(string name, int value, int roundNumber){
        metricsTrace() << "#round_metrics#"
        << roundNumber << "#" << self //identifiers: time and place
        << "#" << name << "#" << value; //topology for this round
    }
    void singleRoundMetricLog(string name, double value, int roundNumber){
        metricsTrace() << "#round_metrics#"
        << roundNumber << "#" << self //identifiers: time and place
        << "#" << name << "#" << value; //topology for this round
    }
};

#endif
