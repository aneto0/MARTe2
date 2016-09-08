/**
 * @file CLASSMETHODREGISTER.h
 * @brief Header file for class CLASSMETHODREGISTER
 * @date 20/07/2016
 * @author Ivan Herrero
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
 *
 * @details This header file contains the declaration of the class CLASSMETHODREGISTER
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASS_METHOD_REGISTER_H_
#define CLASS_METHOD_REGISTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ClassMethodInterfaceMapper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Helper class used only by the macro CLASS_METHOD_REGISTER to register a ClassMethodInterfaceMapper instance in the provided ClassRegistryItem.
 */
class ClassMethodInterfaceMapperLoader {
public:
    /**
     * @brief Register a ClassMethodInterfaceMapper in a ClassRegistryItem and sets the name in the ClassMethodInterfaceMapper to methodName.
     * @param[in] className the name of the class with the method to register.
     * @param[in] methodName the name of the method to register.
     * @param[in] classRegistryItem the ClassRegistryItem associated this \a className.
     * @param[in] mapper the ClassMethodInterfaceMapper where this method is registered.
     */
    ClassMethodInterfaceMapperLoader(CCString className,
                                     CCString methodName,
                                     ClassRegistryItem *classRegistryItem,
                                     ClassMethodInterfaceMapper *mapper) {
        if ((mapper != NULL) && (classRegistryItem != NULL)) {
            classRegistryItem->AddMethod(mapper);
            mapper->SetMethodName(methodName);
        }
    }
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * This macro has to be inserted in the unit file of the class with the method to register.
 * The methodName of the className will be registered in the classRegistryItem associated to this className
 */
#define CLASS_METHOD_REGISTER(className,methodName)\
    static MARTe::ClassMethodInterfaceMapperLoader className ## methodName ## MethodLoader(#className,#methodName,className ::GetClassRegistryItem_Static(),new MARTe::ClassMethodInterfaceMapper(& className :: methodName));

#endif /* CLASS_METHOD_REGISTER_H_ */

