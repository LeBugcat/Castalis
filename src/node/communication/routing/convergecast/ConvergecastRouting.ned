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

package node.communication.routing.convergecast;

simple ConvergecastRouting like node.communication.routing.iRouting{

parameters:
    
    //=========FOR PACKET TRANSFERING==================//
	bool collectTraceInfo = default(true);
	int maxNetFrameSize = default(1024);		// in bytes
	int netDataFrameOverhead = default(8);	// in bytes
	int netBufferSize = default(32);			// in number of messages
	double dataPacketSize = default (9); //This prob wont change
	int dataScaleFactor = default(1);
	double controlPacketSize = default (9); // Type + Source + Destination + CHid + nextCH + isCH = 16 bytes	

	//========= FOR CLUSTERED CONVERGECAST ROUTING ===============//
    string applicationID = default ("SimpleAggregation");
    bool isSink = default (false);
    double percentage = default (0.05);//???
    double roundLength @unit(s) = default(10s);
	int numNodesPerCluster = default (20);

    
    //========= WITH UAV ASSISTANCE      ===============//
	double offsetSendToUAV @unit(s) = default(8s); //in seconds
	double maxUAVFlyingDistance = default (24000);
		
	int k0 = default (5);

gates: 
	output toCommunicationModule @loose;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
	input fromDirect @directIn;
	
}