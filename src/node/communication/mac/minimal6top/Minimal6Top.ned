package node.communication.mac.minimal6top;

import node.communication.mac.iMac;

simple Minimal6Top like node.communication.mac.iMac {
    parameters:
		bool collectTraceInfo = default(true);
		int macMaxPacketSize = default(1024);
		int macBufferSize = default(50);
		int macPacketOverhead = default(10);
        int timeslotDuration @unit(ms) = default(10ms);            // Duration of each timeslot (modifiable)
        int slotframeLength = default(101);              // Number of slots in a slotframe (modifiable)
        
    gates:
        // Gate for Network Layer (output and input)
        output toNetworkModule;
        input fromNetworkModule;
        
        // Gate for Radio Layer (output and input)
        output toRadioModule;
        input fromRadioModule;
        
        // Gate for communication module resource manager
        input fromCommModuleResourceMgr @loose;

        // Direct input for immediate processing (e.g., control messages)
        input fromDirect @directIn; 
}