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

#include "../../../mac802154TSCHlite/distributed/MCMTMac/MCMTMac.h"

#include "../../../mac802154TSCHlite/distributed/MCMTMac/MCMTMacPacket_m.h"

Define_Module(MCMTMac);

void MCMTMac::handleTSCHControlPacket(cPacket* pkt){

    MCMTMACPacket* mcmtPkt = check_and_cast<MCMTMACPacket*>(pkt);
    int mcmtPkt_type = mcmtPkt->getMCMTMACPacketType();

    //After the call RETURN does not delete the pkt but BREAK does.

    if(!this->routingModule->routingTreeReady){
        macTrace() << "Net topo not created, stray control packet";
        return;
    }

    switch(mcmtPkt_type){
    case HELLO: {
        handleHELLOPacket(mcmtPkt);
        break;
    }
    case ASSIGN: {
        //chain assign
        handleASSIGNPacket(mcmtPkt);
        break;
    }
    case INFO: {
        handleINFOPacket(mcmtPkt);
        break;
    }
    case BD_CLAIM: {
        //chain assign
        handleCLAIMPacket(mcmtPkt, true);
        break;
    }
    case BATCH_CLAIM: {
        //chain assign
        handleCLAIMPacket(mcmtPkt, false);
        break;
    }
    case DONE: {
        handleDONEPacket(mcmtPkt);
        break;
    }
    }
}

void MCMTMac::continueChainAssign(){

    vector<int> childrenIds = this->routingModule->config.getChildren(self);

    for(int childId: childrenIds){
        MCMTMACPacket* mcmtPkt = new MCMTMACPacket();
        mcmtPkt->setSource(self);
        mcmtPkt->setDestination(childId);
        mcmtPkt->setCHId(this->routingModule->config.clus_id[self]);
        mcmtPkt->setMCMTMACPacketType(ASSIGN);
        mcmtPkt->setIsBlankChainAssign(true);
        mcmtPkt->setSourceGeoColor(this->geocolorIndex); //TODO: change

        this->toMacLayerDirect(mcmtPkt, childId);
    }
}

void MCMTMac::handleHELLOPacket(MCMTMACPacket*pkt)
{
//    MCMTNeighborRecord newNbRecord = new MCMTNeighborRecord<int, int, int, int, int>();

    int id                = pkt->getSource();
    int parentId          = pkt->getParentId();
    int CHId              = pkt->getCHId();
    int transmissionCount = pkt->getTransmissionCount();
    int geocolorIndex     = pkt->getSourceGeoColor();
    MCMTNb_type type;

    if(this->routingModule->config.nextHop[id] == self)
        type = CHILD;
    else if (this->routingModule->config.nextHop[id] == this->routingModule->config.nextHop[self])
        type = SAME_PARENT;
    else if (this->routingModule->config.clus_id[id] == this->routingModule->config.clus_id[self])
        type = SAME_CLUSTER;
    else if (this->routingModule->config.clus_id[id] != this->routingModule->config.clus_id[self])
    {
        type = DIFF_CLUSTER;
        if(!this->isBD) this->isBD = true;
        continueChainAssign();
    }
    else throw cRuntimeError("No matching case for this HELLO packet source");

    nbRecords.push_back({
            type,
            id,
            CHId,
            parentId,
            transmissionCount,
            geocolorIndex
        });
}

void MCMTMac::handleASSIGNPacket(MCMTMACPacket *pkt)
{
    if(this->isCH) throw cRuntimeError("CH must never get this ASSIGN message");
    else if(this->getParentId() == pkt->getSource()){
        this->isBD = true;

        // Not a chain assignment, must throw this into the pre-LQ
        if(!pkt->getIsBlankChainAssign()){
            // this is the info of one competing node
            int cmpt_nodeId             = pkt->getEncapsulatedNodeId();
            int cmpt_parentId           = pkt->getEncapsulatedParentId();
            int cmpt_chId               = pkt->getEncapsulatedCHId();
            int cmpt_transmissionsCount = pkt->getEncapsulatedTransmissionCount();
            int cmpt_geocolor           = pkt->getEncapsulatedGeocolor();
            MCMTNb_type type;
            if(cmpt_chId == routingModule->config.clus_id[self]){
                if(cmpt_parentId == self){
                    type = CHILD;
                } else if(cmpt_parentId == routingModule->config.nextHop[self]){
                    type = SAME_PARENT;
                } else type = SAME_CLUSTER;
            } else {
                type = DIFF_CLUSTER;
            }
            MCMTNeighborRecord new_itfr = {
                    type, cmpt_nodeId, cmpt_chId, cmpt_parentId,
                    cmpt_transmissionsCount, cmpt_geocolor
                };
            interferersSet.push_back(new_itfr );
        }

        //chain BD assignment to children, regardless
        continueChainAssign();
    }
    else throw cRuntimeError("Wrong logic in code, got an ASSIGN message not from parent");
}

void MCMTMac::handleINFOPacket(MCMTMACPacket* infoPkt)
{
    // INFO packets are only received when
    int newIfr_nodeId             = infoPkt->getEncapsulatedNodeId();
    int newIfr_parentId           = infoPkt->getEncapsulatedParentId();
    int newIfr_chId               = infoPkt->getEncapsulatedCHId();
    int newIfr_transmissionsCount = infoPkt->getEncapsulatedTransmissionCount();
    int newIfr_geocolor           = infoPkt->getEncapsulatedGeocolor();
    MCMTNb_type type;

    //INFO PACKETS CAN ONLY BE COMPETING NODES
    if(newIfr_chId == routingModule->config.clus_id[self]){
        if(newIfr_parentId == self){
            type = CHILD;
        } else if(newIfr_parentId == routingModule->config.nextHop[self]){
            type = SAME_PARENT;
        } else type = SAME_CLUSTER;
    } else {
        type = DIFF_CLUSTER;
    }

    MCMTNeighborRecord new_itfr = {
            type, newIfr_nodeId, newIfr_chId, newIfr_parentId, newIfr_transmissionsCount, newIfr_geocolor
        };
    interferersSet.push_back(new_itfr );
}

void MCMTMac::checkAndSendCLAIMPacket(){
//    throw cRuntimeError("Not implemented");
}

//void MCMTMac::checkAndSendCLAIMPacket(){
//    throw cRuntimeError("Not implemented");
//}

void MCMTMac::handleCLAIMPacket(MCMTMACPacket* infoPkt, bool isSingleClaim)
{
    //TODO: This needs a check every time called
    //      so that node can be sure if it is allow to claim now

    // source is single for either batch and single
    int sourceId  = infoPkt->getSource();

    //saves the claim
    if(isSingleClaim){
        int timeslot  = infoPkt->getSingleClaimTimeslot();
        int opChannel = infoPkt->getSingleChannelId();

        updateSchedule(sourceId, timeslot, opChannel, ATTEMPT_TX);

        //that handle this kinda logic for us, since
        //this is the op of the lower abstraction, and schedule belongs there

        //go to sleep on this
        this->schedule->slotActions[timeslot]      = SLEEP_TSCH;
        this->schedule->operatingChannel[timeslot] = opChannel;

        checkAndSendCLAIMPacket();
    }
    else{
        int claimSize = infoPkt->getBatchClaimChannelIdsArraySize();
        for(int i=0;i<claimSize;i++){

            int timeslot  = infoPkt->getBatchClaimTimeslots(i);
            int opChannel = infoPkt->getBatchClaimChannelIds(i);

            updateSchedule(sourceId, timeslot, opChannel, ATTEMPT_TX);
        }
    }
}

void MCMTMac::handleDONEPacket(MCMTMACPacket* infoPkt)
{
    // INFO packets are only received when
    throw cRuntimeError("Under-Constructions!");
}


//void MCMTMac::assignBDtoChildren(){
//    vector<int> neighborIds = this->routingModule->getNeighborList();
//
//    for(int childId: neighborIds){
//        MCMTMACPacket* mcmtPkt = new MCMTMACPacket();
//        mcmtPkt->setSource(self);
//        mcmtPkt->setDestination(childId);
//        mcmtPkt->setCHId(this->routingModule->config.clus_id[self]);
//        mcmtPkt->setMCMTMACPacketType(ASSIGN);
//        mcmtPkt->setIsBlankChainAssign(false);
//        mcmtPkt->setSourceGeoColor(this->geocolorIndex); //TODO: change
//
//        this->toMacLayerDirect(mcmtPkt, childId);
//    }
//}

void MCMTMac::macSpecificPreCFPSetup(){
    //check if is BD, it it is then
    //build IFS and LQ
    if(this->isBD){
        macTrace()<<"NODE ANNOUNCE IS BD " <<self;
//        buildLQ();
        checkAndSendCLAIMPacket();
    }
// Claim time !
   // For BDs:
   //     Create the IFS
   // For Non-BDs:
   //     Create the LQ
   // If is Leaf:
   //     CLAIM
   // Else:
   //     handleClaim (automated, no need code)
}

void MCMTMac::relayInfoPkt(
        MCMTMACPacket* injectedPkt, MCMTNeighborRecord data, MCMTMACPacket_type pktType, int dstId
    ){
    //initialize a packet
    injectedPkt->setSource(self);
    injectedPkt->setDestination(dstId);
    injectedPkt->setCHId(this->routingModule->config.clus_id[self]);
    injectedPkt->setMCMTMACPacketType(pktType);
    injectedPkt->setSourceGeoColor(this->geocolorIndex); //TODO: change

    injectedPkt->setHasEncapsulatedPacket(true);
    injectedPkt->setEncapsulatedNodeId(data.id);
    injectedPkt->setEncapsulatedParentId(data.parentId);
    injectedPkt->setEncapsulatedCHId(data.CHId);
    injectedPkt->setEncapsulatedGeocolor(data.geocolorIndex);
    injectedPkt->setEncapsulatedTransmissionCount(data.transmissionCount);

    if(pktType==ASSIGN){
        injectedPkt->setIsBlankChainAssign(false);
    }
    toMacLayerDirect(injectedPkt, dstId);
}

void MCMTMac::sendPairsInfo(MCMTNeighborRecord nb1data, MCMTNeighborRecord nb2data){
    if(nb1data.type==CHILD){
        MCMTMACPacket* INFOPkt   = new MCMTMACPacket();
        MCMTMACPacket* ASSIGNPkt = new MCMTMACPacket();
        relayInfoPkt(INFOPkt, nb1data, INFO, nb2data.id);
        relayInfoPkt(ASSIGNPkt, nb2data, ASSIGN, nb1data.id);
    }else if(nb2data.type==CHILD){
        MCMTMACPacket* INFOPkt   = new MCMTMACPacket();
        MCMTMACPacket* ASSIGNPkt = new MCMTMACPacket();
        relayInfoPkt(INFOPkt, nb2data, INFO, nb1data.id);
        relayInfoPkt(ASSIGNPkt, nb1data, ASSIGN, nb2data.id);
    }else throw cRuntimeError("False input data of pairs");
};

void MCMTMac::doPairingAndRelayPairInfo(){
    vector<int> childrenList = this->routingModule->config.getChildren_(self);
    vector<pair<int, int>> pairedPairs;
    if(childrenList.size()==0){
        macTrace() << "Leaf BD node announcement at "<< self;
    }
    else{
        for(MCMTNeighborRecord nb1data: nbRecords){
            for(MCMTNeighborRecord nb2data: nbRecords){
                if(nb1data.geocolorIndex!=nb2data.geocolorIndex) continue;
                else if((nb1data.type==CHILD && nb2data.type!=CHILD)
                  || (nb2data.type==CHILD && nb1data.type!=CHILD)){

                    pair<int, int> pairing1(nb1data.id, nb2data.id);
                    pair<int, int> pairing2(nb2data.id, nb1data.id);
                    auto it1 = find(pairedPairs.begin(), pairedPairs.end(), pairing1);
                    auto it2 = find(pairedPairs.begin(), pairedPairs.end(), pairing2);

                    if(it1!=pairedPairs.end() || it2!=pairedPairs.end()) continue; //found
                    else{
                        if(nb1data.geocolorIndex!=nb2data.geocolorIndex) throw cRuntimeError("Algo logic failure, trying to pair different geocolored nodes");
                        macTrace() << "pairing " << nb1data.type << '#' << nb1data.id << '#' << nb1data.geocolorIndex << " with " << nb2data.type << nb2data.id;
                        pairedPairs.push_back({nb1data.id, nb2data.id});
                        sendPairsInfo(nb1data, nb2data);
                    }
                }
                else if(nb1data.type==CHILD && nb2data.type==CHILD){
                } else continue;
//                    throw cRuntimeError("Edge case reached");
            }
        }
    }
}

bool comp(MCMTNeighborRecord nb1, MCMTNeighborRecord nb2){
    if(nb1.transmissionCount == nb2.transmissionCount){
        return nb1.id<nb2.id;
    } else return nb1.transmissionCount<nb2.transmissionCount;
};


void MCMTMac::buildLQ(){
    for(MCMTNeighborRecord nbR: interferersSet){
        if(nbR.geocolorIndex != this->geocolorIndex){
            throw cRuntimeError("feature not implemented");
        }
    }
    //copy
    int LQsize = this->interferersSet.size();
//    this->localQueue.resize(LQsize);
    this->localQueue.insert(this->localQueue.begin(), this->interferersSet.begin(), this->interferersSet.end());
//    std::copy(this->interferersSet.begin(), this->interferersSet.end(), std::back_inserter(this->localQueue));
    std::sort(this->localQueue.begin(), this->localQueue.end(), comp);
    this->localQueue;
//    throw cRuntimeError("Check sorted LQ");
}

void MCMTMac::CAPprocessing(){

    //broadcast ONE HELLO packet
    vector<int> neighborIds = this->routingModule->getNeighborList();

    for(int nbId: neighborIds){
        MCMTMACPacket* mcmtPkt = new MCMTMACPacket();
        mcmtPkt->setSource(self);
        mcmtPkt->setDestination(nbId);
        mcmtPkt->setCHId(this->routingModule->config.clus_id[self]);
        mcmtPkt->setMCMTMACPacketType(HELLO);
        mcmtPkt->setSourceGeoColor(this->geocolorIndex);
        mcmtPkt->setTransmissionCount(this->routingModule->config.getSubtreeSize(self)); //TODO: this is not working for hetero trafic

        this->toMacLayerDirect(mcmtPkt, nbId);
    }

    //pair and send to each pairs
    doPairingAndRelayPairInfo();
}

void MCMTMac::processCurrentCAPSlot(){
    macTrace() << "RUNNING CAP SLOT "<< currentCAPslot;
}

void MCMTMac::macSpecificPreCAPSetup(){

    macTrace()<<"entering MCMTMac::macSpecificPreCAPSetup";

    this->isBD = false;
    this->currentTransmissionCount = this->TXBuffer.size();

    this->isCH = false;
    if(this->routingModule->config.nextHop[self]==-1 and this->routingModule->config.clus_id[self]==-1)
        this->isCH = true;

};
