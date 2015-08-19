/**
 * @file ReferenceContainerFilterObjects.h
 * @brief Header file for class ReferenceContainerFilterObjects
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

 * @details This header file contains the declaration of the class ReferenceContainerFilterObjects
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
/**
 * @brief ReferenceContainerFilter for object names.
 */
class ReferenceContainerFilterObjectName: public ReferenceContainerFilter {
public:
    /**
     * @brief Default constructor.
     */
    ReferenceContainerFilterObjectName();

    /**
     * @brief Sets the filter to look for the \a address.
     * @param occurrenceNumber the ordinal occurrence number (i.e. find the first, the second, ...) of the finding of
     * a node which meets a given criteria or -1 to look for all occurrences. This parameter is indexed to 1.
     * @param modeToSet any ored combination of ReferenceContainerFilterMode::PATH, ReferenceContainerFilterMode::RECURSIVE,
     * ReferenceContainerFilterMode::REVERSE and ReferenceContainerFilterMode::REMOVE.
     * @param address a string with the name of the object to find or with the full path (from the root) to the object.
     * A full path is specified by separating with dots the path (e.g. A.B.C). If a full path is specified only the first
     * occurrence can be searched and the \a mode will be set to ReferenceContainerFilterMode::RECURSIVE and ReferenceContainerFilterMode::PATH.
     */
    ReferenceContainerFilterObjectName(const int32 &occurrenceNumber,
                                       const uint32 &modeToSet,
                                       const char8 * const address);

    /**
     * @brief Deep copy constructor.
     * @details Copies all the parameters from another filter. This instance of the filter will be Reset().
     * @param other the source ReferenceContainerFilterObjectName where to copy the information from.
     */
    ReferenceContainerFilterObjectName(const ReferenceContainerFilterObjectName &other);

    /**
     * @brief Assignment operator.
     * @details Copies all the parameters from another filter. This instance of the filter will be Reset().
     * @param other the source ReferenceContainerFilterObjectName where to copy the information from.
     */
    ReferenceContainerFilterObjectName &operator =(const ReferenceContainerFilterObjectName &other);

    /**
     * @brief Destructor. Free all the allocated memory.
     */
    virtual ~ReferenceContainerFilterObjectName();

    /**
     * @brief Tests if the name of the object hold by \a referenceToTest is compatible with the \address.
     * @details This method will return true when the name of object hold by \a referenceToTest is the same
     * of the last node in the address specified in the constructor. If a full path was specified as the address,
     * this method will return true if, and only if, all the elements in \a previouslyFound where found in the
     * exact same order before finding the element specified in \a referenceToTest.
     * Each time this method finds a matching reference the number of occurrences will be incremented.
     * @param previouslyFound the history of the path found so far.
     * @param referenceToTest the reference to test (where the name of object will be extracted from).
     * @return true if the name of the object hold by \a referenceToTest is compatible with the \address.
     */
    virtual bool Test(ReferenceContainer &previouslyFound, Reference &referenceToTest);

    /**
     * @brief Return true if the search is recursive in all the tree nodes or if a full path address was specified in the constructor.
     * @return true if the search is recursive in all the tree nodes or if a full path address was specified in the constructor.
     */
    virtual bool IsRecursive() const;

    /**
     * @brief Return true if all occurrences of a given pattern are to be searched and if a full path address was not specified in the constructor.
     * @return true if all occurrences of a given pattern are to searched and if a full path address was not specified in the constructor.
     */
    virtual bool IsSearchAll() const;

    /**
     * @brief Return true if all the nodes leading to a given occurrence are to be stored or if a full path address was specified in the constructor.
     * @return true true if all the nodes leading to a given occurrence are to be stored or if a full path address was specified in the constructor.
     */
    virtual bool IsStorePath() const;

private:
    /**
     * Broken-down list of the address to search.
     */
    char8 **addressToSearch;

    /**
     * Number of nodes in the address.
     */
    uint32 addressNumberNodes;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTEROBJECTNAME_H_ */

