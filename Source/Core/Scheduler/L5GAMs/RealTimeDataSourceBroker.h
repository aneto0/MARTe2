/**
 * @file RealTimeDataSourceBroker.h
 * @brief Header file for class RealTimeDataSourceBroker
 * @date 09/03/2016
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

 * @details This header file contains the declaration of the class RealTimeDataSourceBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATASOURCEBROKER_H_
#define REALTIMEDATASOURCEBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicRealTimeDataSourceBroker.h"
#include "EventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The interface between GAMs and RealTimeDataSource.
 */
class RealTimeDataSourceBroker: public BasicRealTimeDataSourceBroker {

public:

    /**
     * @brief Constructor.
     * @post
     *   GetData(*) == NULL &&
     *   GetMemoryPointer(*) == NULL
     */
    RealTimeDataSourceBroker();


    /**
     * @brief Finalises the object.
     * @details This function has to be called after all the variables are added.
     * @return false in case of errors, true otherwise.
     */
    virtual bool Finalise();

protected:

    EventSem * eventSem;


private:

    /**
     * @brief Links a GAM variable with the RealTimeDataSource.
     * @details This function can call itself recursively if the variable to be allocated is a structure.
     * @param[in] def is the variable definition.
     * @param[in] ptr is the pointer to be added in the \a GAMPointers array. If NULL, the memory
     * will be allocated.
     * @param[in] memStart is the begin of the variable memory area.
     * @return false in case of errors, true otherwise.
     */
    bool AddVariablePrivate(ReferenceT<RealTimeDataDefI> def,
                            void * ptr,
                            void * memStart);

    /**
     * @brief Retrieves the pointer at the \a n-th position before the Finalise().
     * @param[in] n is the index of the pointer to be returned.
     * @return the pointer at the \a n-th position.
     */
    void *GetMemoryPointerPrivate(const uint32 n) const;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCEBROKER_H_ */

