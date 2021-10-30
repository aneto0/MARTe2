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
#include "DataSourceI.h"
#include "Object.h"
#include "ExecutableI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Provides the copying mechanism between the DataSourceI and the GAM signals.
 * @details A class that implements this interface is capable of copying signals from a GAM to/from
 *  DataSourceI.
 */
class DLL_API BrokerI: public Object, public ExecutableI {

public:

    /**
     * @brief Constructor. NOOP.
     * @post
     *   GetNumberOfCopies() == 0
     */
    BrokerI();

    /**
     * @brief NOOP
     */
    virtual ~BrokerI();

    /**
     * @brief Initialises the BrokerI.
     * @details After this function is executed, the Broker is expected to be capable of copying signals
     * to/from a DataSourceI to/from a GAM (see InitFunctionPointers).
     * @param[in] direction the signal direction (InputSignals or OutputSignals).
     * @param[in] dataSourceIn the DataSourceI to be queried.
     * @param[in] functionName the name of the to which this BrokerI is being allocated to.
     * @param[in] gamMemoryAddress the base address of the GAM memory (where signal data is stored).
     * @return true if all the copy information related to \a functionName can be successfully retrieved.
     */
    virtual bool Init(SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 *const functionName,
                      void *gamMemoryAddress) = 0;

    /**
     * @brief Initialises the BrokerI.
     * @details After this function is executed, the Broker is expected to be capable of copying signals
     * to/from a DataSourceI to/from a GAM (see InitFunctionPointers).
     * @param[in] direction the signal direction (InputSignals or OutputSignals).
     * @param[in] dataSourceIn the DataSourceI to be queried.
     * @param[in] functionName the name of the to which this BrokerI is being allocated to.
     * @param[in] gamMemoryAddress the base address of the GAM memory (where signal data is stored).
     * @param[in] optim if true the algorithm tries to optimise the number of copies by means of grouping consecutive memory.
     * @return true if all the copy information related to \a functionName can be successfully retrieved.
     */
    virtual bool Init(SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 *const functionName,
                      void *gamMemoryAddress,
                      const bool optim);

    /**
     * @brief Gets the number of copy operations performed by this BrokerI.
     * @details If no Ranges are defined for any of the signals, the number of copies is equivalent
     *  to the number of signals assigned to this broker. Otherwise the number of copies will be
     *  greater than the number of signals.
     * @return the number of copy operations.
     * @pre
     *   InitFunctionPointers() &&
     *   copyIdx < GetNumberOfCopies()
     */
    uint32 GetNumberOfCopies() const;

    /**
     * @brief Gets the number of bytes to copy for the copy operation with index \a copyIdx.
     * @param[in] copyIdx Index of the current copy operation.
     * @return the number of bytes to copy for the operation \a copyIdx or 0 if copyIdx >= GetNumberOfCopies().
     * @pre
     *   InitFunctionPointers()
     */
    uint32 GetCopyByteSize(const uint32 copyIdx) const;

    /**
     * @brief Gets the number of bytes to offset (w.r.t. to the Signal base address in the DataSourceI memory) for the copy operation with index \a copyIdx.
     * @param[in] copyIdx Index of the current copy operation.
     * @return the number of bytes to offset for the copy operation \a copyIdx or 0 if copyIdx >= GetNumberOfCopies().
     * @pre
     *   InitFunctionPointers() &&
     *   copyIdx < GetNumberOfCopies()
     */
    uint32 GetCopyOffset(const uint32 copyIdx) const;

    /**
     * @brief Gets the dataSource signal index to be used for the copy with index \a copyIdx.
     * @param[in] copyIdx Index of the current copy operation.
     * @return dataSource signal index for the copy operation \a copyIdx or 0 if copyIdx >= GetNumberOfCopies().
     * @pre
     *   InitFunctionPointers() &&
     *   copyIdx < GetNumberOfCopies()
     */
    uint32 GetDSCopySignalIndex(const uint32 copyIdx) const;

    /**
     * @brief Gets the pointer in the GAM memory where the signal is to be copied to/from in the copy operation with index \a copyIdx.
     * @param[in] copyIdx Index of the current copy operation.
     * @return the number of bytes to offset for the copy operation \a copyIdx or NULL if copyIdx >= GetNumberOfCopies().
     * @pre
     *   InitFunctionPointers() &&
     *   copyIdx < GetNumberOfCopies()
     */
    void* GetFunctionPointer(const uint32 copyIdx) const;

    /**
     * @brief Gets the name of the function that owns the Broker.
     * @return the name of the function that owns the Broker.
     */
    StreamString GetOwnerFunctionName() const;

    /**
     * @brief Gets the name of the data source that owns the Broker.
     * @return the name of the data source that owns the Broker.
     */
    StreamString GetOwnerDataSourceName() const;

protected:
    /**
     * @brief Helper function which retrieves from the \a dataSource all the copy information related to the GAM.
     * @param[in] direction the signal direction (InputSignals or OutputSignals).
     * @param[in] dataSource the DataSourceI to be queried.
     * @param[in] functionName the name of the GAM  to which this BrokerI is being allocated to.
     * @param[in] gamMemoryAddress the base address of the GAM memory (where the GAM signal data is stored).
     * @return true if all the copy information related to \a functionName can be successfully retrieved.
     */
    bool InitFunctionPointers(const SignalDirection direction,
                              DataSourceI &dataSource,
                              const char8 *const functionName,
                              void *const gamMemoryAddress);

    /**
     * @brief Helper function which retrieves from the \a dataSource all the copy information related to the GAM.
     * @details This function optimise the number of copies to be perform by means of grouping consecutive memory. The concept of signals, samples and ranges are replaced by memory areas, therefore some dataSources
     * like \a MemoryMapInterpolatedInputBroker cannot benefit from this optimisation.
     * @param[in] direction the signal direction (InputSignals or OutputSignals).
     * @param[in] dataSource the DataSourceI to be queried.
     * @param[in] functionName the name of the GAM  to which this BrokerI is being allocated to.
     * @param[in] gamMemoryAddress the base address of the GAM memory (where the GAM signal data is stored).
     * @return true if all the copy information related to \a functionName can be successfully retrieved.
     */
    bool InitFunctionPointersOptim(const SignalDirection direction,
                                   DataSourceI &dataSource,
                                   const char8 *const functionName,
                                   void *const gamMemoryAddress);

    /**
     * Number of copy operations to be performed by this BrokerI.
     */
    uint32 numberOfCopies;

private:

    //Holds all the necessary information to build 1 operation of the copyTable
    struct memoryInfo {
        /**
         * Number of bytes to be copied on each copy operation (copyByteSize[numberOfCopies]).
         */
        uint32 copyByteSize;

        /**
         * Byte offset in the DataSource memory for each copy operation (copyOffset[numberOfCopies]).
         */
        uint32 copyOffset;

        /**
         * Initial signal index of the DataSoure memory  for each copy operation. If DS memory is consecutive, the copy may imply more than one signal, nevertheless the copySignalIndex[cpIdx] indicates the first index involved
         */
        uint32 copySignalIndex;

        /**
         * Base address of each copy operation in the GAM memory (functionSignalPointers[numberOfCopies]).
         */
        void *functionSignalPointers;
    };
    /**
     * Holds all the necessary information to build the copyTable for all brokers.
     */
    memoryInfo *copyTableInfo;
    /**
     * The data source holder
     */
    StreamString ownerDataSourceName;

    /**
     * The function name
     */
    StreamString ownerFunctionName;

    struct basicCopyTable {
        /**
         * The pointer to the GAM.
         */
        void *gamPointer;
        /**
         * The pointer to the datasource
         */
        void *dataSourcePointer;
        /**
         * The size of the copy of the signalDSIdx
         */
        uint32 copySize;

        /**
         * signal dataSource index. It indexes all the dataSource signals
         */
        uint32 signalDSIdx;

        /*
         * Byte offset from the beginning of the signal specified by signalDSIdx
         */
        uint32 signalDSByteOffsets;
    };

    /**
     * @brief Get the number of copies before copy optimisation.
     * @details The function interprets each sample as a possible different copy.
     * @return the number of copies before copy optimisation
     */
    uint32 GetNumberOfRawCopies(const SignalDirection direction,
                                const uint32 functionIdx,
                                const uint32 functionNumberOfSignals,
                                DataSourceI &dataSource,
                                const char8 *const brokerClassName) const;
    /**
     * @brief Collects all the information about the all possible copies (before optimisation).
     * @details fills bcp with relevant information about the possible copy memory operations (@see basicCopyTable for more details about the information needed).
     * @param[in] direction direction of the signals. It can be InputSignals or OutputSignals
     * @param[in] functionIdx is the function index which indicates the GAM involved in the copy operation. One GAM can interact with more than one dataSource
     * @param[in] functionNumberOfSignals is the number of signals which are used by the function with functionIdx an the dataSouce. Notice that maybe the signals uses different brokers.
     * @param[in] dataSouce is the dataSource involved in the copy operation. One dataSource interact with more than function.
     * @param[in] brokerClassName is the name of the broker used to interact between the GAM and the dataSouce. More than one broker may interact with the same GAM and dataSource.
     * @param[in] gamMemoryAddress pointer to the beginning of the GAM memory.
     * @param[out] bcp basic copy table which hold information about all the possible copy operations specified by the direction, which involves the function with functionIdx, the dataSouce and the broker brokerClassName.
     * @return true if the bcp is filled without errors.
     * @pre
     *    bcp memory shall be large enough to hold all the possible copy elements. GetNumberOfRawCopies() can be used to pre-calculate the bcp size.
     */
    bool FillCopyTable(const SignalDirection direction,
                       const uint32 functionIdx,
                       const uint32 functionNumberOfSignals,
                       DataSourceI &dataSource,
                       const char8 *const brokerClassName,
                       void *const gamMemoryAddress,
                       basicCopyTable *const bcp) const;

    /**
     * @brief sort, in ascendent GAM memory order, the bcp.
     * @param[in, out] bcp hold the information to be sorted.
     * @param[in] lements number of elements of the bcp.
     * @pre
     * bcp shall be already filled with valid information.
     */
    void SortByGAMAddress(basicCopyTable *const bcp,
                          const uint32 elements) const;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCEBROKERI_H_ */

