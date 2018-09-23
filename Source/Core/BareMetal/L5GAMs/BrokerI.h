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
                      CCString functionName,
                      void *gamMemoryAddress) = 0;

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
     * @return the number of bytes to copy for the operation \a copyIdx or 0 if copyIdx >= GetNumberOfCopies().
     * @pre
     *   InitFunctionPointers()
     */
    uint32 GetCopyByteSize(const uint32 copyIdx) const ;

    /**
     * @brief Gets the number of bytes to offset (w.r.t. to the Signal base address in the DataSourceI memory) for the copy operation with index \a copyIdx.
     * @return the number of bytes to offset for the copy operation \a copyIdx or 0 if copyIdx >= GetNumberOfCopies().
     * @pre
     *   InitFunctionPointers() &&
     *   copyIdx < GetNumberOfCopies()
     */
    uint32 GetCopyOffset(const uint32 copyIdx) const;

    /**
     * @brief Gets the pointer in the GAM memory where the signal is to be copied to/from in the copy operation with index \a copyIdx.
     * @return the number of bytes to offset for the copy operation \a copyIdx or NULL if copyIdx >= GetNumberOfCopies().
     * @pre
     *   InitFunctionPointers() &&
     *   copyIdx < GetNumberOfCopies()
     */
    void *GetFunctionPointer(const uint32 copyIdx) const;

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
                              CCString functionName,
                              void * const gamMemoryAddress);

    /**
     * Number of copy operations to be performed by this BrokerI.
     */
    uint32 numberOfCopies;

private:

    /**
     * Number of bytes to be copied on each copy operation (copyByteSize[numberOfCopies]).
     */
    uint32 *copyByteSize;

    /**
     * Byte offset in the DataSource memory for each copy operation (copyOffset[numberOfCopies]).
     */
    uint32 *copyOffset;

    /**
     * Base address of each copy operation in the GAM memory (functionSignalPointers[numberOfCopies]).
     */
    void **functionSignalPointers;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCEBROKERI_H_ */

