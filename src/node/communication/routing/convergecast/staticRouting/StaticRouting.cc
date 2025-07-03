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

#include "StaticRouting.h"
#include "rapidcsv.h"

Define_Module(StaticRouting);

void StaticRouting::protocolInitialize(){
    //get the CSV file
    //in this CSV file, there is ONE topology
    topoCSVAbsPath = par("topoCSVAbsPath").stringValue();
//    loadCSVIntoConfig();
};

// this loads the CSV
void StaticRouting::loadCSVIntoConfig(string topoCSVAbsPath){

    // File pointer
    rapidcsv::Document doc(topoCSVAbsPath);

    std::vector<int> nodeIds  = doc.GetColumn<int>("id");
    std::vector<int> nextHops = doc.GetColumn<int>("nextHop");
    std::vector<int> isCH     = doc.GetColumn<int>("isSink");
    std::vector<int> CHIDs    = doc.GetColumn<int>("CHID");

    this->config.clus_id.resize(numNodes, -1);
    this->config.nextHop.resize(numNodes, -1);

    for(int ite=0;ite<nodeIds.size();ite++){
        int nid = nodeIds[ite];
        if(!isCH[ite]){
            this->config.clus_id[nid] = CHIDs[ite];
            this->config.nextHop[nid] = nextHops[ite];
        } else {
            this->config.clus_id[nid] = -1;
            this->config.nextHop[nid] = -1;
        }
    }
}

void StaticRouting::mainAlg(){

    //get the singular topology in our file
    //push it into the "config" object (need better name, no?)
    //make sure that after this function runs,
    //the lower layers is ready to harness the config object
    if(!isSink){
        throw cRuntimeError("Operation mainAlg called on non-sink.");
    } else {
        loadCSVIntoConfig(this->topoCSVAbsPath);
    }
};
