////
//// This program is free software: you can redistribute it and/or modify
//// it under the terms of the GNU Lesser General Public License as published by
//// the Free Software Foundation, either version 3 of the License, or
//// (at your option) any later version.
////
//// This program is distributed in the hope that it will be useful,
//// but WITHOUT ANY WARRANTY; without even the implied warranty of
//// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//// GNU Lesser General Public License for more details.
////
//// You should have received a copy of the GNU Lesser General Public License
//// along with this program.  If not, see http://www.gnu.org/licenses/.
////
//
//#ifndef NODE_APPLICATION_BASICUAV_BASICUAV_H_
//#define NODE_APPLICATION_BASICUAV_BASICUAV_H_
//
//#include "node/application/VirtualApplication.h"
//#include "node/communication/routing/cluster/gambac/GAMBAC.h"
//#include <map>
//#include <vector>
//
//
///*
// * THE IDEA OF THIS CLASS IS THAT OF AN IDEAL UAV
// * IN EACH FLIGHT, THE BATTERY IS REPLENISHED INSTANTLY AND EACH PATH IS GUARANTEED TO NOT FAILING MIDWAY
// * THEREFORE NO BATTERY MECHANICS OR PARAMS ARE !NOT! IMPLEMENTED
// * FUTURE RELEASED WILL IMPROVE THIS TO MAKE IT A TRULY AN INDEPENDENT-[OF-ROUTING-LAYER MODULE
//*/
//
//enum UAVTimers{
//    INITIATE_COLLECTION = 1,
//    FLY = 2
//};
//
//class BasicUAV: public VirtualApplication {
// private:
//    double hoverDistance;
//    double speed;
//    bool scheduledToFly;
//    int nextDestId;
//    int sinkId;
//    vector<int> nodeIdFlightPath;
//
// protected:
//    void setSink(int Id){sinkId=Id;}
//
//    void startup();
//    void timerFiredCallback(int){};
//
//    void flyTo(int);
//    void initiateCollection(double);
//
//public:
//    void schedule(vector<int>);
//
////    void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
////    void fromNetworkLayer(ApplicationPacket *);
//};
//
//#endif /* NODE_APPLICATION_BASICUAV_BASICUAV_H_ */
