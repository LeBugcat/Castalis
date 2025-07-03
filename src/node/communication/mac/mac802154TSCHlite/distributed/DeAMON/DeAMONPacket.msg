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

enum DeAMONPacket_type {
	BUILD  		= 0; //all nodes does
	
	//for scheduling, only unicast, but will simulate broadcast and overhearing
	//and has channel info to simulate signal collisions
	RFS           = 1; 
	RFS_OVERHEARD = 2;
	
	ASG           = 3;
	ASG_OVERHEARD = 4;

	COLLISION     = 5; //special type for CA
}

packet DeAMONPacket extends MAC802154LitePacket {

	int DeAMONPacketType enum (DeAMONPacket_type);

	//for build phase
	int childrenListByPriority[]; //sorted list by priority
	
	//for signalling phase
	int signallingTimeslot;
	int signallingChannel;
	
	//for 6top scheduling phase
	int RFSTimeslots[];
	int RFSChannel;
	int ASGTimeslots[];
	int ASGChannels[];
}