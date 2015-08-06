/**
 * @file SearchFilterT.h
 * @brief Header file for class SearchFilterT
 * @date 06/ago/2015
 * @author pc
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

 * @details This header file contains the declaration of the class SearchFilterT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SEARCHFILTERT_H_
#define SEARCHFILTERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Template search filter interface.
 */
template<typename T>
class SearchFilterT {
public:

    /**
     * @brief Destructor.
     */
    virtual ~SearchFilterT() {

    }

    /**
     * @brief The function that performs the search on the specified input parameter.
     * @param[in] data is a generic template type.
     */
    virtual bool Test(T data)=0;

    /**
     * @brief A more specialized form of the SearchFilterT::Test function to be used on certain applications.
     * @param[in] data is a generic template type.
     * @param[in] mode is a flag used to specify special operations.
     * @return a SFTestType code.
     */
    virtual SFTestType Test2(T data,
                             SFTestType mode = SFTTNull) {
        if (Test(data)) {
            return SFTTFound;
        }
        return SFTTNotFound;
    }
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SEARCHFILTERT_H_ */

