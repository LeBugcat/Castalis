/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Dimosthenis Pediaditakis                 *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *  
 ****************************************************************************/

#include "helpStructures/DebugInfoWriter.h"
#include <sstream>

ofstream DebugInfoWriter::theFile;
string DebugInfoWriter::fileName="Routing-Trace.txt";

ofstream DebugInfoWriter::theUAVFile;
string DebugInfoWriter::uavFileName="UAV-Trace.txt";

ofstream DebugInfoWriter::theAppFile;
string DebugInfoWriter::appFileName="AppTrace.txt";

ofstream DebugInfoWriter::theMACFile;
string DebugInfoWriter::MACFileName="MACTrace.txt";

ofstream DebugInfoWriter::theMetricsFile;
string DebugInfoWriter::metricsFileName;

DebugInfoWriter::DebugInfoWriter(const string & fName)
{
    std::stringstream ss_mfn;

    fileName = fName;
    ss_mfn << "MetricsTrace_" << fName;
}

void DebugInfoWriter::setDebugFileName(const string & fName)
{
    std::stringstream ss_mfn;

    fileName = fName;
    ss_mfn << "MetricsTrace_" << fName;

    metricsFileName = ss_mfn.str();
}

void DebugInfoWriter::setAppDebugFileName(const string & fName)
{
    appFileName = fName;
}

void DebugInfoWriter::setMACDebugFileName(const string & fName)
{
    MACFileName = fName;
}

void DebugInfoWriter::setUAVDebugFileName(const string & fName)
{
    uavFileName = fName;
}

void DebugInfoWriter::setMetricsDebugFileName(const string & fName)
{
    metricsFileName = fName;
}

ofstream & DebugInfoWriter::getStream(void)
{
    if (!theFile.is_open())
        theFile.open(fileName.c_str(), ios::app);
    theFile.setf(ios::left);

    return theFile;
}

ofstream & DebugInfoWriter::getAppStream(void)
{
    if (!theAppFile.is_open())
        theAppFile.open(appFileName.c_str(), ios::app);
    theAppFile.setf(ios::left);
    return theAppFile;
}

ofstream & DebugInfoWriter::getUAVStream(void)
{
    if (!theUAVFile.is_open())
        theUAVFile.open(uavFileName.c_str(), ios::app);
    theUAVFile.setf(ios::left);
    return theUAVFile;
}

ofstream & DebugInfoWriter::getMACStream(void)
{
    if (!theMACFile.is_open())
        theMACFile.open(MACFileName.c_str(), ios::app);
    theMACFile.setf(ios::left);
    return theMACFile;
}

ofstream & DebugInfoWriter::getMetricsStream(void)
{
    if (!theMetricsFile.is_open())
        theMetricsFile.open(metricsFileName.c_str(), ios::app);
    theMetricsFile.setf(ios::left);

    return theMetricsFile;
}

void DebugInfoWriter::closeStream(void)
{
	if (theFile.is_open())
		theFile.close();
}

void DebugInfoWriter::closeAppStream(void)
{
    if (theAppFile.is_open())
        theAppFile.close();
}

void DebugInfoWriter::closeUAVStream(void)
{
    if (theUAVFile.is_open())
        theUAVFile.close();
}

void DebugInfoWriter::closeMACStream(void)
{
    if (theMACFile.is_open())
        theMACFile.close();
}

void DebugInfoWriter::closeMetricsStream(void)
{
    if (theMetricsFile.is_open())
        theMetricsFile.close();
}
