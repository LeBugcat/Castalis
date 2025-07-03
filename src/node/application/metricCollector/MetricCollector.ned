package node.application.metricCollector;

// The sensor node module. Connects to the wireless channel in order to communicate 
// with other nodes. Connects to psysical processes so it can sample them.

simple MetricCollector like node.application.iApplication {
 
	parameters:
     	string applicationID = default ("MetricCollector");
    	bool collectTraceInfo = default (true);
    	int priority = default (1);
    	int packetHeaderOverhead = default (8);	// in bytes
    	int constantDataPayload = default (1000);	// in bytes
    	double sampleInterval = default (10000);	// in ms
    	double sendInterval = default (100000);	// in ms
    	bool isSink = default (false);
    	bool isUAV = default (false);
    	

 	gates:
     	output toCommunicationModule;
    	output toSensorDeviceManager @loose;
    	
    	input fromCommunicationModule;
    	input fromSensorDeviceManager @loose;
		input fromResourceManager;
}

