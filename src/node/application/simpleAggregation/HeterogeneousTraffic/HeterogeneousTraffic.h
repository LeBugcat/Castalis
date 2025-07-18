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

#ifndef NODE_APPLICATION_SIMPLEAGGREGATION_HETEROGENEOUSTRAFFIC_HETEROGENEOUSTRAFFIC_H_
#define NODE_APPLICATION_SIMPLEAGGREGATION_HETEROGENEOUSTRAFFIC_HETEROGENEOUSTRAFFIC_H_

#include "../SimpleAggregation.h"

class HeterogeneousTraffic : public SimpleAggregation {

protected:

    bool isConstantDataPayload;

    int minDataPayload;
    int maxDataPayload;

    void initialize();

    void startup();
    void sendData();
    void refreshAppLayer();//for traffic-related logics

    void timerFiredCallback(int);
};

#endif /* NODE_APPLICATION_SIMPLEAGGREGATION_HETEROGENEOUSTRAFFIC_HETEROGENEOUSTRAFFIC_H_ */
