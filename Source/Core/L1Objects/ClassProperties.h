/**
 * @file ClassProperties.h
 * @brief Header file for class ClassProperties
 * @date 05/08/2015
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

 * @details This header file contains the declaration of the class ClassProperties
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_CLASSPROPERTIES_H_
#define SOURCE_CORE_L1OBJECTS_CLASSPROPERTIES_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "../L0Portability/HeapInterface.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Properties of a framework base class.
 * @details Provides introspection information about any framework base class (i.e. one that inherits from Object).
 *
 * This information is automatically computed by the framework at initialisation time (see ClassRegistryDatabase).
 */
class ClassProperties {
public:

    /**
     * Default constructor. Initialises all members to NULL.
     */
    ClassProperties();

    /**
     * @brief Constructor. Initialises all members as per input variables.
     * @param[in] cName the name of the class.
     * @param[in] cVersion the version of the class.
     */
    ClassProperties(const char8 * const cName, const char8 * const cVersion);

    /**
     * @brief Returns the name of the class.
     * @return the name of the class.
     */
    const char8 *GetName() const;

    /**
     * @brief Returns the version of the class against which the code was compiled.
     * @return the version of the class against which the code was compiled.
     */
    const char8 *GetVersion() const;

private:
    /**
     * The name of the class.
     */
    const char8 *className;

    /**
     * The version of the class.
     */
    const char8 *classVersion;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_CLASSPROPERTIES_H_ */

