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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {




#define REMOTE_FUNCTION_REGISTER(myClass,...)\
typedef void (myClass::*RPCFun)(ReferenceContainer &ref);\
virtual  bool RemoteFunction(const Message &message){\
    const char8 * function = message.GetFunctionName();\
    static const RPCFun myClass ## RPCFtable[] = {__VA_ARGS__};\
    int index = 0;\
    static StreamString names = #__VA_ARGS__;\
    StreamString token;\
    while (names.GetToken(token,",")){\
        if (token == function){\
            return RPCFtable[index](message);\
        }\
        token.SetSize(0);\
        index++;\
    }\
    return false;\
}\

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
	
