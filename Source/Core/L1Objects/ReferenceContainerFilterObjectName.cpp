/**
 * @file ReferenceContainerFilterObjects.cpp
 * @brief Source file for class ReferenceContainerFilterObjects
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

 * @details This source file contains the definition of all the methods for
 * the class ReferenceContainerFilterObjects (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Memory.h"
#include "StringHelper.h"
#include "ReferenceContainerFilterObjectName.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ReferenceContainerFilterObjectName::ReferenceContainerFilterObjectName() :
        ReferenceContainerFilter() {
    addressNumberNodes = 0u;
    addressToSearch = static_cast<char8 **>(NULL);
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
ReferenceContainerFilterObjectName::ReferenceContainerFilterObjectName(const int32 &occurrenceNumber,
                                                                       const uint32 &mode,
                                                                       const char8 * const address) :
        ReferenceContainerFilter(occurrenceNumber, mode) {

    addressNumberNodes = 0u;
    const char8 *lastOccurrence = address;
    addressToSearch = static_cast<char8 **>(NULL);

    if (address != NULL) {
        //Count the number of dots found. The first and last dot are ignored. Two consecutive dots result
        //in addressNumberNodes = 0

        uint32 length = StringHelper::Length(address);
        if (length > 0u) {

            //there is at least one node
            addressNumberNodes++;
            uint32 i = 0u;
            //skip the first '.'
            if (address[0] == '.') {
                i++;
            }
            uint32 lastFoundIndex = i - 1u;
            for (; i < length; i++) {
                if (address[i] == '.') {
                    //increment the number of nodes where a '.' is found
                    addressNumberNodes++;
                    //No consecutive dots
                    if ((i - lastFoundIndex) == 1u) {
                        addressNumberNodes = 0u;
                        break;
                    }
                    // save the last to check consecutive dots
                    lastFoundIndex = i;
                }
            }

            //ignore the last dot if it exists.
            if (address[length - 1u] == '.') {
                addressNumberNodes--;
            }
        }
    }

    if (addressNumberNodes > 0u) {
        //create an array of strings for nodes
        addressToSearch = new char8*[addressNumberNodes];
        lastOccurrence = address;

        for (uint32 i = 0u; i < addressNumberNodes; i++) {
            uint32 startIn = 0u;

            //skip the first dot
            if (lastOccurrence[0] == '.') {
                startIn = 1u;
            }

            //-1 is impossible since lastOccurrence cannot be NULL
            uint32 length = static_cast<uint32>(StringHelper::SearchIndex(&lastOccurrence[startIn], "."));

            //this means that there are no dots so there is only a node
            if (length < 1u) {
                length = StringHelper::Length(lastOccurrence);
            }

            addressToSearch[i] = static_cast<char8 *>(Memory::Malloc(length + 1u));
            (void) StringHelper::CopyN(addressToSearch[i], &lastOccurrence[startIn], length);
            addressToSearch[i][length] = '\0';
            lastOccurrence = StringHelper::SearchChar(&lastOccurrence[1], '.');
        }
    }

    //Only one thing is possible. Either look for the absolute path or for multiple occurrences.
    if ((occurrence == -1) && (addressNumberNodes > 1u)) {
        //Look for the first occurrence of the path
        occurrence = 1;
    }
}

ReferenceContainerFilterObjectName::ReferenceContainerFilterObjectName(const ReferenceContainerFilterObjectName& other) :
        ReferenceContainerFilter(other.GetOriginalSetOccurrence(), other.GetMode()) {
    addressNumberNodes = other.addressNumberNodes;
    if (addressNumberNodes > 0u) {
        addressToSearch = new char8*[addressNumberNodes];
        uint32 i = 0u;
        for (i = 0u; i < addressNumberNodes; i++) {
            uint32 length = StringHelper::Length(other.addressToSearch[i]) + 1;
            addressToSearch[i] = static_cast<char8 *>(Memory::Malloc(length));
            Memory::Copy(addressToSearch[i], other.addressToSearch[i], length);
        }
    }
    Reset();
}

ReferenceContainerFilterObjectName &ReferenceContainerFilterObjectName::operator =(const ReferenceContainerFilterObjectName& other) {
    if (this != &other) {
        if (addressNumberNodes > 0) {
            uint32 i;
            for (i = 0; i < addressNumberNodes; i++) {
                Memory::Free(reinterpret_cast<void *&>(addressToSearch[i]));
            }
            delete[] addressToSearch;
        }
        originallySetOccurrence = other.GetOriginalSetOccurrence();
        SetMode(other.GetMode());
        addressNumberNodes = other.addressNumberNodes;
        if (addressNumberNodes > 0) {
            addressToSearch = new char8*[addressNumberNodes];
            uint32 i = 0;
            for (i = 0u; i < addressNumberNodes; i++) {
                uint32 length = StringHelper::Length(other.addressToSearch[i]) + 1;
                addressToSearch[i] = static_cast<char8 *>(Memory::Malloc(length));
                Memory::Copy(addressToSearch[i], other.addressToSearch[i], length);
            }
        }
    }
    Reset();
    return *this;
}

/*lint -e{1551} The free could cause a segmentation fault.*/
/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
ReferenceContainerFilterObjectName::~ReferenceContainerFilterObjectName() {
    if (addressNumberNodes > 0) {
        uint32 i;
        for (i = 0; i < addressNumberNodes; i++) {
            Memory::Free(reinterpret_cast<void *&>(addressToSearch[i]));
        }
        delete[] addressToSearch;
    }
}

bool ReferenceContainerFilterObjectName::IsStorePath() const {
    return (ReferenceContainerFilter::IsStorePath() || (addressNumberNodes > 1u));
}

bool ReferenceContainerFilterObjectName::IsRecursive() const {
    return (ReferenceContainerFilter::IsRecursive() || (addressNumberNodes > 1u));
}

bool ReferenceContainerFilterObjectName::IsSearchAll() const {
    return (ReferenceContainerFilter::IsSearchAll() && (addressNumberNodes == 1u));
}

bool ReferenceContainerFilterObjectName::Test(ReferenceContainer &previouslyFound,
                                              Reference &referenceToTest) {
    bool found = false;
    uint32 idx = 0;
    if (addressNumberNodes > 0u) {
        if (referenceToTest.IsValid()) {
            if (referenceToTest->GetName() != NULL) {
                //Looking for an absolute path
                if (addressNumberNodes > 1u) {
                    idx = previouslyFound.Size();
                }
                found = (StringHelper::Compare(referenceToTest->GetName(), addressToSearch[idx]) == 0);
            }
        }
        if (found) {
            if (addressNumberNodes == 1u) {
                IncrementFound();
            }
            else if (addressNumberNodes == (previouslyFound.Size() + 1)) {
                IncrementFound();
            }
        }
    }
    return found;
}

