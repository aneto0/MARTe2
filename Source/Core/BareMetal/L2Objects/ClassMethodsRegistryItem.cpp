/**
 * @file ClassMethodsRegistryItem.cpp
 * @brief Source file for class ClassMethodsRegistryItem
 * @date Apr 12, 2016
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

 * @details This source file contains the definition of all the methods for
 * the class ClassMethodsRegistryItem (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodsRegistryItem.h"
#include "StringHelper.h"
#include "ClassRegistryItem.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ClassMethodsRegistryItem::~ClassMethodsRegistryItem() {
    /*
     * TODO
     */
}

/**
 * TODO
 * */
int32 ClassMethodsRegistryItem::Find(const char8 * const name) {
    const char8 *names = functionNames;

    uint32 nameSize = StringHelper::Length(name);
    uint32 namesSize = StringHelper::Length(names);

    bool notFound = true;
    int32 functionIndex = 0;
    while ((namesSize > nameSize) && notFound) {
        //Skipping until nameSize characters forward there is a comma
        while ((names[nameSize] != ',') && (names[0] != '\0')) {
            names = &names[1];
        }

        //Matching of function,
        // the string is found if the strings are equal and the the list finishes with a "0" or a ","
        notFound = (StringHelper::CompareN(names, name, nameSize) != 0);

        if (notFound) {
            functionIndex++;
            names=&names[nameSize];
            namesSize = StringHelper::Length(names);
        }
    }
    if (notFound) {
        functionIndex = -1;
    }
    return functionIndex;
}

/**
 * TODO
 * */
ClassMethodInterfaceMapper *ClassMethodsRegistryItem::FindFunction(const char8 * const name) {
    int32 functionIndex = Find(name);
    ClassMethodInterfaceMapper *fmp = NULL_PTR(ClassMethodInterfaceMapper *);
    if (functionIndex >= 0) {
        fmp = &functionTable[functionIndex];
    }
    return fmp;
}

/**
 * TODO
 * */
ClassMethodsRegistryItem::ClassMethodsRegistryItem(ClassRegistryItem * const cri,
                                                   ClassMethodInterfaceMapper * const functionTable_In,
                                                   const char8 * const functionNames_In) :
        LinkedListable(),
        functionTable(functionTable_In) {
    functionNames = functionNames_In;
    // register in Object the record
    if (cri != NULL) {
        cri->RegisterMethods(this);
    }
}

ErrorManagement::ErrorType ClassMethodsRegistryItem::CallFunction(Object * const context,
                                                                  const char8 * const name) {
    ErrorManagement::ErrorType returnValue;

    if (context == NULL) {
        returnValue.parametersError = true;
    }
    if (name == NULL) {
        returnValue.parametersError = true;
    }

    ClassMethodInterfaceMapper * fmp = NULL_PTR(ClassMethodInterfaceMapper *);
    if (returnValue.NoError()) {
        fmp = FindFunction(name);
        if (fmp == NULL) {
            returnValue.unsupportedFeature = true;

        }
    }

    if (returnValue.NoError()) {
        /*lint -e{613} .The NULL checking has been done before entering here*/
        returnValue = fmp->Call(context);
    }

    return returnValue;
}

}

#if 0

namespace MARTe {

#include "Object.h"
//#if 0
    class Dummy:public Object {

    public:

        CLASS_REGISTER_DECLARATION()

        bool Test(MARTe::ReferenceContainer &ref) {
            return true;
        }

        bool Test2(int32 i) {
            return true;
        }

    };

    ClassMethodInterfaceMapper CMIM(&Dummy::Test);
    ClassMethodInterfaceMapper CMIMS[] = {&Dummy::Test,&Dummy::Test};

    CLASS_METHOD_REGISTER(Dummy,&Dummy::Test)

}
#endif

