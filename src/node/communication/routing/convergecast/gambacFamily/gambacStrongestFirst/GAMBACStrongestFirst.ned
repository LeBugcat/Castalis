//****************************************************************************
//*  Copyright (c) Federal University of Para, brazil - 2011                 *
//*  Developed at the Research Group on Computer Network and Multimedia      *
//*  Communication (GERCOM)     				             *
//*  All rights reserved    				                     *
//*                                                                          *
//*  Permission to use, copy, modify, and distribute this protocol and its   *
//*  documentation for any purpose, without fee, and without written         *
//*  agreement is hereby granted, provided that the above copyright notice,  *
//*  and the author appear in all copies of this protocol.                   *
//*                                                                          *
//*  Module:   LEACH Clustering Protocol for Castalia Simulator              *
//*  Version:  0.2                                                           *
//*  Author(s): Adonias Pires <adonias@ufpa.br>                              *
//*             Claudio Silva <claudio.silva@itec.ufpa.br>                   *
//****************************************************************************/

package node.communication.routing.convergecast.gambacFamily.gambacStrongestFirst;

simple GAMBACStrongestFirst extends node.communication.routing.convergecast.gambacFamily.gambac.GAMBAC
{
parameters:
//	string applicationID = default ("throughputTest");
//	bool collectTraceInfo;
//	int maxNetFrameSize = default (0);
//	int netDataFrameOverhead = default (14);
//	int netBufferSize = default (32);		
//	bool isSink = default (false);
//	
//	double percentage = default (0.05);	
//	double roundLength @unit(s);	
	double helloLength @unit(s) = default (0.01s);	
	double discoverLength @unit(s) = default (0.04s);	
	double slotLength = default (0.6);

	//routing layer packet sizes
//	int dataScaleFactor = default(1);
//	double controlPacketSize = default (9); // Type + Source + Destination + CHid + nextCH + isCH = 16 bytes
//	double dataPacketSize = default (9);
//	int maxCHperUAV = default (10);
//
//	int k0 = default (250);
//	double epsilon = default (0.5);
//	double neighborRange = 100;
//	int numNodesPerCluster = 10;
//	double maxUAVFlyingDistance = default (2400);
//	double gamma = default (0.9999999);
//
//	bool debugRecruitProcess = default (true);
//	int numUAVs = default(5);

	//Parameters used to power control.
//	xml powersConfig;

//gates: 
//	output toCommunicationModule;
//	output toMacModule;
//	input fromCommunicationModule;
//	input fromMacModule;
//	input fromCommModuleResourceMgr;
//	input fromDirect @directIn;
	@class(GAMBACStrongestFirst);
}

