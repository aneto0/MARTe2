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
#include "MemoryOperationsHelper.h"
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
    addressToSearch = static_cast<char8 **>(NULL);
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
ReferenceContainerFilterObjectName::ReferenceContainerFilterObjectName(const int32 &occurrenceNumber, const uint32 &modeToSet, const char8 * const address) :
        ReferenceContainerFilter(occurrenceNumber, modeToSet) {

    addressNumberNodes = 0u;
    addressToSearch = static_cast<char8 **>(NULL);
    /*lint -e{1506} the caller must know that the address pointer shall be valid while the class is to be used*/
    SetAddress(address);
}

/*lint -e{929} -e{925} -e{9007} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
void ReferenceContainerFilterObjectName::SetAddress(const char8 * const address) {
    const char8 *lastOccurrence = address;
    addressToSearch = static_cast<char8 **>(NULL);

    if (address != NULL) {
        //Count the number of dots found. The first and last dot are ignored. Two consecutive dots result
        //in addressNumberNodes = 0

        uint32 startIn = 0u;

        uint32 length = StringHelper::Length(address);
        if (length > 0u) {

            //there is at least one node
            addressNumberNodes++;
            //skip the first '.'
            if (address[0] == '.') {
                startIn = 1u;
            }
            uint32 lastFoundIndex = startIn - 1u;
            for (uint32 i = startIn; i < length; i++) {
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
                if (addressNumberNodes > 0u) {
                    addressNumberNodes--;
                }
            }
        }

        if (addressNumberNodes > 0u) {
            //create an array of strings for nodes
            addressToSearch = new char8*[addressNumberNodes];
            lastOccurrence = &address[startIn];

            for (uint32 i = 0u; i < addressNumberNodes; i++) {
                uint32 strLength = static_cast<uint32>(StringHelper::SearchIndex(&lastOccurrence[0], "."));
                addressToSearch[i] = static_cast<char8 *>(HeapManager::Malloc(strLength + 1u));
                lastOccurrence = StringHelper::TokenizeByChars(lastOccurrence, ".", addressToSearch[i]);

            }
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

/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
ReferenceContainerFilterObjectName::ReferenceContainerFilterObjectName(const ReferenceContainerFilterObjectName& other) :
        ReferenceContainerFilter(other) {
    addressToSearch = static_cast<char8 **>(NULL);
    addressNumberNodes = other.addressNumberNodes;
    if (addressNumberNodes > 0u) {
        addressToSearch = new char8*[addressNumberNodes];
        for (uint32 i = 0u; i < addressNumberNodes; i++) {
            uint32 length = StringHelper::Length(other.addressToSearch[i]) + 1u;
            addressToSearch[i] = static_cast<char8 *>(HeapManager::Malloc(length));
            /*lint -e{534} possible failure is not handled nor propagated.*/
            MemoryOperationsHelper::Copy(addressToSearch[i], other.addressToSearch[i], length);
        }
    }
    Reset();
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
ReferenceContainerFilterObjectName &ReferenceContainerFilterObjectName::operator =(const ReferenceContainerFilterObjectName& other) {
    if (this != &other) {
        if (addressNumberNodes > 0u) {

            for (uint32 i = 0u; i < addressNumberNodes; i++) {
                bool ok = HeapManager::Free(reinterpret_cast<void *&>(addressToSearch[i]));
                if (!ok) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError,
                                          "ReferenceContainerFilterObjectName: Failed HeapManager::Free()");
                }
            }
        }

        if (addressToSearch != NULL) {
            delete[] addressToSearch;
        }
        originallySetOccurrence = other.GetOriginalSetOccurrence();
        SetMode(other.GetMode());
        addressNumberNodes = other.addressNumberNodes;
        if (addressNumberNodes > 0u) {
            addressToSearch = new char8*[addressNumberNodes];
            for (uint32 i = 0u; i < addressNumberNodes; i++) {
                uint32 length = StringHelper::Length(other.addressToSearch[i]) + 1u;
                addressToSearch[i] = static_cast<char8 *>(HeapManager::Malloc(length));
                /*lint -e{534} possible failure is not handled nor propagated.*/
                MemoryOperationsHelper::Copy(addressToSearch[i], other.addressToSearch[i], length);
            }
        }
    }
    Reset();
    return *this;
}

/*lint -e{1551} The free could cause a segmentation fault.*/
/*lint -e{929} -e{925} the current implementation of the ReferenceContainerFilterObjects requires pointer to pointer casting*/
ReferenceContainerFilterObjectName::~ReferenceContainerFilterObjectName() {
    if (addressNumberNodes > 0u) {

        for (uint32 i = 0u; i < addressNumberNodes; i++) {
            bool ok = HeapManager::Free(reinterpret_cast<void *&>(addressToSearch[i]));
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError,
                                      "ReferenceContainerFilterObjectName: Failed HeapManager::Free()");
            }
        }
        delete[] addressToSearch;
    }
    addressNumberNodes = 0u;
}

bool ReferenceContainerFilterObjectName::TestPath(ReferenceContainer &previouslyFound) const {
    bool found = true;

    for (uint32 i = 0u; (i < previouslyFound.Size()) && (found); i++) {
        found = false;
        if (previouslyFound.Get(static_cast<uint32>(i)).IsValid()) {
            if (previouslyFound.Get(static_cast<uint32>(i))->GetName() != NULL) {
                found = (StringHelper::Compare(previouslyFound.Get(static_cast<uint32>(i))->GetName(), addressToSearch[i]) == 0);
            }
        }
    }
    return found;
}

bool ReferenceContainerFilterObjectName::Test(ReferenceContainer &previouslyFound, Reference const &referenceToTest) {
    bool found = (addressNumberNodes > 0u);

    if (found) {
        found = referenceToTest.IsValid();
    }
    if (found) {
        found = (referenceToTest->GetName() != NULL);
    }

    if (found) {
        //if addressNumberNodes==1 then just compare with addressToSearch[0]
        uint32 index = (addressNumberNodes > 1u) ? (previouslyFound.Size()) : (0u);

        found = (index < addressNumberNodes);
        if (found) {
            found = (StringHelper::Compare(referenceToTest->GetName(), addressToSearch[index]) == 0);
        }

        if (found) {
            if (addressNumberNodes > 1u) {
                /*lint -e{9007} no side-effects on TestPath*/
                found = (TestPath(previouslyFound));
            }
        }
        bool pathOk = found;

        if (found) {
            if (addressNumberNodes > 1u) {
                found = (previouslyFound.Size() == (addressNumberNodes - 1u));
            }
        }

        //remove the recursive mode if the path is wrong otherwise the Find function
        //will continue deep.
        if (addressNumberNodes > 1u) {

            if (pathOk) {
                SetMode(GetMode() | ReferenceContainerFilterMode::PATH);
            }
            else {
                uint32 modeTemp = GetMode();
                modeTemp &= ~(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH);
                SetMode(modeTemp);
            }
        }
    }

    if (found) {
        IncrementFound();
    }
    return found;
}

}
