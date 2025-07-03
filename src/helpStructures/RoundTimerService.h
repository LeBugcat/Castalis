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

///////////////////////////////////////////////////////////////////////////////

//!!!!!!!!!!!! ATTENTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// THIS ROUND TIMER IS CURRENTLY ONLY USED IN THE ROUTING LAYER
// DONT ASSUME THAT IT CAN BE USED FOR ANY MAC PROTOCOLS
// AS THE STATIC VARIABLES MAY MISMATCH
///////////////////////////////////////////////////////////////////////////////

//CURRENTLY, THE MODEL SUPPORT ONE SINGULAR ACTION
//(like running a centralized Routing Algorithm)
// 0=============|START|==========|ACTION-START|=======|ACTION-END|===|END|====>
//        ^                  ^                     ^
// first round offset    action offset      action duration

#ifndef HELPSTRUCTURES_ROUNDTIMERSERVICE_H_
#define HELPSTRUCTURES_ROUNDTIMERSERVICE_H_

#include "./TimerService.h"
#include "./CastaliaModule.h"

enum RoundTimers_type{
    START_ROUND_TIMER=0,
    END_ROUND_TIMER=1,
    ACTION_START_TIMER=2, //COMPARED TO ROUND_START
    ACTION_END_TIMER=3,
};

class RoundTimerService: public TimerService, CastaliaModule {

public:
    static int roundNumber; //for logging
    static bool timerInitialized; //bookkeep

protected:
    static int roundLengthSec;
    static int firstRoundOffset;// offset the first round, default=0;

    static int actionOffset;// to do something after round START, default=1(sec);
    static int actionDuration;// when the action Ends, default=3(sec),
                              // for when the algo doesnt care ab runtime,
                              //this is purely just to set a hook;
    int localNodeId; //this timer will be inherited by the Routing Layer
    int localSinkId;
    int localRoundNumber;
    bool isSink;

    void initAndStartTimer(int,int,int);
    void timerFiredCallback(int index);

    virtual void startRoundCallback()=0;
    virtual void endRoundCallback()=0;
    virtual void actionStartCallback()=0; //provide the singular action service
    virtual void actionEndCallback()=0; //provide the singular action service
};

#endif /* HELPSTRUCTURES_ROUNDTIMERSERVICE_H_ */
