/**
 * @file CircularBufferThreadInputDataSource.h
 * @brief Header file for class CircularBufferThreadInputDataSource.
 * @date 12/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class
 * CircularBufferThreadInputDataSource with all of its public, protected and
 * private members. It may also include definitions for inline methods which
 * need to be visible to the compiler.
 */

#ifndef CIRCULARBUFFERTHREADINPUTDATASOURCE_H_
#define CIRCULARBUFFERTHREADINPUTDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedServiceMethodBinderI.h"
#include "FastPollingMutexSem.h"
#include "EventSem.h"
#include "MemoryDataSourceI.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Multi circular buffer data source interface. An internal thread acquires sequentially a sample of each signal using the interface \a DriverRead(*).
 *
 * @details This function has to be implemented for the specific data source. An internal counter (nBrokerOpPerSignalCounter) monitors how many brokers have read the target buffer. When this counter gets to zero the buffer
 *  is allowed to be written again by the thread, otherwise a buffer over-run will be generated.
 *
 * Two optional signals can be defined and are recognised by name:
 *   \a InternalTimeStamp: stores the HighResolutionTimer::Counter() time stamp of the signals once they have been read. It must be
 *     uint64 type and it must have a number of elements equal to the number of signals that the data source produces.
 *   \a ErrorCheck: provides an error flag for each signal. It must be uint32 type and it must have a number of elements equal to
 *     the number of the signals that the data source produces. Only 2 bits are used in this implementation:
 *       - bit 0: DriverRead(*) function returns false.
 *       - bit 1: Write overlap. Attempting to write on a sample that hasn't been read yet by the consumers.
 *
 * This data source also allows to specify signals as being "interleaved". In order to achieve this, each signal must declared a field named PacketMemberSizes which describes its structure.
 * As an example PacketMemberSizes={4,2,2,8} would mean that the signal (whose NumberOfElements shall be 16 for an uint8 type) is composed of an uint32, followed by 2 uint16 and finally followed by an uint64.
  * A series of protected accelerators (see numberOfInterleavedSamples, numberOfInterleavedSignalMembers and memberByteSize) allow specialised classes to use this information, together with the number of samples, to go from an interleaved memory representation to a non-interleaved representation (where each signal contains N consecutive samples of its type).
 *
 * If the parameter SignalDefinitionInterleaved is = 1, it is assumed that the defined signals form part of a packet that is interleaved (and replicated for N samples).
 * Again, the protected accelerators (see numberOfInterleavedSamples, numberOfInterleavedSignalMembers and memberByteSize) allow specialised classes to use this information.
 *
 * @details The configuration syntax is (names and signal quantity are only given as an example):
 * <pre>
 * +CircularBuffer_0 = {
 *     Class = (child of) CircularBufferThreadInputDataSource
 *     NumberOfBuffers = N (the number of buffers)
 *     *SignalDefinitionInterleaved = 0 (the listed signals, excluding the InternalTimeStamp and the ErrorCheck, define a packet that is replicated N samples times).
 *     *CpuMask = 0x1 (the cpus where the internal thread is allowed to run: default is 0xFFFF)
 *     *ReceiverThreadPriority = 0-99 (the priority of the internal thread, default is 99)
 *     *ReceiverThreadStackSize = N (the stack size of the internal thread, default is THREADS_DEFAULT_STACKSIZE)
 *     *SleepTime = 0 (the sleep time in mutex in seconds, default is 0.F)
 *     *SignalDefinitionInterleaved = 0/1 (if 0, default, it is assumed that the signal is not defined as interleaved)
 *     *sleepInMutexSec = 1e-6F (the sleep time in mutex in seconds, default is 1e-6F)
 *     *GetFirst = 0/1 (if 0, default, do not wait for the first valid buffer to arrive)
 *     Signals = {
 *         *InternalTimeStamp = {
 *             Type = uint64
 *             NumberOfDimensions = 1
 *             NumberOfElements = N //the number of signals belonging to this dataSource (except InternalTimeStamp and ErrorCheck)
 *         }
 *         *ErrorCheck = {
 *             Type = uint32
 *             NumberOfDimensions = 1
 *             NumberOfElements = N //the number of signals belonging to this dataSource (except InternalTimeStamp and ErrorCheck)
 *         }
 *         Signal1 = {
 *             Type = uint8
 *             NumberOfDimensions = 1
 *             NumberOfElements = 16
 *             PacketMemberSizes={4,2,2,8} //Optional and only relevant for interleaved signals.
 *             HeaderSize = 0 //Optional. If specified, HeaderSize bytes will be skipped from the signal when interleaving.
 *             Samples = 5 //Unpack the interleaved memory representation into individual signals.
 *         }
 *         ....
 *         ....
 *         ....
 *     }
 * }
 * </pre>
 */
class CircularBufferThreadInputDataSource: public MemoryDataSourceI, public EmbeddedServiceMethodBinderI {
public:

    /**
     * @brief Constructor
     * @details Initialises the internal state and parameters
     */
    CircularBufferThreadInputDataSource();

    /**
     * @brief Destructor
     * @details Frees the memory
     */
    virtual ~CircularBufferThreadInputDataSource();

    /**
     * @see MemoryDataSourceI::Intialise
     * @details Initialises the following parameters:
     *   CpuMask: a mask to select the cpus where the internal thread is allowed to be executed (default 0xFFFFu)\n
     *   ReceiverThreadPriority: the priority of the internal thread between 0 (min) and 31 (max) (default 31)\n
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Waits that the number of samples for the synchronising signal arrives.
     * @see DataSourceI::Synchronise
     * @details Denoting with N, the number of samples of the synchronising signal, this function waits that the last N samples arrives.
     *   If between two calls to this function, a number of samples M > N has arrived, it waits the other N-mod(M,N) samples before returning.
     * @details If the internal thread is faster then the readers and all the buffers are set as written but no read, this method fails to
     *   find the last buffer written by the internal thread and returns false.
     * @post
     *   lastReadBuffer[syncSignal] = the index of last buffer written by the internal thread for the synchronising signal.
     */
    virtual bool Synchronise();

    /**
     * @brief Returns the broker names for input and output signals.
     * @see DataSourceI::GetBrokerName
     * @details
     *    if (direction==InputSignals): if(Frequency>=0.F) then returns "MemoryMapSynchronisedMultiBufferInputBroker" else returns "MemoryMapMultiBufferInputBroker"\n
     *    if (direction==OutputSignals): returns NULL_PTR(const char8 *)
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    /**
     * @brief Since this is an input DataSource, no GAMs should require output brokers.
     * @return false.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr);

    /**
     * @brief Creates the memory for the internal state variables and checks the signals types and dimensions.
     * @see DataSourceI::SetConfiguredDatabase
     * @details Denoting with NumberOfChannels the number of signals except eventually the InternalTimeStamp and the ErrorCheck signals, checks that:
     *   InternalTimeStamp signal has (Type==uint64) && (NumberOfElements==NumberOfChannels)
     *   ErrorCheck signal has (Type==uint64) && (NumberOfElements==NumberOfChannels)
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Fills the state dependent variables and launches the internal thread.
     * @see StatefulI::PrepareNextState
     * @details Computes the \a triggerAfterNSamples fields with the maximum of the declared number of samples
     * for each signal and the \a nBrokerOpPerSignal fields with the number of operations that the brokers perform
     * for each signal (depending also by the number of ranges and samples of each operation). This allows to set the
     * buffers as read at the end of all the read operations made by brokers.
     * @post
     *   executor.Start()
     */
    virtual bool PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName);

    /**
     * @brief Sets the lastReadBuffer indexes to the last buffer written by the internal thread for all the signals.
     * @see DataSourceI::PrepareInputOffsets
     * @details This method is called by the MemoryMapMultiBufferInputBroker before the read operations to synchronise the buffers to the latest written.
     * post
     *   for each signal i: lastReadBuffer[i] = the index of last buffer written by the internal thread for the signal i.
     *
     */
    virtual void PrepareInputOffsets();

    /**
     * @brief Returns the offset to the last \a numberOfSamples written for the signal \a signalIdx.
     * @see DataSourceI::GetInputOffset
     */
    virtual bool GetInputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset);

    /**
     * @brief Returns the offset to the last \a numberOfSamples written for the signal \a signalIdx.
     * @see DataSourceI::GetOutputOffset
     */
    virtual bool GetOutputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset);

    /**
     * @brief The internal thread callback. It reads sequentially all the signals calling the DriverRead method.
     * @see EmbeddedServiceMethodBinderI::Execute
     * @details If exists, the InternalThread signal is filled with the HighResolutionTimer::Counter value after the read. The ErrorCheck signal, if exists, is filled
     * with the proper error code in case of errors.
     *
     * @details If DriverRead returns a read size different than the byte size for the signal i, the signal i is assumed to be not written and its buffer number (currentBuffer)
     * is not incremented. If DriverRead explicitly fails returning false, the buffer is not incremented and if the ErrorCheck signal exists, the error is reported with code 0x1.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * The low level interface to be implemented for the specific data source to read the signals.
     * @param[in, out] bufferToFill is the buffer to be filled with the raw data.
     * @param[in, out] sizeToRead specifies the signal's byte size in input and returns in output the number of bytes effectively read.
     * @param[in] signalIdx is the index of the signal to be read.
     */
    virtual bool DriverRead(char8 * const bufferToFill, uint32 &sizeToRead, const uint32 signalIdx)=0;

    /**
     * @see ReferenceContainer::Purge
     * @details Stops the execution of the internal thread.
     */
    virtual void Purge(ReferenceContainer &purgeList);

    /**
     * @brief Sets as read, all the samples read by the brokers for the signal \a signalIdx.
     * @see DataSourceI::TerminateInputCopy
     */
    virtual bool TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples);

protected:

    /**
     * The last buffer written by the internal thread
     */
    uint32 *currentBuffer;

    /**
     * The internal thread executor
     */
    SingleThreadService executor;

    /**
     * The spinlock used to guarantee mutual access to the
     * shared isRefreshed flag memory
     */
    FastPollingMutexSem mutex;

    /**
     * The flags to denote if a buffer is written or read
     * isRefreshed[i]=1: the buffer i is written by the internal thread.
     * isRefreshed[i]=0: the buffer i is read by the brokers.
     */
    uint8 *isRefreshed;

    /**
     * Denotes the last buffer read by the brokers.
     */
    uint32 *lastReadBuffer;

    /**
     * stores the previous value of lastReadBuffer.
     */
    uint32 *lastReadBuffer_1;

    /**
     * The number of samples of the sync.
     */
    uint32 triggerAfterNSamples;

    /**
     * The number of calls made by brokers for each signal.
     */
    uint32 *nBrokerOpPerSignal;

    /**
     * Initialised with the values of nBrokerOpPerSignal is decremented in TerminateInputCopy
     * and when it is zero, the read buffers are set as read for the specified signal.
     */
    uint32 *nBrokerOpPerSignalCounter;

    /**
     * The number of signals removing eventually the special InternalTimeStamp and ErrorCheck signals.
     */
    uint32 numberOfChannels;

    /**
     * The index of the synchronising signal.
     */
    uint32 syncSignal;

    /**
     * The index of the InternalTimeStamp signal.
     */
    uint32 timeStampSignalIndex;

    /**
     * The index of the ErrorCheck signal.
     */
    uint32 errorCheckSignalIndex;

    /**
     * How much time to sleep in lock.
     */
    float32 sleepInMutexSec;

    /**
     * The ratio between the signal size (NOfElements * TypeSize) / Size of the packet will define how many times the packet structure is repeated inside the memory. This is the content that will be transformed from interleaved to flat.
     */
    uint32 *numberOfInterleavedSamples;

    /**
     * Size of each interleaved packet (which is replicated N numberOfInterleavedSamples).
     */
    uint32 *interleavedSignalByteSize;

    /**
     * Number of members for each interleaved input signal.
     */
    uint32 *numberOfInterleavedSignalMembers;

    /**
     * Dimensions of the member sizes for each input signal.
     */
    uint32 *interleavedPacketMemberByteSize;

    /**
     * The size of the packet header.
     */
    uint32 *headerSize;

    /**
     * If true the signal definition is considered to be interleaved.
     */
    bool signalDefinitionInterleaved;

    /**
     * If true get wait for the  first valid buffer to arrive.
     */
    bool getFirst;

    /**
     * If not zero, stops the operations in Synchronise().
     */
    volatile int32 stop;
    
    /**
     * The time to sleep in seconds.
     */
    float32 sleepTime;


private:

    /**
     * @brief Helper function to create the interleaved accelerators
     */
    bool GenererateInterleavedAccelerators();

    /**
     * @brief Helper function to create the interleaved accelerators for the signalDefinitionInterleaved case
     */
    bool GenererateInterleavedAcceleratorsSignalDefinitionInterleaved();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CIRCULARBUFFERTHREADINPUTDATASOURCE_H_ */
