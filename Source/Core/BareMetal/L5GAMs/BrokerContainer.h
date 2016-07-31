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
#include "BrokerI.h"
#include "RealTimeApplication.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A container of DataSourceBrokerI objects.
 * @details This class is a container of DataSourceBrokerI and implements the
 * DataSourceBrokerI interface itself so that it queries and/or propagates
 * all the interface function calls to all of its DataSourceBrokerI contained objects.
 */
class BrokerContainer: public BrokerI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     * @post
     *   GetNumberOfSignals() == 0 &&
     *   IsSync() == false
     */
    BrokerContainer();

    /**
     * @brief Frees allocated memory.
     */
    ~BrokerContainer();

    /**
     * @brief Calls DataSourceBrokerI::AddSignal on all the contained brokers.
     * @details This function starts by checking if it already contains a DataSourceBrokerI
     * which is compatible with \a gamSignalIn. It it does then it calls DataSourceBrokerI::AddSignal
     *  on this broker. If it doesn't then it looks for the DataSourceSignalI associated to \a gamSignalIn and
     *  asks for a DataSourceBrokerI (GetInputReader or GetOutputWriter) which is compatible with \a gamSignalIn.
     * @param[in] gamSignalIn the GAMSignalI to add.
     * @param[in] ptr pointer to the variable memory area of the signal.
     * @return false if \a gamSignalIn could not be added to any of the DataSourceBrokerI supported by
     *  this \a gamSignalIn, true otherwise.
     */
    /*virtual bool AddSignal(ReferenceT<GAMSignalI> gamSignalIn,
                           void * const ptr = NULL_PTR(void*));*/

    /**
     * @see DataSourceBrokerI::GetSignal
     */
    virtual void *GetSignal(const uint32 i);

    /**
     * @see DataSourceBrokerI::GetSignal
     */
    virtual void *GetSignalByName(const char8 * name,
                                  uint32 &index);

    /**
     * @see DataSourceBrokerI::GetSignalNumberOfSamples
     */
    //virtual uint32 GetSignalNumberOfSamples(const uint32 n);

    /**
     * @see DataSourceBrokerI::GetNumberOfSignals
     */
    virtual uint32 GetNumberOfSignals();

    /**
     * @brief Finalises all the contained DataSourceBrokerI.
     * @details Calls DataSourceBrokerI::Finalise on all the contained DataSourceBrokerI elements.
     * @return true if all DataSourceBrokerI::Finalise calls succeed and at most one DataSourceBrokerI::IsSync() exists.
     */
    virtual bool Finalise();

    /**
     * @brief Return true if one (and only one) of the contained DataSourceBrokerI manages a synchronised operation.
     * @return true if one (and only one) of the contained DataSourceBrokerI manages a synchronised operation (DataSourceBrokerI::IsSync())
     */
    virtual bool IsSync() const;

    /**
     * @brief Calls DataSourceBrokerI::Read on all the contained DataSourceBrokerI elements.
     * @return true if all DataSourceBrokerI::Read calls succeed.
     */
    virtual bool Read(const uint8 activeDataSourceBuffer,
                      const TimeoutType &timeout = TTInfiniteWait);

    /**
     * @brief Calls DataSourceBrokerI::Write on all the contained DataSourceBrokerI elements.
     * @return true if all DataSourceBrokerI::Write calls succeed.
     */
    virtual bool Write(const uint8 activeDataSourceBuffer,
                       const TimeoutType &timeout = TTInfiniteWait);

    /**
     * @see DataSourceBrokerI::GetSignal
     */
    virtual void SetApplication(RealTimeApplication &app);

    /**
     * @brief Declares that all the signals to be added are to be either Read (isInputIn = true) or Write (isInputIn = false).
     * @param isInputIn if this parameter is true then in AddSignal the DataSourceSignalI::GetInputReader
     *  will be used to retrieve the DataSourceBrokerI to be added to the broker list, otherwise
     *  DataSourceBrokerI::GetOutputWriter will be called.
     */
    void SetInput(bool isInputIn);

    /**
     * @brief Inserts a new DataSourceBrokerI to the contained elements.
     * @return true if the broker is successfully added.
     */
    bool InsertNewBroker(ReferenceT<BrokerI> item);

private:

    /**
     * List of brokers held by this container.
     */
    ReferenceT<BrokerI> *brokers;

    /**
     * Number of brokers held by this container.
     */
    uint32 numberOfBrokers;

    /**
     * True if one (and only one) of the contained DataSourceBrokerI manages a synchronised operation.
     */
    bool synchronized;

    /**
     * The RealTimeApplication associated with this BrokerContainer.
     */
    RealTimeApplication * application;

    /**
     * Total number of signals in all the contained brokers.
     */
    uint32 numberOfSignals;

    /**
     * The index of the container (in the brokers list) where the signal i-th can be found.
     */
    StaticList<uint32> containerIndexer;

    /**
     * The index inside brokers[containerIndexer.Get(i)] where signal i-th can be found.
     */
    StaticList<uint32> containerSignalIndexer;

    /**
     * True if this BrokerContainer holds DataSourceBrokerI objects for reading (otherwise it is assumed that this
     * BrokerContainer holds DataSourceBrokerI objects for writing).
     */
    bool isInput;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BROKERCONTAINER_H_ */

