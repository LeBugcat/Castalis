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

#ifndef HELPSTRUCTURES_CONNECTIVITYGRAPH_H_
#define HELPSTRUCTURES_CONNECTIVITYGRAPH_H_

#include <map>
#include <vector>
#include <omnetpp.h>

using namespace std;
//this connectivity graph is assuming bi-directional
//uniform TX power for every node in the graph
class ConnectivityGraph {
public:
    int nodeCount;

    map<int, vector<int>> adjacentNodesAt; //again we are assuming
                                          //bidirectional connection

    void constructGraph(cModule*); // passing in the (susually Sink) node's
                                   // MAC/Routing module
    void getNeighbors(int nodeId, int maximumOrder);//get neighbors
};

#endif /* HELPSTRUCTURES_CONNECTIVITYGRAPH_H_ */
