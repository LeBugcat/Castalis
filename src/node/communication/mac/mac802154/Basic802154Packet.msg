//****************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2012                         *
//*  Developed at the ATP lab, Networked Systems research theme              *
//*  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
//*  This file is distributed under the terms in the attached LICENSE file.  *
//*  If you do not find this file, copies can be found by writing to:        *
//*                                                                          *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
//*      Attention:  License Inquiry.                                        *
//*                                                                          *  
//****************************************************************************/

//alterating this to add Enhanced Beacon for IEEE 802.15.4e-2015

cplusplus {{
#include "node/communication/mac/MacPacket_m.h"
}}

class MacPacket;

enum Mac802154Packet_type {
	MAC_802154_BEACON_PACKET = 1001;
	MAC_802154_ENHANCED_BEACON_PACKET = 1006;
	MAC_802154_CAP_CONTROL_PACKET = 1007;
	
	MAC_802154_ASSOCIATE_PACKET = 1002;
	MAC_802154_DATA_PACKET = 1003;
	MAC_802154_ACK_PACKET = 1004;
	MAC_802154_GTS_REQUEST_PACKET = 1005;
	
//	MAC_802154_SELF_BROADCAST_PACKET = 1008; 
	// short heartbeat-like packet used for pronouncing neighborhood, 
	// in this packet, the PANID is the sink in the network of the SourceID
}

struct Basic802154GTSspec {
	int owner;
	int start;
	int length;
}

packet Basic802154Packet extends MacPacket {
	int Mac802154PacketType enum (Mac802154Packet_type);
	int PANid;
	int srcID;
	int dstID;
	int seqNum;

	// those fields belong to beacon packet (MAC_802154_BEACON_PACKET)
	int beaconOrder;
	int frameOrder;
	int BSN;
	int CAPlength;
	int GTSlength;
	Basic802154GTSspec GTSlist[];
}

