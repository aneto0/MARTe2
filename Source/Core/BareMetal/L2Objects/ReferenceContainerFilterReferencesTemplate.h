/**
 * @file ReferenceContainerFilterReferencesTemplate.h
 * @brief Header file for class ReferenceContainerFilterReferencesTemplate
 * @date 08/06/2015
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

 * @details This header file contains the declaration of the class ReferenceContainerFilterReferencesTemplate
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCECONTAINERFILTERREFERENCESTEMPLATE_H_
#define REFERENCECONTAINERFILTERREFERENCESTEMPLATE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainerFilter.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief ReferenceContainerFilter for reference searching with templates.
 */
template<typename T>
class ReferenceContainerFilterReferencesTemplate: public ReferenceContainerFilter {
public:

    /**
     * @brief Default constructor. NOOP.
     */
    ReferenceContainerFilterReferencesTemplate();

    /**
     * @brief Sets the searching mode parameters.
     * @param[in] occurrenceNumber the ordinal occurrence number (i.e. find the first, the second, ...) of the finding of
     * a node which meets a given criteria or -1 to look for all occurrences. This parameter is indexed to 1.
     * @param[in] modeToSet any ored combination of ReferenceContainerFilterMode::PATH, ReferenceContainerFilterMode::RECURSIVE,
     * ReferenceContainerFilterMode::REVERSE and ReferenceContainerFilterMode::REMOVE.
     */
    ReferenceContainerFilterReferencesTemplate(const int32 &occurrenceNumber,
                                               const uint32 &modeToSet);

    /**
     * @brief Tests if the \a referenceToTest is of type T.
     * @details Each time this method finds a matching reference the number of occurrences will be incremented.
     * @param[in] previouslyFound the history of the path found so far. Not used.
     * @param[in] referenceToTest the reference to test.
     * @return true if \a referenceToTest is of type T.
     */
    virtual bool Test(ReferenceContainer &previouslyFound,
                      Reference const &referenceToTest);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
ReferenceContainerFilterReferencesTemplate<T>::ReferenceContainerFilterReferencesTemplate() :
        ReferenceContainerFilter() {

}

template<typename T>
ReferenceContainerFilterReferencesTemplate<T>::ReferenceContainerFilterReferencesTemplate(const int32 &occurrenceNumber,
                                                                                          const uint32 &modeToSet) :
        ReferenceContainerFilter(occurrenceNumber, modeToSet) {
}

template<typename T>
bool ReferenceContainerFilterReferencesTemplate<T>::Test(ReferenceContainer &previouslyFound,
                                                                 Reference const &referenceToTest){
    ReferenceT<T> referenceToSearch = referenceToTest;
    return referenceToSearch.IsValid();
}
}

#endif /* REFERENCECONTAINERFILTERREFERENCESTEMPLATE_H_ */

