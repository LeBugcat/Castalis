#ifndef MINIMAL6TOP_H
#define MINIMAL6TOP_H

#include <omnetpp.h>
#include "node/communication/mac/VirtualMac.h"
#include "helpStructures/TimerService.h"

enum TimerType {
    SLOT_EVENT = 1,   // Repeated slot timer for TDMA operation

    RADIO_TX_ON,
    RADIO_TX_OFF,

    RADIO_RX_ON,
    RADIO_RX_OFF,
    // ... other control events
};

enum SlotType {
    SLOT_TX,
    SLOT_RX,
    SLOT_IDLE
};

// Define the 6Top protocol class
class Minimal6Top : public VirtualMac, TimerService {
    // Declare required parameters, variables, and functions
    protected:
        // Timeslot data structure for 6Top protocol
        struct Timeslot {
            int id;
            bool allocated;
            int ownerModuleId;       // e.g., scheduler module
            SlotType type;
            int channelOffset;
        };

        // A vector to store the list of timeslots
        std::vector<Timeslot> timeslots;

    public:
        // 6Top Control Functions:
        // Adds a new timeslot to the list
        void addTimeslot(int timeslotId);

        // Deletes a timeslot from the list by its ID
        void deleteTimeslot(int timeslotId);

        // Counts the number of active timeslots
        int countTimeslots();

        // Lists all the timeslot IDs in the system
        std::vector<int> listTimeslots();

    protected:
        // Initialization function (called when the module is initialized)
        virtual void initialize();

        // Message handler (called when a message is received)
        void timerFiredCallback(int timerId);

};

#endif  // MINIMAL6TOP_H
