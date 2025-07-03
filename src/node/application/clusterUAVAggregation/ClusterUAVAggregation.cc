#include "node/application/clusterUAVAggregation/ClusterUAVAggregation.h"

Define_Module(ClusterUAVAggregation);

void ClusterUAVAggregation::startup()
{
	sampleInterval = (double)par("sampleInterval") / 1000; //this is in s
	aggregatedValue = 0.0;
	waitingTimeForLowerLevelData = 0.0;
	lastSensedValue = 0.0;
	totalPackets = 0;
	constantDataPayload = par("constantDataPayload");
	if(!isCH)
	{
	    setTimer(REQUEST_SAMPLE, 0);
	    if(!isSink)
	        setTimer(SEND_AGGREGATED_VALUE, 10);
	}
	aggregatedPacketCount = 0;
	totalAggregatedBytes = 0;
}

void ClusterUAVAggregation::timerFiredCallback(int index)
{
	switch (index) {
		
		case REQUEST_SAMPLE:{
			requestSensorReading();
			setTimer(REQUEST_SAMPLE, sampleInterval);
			break;
		}
		
		case SEND_AGGREGATED_VALUE:{
			toNetworkLayer(createGenericDataPacket(aggregatedValue, totalPackets, constantDataPayload), PARENT_NETWORK_ADDRESS);
			routingTrace() << "SENDING AT APP LAYER";
			totalPackets++;
			setTimer(SEND_AGGREGATED_VALUE, 50);
			break;
		}
	}
}

void ClusterUAVAggregation::fromNetworkLayer(ApplicationPacket * rcvPacket,
        const char *source, double rssi, double lqi)
{
    double theData = rcvPacket->getData();
    int byteLength = rcvPacket->getByteLength();
    // do the aggregation bit. For this example aggregation function is a simple max.


    if (isSink)
        applicationTrace() << "from " << source << "received value " << theData;
}

void ClusterUAVAggregation::fromNetworkLayer(ApplicationPacket * rcvPacket)
{
    applicationTrace() << "Packet received at APP Layer";
	double theData = rcvPacket->getData();

	// do the aggregation bit. For this example aggregation function is a simple max.
	if (theData > aggregatedValue)
		aggregatedValue = theData;

	if (isSink)
		trace() << "from "<< "received value " << rcvPacket->getData();
}

void ClusterUAVAggregation::handleSensorReading(SensorReadingMessage * rcvReading)
{
	string sensType(rcvReading->getSensorType());
	double sensValue = rcvReading->getSensedValue();
	lastSensedValue = sensValue;
}

void ClusterUAVAggregation::handleNeworkControlMessage(cMessage * msg){}

