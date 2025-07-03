#include "node/application/simpleAggregation/SimpleAggregation.h"

Define_Module(SimpleAggregation);

void SimpleAggregation::printOutRoundResults(int roundNumber){
    metricsTrace() << "SimpleAggregation::printOutRoundResults";
    radioModule->printRoundStats(roundNumber);
    radioModule->resetRoundStats();
}

void SimpleAggregation::initialize(){

    VirtualApplication::initialize();
    sendInterval = (double)par("sendInterval");
    dataSamplingDisabled = par("dataSamplingDisabled");

    if(hasPar("constantDataPayload")){
        constantDataPayload = (int)par("constantDataPayload");
    } else constantDataPayload = 8;

    currentDataPktSize = constantDataPayload;
    datarate = (8*constantDataPayload/sendInterval)*1000;
}


void SimpleAggregation::startup()
{
    sampleInterval = (double)par("sampleInterval");
    sendInterval = (double)par("sendInterval");
    if(hasPar("constantDataPayload")){
        constantDataPayload = (int)par("constantDataPayload");
    } else constantDataPayload = 8;

    aggregatedValue = 0.0; //this denotes blank data value

    aggregatedPktSize = 0;
	waitingTimeForLowerLevelData = 0.0;
	lastSensedValue = (double)self; //initial data==Node ID, might be useful
	totalPackets = 0; // for bookkeeping

	if(!isSink){
	    if(!dataSamplingDisabled) setTimer(REQUEST_SAMPLE, 0);
	    int myOwnSendOffset = rand() % 50 + 3;
	    setTimer(SEND_AGGREGATED_VALUE, myOwnSendOffset);
	}
    datarate = (8*constantDataPayload/sendInterval)*1000;

	if(!loggedInit){
        applicationTrace() << "currently working with variables:";
        applicationTrace() << "sampleInterval : " << sampleInterval;
        applicationTrace() << "sendInterval : " << sendInterval;
        applicationTrace() << "constantDataPayload: " << constantDataPayload;
        applicationTrace() << "(infered) data-rate: " << datarate << " kbit/sec";
        loggedInit = true;
	}
}

void SimpleAggregation::sendData(){
    toNetworkLayer(createGenericDataPacket(aggregatedValue, totalPackets, aggregatedPktSize),
            "Aggregated Blank Packet with realistic size");
    //reset internal variables
    aggregatedPktSize=0;
    totalPackets++;
};

void SimpleAggregation::timerFiredCallback(int index)
{
	switch (index) {
		
		case REQUEST_SAMPLE:{
			requestSensorReading();
			//set another timer for putting data sampling into loop
			setTimer(REQUEST_SAMPLE, sampleInterval);
			break;
		}
        case SEND_AGGREGATED_VALUE:{
            //set another timer for putting data sampling into loop
            sendData();
            setTimer(SEND_AGGREGATED_VALUE, sendInterval);
            break;
        }
        case RESERVED_TIMER:{
            //set another timer for putting data sampling into loop
            throw cRuntimeError("Unimplemented Timer Logic");
            break;
        }
	}
}

//function for proper handle dataPkt with actual radio variables
void SimpleAggregation::fromNetworkLayer(ApplicationPacket * rcvPacket,
        const char *source, double rssi, double lqi)
{
    double dataPktSize = rcvPacket->getByteLength();

    // do the aggregation bit. This aggregation function is aggregating the size only.
    aggregatedPktSize+=dataPktSize;
//    delete rcvPacket;


//     log if the data pkt arrived at its destined CH
//    if (self == rcvPacket->)
//        trace() << "from " << source << "received packet size " << aggregatedPktSize;
}

//function for quick hand handle dataPkt from lower-in-stack layer
void SimpleAggregation::fromNetworkLayer(ApplicationPacket * rcvPacket)
{
    double dataPktSize = rcvPacket->getByteLength();
//    delete rcvPacket;
//    deleteAndCancel(rcvPacket);
}

//This function does nothing as we dont need the sensed values
void SimpleAggregation::handleSensorReading(SensorReadingMessage * rcvReading){
    lastSensedValue = rcvReading->getSensedValue();
    if(dataSamplingDisabled) throw cRuntimeError("This is not suppose to happen");
    //reaching packet end-of-life
//    delete rcvReading;
}

//Debug trap, for bugs
void SimpleAggregation::handleNeworkControlMessage(cMessage * msg){
    throw cRuntimeError("Debug Trap: Unimplemented behavior, unknown msg");
}

void SimpleAggregation::finishSpecific(){
    double nodePower = resMgrModule->currentNodePower;
    double remnEnergy = resMgrModule->remainingEnergy;

    metricsTrace() << "EndOfSim#metrics#currentNodePower#"<< nodePower<<"#";
    metricsTrace() << "EndOfSim#metrics#remainingEnergy#"<< remnEnergy<<"#";
//    double remnEnergy = resMgrModule->baselineNodePower;
}
