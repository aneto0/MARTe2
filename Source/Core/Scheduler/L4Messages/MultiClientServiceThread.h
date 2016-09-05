/**
 * @file MultiClientServiceThread.h
 * @brief Header file for class MultiClientServiceThread
 * @date Sep 5, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class MultiClientServiceThread
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4MESSAGES_MULTICLIENTSERVICETHREAD_H_
#define L4MESSAGES_MULTICLIENTSERVICETHREAD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedThread.h"
#include "MultiThreadService.h"

namespace MARTe{

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class MultiClientServiceThread: public Object,public EmbeddedThread {

public:

    /**
     * TODO
     */
    MultiClientServiceThread(MethodBinder &binder,MultiThreadService &managerIn);

    /**
     * TODO
     */
    template <typename className>
    MultiClientServiceThread(MethodBinderT<className> &binder,MultiThreadService &managerIn);

    /**
     *
     */
    virtual ~MultiClientServiceThread();

    /**
     * TODO
     * Public to be accessed by the thread launcher subroutine
     */
    virtual void ThreadLoop();


private:
    /**
     *
     */
    MultiThreadService &manager;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <typename className>
MultiClientServiceThread::MultiClientServiceThread(MethodBinderT<className> &binder,MultiThreadService &managerIn): EmbeddedThread(binder),manager(managerIn) {}

}
#endif /* L4MESSAGES_MULTICLIENTSERVICETHREAD_H_ */
	
