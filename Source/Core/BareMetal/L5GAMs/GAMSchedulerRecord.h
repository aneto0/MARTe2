/**
 * @file GAMSchedulerRecord.h
 * @brief Header file for class GAMSchedulerRecord
 * @date 22/mar/2016
 * @author pc
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

 * @details This header file contains the declaration of the class GAMSchedulerRecord
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSCHEDULERRECORD_H_
#define GAMSCHEDULERRECORD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "RealTimeThread.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe{

/**
 * @brief Contains the RealTimeThreads to be executed in a specific RealTimeState.
 */
class DLL_API GAMSchedulerRecord: public ReferenceContainer {

public:

    /**
     * @brief Constructor
     *   GetNumberOfThreads() == 0 &&
     */
    GAMSchedulerRecord();

    /**
     * @brief Destructor. Frees the threads array memory.
     */
    virtual ~GAMSchedulerRecord();

    /**
     * @brief Adds a new thread to the internal array.
     * @param[in] newThread is the new RealTimeThread to be added.
     */
    void AddThread(ReferenceT<RealTimeThread> newThread);

    /**
     * @brief Peeks a specific thread from the internal array.
     * @param[in] index is the position for the required thread inside the array.
     * @return a Reference to the thread in the \a index position of the internal array. The Reference
     * will be invalid if \a index > GetNumberOfThreads.
     */
    ReferenceT<RealTimeThread> Peek(uint32 index);

    /**
     * @brief Retrieves the number of threads stored in the internal array.
     * @return the number of threads stored in the internal array.
     */
    uint32 GetNumberOfThreads() const;

protected:

    /**
     * The RealTimeThread array.
     */
    ReferenceT<RealTimeThread> *threads;

    /**
     * The number of stored RealTimeThreads.
     */
    uint32 numberOfThreads;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSCHEDULERRECORD_H_ */

