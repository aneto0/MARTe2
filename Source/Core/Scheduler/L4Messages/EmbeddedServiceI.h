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
 * Interface to a family of objects that allow interfacing a class method to a thread or a pool of threads
 * The method interface is specified in EmbeddedServiceI::MethodBinderT<class>::MethodPointer.
 * It returns an ErrorType and has one parameter of type EmbeddedServiceI::ExecutionInfo&
 */
class EmbeddedServiceI{
public:
    // start of a thread execution sequence
    static const uint8  startupStage         = 0;

    // main part of a sequence - looping unless an error or completed is returned
    static const uint8  mainStage            = 1;

    // normal termination stage - following the end of the mainStage
    static const uint8  terminationStage     = 2;

    // bad termination stage - after an error returned by the user code or following the Stop()
    static const uint8  badTerminationStage  = 3;

    // after a kill - called by the killing task
    static const uint8  asyncTerminationStage= 4;

    // sub-states of mainStage
    // set when stage is startupStage or terminationStage
    static const uint8  nullStage2           = 0;

    // for client&server model - wait for service request
    static const uint8  waitRequestStage2    = 1;

    // for client&server model - servicing the client
    static const uint8  serviceRequestStage2 = 2;

    /**
     * I communicates to the user code the stage of the thread life, which evolves according to rules specific to the EmbeddedServiceI derived class
     * In addition to stage, it provides a stage2 which is fully custom to the derived class.
     * threadNumber is specific to the user code and allows
     */
    /*lint ++flb*/
    class ExecutionInfo{

    public:
        /**
         * TODO
         */
        ExecutionInfo();

        /**
         * TODO
         */
        void SetThreadNumber(uint16 number);

        /**
         * TODO
         */
        void SetStage(uint8 number);

        /**
         * TODO
         */
        void SetStage2(uint8 number);

        /**
         * TODO
         */
        uint16 GetThreadNumber();

        /**
         * TODO
         */
        uint8 GetStage();

        /**
         * TODO
         */
        uint8 GetStage2();

        /**
         * set all to 0
         */
        void Reset();

    private:
        union {

            /**
             * threadNumber is an unique id that identifies a thread within an EmbeddedService
             * threadNumber is initialised by the user routine during the startupStage
             * it is set to 0 and at the return it can be changed to any value meaningful to the user.
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
//            uint32 formatAsUint32;
        };
    };
    /*lint --flb*/

    /**
     * Interface to the Method Binders.
     * Not usable
     */
    class MethodBinderI{

    public:
        /**
         * TODO
         */
        MethodBinderI(){};

        /**
         * TODO
         */
        virtual ~MethodBinderI(){};

        /**
         *
         */
        virtual ErrorManagement::ErrorType Execute(EmbeddedServiceI::ExecutionInfo info)=0;

    };

    /**
     * TODO
     */
    template <typename className>
    class MethodBinderT: public MethodBinderI{

    public:

        /**
         * @brief Type definition for the method pointer prototype
         */
        typedef ErrorManagement::ErrorType  (className::*MethodPointer)(EmbeddedServiceI::ExecutionInfo &info);

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
    EmbeddedServiceI(MethodBinderI &binder):method(binder) {  }

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

    MethodBinderI &method;
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
	
