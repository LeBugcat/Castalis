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

#include "HeterogeneousTraffic.h"

Define_Module(HeterogeneousTraffic)

void HeterogeneousTraffic::sendData() {

//    currentDataPktSize    //randomize (or not) this Data Packet
    if(isConstantDataPayload){
        //do nothing since everything is initialized
    } else {
        applicationTrace() << "Generated and sending Data Packet with size " << currentDataPktSize;
    }

    toNetworkLayer(createGenericDataPacket(0, totalPackets, currentDataPktSize),
            "Aggregated Blank Packet with realistic size");
    //reset internal variables
    aggregatedPktSize=0;
    totalPackets++;
}

void HeterogeneousTraffic::refreshAppLayer(){
    if(isConstantDataPayload){
        //do nothing since everything is initialized
    } else {
        int range = maxDataPayload - minDataPayload + 1;
        currentDataPktSize = rand() % range + minDataPayload;
    }
}

void HeterogeneousTraffic::timerFiredCallback(int index)
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
            refreshAppLayer();
            setTimer(RESERVED_TIMER, sendInterval);
            break;
        }
    }
}

void HeterogeneousTraffic::startup()
{
    sampleInterval = (double)par("sampleInterval");
    sendInterval = (double)par("sendInterval");

    aggregatedValue = 0.0; //this denotes blank data value

    aggregatedPktSize = 0;
    waitingTimeForLowerLevelData = 0.0;
    lastSensedValue = (double)self; //initial data==Node ID, might be useful
    totalPackets = 0; // for bookkeeping

    if(!isSink){
        if(!dataSamplingDisabled) setTimer(REQUEST_SAMPLE, 0);
        int myOwnSendOffset = rand() % 50 + 3;
        setTimer(SEND_AGGREGATED_VALUE, myOwnSendOffset);
        setTimer(RESERVED_TIMER, myOwnSendOffset+2);
    }
    datarate = (8*constantDataPayload/sendInterval)*1000;

    if(!loggedInit){
        loggedInit = true;
    }
}


void HeterogeneousTraffic::initialize() {

    VirtualApplication::initialize();
    sendInterval = (double)par("sendInterval");
    dataSamplingDisabled = par("dataSamplingDisabled");
    isConstantDataPayload = par("isConstantDataPayload");

//    if(isConstantDataPayload) throw cRuntimeError("ZZZ");

    if(isConstantDataPayload) {
        if(hasPar("constantDataPayload")){
            constantDataPayload = (int)par("constantDataPayload");
        } else {
            applicationTrace() << "WARN: isConstantDataPayload but constantDataPayload not provided, defaulting to 1KB";
            constantDataPayload = 1000;
        }
        currentDataPktSize = constantDataPayload;
    } else {
        minDataPayload = par("minDataPayload");
        maxDataPayload = par("maxDataPayload");
        refreshAppLayer();
    }
    datarate = (8*constantDataPayload/sendInterval)*1000;
}

