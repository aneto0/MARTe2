/**
 * @file RemoteFunctionInterface.h
 * @brief Header file for class RemoteFunctionInterface
 * @date Apr 5, 2016
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

 * @details This header file contains the declaration of the class RemoteFunctionInterface
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REMOTEFUNCTIONINTERFACE_H_
#define REMOTEFUNCTIONINTERFACE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Message.h"
#include "Object.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


#if 0
/*
 * TODO
 */
class FunctionReturn{
public:
    /*
     * TODO
     */
    FunctionReturn(){
        functionFound = false;
        functionReturn = false;
        prototypeMismatch = true;
    }

    /*
     * TODO
     */
    bool FunctionFound(){
        return functionFound;
    }

    /*
     * TODO
     */
    bool FunctionReturn(){
        return functionReturn;
    }

    bool functionFound:1;
    bool functionReturn:1;
    bool prototypeMismatch:1;
    int spares:5;
};

/*
 * TODO
 */
template <class C>
class FunctionMapper {

    /*
     * TODO
     */
    enum FunctionMapperModes {
            voidFunction,
            intFunction,
            refContainerFunction
    };
public:
    /*
     * TODO
     */
    FunctionMapper(void (C::*f)(ReferenceContainer &ref)){
        pFun = f;
        mode = refContainerFunction;
    }
    /*
     * TODO
     */
    FunctionMapper(void (C::*f)(int i)){
        pFun = f;
        mode = intFunction;
    }
    /*
     * TODO
     */
    FunctionMapper(void (C::*f)(void)){
        pVFun = f;
        mode = voidFunction;
    }

    /*
     * TODO
     */
    FunctionReturn Call(C * cp){
        FunctionReturn fr;
        fr.functionFound = true;
        if (mode == voidFunction){
            fr.prototypeMismatch = false;
            fr.functionReturn = (cp->*pVFun)();
        }
        return fr;
    }

    /*
     * TODO
     */
    FunctionReturn Call(C * cp,int i){
        FunctionReturn fr;
        fr.functionFound = true;
        if (mode == intFunction){
            fr.prototypeMismatch = false;
            fr.functionReturn = (cp->*pFun)(i);
        }
        return fr;
    }

    /*
     * TODO
     */
    FunctionReturn Call(C * cp,ReferenceContainer &ref){
        FunctionReturn fr;
        fr.functionFound = true;
        if (mode == refContainerFunction){
            fr.prototypeMismatch = false;
            fr.functionReturn = (cp->*pRFun)(ref);
        }
        return fr;
    }
private:
    /*
     * TODO
     */
    union {
        void (C::*pFun) (int i);
        void (C::*pVFun) (void);
        void (C::*pRFun) (ReferenceContainer &ref);
    };
    FunctionMapperModes mode;

};

/*
 * TODO
 */
class ClassMethodRecord: public LinkedListable{

public:
    /*
     * TODO
     */
    virtual bool CallFunction(const char *name, ReferenceContainer &ref)= 0;
};


/*
 * TODO
 */
template <class C>
class ClassMethodRecordT: public ClassMethodRecord{
    static const FunctionMapper<C> FunctionTable[];
    static const char8 *FunctionNames;
public:
    /*
     * TODO
     */
    ClassMethodRecordT(){
        ClassRegistryItem *cri = C::GetClassRegistryItem();
        if (cri != NULL)cri->RegisterMethods(this);
    }
    /*
     * TODO
     */
    virtual FunctionReturn CallFunction(Object * context, const char8 *name, ReferenceContainer &ref){
        if (context == NULL) return false;
        if (name == NULL) return false;

        C *actualContext = dynamic_cast<C> (context);
        if (actualContext == NULL) return false;

        const char8 *names = FunctionNames;

        uint32 nameSize = StringHelper::Length(name);
        uint32 namesSize = StringHelper::Length(names);
        bool notFound = true;
        int functionIndex = 0;
        while ((namesSize > nameSize) && notFound){
            notFound = (StringHelper::CompareN(names,name,nameSize)!=0) ||
                     ((names[nameSize]!= 0) && (names[nameSize]!= ','));
            if (notFound){
                functionIndex++;
                names += nameSize;
                while ((names[0] != 0) && (names[0] != ',')) names++;
                namesSize = StringHelper::Length(names);
            }
        }

        FunctionReturn returnValue;
        if (!notFound){
            returnValue = FunctionTable[functionIndex].Call(actualContext,ref);
        }

        return returnValue;
    }


};

#define REMOTE_FUNCTION_REGISTER(C,...)\
    static const FunctionMapper<C> ClassMethodRecordT<C>::FunctionTable[] = {__VA_ARGS__};\
    static const char8 *ClassMethodRecordT<C>::FunctionNames = #__VA_ARGS__;

#endif

/**
 *
 * */
class RemoteFunctionInterface{

protected:
    //static struct  {
    // function*, const char *
   //  }  * lookupTable; (null terminated vector of structures)
   // virtual function *lookup(functionName)=0;


    /**
     *
     * */
    virtual bool RemoteFunction(const Message &message) =0;


};

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REMOTEFUNCTIONINTERFACE_H_ */
	
