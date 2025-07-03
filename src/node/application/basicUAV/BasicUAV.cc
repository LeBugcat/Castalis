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
//#include "BasicUAV.h"
//#include "helpStructures/GlobalLocationService.h"
//
//void pointDistance2D(x1, y1, x2, y2){
//    return sqrt(pow(x1-x2, 2)+pow(y1-y2, 2));
//}
//
//void BasicUAV::startup() {
//    hoverDistance = par("hoverDistance");
//    scheduledToFly = false;
//    sinkId = -1;
//}
//
//void BasicUAV::timerFiredCallback(int index){
//    switch(index){
//    case INITIATE_COLLECTION:{
//        initiateCollection();
//    }
//    case FLY: {
//        if(nextDestId == nodeIdFlightPath.size()){
//            if(sinkId==-1) throw cRuntimeError("SINK NOT SET"); //sink not set
//            else flyTo(sinkId); //conclude collection round, move UAV home
//        } else if(nextDestId==0){
//            if(sinkId==-1) throw cRuntimeError("SINK NOT SET"); //sink not set
//            else {
//                Point here = GlobalLocationService::getLocation(sinkId);
//                Point there = GlobalLocationService::getLocation(nextDestId); //temp location data
//
//                flyTo(nodeIdFlightPath[nextDestId]); //move the UAV to first Location
//                double flightTime = pointDistance2D(
//                            here.x(), here.y(),
//                            there.x(), there.y()
//                        )/speed;
//                setTimer(INITIATE_COLLECTION,flightTime+10);
//                nextDestId++;
//            }
//        }
//        else{
//            Point here = GlobalLocationService::getLocation(nodeIdFlightPath[nextDestId-1]);
//            Point there = GlobalLocationService::getLocation(nodeIdFlightPath[nextDestId]); //temp location data
//
//            flyTo(nodeIdFlightPath[nextDestId]); //move the UAV to there
//            double flightTime = pointDistance2D(
//                        here.x(), here.y(),
//                        there.x(), there.y()
//                    )/speed;
//            setTimer(INITIATE_COLLECTION,flightTime+10);
//            nextDestId++;
//            }
//        }
//    }
//}
//
//void BasicUAV::schedule(vector<int> path){
//    nodeIdFlightPath = path;
//    scheduledToFly = true;
//    nextDestId = 0;
//}
//
//void BasicUAV::flyTo(int nodeId) {
//
//    application_trace() << "UAV Current Location: " << mobilityModule->getLocation().cell;
//    Point destLoc = GlobalLocationService::getLocation(nodeId);
//    mobilityModule->setLocation(destLoc.x(),destLoc.y(),hoverDistance);
//    mobilityModule->notifyWirelessChannel();
//    application_trace() << "UAV Current Location: " << mobilityModule->getLocation().cell;
//}
//void BasicUAV::initiateCollection(int nodeId) {
//
//    ApplicationPacket *appPkt = new ApplicationPacket(
//                "App generic packet",
//                NETWORK_UAV_COLLECT_REQUEST
//            );
//    toNetworkLayer(appPkt, nodeId);
//}
//void BasicUAV::fromNetworkLayer(cPacket *pkt){
//    ApplicationPacket *appPkt = dynamic_cast<ApplicationPacket>(pkt);
//    switch(appPkt->getKind( )){
//    case NETWORK_UAV_COLLECT_RESPONSE:{
//        //do something with data given by node below, rnow just add bytelength
//        //be careful, this RESPONSE is the entirety of other Nodes accumulated package
//        int dataLength = appPkt.getByteLength();
//        application_trace() << "UAV " << self << " received packet with length "<< dataLength;
//        setTimer(FLY, 0);
//        }
//    }
//}
