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
#include "ReferenceContainerFilterObjects.h"
#include "Memory.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ReferenceContainerFilterObjects::ReferenceContainerFilterObjects(const int32 &occurrenceNumber,
                                                                 const uint32 &mode,
                                                                 const char8 * const address) :
        ReferenceContainerFilter(occurrenceNumber, mode) {

    addressNumberNodes = 0;
    const char8 *lastOccurrence = address;

    if (address != NULL) {
        //Count the number of dots found. The first and last dot are ignored. Two consecutive dots result
        //in addressNumberNodes = 0

        uint32 length = StringHelper::Length(address);
        if (length > 0) {
            addressNumberNodes++;
            uint32 i = 0;
            if (address[0] == '.') {
                i++;
            }
            uint32 lastFoundIndex = -1;
            for (; i < length; i++) {
                if (address[i] == '.') {
                    addressNumberNodes++;
                    //No consecutive dots
                    if ((i - lastFoundIndex) == 1u) {
                        addressNumberNodes = 0;
                        break;
                    }
                    lastFoundIndex = i;
                }
            }

            //Ignore the last dot if it exists.
            if (address[length - 1] == '.') {
                addressNumberNodes--;
            }
        }
    }

    if (addressNumberNodes > 0) {
        addressToSearch = new char8*[addressNumberNodes];
        lastOccurrence = address;
        uint32 i = 0;
        for (i = 0u; i < addressNumberNodes; i++) {
            uint32 length = 0;
            uint32 startIn = 0;
            if (lastOccurrence[0] == '.') {
                startIn = 1;
            }
            length = StringHelper::SearchIndex(&lastOccurrence[startIn], ".");
            if (length < 1) {
                length = StringHelper::Length(lastOccurrence);
            }

            addressToSearch[i] = static_cast<char8 *>(Memory::Malloc(length + 1));
            StringHelper::CopyN(addressToSearch[i], &lastOccurrence[startIn], length);
            addressToSearch[i][length] = '\0';
            lastOccurrence = StringHelper::SearchChar(&lastOccurrence[1], '.');
        }

        uint32 temp;
        for (temp = 0; temp < addressNumberNodes; temp++) {
            printf("addressToSearch[%d] = %s \n", temp, addressToSearch[temp]);
        }
    }
}

ReferenceContainerFilterObjects::~ReferenceContainerFilterObjects() {
    if (addressNumberNodes > 0) {
        uint32 i;
        for (i = 0; i < addressNumberNodes; i++) {
            Memory::Free(reinterpret_cast<void *&>(addressToSearch[i]));
        }
        delete[] addressToSearch;
    }
}

bool ReferenceContainerFilterObjects::IsRecursive() const {
    return (ReferenceContainerFilter::IsRecursive() || (addressNumberNodes > 1));
}

bool ReferenceContainerFilterObjects::IsSearchAll() const {
    return (ReferenceContainerFilter::IsSearchAll() && (addressNumberNodes == 1));
}

bool ReferenceContainerFilterObjects::Test(ReferenceContainer &previouslyFound,
                                           Reference &referenceToTest) {
    bool found = false;
    uint32 i = 0;
    if (referenceToTest.IsValid()) {
        if(referenceToTest->)
    }
    for (i = 0; i < addressNumberNodes; i++) {

    }
    /*if (referenceToSearch.Is)
     ;
     if (found) {
     IncrementFound();
     }*/
    return found;
}

