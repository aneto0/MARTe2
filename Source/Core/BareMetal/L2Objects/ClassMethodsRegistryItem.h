/**
 * @file ClassMethodsRegistryItem.h
 * @brief Header file for class ClassMethodsRegistryItem
 * @date Apr 7, 2016
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

 * @details This header file contains the declaration of the class ClassMethodsRegistryItem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSMETHODSREGISTRYITEM_H_
#define CLASSMETHODSREGISTRYITEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "LinkedListable.h"
#include "ClassMethodInterfaceMapper.h"
#include "CString.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {



class ClassRegistryItem;
/**
 * TODO
 * */
class ClassMethodsRegistryItem: public LinkedListable {

public:
    /**
     * TODO
     * */
    ClassMethodsRegistryItem(ClassRegistryItem *cri,
                             ClassMethodInterfaceMapper * const functionTable_In,
                             const char *functionNames_In);

    /**
     * TODO
     */
    virtual ~ClassMethodsRegistryItem();

    /**
     * TODO
     * */
    template <typename argType>
    ErrorManagement::ErrorType CallFunction(Object * context,
                            const char8 *name,
                            argType &ref);

private:

    /**
     * TODO
     * */
    int Find(const char8 *name);
    /**
     * TODO
     * */
    ClassMethodInterfaceMapper *FindFunction(const char8 *name);

    /**
     * TODO
     * */
    ClassMethodInterfaceMapper * const functionTable;
    /**
     * TODO
     * */
    CCString functionNames;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * TODO
 * */
template <typename argType>
ErrorManagement::ErrorType ClassMethodsRegistryItem::CallFunction(Object * context,
                                                                  const char8 *name,
                                                                  argType &ref) {
    ErrorManagement::ErrorType returnValue;

    if (context == NULL) {
        returnValue.parametersError = true;
    }
    if (name == NULL) {
        returnValue.parametersError = true;
    }

    ClassMethodInterfaceMapper * fmp = NULL;
    if (returnValue.NoError()) {
        fmp = FindFunction(name);
        if (fmp == NULL) {
            returnValue.unsupportedFeature = true;

        }
    }

    if (returnValue.NoError()) {
        returnValue = fmp->Call(context, ref);
    }

    return returnValue;
}



#define CLASS_METHOD_REGISTER(C,...)\
    static MARTe::ClassMethodInterfaceMapper C ## __ClassMethodsInterfaceMapper[] = {__VA_ARGS__}; \
    static /*const*/ MARTe::ClassMethodsRegistryItem C ## __ClassMethodsRegistryItem(C::GetClassRegistryItem_Static(),C ## __ClassMethodsInterfaceMapper,#__VA_ARGS__);
}
#endif /* CLASSMETHODSREGISTRYITEM_H_ */

