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
cplusplus {{
#include "node/communication/mac/mac802154TSCHlite/MAC802154LitePacket_m.h"
}}

class MAC802154LitePacket;

enum MCMTMACPacket_type {
	HELLO  		= 0; //all nodes does
	ASSIGN   	= 1; //only BD
	INFO     	= 2; //only BD
	BD_CLAIM    = 3; //only BD
	BATCH_CLAIM = 4; //all nodes does
	DONE   		= 5; //all nodes does
}

enum MCMTMACGeoColor_type {
	GREEN = 0;
	RED = 1;
	PURPLE = 2;
	YELLOW = 3;
}

packet MCMTMACPacket extends MAC802154LitePacket {

	//default args
	int MCMTMACPacketType enum (MCMTMACPacket_type);
	
	bool hasEncapsulatedPacket = false;
	bool isBlankChainAssign    = false;
	
	//since omnet++ is not allowing us to have cyclic reference, we have to resort to this
	int encapsulatedNodeId     		  = -1;
	int encapsulatedParentId 		  = -1;
	int encapsulatedCHId 		  	  = -1;
	int encapsulatedTransmissionCount = -1;
	int encapsulatedGeocolor          = -1;

	int parentId;
	int CHId;
	int sourceGeoColor enum (MCMTMACGeoColor_type);
	int transmissionCount;
	
	int batchClaimTimeslots[]; //timeslot, channelId
	int batchClaimChannelIds[]; //timeslot, channelId
	
 	int singleClaimTimeslot;
 	int singleChannelId;
}