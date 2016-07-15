/**
 * @file BrokerI.h
 * @brief Header file for class eBrokerI
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

#ifndef BROKERI_H_
#define BROKERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceSignal.h"
#include "GAMSignalI.h"
#include "RealTimeApplication.h"
#include "ReferenceContainer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Defines the interface between DataSourceSignalI and GAMSignalI signals.
 * @details A class that implements this interface is capable of connecting signals from
 *  DataSourceSignalI components with signals from GAMSignalI components.
 */
class BrokerI : public ReferenceContainer {

public:

    BrokerI();

    virtual ~BrokerI();

    virtual bool InitFromDataSource(ReferenceT<DataSourceI> dataSourceIn, SignalDirection direction, const char8 * const functionName) = 0;

#if 0
    /**
     * @brief Sets the application where this object is involved in.
     * @param[in] rtApp is a Reference to the application to be set.
     */
    void SetApplication(RealTimeApplication &app);

    /**
     * @brief Links a GAMSignalI with a DataSourceSigmalI (at location GAMSignalI::GetPath).
     * @details The path retrieved by GAMSignalI::GetPath must exist and
     * the DataSourceSignalI at this path must be compatible (type, dimension, ...).
     * Moreover, DataSourceSignalI::IsSupportedBroker must support this DataSourceBrokerI.
     * @param[in] gamSignalIn is the GAM signal to be added.
     * @param[in] ptr is the pointer to the GAM signal memory.
     * @return true if the signals can be successfully linked.
     */
    virtual bool AddSignal(ReferenceT<GAMSignalI> gamSignalIn,
                           void * const ptr = NULL_PTR(void*));

    /**
     * @brief Retrieves the signal with the specified name.
     * @details This function is slower than GetSignal(*) thus it is not advisable to use in real-time.
     * The parameter \a index in output retrieves the position of the signal in the broker. This allows
     * to use the faster function GetSignal(\a index) to retrieve the signal memory pointer.
     * @param[in] name is the name of the signal to be searched.
     * @param[out] index is the position of the signal in the broker.
     * @return a pointer to the requested signal or NULL if the signal with the specified name is not found.
     */
    virtual void *GetSignalByName(const char8 * name,
                                  uint32 &index);

    /**
     * @brief Finalises the interface.
     * @details This function is called after all the GAM signals have been added to the broker and allows the
     * final implementation of the class to perform any required actions.
     * @return false if errors are detected after all the addition of all the GAM signals.
     */
    virtual bool Finalise();

    /**
     * @brief Checks if the broker manages a synchronised operation.
     * @details A synchronised operation is one where the reading of a GAMSignalI will trigger the call
     * to a DataSourceSignalI::WaitOnEvent.
     * @return true if the broker manages a synchronised operation.
     */
    virtual bool IsSync() const=0;

    /**
     * @brief Copies the memory from a DataSourceSignalI into the memory of a GAMSignalI previously
     *  added with AddSignal.
     * @param[in] activeDataSourceBuffer is the current active data source buffer index (0 or 1).
     * @param[in] timeout is the timeout in the case of a synchronous blocking operations.
     * @return true if the read operation succeeds, false otherwise,
     */
    virtual bool Read(const uint8 activeDataSourceBuffer,
                      const TimeoutType &timeout = TTInfiniteWait)=0;

    /**
     * @brief Copies the memory from a GAMSignalI into the memory of a DataSourceSignalI previously
     *  added with AddSignal.
     * @param[in] activeDataSourceBuffer is the current active data source buffer index (0 or 1).
     * @param[in] timeout is the timeout in the case of synchronous blocking operations.
     * @return true if the write operation succeeds, false otherwise,
     */
    virtual bool Write(const uint8 activeDataSourceBuffer,
                       const TimeoutType &timeout = TTInfiniteWait)=0;

    uint32 GetNumberOfSamplesBlocks(uint32 signalIndex);

    uint32 GetNumberOfIndexBlocks(uint32 signalIndex);

    uint32 GetNumberOfSignals();

    void *GetDataSourcePointer(uint32 signalIndex,
                               uint32 buffer);

    void *GetDataSourceSignalPointer(uint32 signalIndex,
                                            uint32 sampleBlockNumber,
                                            uint32 indexBlockNumber,
                                            uint32 buffer);

    void *GetGAMSignalPointer(uint32 signalIndex);

    void *GetGAMSignalPointer(uint32 signalIndex,
                              uint32 sampleBlockNumber,
                              uint32 indexBlockNumber);

    //Start index as position in memory! (&char[startIndex])
    bool GetIndexBlockStartIndex(uint32 signalIndex,
                                 uint32 blockIndex,
                                 uint32 &startIndex);

    //Size of the signal in bytes!
    bool GetIndexBlockSize(uint32 signalIndex,
                           uint32 blockIndex,
                           uint32 &blockSize);

    //Start index as position in memory! (&char[startIndex])
    bool GetSamplesBlockStartIndex(uint32 signalIndex,
                                   uint32 blockIndex,
                                   uint32 &startIndex);

    //Size of the samples in bytes!
    bool GetSamplesBlockSize(uint32 signalIndex,
                             uint32 blockIndex,
                             uint32 &size);

    bool IsFinalised();

    void SetFinalised(bool isFinalised);

    uint32 GetTotalNumberOfSampleBlocks();

    uint32 GetTotalNumberOfIndexBlocks();


    ReferenceT<DataSourceSignal> GetDataSourceSignal(ReferenceT<GAMSignalI> gamSignal);

private:
    /**
     * @brief Links a GAM signal with a data source signal.
     * @details This function can call itself recursively if the variable to be allocated is a structure.
     * @param[in] gamSignalIn is the GAM signal.
     * @param[in] initialOffset is the byte offset with respect the begin of memory where the signal memory
     * has to be allocated to.
     * @param[in] offset will be used in recursion to store the pointers of sub-members of structured types.
     * @param[in] allocate must be true if the GAM signal memory has to be allocated by this function, false otherwise.
     * @return false in case of errors, true otherwise.
     */
    virtual bool AddSignalPrivate(ReferenceT<GAMSignalI> gamSignalIn,
                                  void *gamSignalMemory,
                                  uint32 initialOffset,
                                  uint32 offset);

    /**
     * @brief Stores the indexes of the element sub-blocks and the
     * indexes of the samples blocks to be read/write.
     * @details Changes the \a typeSize to the total size of the memory which has to be
     * allocated for the GAM signal ((type size) * #(GAM signal samples) * #(GAM signal elements))
     * @param[in] defIn is the GAM signal.
     * @param[in] dsDefIn is the data source signal.
     * @param[in, out] typeSize is the GAM signal type size.
     */
    virtual bool SetBlockParameters(ReferenceT<GAMSignalI> gamSignal,
                                    ReferenceT<DataSourceSignal> dataSourceSignal,
                                    uint32 &typeSize);

    /**
     * The pointer to the RealTimeApplication
     */
    RealTimeApplication *application;

    /**
     * The number of samples blocks defined for each GAMSignalI
     */
    StaticList<uint32> nSamplesBlocksPerSignal;

    /**
     * The number of index blocks defined for each GAMSignalI
     */
    StaticList<uint32> nIndexBlocksPerSignal;

    /**
     * Offset of each signal w.r.t. to the GAMSignalI (zero if the first member or if the signal is a BasicType).
     */
    StaticList<uint32> signalOffsetInsideGAMSignalI;

    /**
     * The signal index (in this BrokerI instance) of the first structure member of a GAMSignalI.
     * Note that in the case of structures the same GAMSignalI can add several members.
     */
    StaticList<uint32> signalIndexOfFirstMemberInStructure;

    /**
     * Stores the element sub-blocks indexes
     */
    StaticList<uint32**> indexBlocksList;

    /**
     * Stores the samples blocks indexes
     */
    StaticList<uint32**> samplesBlocksList;

    /**
     * Stores the number of cycles that a given GAMSignalI wants to read from a DataSourceSignal
     */
    StaticList<uint32> nCyclesPerGAMSignalList;

    /**
     * Stores the total number of samples per cycle (i.e. the sum of all nIndexBlocksPerSignal * indexBlocksList) that a given GAMSignalI wants to read from a DataSourceSignal.
     */
    StaticList<uint32> nSamplesPerGAMSignalList;

    /**
     * Pointers to each signal in the GAM memory. Note that a GAMSignal may contain a structure with several signals.
     */
    StaticList<void *> gamSignalPointers;

    /**
     * Stores the pointers to the DataSource memory area variables.
     */
    StaticList<void *> dataSourceSignalPointers[2];

    /**
     * All the GAMSignalI that belong to this broker.
     */
    ReferenceContainer gamSignalList;

    /**
     * True if the method Finalise was already called.
     */
    bool finalised;
#endif


};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCEBROKERI_H_ */

