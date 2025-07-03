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
#include "node/communication/routing/RoutingPacket_m.h"
#include "helpStructures/GeoMathHelper.h"
typedef std::vector<int> IntVector;
}}

class noncobject Point;
class noncobject IntVector;

class RoutingPacket;

enum ConvergecastPacket_Type
{
	CC_ROUTING_CONTROL_PACKET = 1;			
	CC_ROUTING_DATA_PACKET = 2;
	CC_ROUTING_JOIN_PACKET = 3;
	CC_ROUTING_ADV_PACKET = 4;
};

packet ConvergecastPacket extends RoutingPacket
{	
	int ConvergecastPacketKind enum (ConvergecastPacket_Type); 
	int sourceCH;
	int sourceRound;
	double remainingEnergy;
	IntVector isCH;
	IntVector nextCH;
	IntVector trace;
};