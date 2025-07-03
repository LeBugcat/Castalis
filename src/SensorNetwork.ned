//********************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2010                             *
//*  Developed at the ATP lab, Networked Systems research theme                  *
//*  Author(s): Athanassios Boulis, Dimosthenis Pediaditakis, Yuriy Tselishchev  *
//*  This file is distributed under the terms in the attached LICENSE file.      *
//*  If you do not find this file, copies can be found by writing to:            *
//*                                                                              *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia                 *
//*      Attention:  License Inquiry.                                            *
//*                                                                              *
//*******************************************************************************/

import wirelessChannel.iWirelessChannel;
import physicalProcess.iPhysicalProcess;
import node.Node;

network SN
{
    parameters:
        double field_x = default(30);			// the length of the deployment field
        double field_y = default(30);			// the width of the deployment field
        double field_z = default(0);			// the height of the deployment field (2-D field by default)

        int numNodes;
        int numUAVs = default(0);
        						// the number of nodes
        double cellWidth;
        double cellHeight;
        
        bool isDebugMode = default(true);
        string deployment = default("");

        int numPhysicalProcesses = default(1);
        string physicalProcessName = default("CustomizablePhysicalProcess");
        string wirelessChannelName = default("WirelessChannel");
        string debugInfoFileName = default("Routing-Trace.txt");
        string debugAppFileName = default("Application-Trace.txt");
        string debugMetricsFileName = default("Metrics-Trace.txt");

    submodules:
        wirelessChannel: wirelessChannel.defaultChannel.WirelessChannel {
            gates:
                toNode[numNodes];
                fromNode[numNodes];
        }
        physicalProcess[numPhysicalProcesses]: <physicalProcessName> like iPhysicalProcess {
            gates:
                toNode[numNodes];
                fromNode[numNodes];
        }

        node[numNodes]: node.Node {
            gates:
                toPhysicalProcess[numPhysicalProcesses];
                fromPhysicalProcess[numPhysicalProcesses];
        }

        //UAV integration: UAV submodules, these are self-defined modules
//        uav[numUAVs]: uav.UAV if numUAVs>0 {
//        }
    connections:
        for i=0..numNodes - 1 {
            node[i].toWirelessChannel --> wirelessChannel.fromNode[i];
            node[i].fromWirelessChannel <-- wirelessChannel.toNode[i];
        }

        for i=0..numNodes - 1, for j=0..numPhysicalProcesses - 1 {
            node[i].toPhysicalProcess[j] --> physicalProcess[j].fromNode[i];
            node[i].fromPhysicalProcess[j] <-- physicalProcess[j].toNode[i];
        }

		//UAV integration: add UAVs connections to WC
//        for i=0..numUAVs - 1 {
//            uav[i].toWirelessChannel --> wirelessChannel.fromNode[numNodes+i]; //UAV is still considered a Node to WC, 
//            uav[i].fromWirelessChannel <-- wirelessChannel.toNode[numNodes+i];//this could cause problem later on
//        }
}
