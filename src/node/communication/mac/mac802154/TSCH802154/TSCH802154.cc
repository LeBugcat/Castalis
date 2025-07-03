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

//#include "node/communication/mac/mac802154/TSCH802154/TSCH802154.h"
//#include "node/communication/mac/mac802154/TSCH802154/TSCH802154Packet_m.h"
//
//
//Define_Module(TSCH802154);
//
///***
// * Initialising some parameters, specific to Static GTS module
// * by overriding the startup() method. Important to call startup()
// * of the parent module in the end, otherwise it will not initialize
// ***/
//void TSCH802154::startup() {
//	// initialise GTS-specific parameters
//	GTSlist.clear(); totalGTS = 0; assignedGTS = 0;
//	requestGTS = par("requestGTS");
//	gtsOnly = par("gtsOnly");
//
//	//every nodes now is FFD nodes
//	isFFD = true;
//
//	//these must be in slots unit
//    currentCAPlength = par("defaultCAPsize");
//    currentFrameSize = par("defaultFramesize");
//
//	// other parameters are from Basic802154,
//    // need to parse them for GTS scheduling
//    // these are never used
//	totalSlots = par("numSuperframeSlots");
//	baseSlot = par("baseSlotDuration");
//	minCap = par("minCAPLength");
//	frameOrder = par("frameOrder");
//	return Basic802154::startup();
//}
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

//void TSCH802154::timerFiredCallback(int index){
//    //overriding mechanism for sink, using new Enhanced Beacon
//
//    switch(index) {
//        // Start of a new superframe
//        case FRAME_START: {
//            if (isPANCoordinator) { //equals to isSink
//                // as a PAN coordinator, create and broadcast beacon packet
//                beaconPacket = new TSCH802154Packet("PAN beacon packet", MAC_LAYER_PACKET);
//                beaconPacket->setDstID(BROADCAST_MAC_ADDRESS);
//                beaconPacket->setPANid(SELF_MAC_ADDRESS);
//                beaconPacket->setMac802154PacketType(MAC_802154_BEACON_PACKET);
//                beaconPacket->setBeaconOrder(beaconOrder);
//                beaconPacket->setFrameOrder(frameOrder);
//                if (++macBSN > 255) macBSN = 0;
//                    beaconPacket->setBSN(macBSN);
//
//                //802154e specific EB variables
//                beaconPacket->setCurrentFramesize(currentFrameSize);
//                beaconPacket->setCAPlength(currentCAPlength);
//
//                // GTS fields and CAP length are set in the decision layer
//                prepareBeacon_hub(beaconPacket);
//
//                beaconPacket->setByteLength(BASE_BEACON_PKT_SIZE + beaconPacket->getGTSlistArraySize() * GTS_SPEC_FIELD_SIZE);
//                CAPlength = beaconPacket->getCAPlength();
//                CAPend = CAPlength * baseSlotDuration * (1 << frameOrder) * symbolLen;
//                sentBeacons++;
//
//                mac_trace() << "Transmitting [PAN enhanced beacon packet] now, BSN = " << macBSN;
//
//                toMacLayerDirect(beaconPacket, BROADCAST_MAC_ADDRESS);
//                beaconPacket = NULL;
//
//                currentFrameStart = getClock() + phyDelayRx2Tx;
//                setTimer(FRAME_START, beaconInterval * symbolLen);
//
//                setTimer(CFP_START, CAPend);
//            } else {    // if not a PAN coordinator, then wait for beacon
//                toRadioLayer(createRadioCommand(SET_STATE, RX));
//                setTimer(BEACON_TIMEOUT, guardTime * 3);
//            }
//            break;
//        }
//        //start of CFP. this timestampt is not parsed
//        //but infered from CAP slots duration
//        case CFP_START: {
//            //single action
//            if(this->scheduledActionStack.empty()){
//                mac_trace() << "No scheduled actions during CFP";
//            } else {
//                //this mass set the actions during CFP
//                setTimerSingleAction();
//            }
//        }
//        //preserving Basic802154 timely mechanisms
//        default: Basic802154::timerFiredCallback(index);
//    }
//}
////
//void TSCH802154::saveSelfCSVschedule_node(){
//    //: implement
//    throw cRuntimeError("MAC802415e Unimplemented behaviour: saveSelfCSVschedule_node");
//};
//void TSCH802154::scheduleTimerLoop_node(){
//    //: implement
//    throw cRuntimeError("Schedule timer looping mechanism not implemented!");
//};
//
//////: investigate, this is an unreliable mechanism
////void TSCH802154::fromMacLayerDirect(cPacket * pkt){
////    fromRadioLayer(pkt, 0, 0);
////}
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

//void TSCH802154::fromRadioLayer(cPacket * pkt, double rssi, double lqi)
//{
//    TSCH802154Packet *rcvPacket = dynamic_cast<TSCH802154Packet*>(pkt);
//    if (!rcvPacket) {
//        return;
//    }
//
//    // add to neighbor list if not exist yet
//    // note that this only works with packets AFTER THE ROUTING TREE CREATION PHASE
//    // for nodes that do not have a parent, this will put the parent into null value
//    if (rcvPacket->getDstID() != SELF_MAC_ADDRESS &&
//            rcvPacket->getDstID() != BROADCAST_MAC_ADDRESS) {
//        return;
//    }
//
//
//    switch (rcvPacket->getMac802154PacketType()) {
//
//        /* received an Enhanced BEACON frame, including the Framesize broadcasted parameter*/
//        case MAC_802154_ENHANCED_BEACON_PACKET: {
//            if (isPANCoordinator)
//                break;          //PAN coordinators ignore beacons from other PANs
//            if (associatedPAN != -1 && associatedPAN != rcvPacket->getPANid())
//                break;          //Ignore, if associated to another PAN or no PANs at all
//
//
//            //cancel beacon timeout message (if present)
//            cancelTimer(BEACON_TIMEOUT);
//            recvBeacons++;
//
//            //this node is connected to this PAN (or will try to connect),
//            //update frame parameters
//            double offset = TX_TIME(rcvPacket->getByteLength());
//            currentFrameStart = getClock() - offset;
//            //frame start is in the past, use datarate and packet bytelength to infer where did frame started
//            lostBeacons = 0;
//            frameOrder = rcvPacket->getFrameOrder();
//            beaconOrder = rcvPacket->getBeaconOrder();
//
//            //time length of a superframe, infered from beacon order
//            beaconInterval = baseSuperframeDuration * (1 << beaconOrder); //beaconInterval = baseSUperframeDuration * (2^beaconOrder)
//            macBSN = rcvPacket->getBSN();
//            CAPlength = rcvPacket->getCAPlength();
//            CAPend = CAPlength * baseSlotDuration * (1 << frameOrder) * symbolLen;
//
//            currentFrameSize = rcvPacket->getCurrentFramesize();
//
//            setMacState(MAC_STATE_CAP);
//            if (associatedPAN == rcvPacket->getPANid()) {
//                if (GTSstart != CAPend)
//                    // set timer to sleep after CAP, unless GTS slots starts right after
//                    setTimer(SLEEP_START, CAPend - offset);
//                if (GTSstart > 0) {
//                    // set GTS timer phyDelaySleep2Tx seconds earlier as radio can be sleeping
//                    setTimer(GTS_START, GTSstart - phyDelaySleep2Tx - offset);
//                }
//            } else {
//                setTimer(BACK_TO_SETUP, CAPend - offset);
//            }
//
//            //forwarding the packet, flood the network with beacons
//            receiveBeacon_node(rcvPacket);
//            attemptTransmission("CAP started");
//            setTimer(FRAME_START, baseSuperframeDuration * (1 << beaconOrder) *
//                 symbolLen - guardTime - offset);
//            break;
//        }
//        case MAC_802154_CAP_CONTROL_PACKET: {
////            handleCAPpacket_node(rcvPacket);
//        }
//        //revert to father packet handler
//        default: Basic802154::fromRadioLayer(pkt,rssi,lqi);
//    }
//}
//
///***
// * GTS request received by hub, need to return the number of
// * slots to be granted. Can return 0 to indicate request denial
// ***/
//int TSCH802154::gtsRequest_hub(Basic802154Packet *gtsPkt) {
//	//Length of CAP after lengths of all GTS slots are subtracted
//	int CAPlength = totalSlots;
//
//	//check if the node already exists in the GTS list
//	vector<Basic802154GTSspec>::iterator i;
//	int total = 0;
//	for (i = GTSlist.begin(); i != GTSlist.end(); i++) {
//		total++;
//		if (i->owner == gtsPkt->getSrcID()) {
//			if (i->length == gtsPkt->getGTSlength()) {
//				return i->length;
//			} else {
//				totalGTS -= i->length;
//				GTSlist.erase(i);
//				total--;
//			}
//		} else {
//			CAPlength -= i->length;
//		}
//	}
//
//	//node not found, or requested slots changed
//	if (total >= 7 || (CAPlength - gtsPkt->getGTSlength()) *
//	    baseSlot * (1 << frameOrder) < minCap) {
//		trace() << "GTS request from " << gtsPkt->getSrcID() <<
//		    " cannot be acocmodated";
//		return 0;
//	}
//
//	Basic802154GTSspec newGTSspec;
//	newGTSspec.length = gtsPkt->getGTSlength();
//	totalGTS += newGTSspec.length;
//	newGTSspec.owner = gtsPkt->getSrcID();
//	GTSlist.push_back(newGTSspec);
//	return newGTSspec.length;
//}
//
///***
// * Hub can alter the beacon before broadcasting it
// * In particular, assign GTS slots and set CAP length
// ***/
//void TSCH802154::prepareBeacon_hub(Basic802154Packet *beaconPacket) {
//    mac_trace() << "Not implemented!: TSCH802154::prepareBeacon_hub";
//}
//
///***
// * If disconnected from PAN, also need to reset GTS slots
// ***/
//void TSCH802154::disconnectedFromPAN_node() {
////	assignedGTS = 0;
//    mac_trace() << "Not implemented! TSCH802154::disconnectedFromPAN_node";
//}
//
///***
// * GTS request was successful
// ***/
//void TSCH802154::assignedGTS_node(int slots) {
//    mac_trace() << "Not implemented!: TSCH802154::assignedGTS_node";
//}
//
//
///***
// * Transmission of data packet requested earlier is complete
// * status string holds comma separated list of outcomes
// * for each transmission attempt
// ***/
//void TSCH802154::transmissionOutcome(Basic802154Packet *pkt, bool success, string status) {
//	if (getAssociatedPAN() != -1) {
//		if (assignedGTS == 0 && requestGTS > 0) {
//			transmitPacket(newGtsRequest(getAssociatedPAN(), requestGTS));
//		} else if (TXBuffer.size()) {
//			Basic802154Packet *packet = check_and_cast<Basic802154Packet*>(TXBuffer.front());
//			TXBuffer.pop();
//			transmitPacket(packet,0,gtsOnly);
//		}
//	}
//}
//
//bool TSCH802154::acceptNewPacket(Basic802154Packet *newPacket)
//{
//	if (getAssociatedPAN() != -1 && getCurrentPacket() == NULL) {
//		transmitPacket(newPacket,0,gtsOnly);
//		return true;
//	}
//	return bufferPacket(newPacket);
//}
