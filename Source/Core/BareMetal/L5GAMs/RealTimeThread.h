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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A container of GAM references.
 * @details The syntax in the configuration stream should be:
 * Thread_name = {\n
 *     Class = RealTimeThread\n
 *     RealTimeThread_name = {\n
 *         Class = RealTimeThread\n
 *         Functions = { GAM1_name, GAMGroup2_name, ... }
 *         ...\n
 *     }\n
 *     ...\n
 * }\n
 */
class RealTimeThread: public ReferenceContainer {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     * @post
     *   GetFunctions() == NULL &&
     *   GetNumberOfFunction == 0;
     */
    RealTimeThread();

    /**
     * @brief Destructor. Frees the array with the function names.
     */
    ~RealTimeThread();

    /**
     * @see RealTimeApplication::Validate()
     */
    bool ConfigureArchitecture(RealTimeApplication &rtApp,
                               RealTimeState &rtState);

    /**
     * @brief Reads the array with the GAM names to be launched by this thread from the StructuredData in input.
     * @param[in] data is the StructuredData to be read from.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Returns the array with the name of the GAMs involved by this thread.
     */
    StreamString * GetFunctions() const;

    /**
     * @brief Returns the number of GAMs involved by this thread.
     */
    uint32 GetNumberOfFunctions() const;

    ReferenceT<GAM> *GetGAMs() const;

    uint32 GetNumberOfGAMs() const;

    virtual bool ToStructuredData(StructuredDataI& data);

private:

    bool ConfigureArchitecturePrivate(Reference functionGeneric,
                                      RealTimeApplication &rtApp,
                                      RealTimeState &rtState);

    void AddGAM(ReferenceT<GAM> element);

    /**
     * The array with the GAM names
     */
    StreamString* functions;

    /**
     * The number of GAMs
     */
    uint32 numberOfFunctions;

    /**
     * The GAM to be executed by this thread.
     */
    ReferenceT<GAM> * GAMs;

    /**
     * The number of GAMs to be executed by this thread
     */
    uint32 numberOfGAMs;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMETHREAD_H_ */

