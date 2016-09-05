/**
 * @file EmbeddedService.h
 * @brief Header file for class EmbeddedService
 * @date Sep 1, 2016
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

 * @details This header file contains the declaration of the class EmbeddedService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4MESSAGES_EMBEDDEDSERVICE_H_
#define L4MESSAGES_EMBEDDEDSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BitRange.h"
#include "BitBoolean.h"
#include "ErrorType.h"
#include "StructuredDataI.h"

namespace MARTe{


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 *
 */
class EmbeddedServiceI{
public:
    static const uint8  startupStage     = 0;
    static const uint8  mainStage        = 1;
    static const uint8  terminationStage = 2;

    // sub-states of mainStage
    // set when stage is startupStage or terminationStage
    static const uint8  nullStage2           = 0;
    static const uint8  waitRequestStage2    = 1;
    static const uint8  serviceRequestStage2 = 2;

    /**
     * EmbeddedServiceI::Info
     * to be inherited and extended by derivatives
     */
    /*lint ++flb*/
    union ExecutionInfo{

        /**
         * threadNumber is an unique id that identifies a thread within an EmbeddedService
         * threadNumber is 0 for a single thread
         * for dynamically created/destroyed threads thread-number is given sequentially up a the next multiple of 32 where max threads fits
         */
        BitRange<uint32, 16u ,0u> threadNumber;


        /**
         * The operating stage of the thread.
         * There are 3 main stages and 64 substages which are custom defined
         */
        BitRange<uint32, 8u ,16u> stage;


        /**
         * The operating stage of the thread.
         * There are 3 main stages and 64 substages which are custom defined
         */
        BitRange<uint32, 8u ,24u> stage2;

        /**
         * To set the Message mode using an 8-bit integer.
         */
        uint32 format_as_uint32;
    };
    /*lint --flb*/

    /**
     *
     */
    class MethodBinder{

    public:
        /**
         * TODO
         */
        MethodBinder(){};

        /**
         * TODO
         */
        virtual ~MethodBinder(){};

        /**
         *
         */
        virtual ErrorManagement::ErrorType Execute(EmbeddedServiceI::ExecutionInfo info)=0;

    };

    /**
     * TODO
     */
    template <typename className>
    class MethodBinderT: public MethodBinder{

    public:

        /**
         * @brief Type definition for the method pointer prototype
         */
        typedef ErrorManagement::ErrorType  (className::*MethodPointer)(EmbeddedServiceI::ExecutionInfo info);

        /**
         * TODO
         */
        MethodBinderT(className &o, MethodPointer f);

        /**
         * TODO
         */
        virtual ~MethodBinderT();

        /**
         * TODO
         */
        virtual ErrorManagement::ErrorType Execute(EmbeddedServiceI::ExecutionInfo info);

    private:

        /**
         * TODO
         */
        className &object;

        /**
         * TODO
         */
        MethodPointer function;
    };


public:

    /**
     * allocated by the user.
     * memory managed by object
     */
    EmbeddedServiceI(MethodBinder &binder):method(binder) {  }

    /**
     * allocated by the user.
     * memory managed by object
     */
    template <typename className>
    EmbeddedServiceI(MethodBinderT<className> &binder):method(binder) {  }

    /**
     *
     */
    virtual ~EmbeddedServiceI(){    }

    /**
    * TODO
    * same as object interface
    */
    virtual bool Initialise(StructuredDataI &data)=0;

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Start()=0;

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Stop()=0;

protected:
    /**
     * TODO
     */
    inline ErrorManagement::ErrorType Execute(ExecutionInfo information){
        return method.Execute(information);
    }

    MethodBinder &method;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/



template <typename className>
EmbeddedServiceI::MethodBinderT<className>::MethodBinderT(className &o, MethodBinderT< className>::MethodPointer f):    object(o),    function(f){};

template <typename className>
EmbeddedServiceI::MethodBinderT<className>::~MethodBinderT(){}

template <typename className>
ErrorManagement::ErrorType EmbeddedServiceI::MethodBinderT< className>::Execute(EmbeddedServiceI::ExecutionInfo info){
    return (object.*function)(info);
}


}

#endif /* L4MESSAGES_EMBEDDEDSERVICE_H_ */
	
