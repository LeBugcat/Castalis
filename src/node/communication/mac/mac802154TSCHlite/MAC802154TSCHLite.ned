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

package node.communication.mac.mac802154TSCHlite;

simple MAC802154TSCHLite like node.communication.mac.iMac {

parameters:
    bool ACKenabled        			= default(false);
    int frameSize          			= default(100);
    int CAPSize                     = default(20);
    double TSCHRoundLength @unit(s) = default(100s);
    
    int channelCount   = default(16);
    
    bool isCoordinator = default(false);
    
    	//mac layer parameters
	int macMaxPacketSize = default (0);		//no limit on frame size
	int macPacketOverhead = default (11);	//DATA frame overhead is described in TMacFrame.msg
	int macBufferSize = default (32);		//buffer of 32 packets by default

	//parameters dependent on physical layer
	double phyDelayForValidCS = default (0.128);
	double phyDataRate = default (250);
	int phyFrameOverhead = default (6);
	
	bool collectTraceInfo = default (true);

 gates:
	output toNetworkModule;
	output toRadioModule;
	input fromNetworkModule;
	input fromRadioModule;
	input fromCommModuleResourceMgr;
	
	input fromDirect @directIn;
}
