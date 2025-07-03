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

#ifndef NODE_COMMUNICATION_ROUTING_CONVERGECAST_STATICROUTING_STATICROUTING_H_
#define NODE_COMMUNICATION_ROUTING_CONVERGECAST_STATICROUTING_STATICROUTING_H_

#include "node/communication/routing/convergecast/ConvergecastRouting.h"

//this class is for testing, we read a CSV and pseudo-init a routing tree
class StaticRouting : public ConvergecastRouting {

protected:
    string topoCSVAbsPath;

    void protocolInitialize();
    void mainAlg();

    void loadCSVIntoConfig(string topoCSVAbsPath);
};

#endif /* NODE_COMMUNICATION_ROUTING_CONVERGECAST_STATICROUTING_STATICROUTING_H_ */
