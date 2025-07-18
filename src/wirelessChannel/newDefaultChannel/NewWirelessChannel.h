/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *  
 ****************************************************************************/

#ifndef _NEWWIRELESSCHANNEL_H
#define _NEWWIRELESSCHANNEL_H

#include "wirelessChannel/WirelessChannelMessages_m.h"
#include "wirelessChannel/defaultChannel/WirelessChannelTemporal.h"
#include "wirelessChannel/defaultChannel/WirelessChannel.h"
#include "node/mobilityManager/VirtualMobilityManager.h"
#include "helpStructures/CastaliaModule.h"

#include "time.h"
#include <list>

using namespace std;

enum PathlossModel_type{
    COST235 = 3,
    ITU_R = 2,
    MED = 1,
    LNS = 0
};

//class PathLossElement {
// public:
//    int cellID;
//    float avgPathLoss;
//    float lastObservedDiffFromAvgPathLoss;
//    simtime_t lastObservationTime;
//
//    PathLossElement(int c, float PL) {
//        cellID = c;
//        avgPathLoss = PL;
//        lastObservedDiffFromAvgPathLoss = PL;
//        lastObservationTime = 0.0;
//    };
//
//    ~PathLossElement() {
//    };
//};

class NewWirelessChannel: public CastaliaModule {
 private:

    /*--- variables corresponding to .ned file's parameters ---*/
    int numOfNodes;

    double xFieldSize;
    double yFieldSize;
    double zFieldSize;
    double xCellSize;
    double yCellSize;
    double zCellSize;

    double expectedFreq;
    double treeDepth;

    PathlossModel_type pathlossModel;

    // variables corresponding to Wireless Channel module parameters
    double pathLossExponent;    // the path loss exponent
    double noiseFloor;          // in dBm
    double PLd0;                // Power loss at a reference distance d0 (in dBm)
    double d0;                  // reference distance (in meters)
    double sigma;               // std of a zero-mean Gaussian RV
    double bidirectionalSigma;  // std of a zero-mean Gaussian RV

    const char *pathLossMapFile;
    const char *temporalModelParametersFile;
    double signalDeliveryThreshold;
    bool onlyStaticNodes;
    double receiverSensitivity;
    double maxTxPower;          // this is derived, by reading all the Tx power levels

    /*--- other class member variables ---*/
    int numOfXCells, numOfYCells, numOfZCells;
    int numOfSpaceCells;
    int xIndexIncrement, yIndexIncrement, zIndexIncrement;

    list <PathLossElement*>*pathLoss;       // an array of lists (numOfSpaceCels long)
                                            // holding info on path loss. Element i of the
                                            // array is a list elements that describe which
                                            // cells are affected (and how) when a
                                            // node in cell i transmits.
    list < list<int>* >*neighborTable;

    list <int>*nodesAffectedByTransmitter;  // an array of lists (numOfNodes long). The list
                                            // at array element i holds the node IDs that are
                                            // affected when node i transmits.

    list <int>*cellOccupation;              // an array of lists (numOfSpaceCels long) that
                                            // tells us which nodes are in cell i.

    NodeLocation_type *nodeLocation;        // an array (numOfNodes long) that gives the
                                            // location for each node.

    bool temporalModelDefined;
    channelTemporalModel *temporalModel;

 protected:
    virtual void initialize(int);
    virtual void handleMessage(cMessage * msg);
    virtual void finishSpecific();

    void readIniFileParameters(void);
    void parsePathLossMap(void);
    int parseInt(const char *, int *);
    int parseFloat(const char *, float *);
    void printRxSignalTable(void);
    void updatePathLossElement(int, int, float);
    float calculateProb(float, int);

    double computePathloss(double, double, double);

    int numInitStages() const;
 public:
    NodeLocation_type* getNodesLoc(){
        return nodeLocation;
    }
};

#endif              //_WIRELESSCHANNEL_H
