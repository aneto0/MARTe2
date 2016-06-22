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

ClassMethodsRegistryItem::~ClassMethodsRegistryItem() {
    /*
     * TODO
     */
}

int32 ClassMethodsRegistryItem::FindFunction(const char8 * const name,
                                             int32 minIndex) {

    const char8 *list = functionNames;

    uint32 nameSize = StringHelper::Length(name);
    uint32 listSize = StringHelper::Length(list);

    bool found = false;
    int32 functionIndex = 0;
    while ((listSize > nameSize) && (!found)) {
        //Skipping until nameSize characters forward there is a comma
        uint32 fullTokenSize = 0u;
        const char8 *cursor = list;
        //Matching of function,
        // the string is found if the strings are equal and the the list finishes with a "0" or a ","
        while ((cursor[0] != ',') && (cursor[0] != '\0')) {
            cursor = &cursor[1];
            fullTokenSize++;
        }

        //skip the ')' and the ' ' before the ','
        uint32 tokenSize = fullTokenSize;
        if (fullTokenSize > 0u) {
            tokenSize--;
            while (tokenSize > 0u) {
                if ((list[tokenSize] == ')') || (list[tokenSize] == ' ')) {
                    tokenSize--;
                }
                else {
                    tokenSize++;
                    break;
                }
            }
        }

        //must include the "::" before the class name
        found = (tokenSize >= (nameSize + 2u));
        if (found) {
            cursor = &list[(tokenSize - nameSize) - 2u];
            found = (cursor[0] == ':') && (cursor[1] == ':');
            cursor = &cursor[2];
            if (found) {
                found = (functionIndex >= minIndex) && (StringHelper::CompareN(cursor, name, nameSize) == 0);
            }
        }

        if (!found) {
            functionIndex++;
            if (list[fullTokenSize] != '\0') {
                list = &list[fullTokenSize + 1];
            }
            else {
                list = &list[fullTokenSize];
            }
            listSize = StringHelper::Length(list);
        }
    }
    if (!found) {
        functionIndex = -1;
    }
    return functionIndex;
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

    if (returnValue.NoError()) {
        ClassMethodInterfaceMapper * fmp = NULL_PTR(ClassMethodInterfaceMapper *);
        int32 minIndex = 0;
        int32 functionIndex = 0;
        while (functionIndex >= 0) {
            returnValue = true;
            functionIndex = FindFunction(name, minIndex);
            if (functionIndex >= 0) {
                fmp = &functionTable[functionIndex];
            }
            else {
                returnValue.unsupportedFeature = true;
            }

            if (returnValue.NoError()) {
                /*lint -e{613} .The NULL checking has been done before entering here*/
                returnValue = fmp->Call(context);
                if (returnValue.unsupportedFeature == true) {
                    // allow function overload, try again to search!!
                    minIndex = functionIndex + 1;
                }
                else {
                    //the function has been executed.. exit
                    functionIndex = -1;
                }
            }
        }
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

