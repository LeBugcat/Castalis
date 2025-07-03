#include "node/application/metricCollector/MetricCollector.h"

Define_Module(MetricCollector);

void MetricCollector::startup()
{
    sendInterval = (double) par("sendInterval") / 1000;
    sampleInterval = (double) par("sampleInterval") / 1000;
    aggregatedValue = 0.0;
    waitingTimeForLowerLevelData = 0.0;
    lastSensedValue = 0.0;

    if(!isSink) setTimer(SEND_AGGREGATED_VALUE, 10);
}

void MetricCollector::timerFiredCallback(int index)
{
    switch (index) {

        case REQUEST_SAMPLE:{
            if(isUAV){
                return;
            }
            requestSensorReading();
            setTimer(REQUEST_SAMPLE, sampleInterval);
            break;
        }

        case SEND_AGGREGATED_VALUE:{
            toNetworkLayer(createGenericDataPacket(aggregatedValue, totalPackets, constantDataPayload), PARENT_NETWORK_ADDRESS);
            routingTrace() << "SENDING AT APP LAYER";
            totalPackets++;
            setTimer(SEND_AGGREGATED_VALUE, sendInterval);
            break;
        }

        default: {
            VirtualApplication::timerFiredCallback(index);
        }
    }
}

void MetricCollector::fromNetworkLayer(ApplicationPacket * rcvPacket,
        const char *source, double rssi, double lqi)
{
    double theData = rcvPacket->getData();

    // do the aggregation bit. For this example aggregation function is a simple max.
    if (theData > aggregatedValue)
        aggregatedValue = theData;

    if (isSink)
        trace() << "from " << source << "received value " << theData;
        applicationTrace() << "Packet received at APP Layer with WC metrics";
}

void MetricCollector::fromNetworkLayer(ApplicationPacket * rcvPacket)
{
    applicationTrace() << "Packet received at APP Layer";
    double theData = rcvPacket->getData();

    // do the aggregation bit. For this example aggregation function is a simple max.
    if (theData > aggregatedValue)
        aggregatedValue = theData;

    if (isSink)
        trace() << "from "<< "received value " << rcvPacket->getData();
}

void MetricCollector::handleSensorReading(SensorReadingMessage * rcvReading)
{
    string sensType(rcvReading->getSensorType());
    double sensValue = rcvReading->getSensedValue();
    lastSensedValue = sensValue;
}

void MetricCollector::handleNeworkControlMessage(cMessage * msg){}

void MetricCollector::reportNodeLifetime(){
    applicationTrace()<<"Node reporting death at"<< resMgrModule->getDeath() <<" at remaining battery "<<resMgrModule->getRemainingEnergy();
}

void MetricCollector::finishSpecific(){
    reportNodeLifetime();
}

