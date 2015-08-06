/**
 * @file SearchFilter.h
 * @brief Header file for class SearchFilter
 * @date 06/08/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class SearchFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SEARCHFILTER_H_
#define SEARCHFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief The type of a function to be used to search on a set.
 * @param[in] data is a LinkedListable object pointer.
 */
typedef bool (SearchFilterFn)(LinkedListable *data);

/**
 * @brief Search filter object interface.
 */
class SearchFilter {
public:
    /**
     * @brief Destructor.
     */
    virtual ~SearchFilter() {

    }

    /**
     * @brief The function that performs the search on a set of data.
     * @param[in] data is a LinkedListable object pointer.
     */
    virtual bool Test(LinkedListable *data)=0;
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SEARCHFILTER_H_ */

