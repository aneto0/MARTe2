/**
 * @file MemoryMapDataSourceBroker.h
 * @brief Header file for class MemoryMapDataSourceBroker
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

 * @details This header file contains the declaration of the class MemoryMapDataSourceBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPDATASOURCEBROKER_H_
#define MEMORYMAPDATASOURCEBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceBrokerI.h"
#include "MemoryArea.h"
#include "ReferenceT.h"
#include "RealTimeApplication.h"
#include "DataSourceSignalI.h"
#include "FastPollingEventSem.h"
#include "DataSource.h"
#include "ConfigurationDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The communication interface between GAMs and DataSource.
 * @details The configuration of this element has to be performed after the
 * RealTimeApplication::ConfigureDataSource() step. It allows to read (write) data blocks
 * of each element and blocks of samples. During the configuration two arrays of pointers
 * will be created, one for the data source signals and the other for the GAM signals.
 * The read - write operation will copy the memory from one pointer to the other, for each of them
 * in this two arrays.\n
 * The Accepted field "Operation" of a GAMSignalI to initialise the broker, is the expression of a
 * nx2 matrix with the initial and final indexes for each block of data to be read (write) from (to) the
 * data source signal.
 */
class DLL_API MemoryMapDataSourceBroker: public DataSourceBrokerI {

public:

    /**
     * @brief Constructor.
     * @post
     *   GetData(*) == NULL &&
     *   GetMemoryPointer(*) == NULL
     */
    MemoryMapDataSourceBroker();

    /**
     * @brief Destructor
     */
    virtual ~MemoryMapDataSourceBroker();

    /**
     * @see DataSourceBrokerI::SetApplication(*)
     */
    virtual void SetApplication(RealTimeApplication &app);

    /**
     * @see DataSourceBrokerI::AddSignal(*)
     */
    virtual bool AddSignal(Reference def,
                           void * const ptr = NULL_PTR(void*));

    /**
     * @see DataSourceBrokerI::GetSignal(*)
     */
    virtual void *GetSignal(const uint32 n);

    /**
     * @see DataSourceBrokerI::GetSignalByName(*)
     */
    virtual void *GetSignalByName(const char8 * name,
                                  uint32 &index);

    /**
     * @see DataSourceBrokerI::GetSignalNumberOfSamples(*)
     */
    virtual uint32 GetSignalNumberOfSamples(const uint32 n);


    virtual uint32 GetSignalSize(const uint32 n);

    /**
     * @see DataSourceBrokerI::Finalise(*)
     */
    virtual bool Finalise();

    /**
     * @see DataSourceBrokerI::GetNumberOfSignals(*)
     */
    virtual uint32 GetNumberOfSignals();

    /**
     * @see DataSourceBrokerI::IsSync(*)
     */
    virtual bool IsSync() const;

    /**
     * @see DataSourceBrokerI::Read(*)
     */
    virtual bool Read(const uint8 activeDataSourceBuffer,
                      const TimeoutType &timeout = TTInfiniteWait);

    /**
     * @see DataSourceBrokerI::Write(*)
     */
    virtual bool Write(const uint8 activeDataSourceBuffer,
                       const TimeoutType &timeout = TTInfiniteWait);

protected:

    /**
     * The pointer to the RealTimeApplication
     */
    RealTimeApplication *application;

    /**
     * The memory area where variables will be allocated
     * on.
     */
    MemoryArea memory;

    /**
     * Stores the indexes of the allocations
     * in memory area.
     */
    StaticList<uint32> GAMOffsets;

    /**
     * Stores the pointers to the begin
     * of memory areas
     */
    StaticList<void *> beginPointers;

    /**
     * Stores the final pointers to the
     * GAM variables.
     */
    StaticList<void *> GAMPointers;

    /**
     * Stores the pointers to the DataSource
     * variables.
     */
    StaticList<void **> DSPointers[2];

    /**
     * Stores the data source signals
     */
    StaticList<DataSourceSignalI *> dataSourcesVars;

    /**
     * Stores the number of cycles to be performed for each operation
     */
    StaticList<uint32> numberOfCyclesPerVar;

    /**
     * Stores the indexes of each variable
     * into the GAMPointers.
     */
    StaticList<uint32> chunkIndex;

    /**
     * Denotes if the link is synchronized.
     */
    bool synchronized;

    /**
     * Specifies if Finalise() is called or not.
     */
    bool finalised;

    /**
     * The index of the synchronising data source
     */
    uint32 syncIndex;

    /**
     * Stores the element sub-blocks indexes
     */
    StaticList<uint32**> blockParamList;

    /**
     * Stores the samples blocks indexes
     */
    StaticList<uint32**> samplesParamList;

    /**
     * Stores the number of element sub-blocks
     */
    StaticList<uint32> blockParamRows;

    /**
     * Stores the number of samples blocks
     */
    StaticList<uint32> samplesParamRows;

    /**
     * Stores the number of samples of each GAM signal
     */
    StaticList<uint32> nSamplesList;


    StaticList<uint32> gamSignalSizes;

    /**
     * @brief Links a GAM signal with a data source signal.
     * @details This function can call itself recursively if the variable to be allocated is a structure.
     * @param[in] defIn is the GAM signal.
     * @param[in] initialOffset is the byte offset with respect the begin of memory where the signal memory
     * has to be allocated to.
     * @param[in] offset will be used in recursion to store the pointers of sub-members of structured types.
     * @param[in] allocate must be true if the GAM signal memory has to be allocated by this function, false otherwise.
     * @return false in case of errors, true otherwise.
     */
    virtual bool AddSignalPrivate(Reference defIn,
                                  uint32 initialOffset,
                                  uint32 offset,
                                  bool allocate);

    /**
     * @brief Retrieves the pointer at the \a n-th position before the Finalise().
     * @param[in] n is the index of the pointer to be returned.
     * @return the pointer at the \a n-th position.
     */
    virtual void *GetMemoryPointerPrivate(const uint32 n) const;

    /**
     * @brief Checks the GAM signal is compatible with its data source signal.
     * @param[in] defIn is the GAM signal.
     * @param[in, out] typeSize is the GAM signal type size in bytes.
     * @return the DataSourceSignalI signal which \a defIn is connected to.
     */
    virtual Reference Verify(Reference defIn,
                             uint32 &typeSize);

    /**
     * @brief Stores the indexes of the element sub-blocks and the
     * indexes of the samples blocks to be read/write.
     * @details Changes the \a typeSize to the total size of the memory which has to be
     * allocated for the GAM signal ((type size) * #(GAM signal samples) * #(GAM signal elements))
     * @param[in] defIn is the GAM signal.
     * @param[in] dsDefIn is the data source signal.
     * @param[in, out] typeSize is the GAM signal type size.
     */
    virtual bool SetBlockParams(Reference defIn,
                                Reference dsDefIn,
                                uint32 &typeSize);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_MEMORYMAPDATASOURCEBROKER_H_ */

