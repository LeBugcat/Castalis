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

#include "RoundTimerService.h"

int RoundTimerService::roundLengthSec = 10;
int RoundTimerService::firstRoundOffset = 1;
int RoundTimerService::actionOffset = 1;
int RoundTimerService::actionDuration = 2;
int RoundTimerService::roundNumber;

bool RoundTimerService::timerInitialized = false;
//We mainly operate on the unit of seconds, floats apply

void RoundTimerService::initAndStartTimer(int roundLengthS, int actionOffsetS, int firstroundOffsetS){

    if(roundLengthS<=0 or actionOffsetS<0 or firstroundOffsetS<0)
        throw cRuntimeError("Invalid negative variables");
    if(roundLengthS<=actionOffsetS)
        throw cRuntimeError("Invalid variables: Round Length <= Action Offset");

    if(!timerInitialized) {
        roundLengthSec = roundLengthS;
        timerInitialized = true;
        roundNumber=0;
    }
    //We run the round immediately after this.

    //first round
    setTimer(START_ROUND_TIMER, firstRoundOffset);
    setTimer(ACTION_START_TIMER,firstRoundOffset+actionOffset);
    setTimer(ACTION_END_TIMER,firstRoundOffset+actionOffset+actionDuration);
}

void RoundTimerService::timerFiredCallback(int index){
    switch(index){
    case START_ROUND_TIMER: {
        roundNumber++;
        startRoundCallback();
        //next round
        setTimer(START_ROUND_TIMER, roundLengthSec);
        break;
    }
    case END_ROUND_TIMER: {
        endRoundCallback();
        //next round
        setTimer(END_ROUND_TIMER, roundLengthSec);
        break;
    }
    case ACTION_START_TIMER: {
        actionStartCallback();
        setTimer(ACTION_START_TIMER, roundLengthSec);
        break;
    }
    case ACTION_END_TIMER: {
        actionEndCallback();
        setTimer(ACTION_START_TIMER, roundLengthSec);
        break;
    }
    }
}

//void RoundTimerService::startRoundCallback(){};
//void RoundTimerService::endRoundCallback(){};
//void RoundTimerService::actionStartCallback(){}; //provide the singular action service
//void RoundTimerService::actionEndCallback(){}; //provide the singular action service
