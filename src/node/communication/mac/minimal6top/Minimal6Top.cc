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

#include "node/communication/mac/minimal6top/Minimal6Top.h"

int Minimal6Top::timeslotDuration = 10; //in ms, can not be overriden
int Minimal6Top::timeslotGuard    = 3; //in ms, can not be overriden

void Minimal6Top::timerFiredCallback(int timerId){

    switch (timerId) {
        case SLOT_EVENT: {
            SlotType currentSlot = slotframe[currentSlotIndex];

            switch (currentSlot) {
                case SLOT_TX:
                    handleTxSlot();
                    break;
                case SLOT_RX:
                    handleRxSlot();
                    break;
                case SLOT_IDLE:
                    handleIdleSlot();
                    break;
            }

            // Daisy chain to next slot
            currentSlotIndex = (currentSlotIndex + 1) % slotframe.size();
            setTimer(SLOT_EVENT, timeslotDuration);
            break;
        }

        case RADIO_TX_ON:
            toRadioLayer(createRadioCommand(SET_STATE, TX));
            break;

        case RADIO_RX_ON:
            toRadioLayer(createRadioCommand(SET_STATE, RX));
            break;

        case RADIO_TX_OFF or RADIO_RX_OFF:
            toRadioLayer(createRadioCommand(SET_STATE, SLEEP));
            break;
        // Add more control logic as needed
    }
}
