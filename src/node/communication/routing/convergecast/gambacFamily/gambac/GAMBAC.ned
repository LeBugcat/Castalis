package node.communication.routing.convergecast.gambacFamily.gambac;

simple GAMBAC extends node.communication.routing.convergecast.ConvergecastRouting
{
parameters:

	//Network-wide all nodes params
   //====Application Layer
//    string applicationID = default ("throughputTest");
//	
//	//====Routing/Upper Networking Layer	
//	//switch to true on Sink only
//	bool isSink = default (false);
//	
//	//GAMBAC-specific params
//	//====GAMBAC-specific params: Timely round-related params
//	double percentage = default (0.05);//???
//	double roundLength @unit(s); //GAMBAC param: possible lmax, in secs
//	double roundLengthGuard @unit(s) = default (2s);
//	double helloLength @unit(s) = default (0.01s);	//in secs
//	double discoverLength @unit(s) = default (0.04s);	//in secs
//	double slotLength;//slot? TODO: define this
//
//	//====GAMBAC-specific params: packet sizes
//	int dataScaleFactor = default(1);
//	double controlPacketSize = default (9); // Type + Source + Destination + CHid + nextCH + isCH = 16 bytes
//	double dataPacketSize = default (9);
	
	//====GAMBAC-specific params: UAV-related params
	int maxCHperUAV = default (1000); //GAMBAC param: M
//	int k0 = default (5);
	double epsilon = default (0.1);
	double neighborRange = default (100);
//	int numNodesPerCluster = default (50);
//	double maxUAVFlyingDistance = default (24000);
	double gamma = default (0.9999999);

	//====Used in the first phase of the algorithm
	bool debugRecruitProcess = default (true);
	bool bypassRoutingBuffer=default(true);

	//Parameters used to power control.
	@class(GAMBAC);
}