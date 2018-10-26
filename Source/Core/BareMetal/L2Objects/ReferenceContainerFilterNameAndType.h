/**
 * @file ReferenceContainerFilterNameAndType.h
 * @brief Header file for class ReferenceContainerFilterNameAndType
 * @date 18/08/2018
 * @author Giuseppe Ferro
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
 * @details This header file contains the declaration of the class ReferenceContainerFilterNameAndType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCECONTAINERFILTEROBJECTNAME_H_
#define REFERENCECONTAINERFILTEROBJECTNAME_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainer.h"
#include "StringHelper.h"
#include "ErrorManagement.h"
#include "ReferenceT.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief ReferenceContainerFilter for object names and type.
 */
template<typename T>
class ReferenceContainerFilterNameAndType: public ReferenceContainerFilterObjectName {
public:

    /**
     * @brief Default constructor.
     */
    ReferenceContainerFilterNameAndType();

    /**
     * @brief Sets the filter to look for the address.
     * @param[in] occurrenceNumber the ordinal occurrence number (i.e. find the first, the second, ...) of the finding of
     * a node which meets a given criteria or -1 to look for all occurrences. This parameter is indexed to 1.
     * @param[in] modeToSet any ored combination of ReferenceContainerFilterMode::PATH, ReferenceContainerFilterMode::RECURSIVE,
     * ReferenceContainerFilterMode::REVERSE and ReferenceContainerFilterMode::REMOVE.
     * @param[in] address a string with the name of the object to find or with the full path (from the root) to the object.
     * A full path is specified by separating with dots the path (e.g. A.B.C). If a full path is specified only the first
     * occurrence can be searched and the mode will be set to ReferenceContainerFilterMode::PATH.
     */
    ReferenceContainerFilterNameAndType(const int32 &occurrenceNumber,
                                        const uint32 &modeToSet,
                                        const char8 * const address);

    /**
     * @brief Deep copy constructor.
     * @details Copies all the parameters from another filter. This instance of the filter will be Reset().
     * @param[in] other the source ReferenceContainerFilterNameAndType where to copy the information from.
     */
    ReferenceContainerFilterNameAndType(const ReferenceContainerFilterNameAndType<T> &other);

    /**
     * @brief Destructor. Frees all the allocated memory.
     */
    virtual ~ReferenceContainerFilterNameAndType();

    /**
     * @brief Tests if the name of the object hold by referenceToTest is compatible with the address.
     * @details This method will return true when the name of object hold by referenceToTest is the same
     * of the last node in the address specified in the constructor. If a full path was specified as the address,
     * this method will return true if, and only if, all the elements in previouslyFound where found in the
     * exact same order set in the constructor address.
     * Each time this method finds a matching reference the number of occurrences will be incremented.
     * @param[in] previouslyFound the history of the path found so far.
     * @param[in] referenceToTest the reference to test (where the name of object will be extracted from).
     * @return true if the name of the object hold by referenceToTest is compatible with the address.
     */
    virtual bool Test(ReferenceContainer &previouslyFound,
                      Reference const &referenceToTest);
                      
    /**                      
     * @brief Returns index of the address (word) still to be read.
     * @return the index of the address still to be read.
     */
    uint32 GetRemainedAddrIndex();
protected:
    /**
     * The current index of the address being read.
     */
    uint32 nodeStrIndex;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename T>
ReferenceContainerFilterNameAndType<T>::ReferenceContainerFilterNameAndType() :
        ReferenceContainerFilterObjectName() {
    nodeStrIndex = 0u;
}

template<typename T>
ReferenceContainerFilterNameAndType<T>::ReferenceContainerFilterNameAndType(const int32 &occurrenceNumber,
                                                                            const uint32 &modeToSet,
                                                                            const char8 * const address) :
        ReferenceContainerFilterObjectName(occurrenceNumber, modeToSet, address) {
    nodeStrIndex = 0u;

}

template<typename T>
ReferenceContainerFilterNameAndType<T>::ReferenceContainerFilterNameAndType(const ReferenceContainerFilterNameAndType<T> &other) :
        ReferenceContainerFilterObjectName(other) {
    nodeStrIndex = 0u;

}

template<typename T>
ReferenceContainerFilterNameAndType<T>::~ReferenceContainerFilterNameAndType() {

}

template<typename T>
bool ReferenceContainerFilterNameAndType<T>::Test(ReferenceContainer &previouslyFound,
                                                  Reference const &referenceToTest) {
    bool found = (addressNumberNodes > 0u);
    nodeStrIndex = 0u;
    if ((addressNumberNodes > 1u)) {
        uint32 currentIndex = previouslyFound.Size();
        found = (found) && (addressNumberNodes > currentIndex);
        nodeStrIndex = 0u;
        if (found) {
            bool ok = true;
            found = false;
            for (uint32 i = 0u; (i < currentIndex) && (ok); i++) {
                Reference ref = previouslyFound.Get(i);
                ok = (StringHelper::Compare(ref->GetName(), addressToSearch[i]) == 0);
                if (ok) {
                    ReferenceT<T> reft = ref;
                    if (reft.IsValid()) {
                        found = true;
                        ok = false;
                    }
                }
                //add the '.'
                nodeStrIndex += (StringHelper::Length(addressToSearch[i]) + 1u);
            }
            if (!found) {
                //test the last one
                found = (StringHelper::Compare(referenceToTest->GetName(), addressToSearch[currentIndex]) == 0);
                if (found) {
                    ReferenceT<T> reft = referenceToTest;
                    found = reft.IsValid();
                }
                if (found) {
                    nodeStrIndex += StringHelper::Length(addressToSearch[currentIndex]);

                    if (currentIndex < (addressNumberNodes - 1u)) {
                        nodeStrIndex++;
                    }
                }
            }
        }

    }
    else {
        //test the last one
        found = (StringHelper::Compare(referenceToTest->GetName(), addressToSearch[0]) == 0);
        if (found) {
            ReferenceT<T> reft = referenceToTest;
            found = reft.IsValid();
        }
        if (found) {
            nodeStrIndex = StringHelper::Length(addressToSearch[0]);
        }
    }

    if (found) {
        IncrementFound();
    }
    return found;
}

template<typename T>
uint32 ReferenceContainerFilterNameAndType<T>::GetRemainedAddrIndex() {
    return nodeStrIndex;
}

}

#endif /* REFERENCECONTAINERFILTEROBJECTNAME_H_ */

