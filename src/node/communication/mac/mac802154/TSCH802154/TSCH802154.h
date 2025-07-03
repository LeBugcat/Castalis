///****************************************************************************
// *  Copyright: National ICT Australia,  2007 - 2012                         *
// *  Developed at the ATP lab, Networked Systems research theme              *
// *  Author(s): Yuriy Tselishchev                                            *
// *  This file is distributed under the terms in the attached LICENSE file.  *
// *  If you do not find this file, copies can be found by writing to:        *
// *                                                                          *
// *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
// *      Attention:  License Inquiry.                                        *
// *                                                                          *
// ****************************************************************************/
//

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////                                                                 /////////////////
///////////////          ATTENTION!!! THIS MODULE IS NOW STOPPED DEV-ING        /////////////////
///////////////          DUE TO THE IMMENSE COMPLEXITY, THE TSCH NOW IS DEV-ED  /////////////////
///////////////          DIRECTLY ABOVE THE MAC VIRTUAL CLASS                   /////////////////
///////////////                                                                 /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

//#ifndef TSCH_802154_H_
//#define TSCH_802154_H_
//
//#include <string>
//#include <vector>
//#include <map>
//
//#include "node/communication/mac/mac802154/Basic802154.h"
//#include "node/communication/mac/mac802154/Basic802154Packet_m.h"
//#include "node/communication/mac/mac802154/TSCH802154/TSCH802154Packet_m.h"
//
//enum TSCH802154Action_type{
//    TSCH802154_SCHEDULED_ATTEMPT_TX = 1,
//    TSCH802154_SCHEDULED_OPEN_RX = 2,
//    TSCH802154_SCHEDULED_SLEEP = 3
//};
//

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////                                                                 /////////////////
///////////////          ATTENTION!!! THIS MODULE IS NOW STOPPED DEV-ING        /////////////////
///////////////          DUE TO THE IMMENSE COMPLEXITY, THE TSCH NOW IS DEV-ED  /////////////////
///////////////          DIRECTLY ABOVE THE MAC VIRTUAL CLASS                   /////////////////
///////////////                                                                 /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


//struct Single_Action{
//    TSCH802154Action_type type;
//    int timeslot; //relative to the current superframe
//    int channel;
//    simtime_t simClockTime;
//};
//
//struct Neighbor_Info{
//    int ID;
//    int parentID;
////    int ;
//    vector<Single_Action> broadcastedActions;
//
//    Neighbor_Info(int id, int pid){
//        this->ID=id;
//        this->parentID=pid;
//    }
//
//    Neighbor_Info(int id){
//        this->ID=id;
//        this->parentID=-1;
//    }
//};
//
//class TSCH802154: public Basic802154 {
// protected:
//    /*--- 802154e variables --- */
//    static bool frameVarsLock; //we lock it when
//    static int currentCAPlength;
//    static int currentFrameSize;
//
//    // map here: name and type, access by: scheduledActionStack[index] = = (action_type, timeslot, length)
//    vector<Single_Action*> scheduledActionStack;
//
//    //these are action scheduling functions.
//    //will return false if cant be scheduled
//    //TIME MODEL of ONE FRAME: |slot 0|slot 1|slot 2|slot 3|...|slot n|
//    //                         |        CAP         |       CFP       |
//    bool scheduleTxAt(int slot, int channel){
//        Single_Action* action = new Single_Action();
//        action->type = TSCH802154_SCHEDULED_ATTEMPT_TX;
//        action->channel = channel;
//        action->timeslot = slot;
//        scheduledActionStack.push_back(action);
//    };
//    bool scheduleRxAt(int slot, int channel){
//        Single_Action* action = new Single_Action();
//        action->type = TSCH802154_SCHEDULED_OPEN_RX;
//        action->channel = channel;
//        action->timeslot = slot;
//        scheduledActionStack.push_back(action);
//    }; //pass in: scheduled in-frame RX slot
//    bool scheduleSleep(int slot, int duration){ //duration in slots
//        for(int i=0;i<duration;i++){
//            Single_Action* action = new Single_Action();
//            action->type = TSCH802154_SCHEDULED_SLEEP;
//            action->channel = -1;
//            action->timeslot = slot;
//            scheduledActionStack.push_back(action);
//        }
//    }; //pass in: start, duration
//
//    //map here: ID (send ID),  their broadcasted actions, the neighborhood-degree
////    vector<>
//
// 	/*--- 802154Mac GTS list --- */
//	vector<Basic802154GTSspec> GTSlist;	// list of GTS specifications (for PAN coordinator)
//	int assignedGTS,requestGTS,totalGTS,totalSlots,baseSlot,minCap,frameOrder;
//	bool gtsOnly;
//
//	//TSCH-related params, set to default if not configured
//	int currentChannel = 0;
//	int maximalChannelCount = 16;
//
//	//new TSCH-specific Enhanced Beacon
//	TSCH802154Packet* beaconPacket;
//
////CHECKER: this lists the needed implementations
////         of this class's father, delete func
////	       if the func is designed and written
////
////    virtual void startup();
////    virtual void timerFiredCallback(int);
////    virtual void finishSpecific();
////
////    /*--- HUB-specific desicions ---*/
////    virtual void dataReceived_hub(Basic802154Packet *) {}
////    virtual int gtsRequest_hub(Basic802154Packet *) { return 0; }           //default: always reject
////    virtual int associationRequest_hub(Basic802154Packet *) { return 1; }   //default: always accept
////    virtual void prepareBeacon_hub(Basic802154Packet *) {}
////
////    /*--- Node-specific desicions ---*/
////    virtual void connectedToPAN_node() {}
////    virtual void disconnectedFromPAN_node() {}
////    virtual void assignedGTS_node(int) {}
////    virtual void receiveBeacon_node(Basic802154Packet *);
////    virtual void startedGTS_node() {}
////
////    /*--- General desicions ---*/
////    virtual bool acceptNewPacket(Basic802154Packet *);
////    virtual void transmissionOutcome(Basic802154Packet *, bool, string);
//
//
//	void startup();
//	void timerFiredCallback(int);
//	void fromRadioLayer(cPacket *, double , double);
//	void fromMacLayerDirect(cPacket *){
//	    throw cRuntimeError("Undefined behaviour");
//	};

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////                                                                 /////////////////
///////////////          ATTENTION!!! THIS MODULE IS NOW STOPPED DEV-ING        /////////////////
///////////////          DUE TO THE IMMENSE COMPLEXITY, THE TSCH NOW IS DEV-ED  /////////////////
///////////////          DIRECTLY ABOVE THE MAC VIRTUAL CLASS                   /////////////////
///////////////                                                                 /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

//
//	int gtsRequest_hub(Basic802154Packet *);
//	void prepareBeacon_hub(Basic802154Packet *);
//	void disconnectedFromPAN_node();
//	void assignedGTS_node(int);
//	void saveSelfCSVschedule_node();
//	void scheduleTimerLoop_node();
//
//	bool acceptNewPacket(Basic802154Packet *);
//	void transmissionOutcome(Basic802154Packet *, bool, string);
//
//
////     decide on the buffer-based packet sending mechanism
//	//normal data packets transmission in pre-defined GTS slots
//	virtual void attemptGTStransmission_node(TSCH802154Packet * dataPacket, const char* desc){
//	    Basic802154::attemptTransmission(desc);
//	};
//
//	//timely function that acts as a pseudo loop for multiple GTSs data transmissions
//	//this function PERFORMS the scheduledActionStack, this function is recursive
//	virtual void setTimerSingleAction(){
//
//	    //pop the action queue
//
//	    //set MAC state to GTS
////	    setMacState();
//
//	    //set Radio state accordingly
//
//	    //attemp transmission
//
//	    //timely modifications are at timerFiredCallback override
//	};
//
//	virtual void broadcastFrameStructure(int frameSize, int CAPsize){};
//
//	virtual void hopChannel(int targetChannel){
//	    if (targetChannel < maximalChannelCount and targetChannel>=0){
//	        this->currentChannel = targetChannel;
//	        double carrierFreq = 2400+(this->currentChannel*100);
//	        toRadioLayer(createRadioCommand(SET_CARRIER_FREQ, carrierFreq));
//	    } else {
//	        throw cRuntimeError("Hopping channel that doesn't exist");
//	    }
//	}
//};
//
//#endif	//TSCH802154
