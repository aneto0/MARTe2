/**
 * @file LoadableLibrary.h
 * @brief Header file for class LoadableLibrary
 * @date 22/07/2015
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

 * @details This header file contains the declaration of the class LoadableLibrary
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOADABLE_LIBRARY_H
#define LOADABLE_LIBRARY_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/** 
 * @brief This functions allows to load dinamically a library and its functions.
 * Most of the implementation is delegated to LoadableLibraryOS.h which is different
 * for each operating systems and contains the non portable system calls to load libraries.
 */
class LoadableLibrary {

public:

    /**
     * @brief Default constructor
     * @post GetModule == NULL
     */
    inline LoadableLibrary();

    /**
     * @brief Destructor
     */
    inline ~LoadableLibrary();

    /**
     * @brief Opens a dynamic library
     * @param[in] dllName is the name of the library to open.
     * @return true if the library is opened correctly, false otherwise
     */
    bool Open(char8 const * const dllName);

    /**
     * @brief Closes the dynamic library currently open
     */
    void Close();

    /**
     * @brief Returns the requested function
     * @param[in] name is the name of the requested function
     * @return the pointer to the requested function in the dynamic
     * library currently open, NULL otherwise
     */
    void *Function(char8 const * const name);

    /** 
     * @brief Returns the requested function
     * @details It does the same that the Function method, but with
     * square brackets syntax
     * @param[in] name is the name of the requested function
     * @return the pointer to the requested function in the dynamic
     * library currently open, NULL otherwise
     */
    inline void *operator[](char8 const * const name);

    /**
     * @brief Gets the module handle
     * @return a copy of the module handle
     */
    HANDLE GetModule();

    /**
     * @brief Sets the module handle
     * @param[in] m The module handle
     */
    void SetModule(HANDLE const m);

private:

    /**
     * Module's handle for the dynamic library
     */
    HANDLE module;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

LoadableLibrary::LoadableLibrary() {
    module = static_cast<HANDLE>(NULL);
}

LoadableLibrary::~LoadableLibrary() {
    module = static_cast<HANDLE>(NULL);
}

void *LoadableLibrary::operator[](char8 const * const name) {
    return Function(name);
}

#endif
