//****************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2011                         *
//*  Developed at the ATP lab, Networked Systems research theme              *
//*  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
//*  This file is distributed under the terms in the attached LICENSE file.  *
//*  If you do not find this file, copies can be found by writing to:        *
//*                                                                          *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
//*      Attention:  License Inquiry.                                        *
//*                                                                          *  
//****************************************************************************/

cplusplus {{
#include "node/communication/mac/MacPacket_m.h"
}}

class MacPacket;

enum MAC802154LitePacket_type {
	
	//packet type for data sending through the WC
	DATA_TSCH_PACKET = 0;
	
	//packet types for controls signalling (if neededs), bypassing WC for now
	ACK_TSCH_PACKET = 1;
	CONTROL_TSCH_PACKET = 2;
}

packet MAC802154LitePacket extends MacPacket {

	int TSCHPacketType enum (MAC802154LitePacket_type);	
}

