/**
 * @file Object.h
 * @brief Header file for class Object
 * @date 04/08/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class Object
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_OBJECT_H_
#define SOURCE_CORE_L1OBJECTS_OBJECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "StructuredData.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief The root object of the framework.
 *
 * @details The MARTe Object is a class which offers the following functionality:
 *  - Its life cycle is managed by a smart pointer mechanism (@see Reference);
 *  - Can be automatically constructed in runtime by the class name of its derived class;
 *  - Provides a standard initialisation/construction interface;
 *  - Is introspectable and enables reflection of derived classes;
 *  - The allocation heap can be selected by the end-user.
 */
class Object {
public:
    /**
     * Virtual destructor. NOOP.
     */
    virtual ~Object() {

    }

    /**
     * @brief Returns the name of the class.
     * @return the name of the class.
     */
    const char8* ClassName() const;

    /**
     * @brief Returns the version of the class against which the code was compiled.
     * @return the version of the class against which the code was compiled.
     */
    const char8* ClassVersion() const;

    /**
     * @brief Initialises the object against a structured list of elements.
     * @details Upon a successful instantiation of the Object the framework will
     * call the Initialise method. The Object instance is then responsible for
     * retrieving the initialisation data from the input data and of assigning
     * these value to its internal variables.
     * @param data the input initialisation data.
     * @return true if all the input data is valid and can be successfully assigned
     * to the member variables.
     */
    virtual bool Initialise(const StructuredData &data) = 0;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


#endif /* SOURCE_CORE_L1OBJECTS_OBJECT_H_ */

