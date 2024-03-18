/**
 * @file ReferenceContainerFilterObjectName.h
 * @brief Header file for class ReferenceContainerFilterObjectName
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
 * @details This header file contains the declaration of the class ReferenceContainerFilterObjectName
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTEROBJECTNAME_H_
#define SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTEROBJECTNAME_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerFilter.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief ReferenceContainerFilter for object names.
 */
class DLL_API ReferenceContainerFilterObjectName: public ReferenceContainerFilter {
public:

    /**
     * @brief Default constructor.
     */
    ReferenceContainerFilterObjectName();

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
    ReferenceContainerFilterObjectName(const int32 &occurrenceNumber,
                                       const uint32 &modeToSet,
                                       const char8 * const address);

    /**
     * @brief Deep copy constructor.
     * @details Copies all the parameters from another filter. This instance of the filter will be Reset().
     * @param[in] other the source ReferenceContainerFilterObjectName where to copy the information from.
     */
    ReferenceContainerFilterObjectName(const ReferenceContainerFilterObjectName &other);

    /**
     * @brief Assignment operator.
     * @details Copies all the parameters from another filter. This instance of the filter will be Reset().
     * @param[in] other the source ReferenceContainerFilterObjectName where to copy the information from.
     * @return this object.
     */
    ReferenceContainerFilterObjectName &operator =(const ReferenceContainerFilterObjectName &other);

    /**
     * @brief Destructor. Frees all the allocated memory.
     */
    virtual ~ReferenceContainerFilterObjectName();

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
     * @brief Sets the address of the object to search.
     * @param[in] address the address of the object to search.
     */
    virtual void SetAddress(const char8 * const address);

protected:

    /**
     * Broken-down list of the address to search.
     */
    char8 **addressToSearch;

    /**
     * Number of nodes in the address.
     */
    uint32 addressNumberNodes;

    /**
     * @brief Verifies if the previouslyFound path is consistent with the originally set address.
     * @param[in] previouslyFound a container with all the nodes that make the path leading to the node to be tested.
     * @return true if all all the elements in previouslyFound are in the exact same order specified in
     * the address parameter of the constructor.
     */
    bool TestPath(ReferenceContainer &previouslyFound) const;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCECONTAINERFILTEROBJECTNAME_H_ */

