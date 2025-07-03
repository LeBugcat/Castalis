#ifndef NODE_APPLICATION_METRICCOLLECTOR_METRICCOLLECTOR_H_
#define NODE_APPLICATION_METRICCOLLECTOR_METRICCOLLECTOR_H_

#include "helpStructures/CastaliaModule.h"
#include "helpStructures/TimerService.h"
#include "helpStructures/DebugInfoWriter.h"
#include "node/application/VirtualApplication.h"

using namespace std;

enum MetricCollectorTimers {
    REQUEST_SAMPLE = 1,
    SEND_AGGREGATED_VALUE = 2
};

class MetricCollector: public VirtualApplication {
 private:
    double aggregatedValue;
    double waitingTimeForLowerLevelData;
    double lastSensedValue;
    double sampleInterval;
    int constantDataPayload;
    double sendInterval;
    int totalPackets;

    int inboundPacketCount;
    int outboundPacketCount;

    bool isUAV;


 protected:
    void startup();
    void timerFiredCallback(int);
    void handleSensorReading(SensorReadingMessage *);
    void handleNeworkControlMessage(cMessage *);
    void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
    void fromNetworkLayer(ApplicationPacket *);
    void finishSpecific();
    void reportNodeLifetime();
};

#endif /* NODE_APPLICATION_METRICCOLLECTOR_METRICCOLLECTOR_H_ */
