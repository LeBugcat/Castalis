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

#include "../mac802154TSCHlite/MAC802154TSCHLite.h"

//Define_Module(MAC802154TSCHLite);

int    MAC802154TSCHLite::frameSize             =0;
int    MAC802154TSCHLite::CAPSize               =0;
int    MAC802154TSCHLite::CFPSize               =0; // inferred, not passed
double MAC802154TSCHLite::slotSizeInMiliSec     =0; // this is for timers
double MAC802154TSCHLite::slotSizeInSec         =0; // this is for timers
double MAC802154TSCHLite::stateTransTimeGuardMs =0.5; // for state transition and RX ready before
double MAC802154TSCHLite::TSCHRoundLength       =0;
int    MAC802154TSCHLite::roundNumber           =0;
double MAC802154TSCHLite::TSCHRoundPadding      =7;
double MAC802154TSCHLite::CAPpadding            =1;
double MAC802154TSCHLite::CFPpadding            =1;
double MAC802154TSCHLite::coordinatorPadding    =0.5;
// TX this is milisec,
bool MAC802154TSCHLite::globalVarsInitialized =false;
int  MAC802154TSCHLite::channelCount          =16;
bool MAC802154TSCHLite::ACKenabled            =false;
int  MAC802154TSCHLite::currentCAPslot        =0;
int  MAC802154TSCHLite::currentCFPslot        =0;
MacState MAC802154TSCHLite::currentMACState   =CAP;


Define_Module(MAC802154TSCHLite);

void MAC802154TSCHLite::specificInitialize(){

    //TODO: the global variables should be
    // initialized in the first stage
    // specifically using the PAN coordinator
    // (aka network Sink)
    this->isCoordinator = par("isCoordinator");

    if(!globalVarsInitialized){
        if(this->isCoordinator){
            frameSize       = par("frameSize");
            CAPSize         = par("CAPSize");
            CFPSize         = frameSize - CAPSize;
            TSCHRoundLength = par("TSCHRoundLength");

            double datarate   = radioModule->datarate; //bit per sec
            int maxPacketSize = macMaxFrameSize+macFrameOverhead+radioModule->PhyFrameOverhead;
            int ACKPacketSize = macFrameOverhead+radioModule->PhyFrameOverhead;

            //NOTE: ACKPacket is a control pkt with 0 data byte.
            ACKenabled = par("ACKenabled");


            if(!ACKenabled)
                slotSizeInSec = (maxPacketSize)*8/(datarate) + stateTransTimeGuardMs/1000;
            else //account for 2 times state changes during a single slot
                slotSizeInSec = (maxPacketSize+ACKPacketSize)*8/(datarate) + 2*stateTransTimeGuardMs/1000;

            slotSizeInMiliSec = slotSizeInSec*1000;
            frameSize = par("frameSize");
            channelCount = par("channelCount");
            globalVarsInitialized = true;
            roundNumber = 0;
//            setTimer(START_TSCH_ROUND, slotSizeInSec*frameSize); //loop the round/frame, account for CFP slots guard
        }
        this->routingModule = check_and_cast<ConvergecastRouting*>(getParentModule()->getSubmodule("Routing"));

        if(CFPSize<=0) throw cRuntimeError("logic failure with negative CFP");

        //datarate: for CC2420 is 250kbps, for CC1000 is 19.2kbps (b=bit)
        // datarate is physical data transfer rate, not data generation rate

        //base on this rate, we deduce the slotSizeInMiliSec
        //a slot is a bit bigger than the needed time for the
        //(uniform accross all nodes) data-packet to transmit
        // this packetsize is calculated at the Radio layer (the physical packet)
        //added timeguards to each slots
        //the packet max size is determined here
    } else {}
}

void MAC802154TSCHLite::startup(){
    VirtualMac::startup();
    currentChannel = 0;

    schedule = new CFPSchedule(frameSize, CAPSize);
    currentRadioState = SLEEPING;
    if(TSCHRoundLength<=TSCHRoundPadding+slotSizeInSec*frameSize) throw cRuntimeError("Invalid frame length, shorter than TSCH round length");

    //loop the round/frame, account for CFP slots guard
    if(this->isCoordinator){
        setTimer(START_TSCH_ROUND,0+TSCHRoundPadding+TSCHRoundLength, "TSCH ROUND START");
    }
    else setTimer(START_TSCH_ROUND,coordinatorPadding+TSCHRoundPadding+TSCHRoundLength, "TSCH ROUND START"); //immediate round start
}

void MAC802154TSCHLite::toMacLayerDirect(cPacket * pkt, int dstId){
    MAC802154LitePacket* tschPkt = check_and_cast<MAC802154LitePacket*>(pkt);
    tschPkt->setTSCHPacketType(CONTROL_TSCH_PACKET);
    VirtualMac::toMacLayerDirect(tschPkt, dstId);
}

void MAC802154TSCHLite::setUpOnCurrentCFPSlot(){

    if( currentCFPslot>=(frameSize-CAPSize) or currentCFPslot<0 ) {
        //TODO: formal log here, warning
        //guard for the last slot
        return;
    }

    //setup first slot
    if( currentCFPslot==0 ){
        hopChannel(schedule->operatingChannel[currentCFPslot]);
        return;
    }
    //switch for other slots
    //setups: switch channel ONLY if needed
    if(schedule->operatingChannel[currentCFPslot-1]!=schedule->operatingChannel[currentCFPslot])
        hopChannel(schedule->operatingChannel[currentCFPslot]);

    //switch from TX to SLEEP if the node has nothing to send
    if(TXBuffer.empty() and (schedule->slotActions[currentCFPslot]==ATTEMPT_TX)){
        schedule->slotActions[currentCFPslot] = SLEEP_TSCH;
        schedule->operatingChannel[currentCFPslot] = 0;
    } else if (
        !TXBuffer.empty() and (schedule->slotActions[currentCFPslot]==ATTEMPT_TX)
    ){
        //dispatch to Radio one packet, this is done before any state switch
        toRadioLayer(TXBuffer.front());
        TXBuffer.pop();
    }
    switchRadioState(schedule->slotActions[currentCFPslot]);
};

void MAC802154TSCHLite::setUpOnCurrentCAPSlot(){
    if(currentMACState==CFP) throw cRuntimeError("Overlapping CAP and CFP, timing failure. Consider extending the CAP size!");
    if(this->isCoordinator){
        currentCAPslot+=1;
    }
    exactOnceCAPactionFlag = false;
};

void MAC802154TSCHLite::processCurrentCAPSlot(){
    throw cRuntimeError("CAP by-slot processing is not implemented, use {} if necessary");
};

void MAC802154TSCHLite::setTimerTSCHRoundLooped(double padding){

    setTimer(START_CAP, TSCHRoundPadding
                        +padding, "TSCH CAP START");

    setTimer(START_CFP, TSCHRoundPadding
                        +CAPpadding+slotSizeInSec*CAPSize
                        +CFPpadding
                        +padding, "TSCH CFP START");
    setTimer(
        START_TSCH_ROUND,
        TSCHRoundLength
        +TSCHRoundPadding
        +CAPpadding+CFPpadding
        +padding
    );
}

void MAC802154TSCHLite::processAndLoopCAPSlot(){

    setUpOnCurrentCAPSlot();
    processCurrentCAPSlot();

    //loop to new slot
    if(currentCAPslot<CAPSize-1) //guarded for last CAP slot, discarding this
        setTimer(START_CAP_SLOT, slotSizeInSec,"CAPprocessing");

};

void MAC802154TSCHLite::timerFiredCallback(int index_){

    switch(index_) {
        case START_TSCH_ROUND: {
            if(isCoordinator){
                macTrace() << "START TSCH ROUND " << roundNumber;
                switchMacState(CAP);
                setTimerTSCHRoundLooped(0);
            }
            setTimerTSCHRoundLooped(coordinatorPadding);

            //loop the round, not accounting for CFP slots guard

            //TODONE: immediate beacon here,
            //beacon is sent directly and not timely handled
            break;
        }
        case START_CAP: {
            //change state at MAC
            switchMacState(CAP);
            preCAPsetup();
            setTimer(START_CAP_SLOT, CAPpadding,"CAP slot zero");
            break;
        }
        case START_CAP_SLOT: {
            //change state at MAC
            processAndLoopCAPSlot();
            break;
        }
        case START_CFP: {
            preCFPsetup();
            setTimer(START_CFP_SLOT, 0); //start the CFP with the first slot
            break;
        }
        case START_CFP_SLOT: { //at here we loop and all
            if(currentCFPslot<CFPSize){
                setUpOnCurrentCFPSlot();
                setTimer(START_CFP_SLOT, slotSizeInSec);
                setTimer(POST_STATE_TRANS, stateTransTimeGuardMs/1000);
            }
            break;
        }
        case POST_STATE_TRANS: {
            //this is performed once every slots
            //now the node will have enough time to do whatever
            performCurrentCFPAction();
            if(ACKenabled)
                setTimer(END_CFP_SLOT, slotSizeInSec-2*stateTransTimeGuardMs/1000);
            else //account for 2 times state trans
                setTimer(END_CFP_SLOT, slotSizeInSec-stateTransTimeGuardMs/1000);
            break;
        } //have to do this to preserve current slot states (if has)
        case END_CFP_SLOT: {
            postCFPSlotSetUp();
        }
    }
}

//void MAC802154TSCHLite::macSpecificPreCAPSetup(){};
//void MAC802154TSCHLite::macSpecificPreCFPSetup(){};

void MAC802154TSCHLite::preCAPsetup(){

    currentChannel = 0;
    currentPacket = NULL;

    if(this->isCoordinator){
        currentCAPslot = 0;
    }

    schedule = new CFPSchedule(frameSize, CAPSize);
    currentRadioState = SLEEPING;

//    this->knownActionByTimeslot = new vector<vector<ActionRecord>>;
    this->knownActionByTimeslot.resize(CFPSize);

    macSpecificPreCAPSetup();
}

void MAC802154TSCHLite::updateSchedule(
        int sourceId, int timeslot, int opChannel, Action_type action
){ //this mostly work with CLAIM signals, which are ATTEMPT_TX-type action records
    if(this->routingModule->config.nextHop[sourceId]==self){
        if(action == ATTEMPT_TX){
            macTrace() << "Collected child claim, set up open RX at slot " << timeslot;
            this->schedule->slotActions[timeslot]        = OPEN_RX;
            this->schedule->operatingChannel[timeslot]   = opChannel;
            this->knownActionByTimeslot[timeslot].push_back({action, sourceId, opChannel});
        }
        else{
            throw cRuntimeError("Signal from child but not TX, unknown logic");
        }
    } else {
        this->knownActionByTimeslot[timeslot].push_back({action, sourceId, opChannel});
    }
};

void MAC802154TSCHLite::postCFPSlotSetUp(){
    if(isCoordinator){
        macTrace() << "Switching to CFP slot " << currentCFPslot+1;
        currentCFPslot++;
    }
}

//this doesnt care about state, the state switch
//must happen before this function run
//this access the schedule object and do things within the current CFP slot
void MAC802154TSCHLite::performCurrentCFPAction(){

    if(currentCFPslot<0 or currentCFPslot>=(frameSize-CAPSize)){
        switchRadioState(SLEEP_TSCH);
//        macTrace() << "-ACTIONLOG::SLEEP_CFP::SLOT_" << currentCFPslot <<"-";
        return; //early abort fot last slot
    }

    if(currentMACState != CFP) throw cRuntimeError("Action performed at out of CFP");
    this->hopChannel(this->schedule->operatingChannel[currentCFPslot]);

    //TODO: remodel schedule to better define CFP slots, there is no CAP slots
    switch(schedule->slotActions[currentCFPslot]){
        case OPEN_RX: {
            switchRadioState(OPEN_RX);
            macTrace() << "-ACTIONLOG::RX_CFP::SLOT_" << currentCFPslot <<"-";
            break;
        }
        case SLEEP_TSCH: {
            switchRadioState(SLEEP_TSCH);
//            macTrace() << "-ACTIONLOG::SLEEP_CFP::SLOT_" << currentCFPslot <<"-";
            break;
        }
        case ATTEMPT_TX: {
            switchRadioState(ATTEMPT_TX);
            macTrace() << "-ACTIONLOG::TX_CFP::SLOT_" << currentCFPslot <<"-";
            break;
        }
    }
};
