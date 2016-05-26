/**
 * @file DataSourceBrokerI.h
 * @brief Header file for class DataSourceBrokerI
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

 * @details This header file contains the declaration of the class DataSourceBrokerI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCEBROKERI_H_
#define DATASOURCEBROKERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "RealTimeApplication.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The interface of the broker between GAMs and DataSource.
 */
class DataSourceBrokerI: public ReferenceContainer {

public:

    /**
     * @brief Sets the application where this object is involved in.
     * @param[in] rtApp is a Reference to the application to be set.
     */
    virtual void SetApplication(RealTimeApplication &app)=0;

    /**
     * @brief Links a GAM signal with a data source signal.
     * @details If \a ptr is NULL, this function allocates memory for the GAM signal otherwise
     * the GAM signal memory it is supposed to be associated to \a ptr then the allocation is not performed.
     * @param[in] def is the GAM signal to be added.
     * @param[in] ptr is the pointer to the GAM signal memory.
     * @return false in case of errors, true otherwise.
     */
    virtual bool AddSignal(Reference def,
                           void * const ptr = NULL_PTR(void*))=0;

    /**
     * @brief Returns the number of signals managed by this broker.
     * @return the number of signals managed by this broker.
     */
    virtual uint32 GetNumberOfSignals() =0;

    /**
     * @brief Retrieves the pointer to the \a n-th GAM signal.
     * @param[in] n is the variable position.
     * @return the pointer to the \a n-th GAM signal. NULL pointer is returned
     * if this broker is not finalised (see Finalise()) or if (n > GetNumberOfSignals()).
     */
    virtual void *GetSignal(const uint32 n) =0;

    /**
     * @brief Retrieves the signal with the specified name.
     * @details This function is slower than GetSignal(*) thus is not advisable using of it during real-time.
     * The parameter \a index in output retrieves the position of the signal in the broker in such a way it is possible
     * use the faster function GetSignal(\a index) to retrieve the signal memory pointer.
     * @param[in] name is the name of the signal to be searched.
     * @param[in] index is the position of the signal in the broker.
     * @return a pointer to the requested signal or NULL if the signal with the specified name is not found.
     */
    virtual void *GetSignalByName(const char8 * name,
                                  uint32 &index) =0;

    /**
     * @brief Retrieves the number of samples of a specific signal.
     * @param[in] n is the signal position in the broker.
     * @return The number of samples of a specific signal.
     */
    virtual uint32 GetSignalNumberOfSamples(const uint32 n)=0;

    virtual uint32 GetSignalSize(const uint32 n)=0;

    /**
     * @brief Finalises the object.
     * @details This function has to be called after all the GAM signals were added to the broker.
     * It checks if the broker contains more than one GAM signals who wants to perform a synchronised
     * operation.
     * @return false if more than one GAM signal wants to perform a synchronised operation. True otherwise.
     */
    virtual bool Finalise()=0;

    /**
     * @brief Checks if the broker manages a synchronised operation.
     * @return true if the broker manages a synchronised operation.
     */
    virtual bool IsSync() const=0;

    /**
     * @brief Reads a signal from the data source.
     * @param[in] activeDataSourceBuffer is the current active data source buffer index (0 or 1).
     * @param[in] timeout is the timeout in case of synchronous blocking operations.
     * @return true if the read operation succeeds, false otherwise,
     */
    virtual bool Read(const uint8 activeDataSourceBuffer,
                      const TimeoutType &timeout = TTInfiniteWait)=0;

    /**
     * @brief Writes a signal to the data source.
     * @param[in] activeDataSourceBuffer is the current active data source buffer index (0 or 1).
     * @param[in] timeout is the timeout in case of synchronous blocking operations.
     * @return true if the write operation succeeds, false otherwise,
     */
    virtual bool Write(const uint8 activeDataSourceBuffer,
                       const TimeoutType &timeout = TTInfiniteWait)=0;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_DATASOURCEBROKERI_H_ */

