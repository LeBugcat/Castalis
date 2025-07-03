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

//TODO:urgent: formalize the logging into logging levels:
// INFO -> DEBUG -> WARN -> ERROR (err is logical, not cRuntimeErr)
// bake this into the CastaliaModule, utilizing the , create a set of simple logging functions
// string format is setup at CastaliaModule, using its built in logging function
// <LEVEL>::<log string>   <-- simple like that he-he
// string format at MAC level:
// mac_trace(enum level) << PHASE: BEACON/CAP/CFP/... << SLOT::<currentCFP> << log string
// and remember, least friction==most likely to last
// (that also means the Routing layer is prob intact! all classes!)

#ifndef NODE_COMMUNICATION_MAC_MAC802154TSCHLITE_MAC802154TSCHLITE_H_
#define NODE_COMMUNICATION_MAC_MAC802154TSCHLITE_MAC802154TSCHLITE_H_

#include <omnetpp.h>

#include "../mac802154TSCHlite/MAC802154LitePacket_m.h"
#include "node/communication/mac/VirtualMac.h"
#include "node/communication/routing/convergecast/ConvergecastRouting.h"

//this SM is only known by the Node it is the only source of truth
//Nodes sleep on default
enum Action_type {
    SLEEP_TSCH = 0,
    ATTEMPT_TX = 1,
    OPEN_RX = 2,
};

enum RadioState {
    SLEEPING = 0,
    TXING = 1,
    RXING = 2,
    BEACONING = 3
};

enum MacState {
    CAP = 0,
    CFP = 1
};

enum TSCH_Timer {
    //round timers
    START_TSCH_ROUND = 101,
    START_CAP        = 102, //after beacon processing
    CAP_PROCESS      = 110, //after beacon processing
    START_CFP        = 103, //after CAP

    //node action timers
    SCHEDULED_SLEEP      = 104,
    SCHEDULED_ATTEMPT_TX = 105, //for one slot duration
    SCHEDULED_OPEN_RX    = 106, //for one slot duration

    START_CAP_SLOT   = 111,
    PROCESS_CAP_SLOT = 112,

    START_CFP_SLOT   = 107,
    POST_STATE_TRANS = 108,
    END_CFP_SLOT     = 109
};

struct ActionRecord{
    Action_type type;
    int         sourceId;
    int         channelId;
};

struct CFPSchedule {

    vector<Action_type> slotActions; //per-slot
    vector<int> operatingChannel; //per-slot
    int localCAPsize;
    int localCFPsize;
    int frameSize;

    //Nodes sleep on default
    CFPSchedule(int framesize, int CAPsize){
        if(framesize<=CAPsize)
            throw cRuntimeError("Negative CFP slots count");
        frameSize = framesize;
        localCAPsize = CAPsize;
        localCFPsize = framesize - CAPsize;

        operatingChannel.resize(localCFPsize, 0); //all start out on channel 0
        slotActions.resize(localCFPsize, SLEEP_TSCH); //sleeps on default
    }

    Action_type getActionOnSlot(int slotId){
        return slotActions[slotId];
    }

    bool isSleep(int slotId){
        return (slotActions[slotId]==SLEEP_TSCH);
    }
};

class MAC802154TSCHLite: public VirtualMac {

  public:

    /* Variables parsed from NED start */

    // GLOBAL ACCROSS MAC OBJECT VARS
    // Timeframe //
    static int    frameSize;
    static int    CAPSize;
    static int    CFPSize; // inferred, not passed
    static double slotSizeInMiliSec; // this is for timers
    static double slotSizeInSec; // this is for timers
    static double stateTransTimeGuardMs; //for transition delays
    static int    roundNumber;
    static double TSCHRoundPadding;
    static double CAPpadding;
    static double CFPpadding;

    static double coordinatorPadding;

    static double TSCHRoundLength;
    // other TSCH vars
    static int  channelCount;
    static bool ACKenabled;

    // Act as sync-agent
    static bool     globalVarsInitialized;
    static MacState currentMACState;
    static int      currentCAPslot;
    static int      currentCFPslot;

    void   switchMacState(MacState newState){
        //coordinator logics
        if(this->isCoordinator){
            macTrace() << "Switching from " << currentMACState << " to " << newState;
            currentMACState = newState;
        }
        //normal nodes logics
    };
    /* Variables parsed from NED end */


    /* LOCAL internal variables */
    bool       isCoordinator; //TODO: cram this to init
    bool       isCH;          //TODO: cram this to init
    int        currentChannel;
    cPacket*   currentPacket; //packet that is attempting to send
    RadioState currentRadioState;
    ConvergecastRouting* routingModule;

    bool exactOnceCAPactionFlag;

    /*THE SCHEDULE MATRIX*/
    CFPSchedule* schedule;
    vector<vector<ActionRecord>> knownActionByTimeslot;

    // LOGIC HANDLERS OF TIME
    void timerFiredCallback(int index_); //we are overriding this bc MAC normally has no time-logic

    // START- these applies to all nodes, with a filter inside
    void setTimerTSCHRoundLooped(double);
    void preCAPsetup();                  //general TSCH method DONOTOVERRIDE
    void processAndLoopCAPSlot();
    void updateSchedule(int sourceId, int timeslot, int opChannel, Action_type action);
    void toMacLayerDirect(cPacket * pkt, int dstId);

    void preCFPsetup(){
        if(this->isCoordinator){
            currentCFPslot=0;
            switchMacState(CFP);
        }
        hopChannel(0);
        macSpecificPreCFPSetup();
    };
    void performCurrentCFPAction();

    virtual void macSpecificPreCAPSetup(){
        throw cRuntimeError("not implemented");
    };     //for MAC protocols, runs after general setup is done
    virtual void CAPprocessing(){
        throw cRuntimeError("not implemented");
    };
    virtual void macSpecificPreCFPSetup(){
        throw cRuntimeError("not implemented");
    };     // ---
    // END

    // LOGIC HANDLERS OF PACKETS

    // timely logic
    void setUpOnCurrentCAPSlot();
    virtual void processCurrentCAPSlot();

    void setUpOnCurrentCFPSlot();
    void postCFPSlotSetUp();

    //slot-related timely logic

    //call this after every timer handling, AFTER the action is done
    //does nothing on the last slot
    void switchRadioState(Action_type state){ //shorthand for switching radio states
        switch(state){
            case SLEEP_TSCH: {
                toRadioLayer(createRadioCommand(SET_STATE, SLEEP));
//                macTrace() << "Radio state set to SLEEP";
                break;
            }
            case ATTEMPT_TX: {
                toRadioLayer(createRadioCommand(SET_STATE, TX));
                macTrace() << "Radio state set to TX";
                break;
            }
            case OPEN_RX: {
                toRadioLayer(createRadioCommand(SET_STATE, RX));
                macTrace() << "Radio state set to RX";
                break;
            }
        }
    }

    // life-cycle logic
    void specificInitialize();
    void startup();
    void finishSpecific() {}

    void fromNetworkLayer(cPacket * pkt, int id){

        MAC802154LitePacket *macPkt = new MAC802154LitePacket("TSCH data packet", MAC_LAYER_PACKET);
        macPkt->setSource(SELF_MAC_ADDRESS);
        macPkt->setDestination(id);
        macPkt->setTSCHPacketType(DATA_TSCH_PACKET);
        encapsulatePacket(macPkt, pkt);

        //buffer this, dispatch to Radio Layer later
        bufferPacket(macPkt);
    };

    void fromRadioLayer(cPacket * pkt, double rssi, double lsi ){
        MAC802154LitePacket* tschPkt = check_and_cast<MAC802154LitePacket*>(pkt);

        switch(tschPkt->getTSCHPacketType()){
        case DATA_TSCH_PACKET:{
            macTrace() <<"SLOT " << currentCFPslot << "collected pkt from " << tschPkt->getSource();

            toNetworkLayer(decapsulatePacket(tschPkt));

            if(ACKenabled) immediateACKrespond(pkt);
            break; //this deletes the tschPkt or MacPkt and its legal
        }
        case ACK_TSCH_PACKET:{
            if(!this->ACKenabled) throw cRuntimeError("ACK not enabled");
            else handleACKresponse(pkt);
            break;
        }
        case CONTROL_TSCH_PACKET:{
            //Note that this is designed to be written-over
            throw cRuntimeError("Control packets should bypass the WC");
        }
        }
    };

    virtual void handleTSCHControlPacket(cPacket* pkt){
        throw cRuntimeError("virtual method not implemented");
    };

    void immediateACKrespond(cPacket* pkt){
        MAC802154LitePacket* tschPkt = check_and_cast<MAC802154LitePacket*>(pkt);
        int ACKsource = tschPkt->getSource();

        MAC802154LitePacket* resPkt = new MAC802154LitePacket("ACK response", MAC_LAYER_PACKET);
        resPkt->setTSCHPacketType(ACK_TSCH_PACKET);
        resPkt->setDestination(tschPkt->getSource());
        resPkt->setSource(self);

        cPacket *sendToMAC = check_and_cast<cPacket*>(resPkt);
        toMacLayerDirect(sendToMAC, ACKsource);
    }

    //this should be over-ridden per specific MAC protocols
    void handleACKresponse(cPacket* pkt){
        MAC802154LitePacket* tschPkt = check_and_cast<MAC802154LitePacket*>(pkt);
        macTrace() <<"SLOT " << currentCFPslot << " got ACK from " << tschPkt->getSource();
    }

    void fromMacLayerDirect(cPacket * pkt){
        MAC802154LitePacket* tschPkt = check_and_cast<MAC802154LitePacket*>(pkt);

        switch(tschPkt->getTSCHPacketType()){
        case CONTROL_TSCH_PACKET: {
            handleTSCHControlPacket(pkt);
            break;
        } case DATA_TSCH_PACKET: {
            throw cRuntimeError("Data packets are forbidden from this method of transferring");
        } case ACK_TSCH_PACKET: {
            throw cRuntimeError("ACK packets are forbidden from this method of transferring");
        }
        }
//        throw cRuntimeError("fromMacLayerDirect");
    };

    //channel hopping from MAC layer
    virtual void hopChannel(int targetChannel){
      if (targetChannel < channelCount and targetChannel>=0){

          //only announce when channel switching
          if(this->currentChannel!=targetChannel){
              macTrace() << "hopping to channel: " << targetChannel;
          }
          this->currentChannel = targetChannel;

          //each channels have a 100Hz guard against their adjacent channels
          // to completely avoid the other channels
          double carrierFreq = 2400+(this->currentChannel*10);
          toRadioLayer(createRadioCommand(SET_CARRIER_FREQ, carrierFreq));
      } else {
          throw cRuntimeError("Hopping channel that doesn't exist");
      }
    }
};

#endif /* NODE_COMMUNICATION_MAC_MAC802154TSCHLITE_MAC802154TSCHLITE_H_ */
