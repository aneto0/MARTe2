/**
 * @file ReferenceContainerFilterReferences.h
 * @brief Header file for class ReferenceContainerFilterReferences
 * @date 13/08/2015
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

 * @details This header file contains the declaration of the class ReferenceContainerFilterReferences
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTERREFERENCES_H_
#define SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTERREFERENCES_H_

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
 * @brief ReferenceContainerFilter for reference searching.
 */
class ReferenceContainerFilterReferences: public ReferenceContainerFilter {
public:

    /**
     * @brief Default constructor. NOOP.
     */
    ReferenceContainerFilterReferences();

    /**
     * @brief Set the searching mode parameters.
     * @param occurenceNumber the ordinal occurrence number (i.e. find the first, the second, ...) of the finding of
     * a node which meets a given criteria or -1 to look for all occurrences. This parameter is indexed to 1.
     * @param modeToSet any ored combination of ReferenceContainerFilterMode::PATH, ReferenceContainerFilterMode::RECURSIVE,
     * ReferenceContainerFilterMode::REVERSE and ReferenceContainerFilterMode::REMOVE.
     * @param refToSearch the reference to be searched.
     */
    ReferenceContainerFilterReferences(const int32 &occurrenceNumber,
                                       const uint32 &mode,
                                       Reference refToSearch);

    /**
     * @brief Tests if the \a referenceToTest is equal to the \a referenceToSearch specified in the constructor.
     * @detail Each time this method finds a matching reference the number of occurrences will be incremented.
     * @param previouslyFound the history of the path found so far. Not used.
     * @param referenceToTest the reference to test.
     * @return true if \a referenceToTest == \a referenceToSearch.
     */
    virtual bool Test(ReferenceContainer &previouslyFound,
                      Reference &referenceToTest);

private:
    /**
     * The reference to be found.
     */
    Reference referenceToSearch;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTERREFERENCES_H_ */

