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
//*******************************************************************************/*

package wirelessChannel.newDefaultChannel;
// The wireless channel module simulates the wireless medium. Nodes sent packets to it
// and according to various conditions (fading, interference etc) it is decided which
// nodes can receive this packet

// This ned implements the new WC, using a more modular design aimed for integration of newer PL Models
import wirelessChannel.iWirelessChannel;
        


simple NewWirelessChannel like iWirelessChannel {
 parameters:
//	bool collectTraceInfo = default (false);
//	bool onlyStaticNodes = default (true);		// if NO mobility, set it to true for greater efficiency 	
	int pathlossModel = default(0);	// LNS-> Log Normal Shadowing
										// MED-> Weissberg MED
										// ITU-R-> ITU rec (not implemented)
										// COST235 -> COST235 (not implemented)
										// UDM-> User Defined Model (not implemented)
	double treeDepth @unit(m) = default(0.05m);
	double expectedFreq @unit(Hz) = default(13Hz);
		
	bool collectTraceInfo = default (false);
	bool onlyStaticNodes = default (true);		// if NO mobility, set it to true for greater efficiency 

	double xCellSize = default (5);		// if we define cells (to handle mobility)
	double yCellSize = default (5);		// how big are the cells in each dimension
	double zCellSize = default (1);

	double pathLossExponent = default (2.4);	// how fast is the signal strength fading
	double PLd0 = default (55);					// path loss at reference distance d0 (in dBm)
	double d0 = default (1.0);					// reference distance d0 (in meters)

	double sigma = default (4.0);				// how variable is the average fade for nodes at the same distance
												// from eachother. std of a gaussian random variable.

	double bidirectionalSigma = default (1.0);	// how variable is the average fade for link B->A if we know
												// the fade of link A->B. std of a gaussian random variable

	string pathLossMapFile = default ("");		// describes a map of the connectivity based on pathloss
												// if defined, then the parameters above become irrelevant

	string temporalModelParametersFile = default ("");	
												// the filename that contains all parameters for 
												// the temporal channel variation model

	double signalDeliveryThreshold = default (-100);	
												// threshold in dBm above which, wireless channel module
												// is delivering signal messages to radio modules of 
												// individual nodes
	
 gates:
 	output toNode[];
	input fromNode[];
	input fromMobilityModule @ directIn;
}
