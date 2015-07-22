/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the 
 Licence.
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id: $
 *
 **/
/**
 * @file
 * @brief Implements all the required functions to load and unload libraries
 */
#ifndef LOADABLE_LIBRARY_H
#define LOADABLE_LIBRARY_H

#include "GeneralDefinitions.h"


/** 
 * @brief This functions allows to load dinamically a library and its functions.
 *
 * Most of the implementation is delegated to LoadableLibraryOS.h which is different
 * for each operating systems and contains the non portable system calls to load libraries.
 */

class LoadableLibrary {
private:
    HANDLE module;

public:

    /** @brief Constructor.*/
    LoadableLibrary() {
        module = static_cast<HANDLE>(NULL);
    }

    /** @brief Destructor.*/
    ~LoadableLibrary() {
        module = static_cast<HANDLE>(NULL);
    }

    /**
     * @brief Open dinamically a library.
     * The function uses OS functions to load a dynamic loadable library.
     * The VxWorks implementation however, does not provide a reliable method
     * to unload a module so this function is implemented in a different way.
     * @see LoadableLibraryOpen(). */
    bool Open(char8 const * const dllName);

    /**
     * @brief Close a library previously opened dinamically.
     * The function uses OS functions to unload a dynamic loadable library.
     * @see LoadableLibraryClose(). */
    void Close();

    /**
     * @brief Returns the requested function. 
     * Returns the entry point of the module function specified by name.
     * @see LoadableLibraryFunction(). */
    void *Function(char8 const * const name);

    /** 
     * @brief Operator to make easier the function calls.
     * Returns the entry point of the module using the [] syntax.
     * @see LoadableLibraryFunction(). */
    void *operator[](char8 const * const name) {
        return Function(name);
    }

    /**
     * @brief Getter for module.
     * @return a copy of the module variable. */
    HANDLE GetModule( void );

    /**
     * @brief Getter for module.
     * @return a copy of the module variable. */
    void SetModule( HANDLE const m );
};

#endif
