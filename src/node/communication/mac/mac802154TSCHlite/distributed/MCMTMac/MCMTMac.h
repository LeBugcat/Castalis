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

#ifndef NODE_COMMUNICATION_MAC_MAC802154TSCHLITE_DISTRIBUTED_MCMTMAC_MCMTMAC_H_
#define NODE_COMMUNICATION_MAC_MAC802154TSCHLITE_DISTRIBUTED_MCMTMAC_MCMTMAC_H_

#include "node/communication/routing/convergecast/ConvergecastRouting.h"
#include "node/communication/mac/mac802154TSCHlite/distributed/MCMTMac/MCMTMacPacket_m.h"
#include "node/communication/mac/mac802154TSCHlite/MAC802154TSCHLite.h"

#include <cmath>

enum MCMTNb_type{ //higher index overrides the lower ones, we keep only one in each records
    CHILD        = 1,
    SAME_CLUSTER = 2,
    SAME_PARENT  = 3,
    DIFF_CLUSTER = 4
};

struct MCMTNeighborRecord{
    MCMTNb_type type;
    int         id;
    int         CHId;
    int         parentId;
    int         transmissionCount;
    int         geocolorIndex;
};

//struct Interferer{
//    int nodeId;
//    int parentId;
//    int CHId;
//    int geocolor;
//    int transmissionsCount;
//};

class MCMTMac: public MAC802154TSCHLite {

protected:

    bool isBD;
    bool isCH;

    MCMTMACGeoColor_type       geocolorIndex;
    int                        currentTransmissionCount;
    vector<MCMTNeighborRecord> nbRecords;      //saves ALL onehop records
    vector<MCMTNeighborRecord> interferersSet; //saves only Interferers records, these can be 2-3 hops neighbors
    vector<MCMTNeighborRecord> localQueue;     //Local Queue

    void handleTSCHControlPacket(cPacket* pkt);
    void macSpecificPreCAPSetup();
    void CAPprocessing();
    void macSpecificPreCFPSetup();

    void processCurrentCAPSlot();

    void handleHELLOPacket(MCMTMACPacket *mcmtPkt);
    void handleASSIGNPacket(MCMTMACPacket *mcmtPkt);
    void handleINFOPacket(MCMTMACPacket *mcmtPkt);
    void handleCLAIMPacket(MCMTMACPacket *mcmtPkt, bool isSingleClaim);
    void handleDONEPacket(MCMTMACPacket *mcmtPkt);

    void doPairingAndRelayPairInfo();
    void relayInfoPkt(MCMTMACPacket*, MCMTNeighborRecord, MCMTMACPacket_type, int);
    void sendPairsInfo(MCMTNeighborRecord nb1, MCMTNeighborRecord nb2);
    void buildLQ();

//    void assignBDtoChildren();
    void continueChainAssign();

    void startPreSchedulingProcess();

    void checkAndSendCLAIMPacket();
    // we call this to initiate the pre-schedule process
    // this will do the pairings, send INFO and ASSIGN packets
    // this is shoved in the handleHELLOPacket,
    // runs when all neighbors has sent their hello packets

    int getParentId()
    {
        if(this->routingModule->config.isCH(self)) {
            throw cRuntimeError("CH must never call this!?");
        } else return this->routingModule->config.nextHop[self];
    };

    int getCHId()
    {
        if(this->routingModule->config.isCH(self)) {
            this->isCH = true;
            return self;
        } else return this->routingModule->config.clus_id[self];
    };
    //calculate the geocolor right away
    void specificInitialize(int stage){

        MAC802154TSCHLite::specificInitialize();

        //get routing vars
        this->isCH = false;
        this->routingModule = check_and_cast<ConvergecastRouting*>(getParentModule()->getSubmodule("Routing"));

        //decide geocolor right here
        double xCoordinate = getParentModule()->getParentModule()->par("xCoor");
        double yCoordinate = getParentModule()->getParentModule()->par("yCoor");

        double geocolorOffsetX = par("geocolorOffsetX");
        double geocolorOffsetY = par("geocolorOffsetY");

        double nbRange            = this->routingModule->getNeighborRange();
        int    geocolorTileFactor = par("getcolorSizeFactor");
        double geocolorTileSize   = nbRange * geocolorTileFactor;

        int xTilePassed = floor((xCoordinate-geocolorOffsetX)/geocolorTileSize);
        int yTilePassed = floor((yCoordinate-geocolorOffsetX)/geocolorTileSize);

        //safeguard
        if(xTilePassed < 0 or xTilePassed<0) throw cRuntimeError("Invalid coordinates or offsets");

        double tileStartX = geocolorOffsetX + geocolorTileSize*xTilePassed;
        double tileEndX   = geocolorOffsetX + geocolorTileSize*(xTilePassed+1);
        double tileMidX   = (tileStartX+tileEndX)/2;

        double tileStartY = geocolorOffsetY + geocolorTileSize*yTilePassed;
        double tileEndY   = geocolorOffsetY + geocolorTileSize*(yTilePassed+1);
        double tileMidY   = (tileStartY+tileEndY)/2;

        if     (xCoordinate <= tileMidX and yCoordinate <= tileMidY) this->geocolorIndex = GREEN;
        else if(xCoordinate >  tileMidX and yCoordinate <= tileMidY) this->geocolorIndex = RED;
        else if(xCoordinate <= tileMidX and yCoordinate >  tileMidY) this->geocolorIndex = YELLOW;
        else if(xCoordinate >  tileMidX and yCoordinate >  tileMidY) this->geocolorIndex = PURPLE;
        else throw cRuntimeError("Invalid node coordinates");
    };

    void continueChainAssign(cPacket* pkt);
};

#endif /* NODE_COMMUNICATION_MAC_MAC802154TSCHLITE_DISTRIBUTED_MCMTMAC_MCMTMAC_H_ */
