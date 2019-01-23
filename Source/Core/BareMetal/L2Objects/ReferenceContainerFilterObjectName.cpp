/**
 * @file ReferenceContainerFilterObjectName.cpp
 * @brief Source file for class ReferenceContainerFilterObjectName
 * @date 14/08/2015
 * @author Andre Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.
 *
 * @details This source file contains the definition of all the methods for
 * the class ReferenceContainerFilterObjectName (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HeapManager.h"
#include "StringHelper.h"
#include "ReferenceContainerFilterObjectName.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ReferenceContainerFilterObjectName::ReferenceContainerFilterObjectName() :
        ReferenceContainerFilter() {
    addressNumberNodes = 0u;
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
ReferenceContainerFilterObjectName::ReferenceContainerFilterObjectName(const int32 &occurrenceNumber,
                                                                       const uint32 &modeToSet,
                                                                       CCString const address) :
        ReferenceContainerFilter(occurrenceNumber, modeToSet) {

    addressNumberNodes = 0u;
    SetAddress(address);
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
void ReferenceContainerFilterObjectName::SetAddress(CCString const address) {

	// cleanup
	addressToSearchArray().Truncate(0);

    if (!address.IsNullPtr()) {
        //Count the number of dots found. The first and last dot are ignored. Two consecutive dots result
        //in addressNumberNodes = 0

    	// copy address
    	addressToSearchWhole().Append(address);

    	// count dots
    	char8 *p = addressToSearchWhole.GetList();
    	addressNumberNodes = 0;
    	// to skip initial . if present
    	bool start  = true;
    	// to deal with consecutive . --> restart
    	bool justFound = false;

    	while (*p != '\0'){
    		// everytime a . is found is replaced with /0
    		if (*p == '.') {
    			// place a character terminator ...
    			*p = '\0';
        		if (!start) {
        			// consecutive .. --> reset
        			if (justFound){
        				addressNumberNodes= 0u;
        				addressToSearchArray().Truncate(0u);
        			} else {
        				addressNumberNodes++;
        				// next must not be the end
        				// deals with trailing .
        				if (p[1]!= '\0'){
        					addressToSearchArray().Append(CCString(p));
        				}
        				justFound = true;
        			}
        		}
    		} else {
    			justFound = false;
    		}
    		p++;
    		start = false;
    	}
    }

    // If the number of nodes is greater than one, then the number of occurrence must be =1
    // and the path mode must be enabled (and it automatically enables the recursive mode)
    // So, if you want to search a path you have to be in path mode and you cannot search more than one path.
    if (addressNumberNodes > 1u) {
        //Note: in this case IsSearchAll returns false
        // IsRecursive returns true
        // IsStorePath returns true
        SetMode(GetMode() | ReferenceContainerFilterMode::PATH);
    }
}

void ReferenceContainerFilterObjectName::GetPath(DynamicCString &path) const{
	path().Truncate(0);
	CCString *pList = addressToSearchArray.GetList();
	bool start = true;
	while (pList != NULL_PTR(CCString *)){
		if (!start){
			path().Append('.');
		}
		path().Append(*pList);
		start = false;
		pList++;
	}
}


/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
ReferenceContainerFilterObjectName::ReferenceContainerFilterObjectName(const ReferenceContainerFilterObjectName& other) :
        ReferenceContainerFilter(other) {

// must create a dummy path string and the initialize from that
	DynamicCString dummy;
	other.GetPath(dummy);

	// set the address via the usual scheme
	SetAddress(dummy);

    Reset();
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
ReferenceContainerFilterObjectName &ReferenceContainerFilterObjectName::operator =(const ReferenceContainerFilterObjectName& other) {
    // wierd check
	if (this == &other) return *this;

	// must create a dummy path string and the initialize from that
	DynamicCString dummy;
	other.GetPath(dummy);

	// set the address via the usual scheme
	SetAddress(dummy);

    Reset();

    return *this;
}

/*lint -e{1551} The free could cause a segmentation fault.*/
/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
ReferenceContainerFilterObjectName::~ReferenceContainerFilterObjectName() {

    addressNumberNodes = 0u;
}

bool ReferenceContainerFilterObjectName::TestPath(ReferenceContainer &previouslyFound) const {
    bool found = (previouslyFound.Size() == (addressNumberNodes - 1u));
    int32 i;
    for (i = (static_cast<int32>(previouslyFound.Size()) - 1); (found) && (i >= 0); i--) {
        found = false;
        uint32 ui = static_cast<uint32>(i);
        if (previouslyFound.Get(ui).IsValid()) {
        	CCString name = previouslyFound.Get(ui)->GetName();
            if (!name.IsNullPtr()) {
                found = (name == addressToSearchArray[static_cast<uint32>(i)]) ;
            }
        }
    }
    return found; 
}

bool ReferenceContainerFilterObjectName::Test(ReferenceContainer &previouslyFound,
                                              Reference const &referenceToTest) {
    bool found = (addressNumberNodes > 0u);

    if (addressNumberNodes > 1u) {
        /*lint -e{9007} no side-effects on TestPath*/
        found = (found && TestPath(previouslyFound));
    }

    //Check if this is the last node and if it matches the last part of the addressToSearch
    if (found && referenceToTest.IsValid()) {
    	CCString name = referenceToTest->GetName();
        if ( !name.IsNullPtr()) {
            /*lint -e{661} -e{662} safe given that addressToSearch is always created with the size of addressNumberNodes
             * and its size cannot be modified in runtime*/
            found = (name ==addressToSearchArray[addressNumberNodes - 1u]);
        }
        else {
            found = false;
        }
    }

    if (found) {
        IncrementFound();
    }
    return found;
}

}
