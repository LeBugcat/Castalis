/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Yuriy Tselishchev, Athanassios Boulis                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include "VirtualRouting.h"

void VirtualRouting::initialize(int stage)
{
  if (stage==0) {
//    initialized = true;
    srand(time(NULL)); // this seeds the rand function,
    //so that runs will be different although same config
    string mfn = getParentModule()->getParentModule()->getParentModule()->par("debugMetricsFileName");
    DebugInfoWriter::setMetricsDebugFileName(mfn);
  }
  if (stage==1){
    //legacy Virtual Routing vars
    maxNetFrameSize = par("maxNetFrameSize");
    netDataFrameOverhead = par("netDataFrameOverhead");
    netBufferSize = par("netBufferSize");

    // Get a valid references to the Resources Manager module and the
    // Radio module, so that we can make direct calls to their public methods
    radioModule = check_and_cast <Radio*>(getParentModule()->getSubmodule("Radio"));
    resMgrModule = check_and_cast <ResourceManager*>(getParentModule()->getParentModule()->getSubmodule("ResourceManager"));
    if (!resMgrModule || !radioModule)
        throw cRuntimeError("\n Virtual Routing init: Error in geting a valid reference module(s).");

    //identity functions
    self = getParentModule()->getParentModule()->getIndex();
    stringstream out; out << self; selfAddress = out.str();

    //misc vars
    cpuClockDrift = resMgrModule->getCPUClockDrift();
    setTimerDrift(cpuClockDrift);
    disabled = true;
    currentSequenceNumber = 0;

    pktHistory.clear();

    declareOutput("Buffer overflow");
    declareOutput("Setup energy"); //renamed output
    declareOutput("Setup time");
    declareOutput("Collect energy");
    declareOutput("Collect time");
    declareOutput("End energy");
    declareOutput("End time");
    declareOutput("Round energy");
    declareOutput("preList size");
  }
}

// A function to send control messages to lower layers
void VirtualRouting::toMacLayer(cMessage * msg)
{
    if (msg->getKind() == NETWORK_LAYER_PACKET)
        throw cRuntimeError("toMacLayer() function used incorrectly to send NETWORK_LAYER_PACKET without destination MAC address");
    send(msg, "toMacModule");
}

// A function to send packets to MAC, requires a destination address
void VirtualRouting::toMacLayer(cPacket * pkt, int destination)
{
    RoutingPacket *netPacket = check_and_cast <RoutingPacket*>(pkt);
    netPacket->getNetMacInfoExchange().nextHop = destination;
    send(netPacket, "toMacModule");
}

void VirtualRouting::toApplicationLayer(cMessage * msg)
{
    send(msg, "toCommunicationModule");
}

void VirtualRouting::encapsulatePacket(cPacket * pkt, cPacket * appPkt)
{
    RoutingPacket *netPkt = check_and_cast <RoutingPacket*>(pkt);
    // set the size to just the overhead. encapsulate(appPkt)
    // will add the size of the app packet automatically
    netPkt->setByteLength(netDataFrameOverhead);
    netPkt->setKind(NETWORK_LAYER_PACKET);
    netPkt->setSourceAddress(SELF_NETWORK_ADDRESS);
    netPkt->setSequenceNumber(currentSequenceNumber++);
    netPkt->encapsulate(appPkt);
}

cPacket* VirtualRouting::decapsulatePacket(cPacket * pkt)
{
    RoutingPacket *netPkt = check_and_cast <RoutingPacket*>(pkt);
    ApplicationPacket *appPkt = check_and_cast <ApplicationPacket*>(netPkt->decapsulate());

    appPkt->getAppNetInfoExchange().RSSI = netPkt->getNetMacInfoExchange().RSSI;
    appPkt->getAppNetInfoExchange().LQI = netPkt->getNetMacInfoExchange().LQI;
    appPkt->getAppNetInfoExchange().source = netPkt->getSourceAddress();
    return appPkt;
}

void VirtualRouting::handleMessage(cMessage * msg)
{
    int msgKind = msg->getKind();
    if (disabled && msgKind != NODE_STARTUP)
    {
        delete msg;
        return;
    }

    switch (msgKind) {

        case NODE_STARTUP:
        {
            disabled = false;
            send(new cMessage("Network --> Mac startup message", NODE_STARTUP), "toMacModule");
            startup();
            break;
        }

        case APPLICATION_PACKET:
        {
            ApplicationPacket *appPacket = check_and_cast <ApplicationPacket*>(msg);
            if (maxNetFrameSize > 0 && maxNetFrameSize < appPacket->getByteLength() + netDataFrameOverhead)
            {
                trace() << "Oversized packet dropped. Size:" << appPacket->getByteLength() <<
                    ", Network layer overhead:" << netDataFrameOverhead <<
                    ", max Network packet size:" << maxNetFrameSize;
                break;
            }

            /* Control is now passed to a specific routing protocol by calling fromApplicationLayer()
             * Notice that after the call we RETURN (not BREAK) so that the packet is not deleted.
             * This is done since the packet will most likely be encapsulated and forwarded to the
             * MAC layer. If the protocol specific function wants to discard the packet is has
             * to delete it.
             */
            fromApplicationLayer(appPacket, appPacket->getAppNetInfoExchange().destination.c_str());
            return;
        }

        case NETWORK_LAYER_PACKET:
        {
            //cast type and get packet's Physical Info
            RoutingPacket *netPacket = check_and_cast <RoutingPacket*>(msg);
            NetMacInfoExchange_type info = netPacket->getNetMacInfoExchange();

            /* Control is now passed to a specific routing protocol by calling fromMacLayer()
                         * Notice that after the call we BREAK so that the NET packet gets deleted.
                         * This will not delete the encapsulated APP packet if it gets decapsulated
                         * by fromMacLayer(), i.e., the normal/expected action.
                         */
            if (netPacket->getTTL() > 0) {
                int dst = netPacket->getDestination();
                netPacket->setTTL(netPacket->getTTL() - 1);
                netPacket->setHopCount(netPacket->getHopCount() + 1);
            }

            //passing normally onto protocol-specific function
            fromMacLayer(netPacket, info.lastHop, info.RSSI, info.LQI);
            break;
        }

        case TIMER_SERVICE:{
            handleTimerMessage(msg);
            break;
        }

        case MAC_CONTROL_MESSAGE:{
            handleMacControlMessage(msg);
            return; // msg not deleted
        }

        case RADIO_CONTROL_MESSAGE:{
            handleRadioControlMessage(msg);
            return; // msg not deleted
        }

        case MAC_CONTROL_COMMAND:{
            toMacLayer(msg);
            return; // msg not deleted
        }

        case RADIO_CONTROL_COMMAND:{
            toMacLayer(msg);
            return; // msg not deleted
        }

        case NETWORK_CONTROL_COMMAND:{
            throw cRuntimeError("Event logic NETWORK_CONTROL_COMMAND un-implemented at Network Module!");
            break;
        }

        case OUT_OF_ENERGY:{
            disabled = true;
            break;
        }

        case DESTROY_NODE:{
            disabled = true;
            break;
        }

        default:{
            throw cRuntimeError("Network module recieved unexpected message: [%s]", msg->getName());
        }
    }

    delete msg;
}

// handleMacControlMessage needs to either process and DELETE the message OR forward it
void VirtualRouting::handleMacControlMessage(cMessage * msg)
{
    toApplicationLayer(msg);
}

// handleRadioControlMessage needs to either process and DELETE the message OR forward it
void VirtualRouting::handleRadioControlMessage(cMessage * msg)
{
    toApplicationLayer(msg);
}

void VirtualRouting::finish()
{
    // clear the buffer from all remaining packets
    cPacket* pkt;

    while (!TXBuffer.empty()) {
        pkt = TXBuffer.front();
        TXBuffer.pop();
        cancelAndDelete(pkt);
    }
    CastaliaModule::finish();
}

int VirtualRouting::bufferPacket(cPacket * rcvFrame)
{
    if ((int)TXBuffer.size() >= netBufferSize) {
        collectOutput("Buffer overflow");
        cancelAndDelete(rcvFrame);
        return 0;
    } else {
        TXBuffer.push(rcvFrame);
        return 1;
    }
}

int VirtualRouting::resolveNetworkAddress(const char *netAddr)
{
    if (!netAddr[0] || netAddr[0] < '0' || netAddr[0] > '9')
        return BROADCAST_MAC_ADDRESS;
    return atoi(netAddr);
}

bool VirtualRouting::isNotDuplicatePacket(cPacket * pkt)
{
    //extract source address and sequence number from the packet
    RoutingPacket *netPkt = check_and_cast <RoutingPacket*>(pkt);
    int src = resolveNetworkAddress(netPkt->getSourceAddress());
    unsigned int sn = netPkt->getSequenceNumber();

    //resize packet history vector if necessary
    if (src >= (int)pktHistory.size())
        pktHistory.resize(src+1);

    //search for this sequence number in the list, corresponding to address 'src'
    list<unsigned int>::iterator it1;
    for (it1 = pktHistory[src].begin(); it1 != pktHistory[src].end(); it1++) {
        //if such sequence number is found, packet is duplicate
        if (*it1 == sn) return false;
    }

    //no such sequence number found, this is new packet
    pktHistory[src].push_front(sn);
    if (pktHistory[src].size() > PACKET_HISTORY_SIZE)
        pktHistory[src].pop_back();
    return true;
}
