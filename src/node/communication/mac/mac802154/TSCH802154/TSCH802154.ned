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

package node.communication.mac.mac802154.TSCH802154;
//****************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2012                         *
//*  Developed at the ATP lab, Networked Systems research theme              *
//*  Author(s): Yuriy Tselishchev                                            *
//*  This file is distributed under the terms in the attached LICENSE file.  *
//*  If you do not find this file, copies can be found by writing to:        *
//*                                                                          *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
//*      Attention:  License Inquiry.                                        *
//*                                                                          *  
//****************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////                                                                 /////////////////
///////////////          ATTENTION!!! THIS MODULE IS NOW STOPPED DEV-ING        /////////////////
///////////////          DUE TO THE IMMENSE COMPLEXITY, THE TSCH NOW IS DEV-ED  /////////////////
///////////////          DIRECTLY ABOVE THE MAC VIRTUAL CLASS                   /////////////////
///////////////                                                                 /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////                                                                 /////////////////
///////////////          ATTENTION!!! THIS MODULE IS NOW STOPPED DEV-ING        /////////////////
///////////////          DUE TO THE IMMENSE COMPLEXITY, THE TSCH NOW IS DEV-ED  /////////////////
///////////////          DIRECTLY ABOVE THE MAC VIRTUAL CLASS                   /////////////////
///////////////                                                                 /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

package node.communication.mac.mac802154.TSCH802154;

simple TSCH802154 extends node.communication.mac.mac802154.Basic802154 {

     parameters:
    int channelsCount = default(16); //0->15 for 2.4Ghz, 0->23 for 5Ghz, might be more or less for each systems
	
	//preserving code from Static 802154
	bool gtsOnly = default (false);
	int requestGTS = default (0);
	int defaultFramesize @unit(slot) = default(20slot); //total frameslots counts, including CAP's
	int defaultCAPsize @unit(slot) = default(10slot); //CAP slots count

	@class(TSCH802154);
}

