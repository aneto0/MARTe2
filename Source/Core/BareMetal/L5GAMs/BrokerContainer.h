/**
 * @file BrokerContainer.h
 * @brief Header file for class BrokerContainer
 * @date 11/04/2016
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

 * @details This header file contains the declaration of the class BrokerContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BROKERCONTAINER_H_
#define BROKERCONTAINER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceBrokerI.h"
#include "ReferenceT.h"
#include "RealTimeApplication.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class BrokerContainer: public DataSourceBrokerI {

public:
    CLASS_REGISTER_DECLARATION()

    BrokerContainer();

    ~BrokerContainer();

    /**
     * @brief Links a GAM variable with the RealTimeDataSource.
     * @details If \a ptr is null, this function allocates memory for the variable and stores the pointer
     * in a vector. The pointer to the related variable memory, allocated in the RealTimeDataSource, will be
     * stored into another vector. If \a ptr is not NULL, this means that the GAM provides the memory of that
     * variable by itself, then the allocation is not performed.
     * @param[in] def is the variable definition.
     * @param[in] ptr is the pointer to the variable memory area.
     * @return false in case of errors, true otherwise.
     */
    virtual bool AddSignal(ReferenceT<GAMSignalI> gamSignalIn,
                           void * const ptr = NULL_PTR(void*));

    /**
     * @brief Retrieves the pointer to the \a i-th variable.
     * @param[in] i is the variable index.
     * @return the pointer to the \a i-th variable. NULL if this object is not finalised (see Finalise()).
     */
    virtual void *GetSignal(const uint32 i);

    virtual void *GetSignalByName(const char8 * name,
                                  uint32 &index);

    virtual uint32 GetSignalNumberOfSamples(const uint32 n);

    virtual uint32 GetNumberOfSignals();

    /**
     * @brief Finalises the object.
     * @details This function has to be called after all the variables are added. No more than one data source
     * has to be synchronizing for this link.
     * @return false in case of errors, true otherwise.
     */
    virtual bool Finalise();

    /**
     * @brief Returns true if this link is synchronized.
     */
    virtual bool IsSync() const;

    virtual bool Read(const uint8 activeDataSourceBuffer,
                      const TimeoutType &timeout = TTInfiniteWait);

    virtual bool Write(const uint8 activeDataSourceBuffer,
                       const TimeoutType &timeout = TTInfiniteWait);

    virtual bool InsertNewBroker(ReferenceT<DataSourceBrokerI> item);

    void SetAsInput(bool isInputIn);

    virtual void SetApplication(RealTimeApplication &app);

protected:

    ReferenceT<DataSourceBrokerI> *brokers;

    uint32 numberOfBrokers;

    bool synchronized;

    bool isInput;

    RealTimeApplication * application;

    uint32 numberOfSignals;

    StaticList<uint32> containerIndexer;

    StaticList<uint32> containerSignalIndexer;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_BROKERCONTAINER_H_ */

