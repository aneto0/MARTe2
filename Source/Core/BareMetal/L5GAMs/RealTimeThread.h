/**
 * @file RealTimeThread.h
 * @brief Header file for class RealTimeThread
 * @date 19/02/2016
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

 * @details This header file contains the declaration of the class RealTimeThread
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMETHREAD_H_
#define REALTIMETHREAD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "RealTimeState.h"
#include "StreamString.h"
#include "GAM.h"
#include "ProcessorType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Defines the Function elements (aka GAMs) to be executed in a real-time thread.
 * @details The syntax in the configuration stream shall be:
 * +RealTimeThread_name = {\n
 *     Class = RealTimeThread\n
 *     Functions = { GAM1_name, GAMGroup2_name, ... }
 *     CPUs = 0xf //CPU affinity mask for the thread. Optional parameter.
 *     StackSize = 32768 //Stack size for the thread. Optional parameter.
 * }\n
 */
class DLL_API RealTimeThread: public ReferenceContainer {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     * @post
     *   GetFunctions() == NULL &&
     *   GetNumberOfFunctions() == 0 &&
     *   GetGAMs() == NULL &&
     *   GetNumberOfGAMs() == 0 &&
     *   GetCPU() == ProcessorType::GetDefaultCPUs() &&
     *   GetStackSize() == THREADS_DEFAULT_STACKSIZE
     */
    RealTimeThread();

    /**
     * @brief Destructor.
     * @post
     *   free(GetFunctions()) &&
     *   GetNumberOfFunction() == 0 &&
     *   free(GetGAMs()) &&
     *   GetNumberOfGAMs() == 0
     */
    virtual ~RealTimeThread();

    /**
     * @brief Reads the array with the GAM names to be executed by this thread.
     * @details The following fields must be defined:
     *
     *   Functions = { function1_path, function2_path, ... }
     *
     * Each element must be the path of the function to be executed by this thread with respect to the position
     * of the "Functions" node.
     *
     * The following fields can be defined
     *
     *   StackSize = (the memory stack size in byte to be associated to the this thread)
     *   CPUs = cpu mask where this thread is preferable to be executed (i.e 0x1 means the first cpu, 0x2 means the second, 0x3 first and second, ...).
     *
     * The default value for StackSize is THREADS_DEFAULT_STACKSIZE, while for CPUs is ProcessorType::GetDefaultCPUs().\n
     * @param[in] data is the StructuredData to be read from.
     * @return true if the parameters Functions is declared in \a data and the number of elements in Functions is greater than zero.
     * @post
     *   GetFunctions() != NULL  &&
     *   GetNumberOfFunctions() > 0
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Maps all the declared Function references (GAM, GAMGroup, ...) to this state.
     * @return true if all the declared Function references are valid AND if this thread can be
     * successfully added to the scheduler AND if all the declared GAMs support this \a rtState.
     * @pre
     *   Initialise() &&
     *   GetFunctions() != NULL  &&
     *   GetNumberOfFunctions() > 0
     * @post
     *   GetGAMs() != NULL &&
     *   GetNumberOfGAMs() != 0
     */
    bool ConfigureArchitecture();

    /**
     * @brief Returns the array with the name of the Functions (GAM, GAMGroup, ...) executed by this thread.
     */
    StreamString * GetFunctions();

    /**
     * @brief Returns the number of GAMs executed by this thread.
     * @return the number of GAMs executed by this thread.
     */
    uint32 GetNumberOfFunctions() const;

    /**
     * @brief Returns in the \a gamList the GAMs executed by this thread.
     * @param[out] gamList the list of GAMs executed by this thread.
     * @pre
     *    Initialise()
     */
    bool GetGAMs(ReferenceContainer &gamList);

    /**
     * @brief Retrieves the number of GAMs executed in this thread.
     * @return the number of GAMs executed in this thread.
     */
    uint32 GetNumberOfGAMs() const;

    /**
     * @brief Retrieves the stack size associated to this thread.
     * @return the stack size associated to this thread.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Retrieves the CPUs mask associated to this thread.
     * @return the CPUs mask associated to this thread.
     */
    ProcessorType GetCPU() const;

    /**
     * @see Object::ToStructuredData(*)
     */
    virtual bool ToStructuredData(StructuredDataI& data);

    /**
     * @see ReferenceContainer::Purge()
     */
    virtual void Purge(ReferenceContainer &purgeList);

private:

    /**
     * The array with the GAM names
     */
    StreamString* functions;

    /**
     * The number of GAMs
     */
    uint32 numberOfFunctions;

    /**
     * The GAMs to be executed by this thread.
     */
    ReferenceContainer GAMs;

    /**
     * The number of GAMs to be executed by this thread
     */
    uint32 numberOfGAMs;

    /**
     * The thread CPUs mask.
     */
    ProcessorType cpuMask;

    /**
     * The thread stack size.
     */
    uint32 stackSize;

    /**
     * Set to true after ConfigureArchitecture has been called at least once
     */
    bool configured;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMETHREAD_H_ */

