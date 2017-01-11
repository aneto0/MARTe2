/**
 * @file ExceptionHandler.h
 * @brief Header file for class ExceptionHandler
 * @date 06/07/2015
 * @author Riccardo Vitelli
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

 * @details This header file contains the declaration of the class ExceptionHandler
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXCEPTIONHANDLER_H_
#define EXCEPTIONHANDLER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Class which represents the action to be executed after an exception
 * has arisen.
 */
class DLL_API ExceptionHandler {

public:

    /**
     * Pass handling back to OS.
     */
    static const uint32 NotHandled = 0x1u;

    /**
     * Handled: program can continue.
     */
    static const uint32 ContinueExec = 0x2u;

    /**
     * Handled: jump to thread start and proceed to termination.
     */
    static const uint32 KillThread = 0x3u;

    /**
     * Handled: jump to thread start and terminate task.
     */
    static const uint32 KillTask = 0x4u;

    /**
     * Handled: jump to thread start and retry.
     */
    static const uint32 TryAgain = 0x5u;

    /**
     * Pass handling back to other handlers in stack.
     */
    static const uint32 TryOther = 0x6u;

    /**
     * Do not report error information (ORed).
     */
    static const uint32 NoReport = 0x10000u;

    /**
     * @brief Default constructor.
     */
    ExceptionHandler();

    /**
     * @brief Constructor initialized with a default action.
     */
    ExceptionHandler(const uint32 &defaultAction);

    /**
     * @brief Destructor
     */
    virtual ~ExceptionHandler();

//    /**
//     * This is the action to be performed.
//     */
//    virtual uint32 Catch(ExceptionInformation &info);

private:

    /**
     * What to do in case of exception.
     */
    uint32 action;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

inline ExceptionHandler::ExceptionHandler() {
    this->action = 0x1u;
}

inline ExceptionHandler::ExceptionHandler(const uint32 &defaultAction) {
    this->action = defaultAction;
}

inline ExceptionHandler::~ExceptionHandler() {
}

//virtual uint32 ExceptionHandler::Catch(ExceptionInformation &info) {
//    return action;
//}

}

#endif /* EXCEPTIONHANDLER_H_ */
