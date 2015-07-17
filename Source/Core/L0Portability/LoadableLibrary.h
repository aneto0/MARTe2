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


class LoadableLibrary;
extern "C" {
/**
 * @brief Open dinamically a library passing its name.
 * @param ll is the opened library in return.
 * @param dllName is the name of the library to open.
 * @return true if the library is opened correctly, false otherwise. */
bool LoadableLibraryOpen(LoadableLibrary &ll, const char *dllName);

/**
 * @brief Close the library passed by argument.
 * @param ll is the library to close. */
void LoadableLibraryClose(LoadableLibrary &ll);

/**
 * @brief Return the pointer of the requested function of ll library.
 * @param ll is the library in which to extract the function.
 * @param name is the name of the requested function.
 * @return the pointer to the requested function in ll library, NULL otherwise. */
void *LoadableLibraryFunction(LoadableLibrary &ll, const char *name);
}


/** 
 * @brief This functions allows to load dinamically a library and its functions.
 *
 * Most of the implementation is delegated to LoadableLibraryOS.h which is different
 * for each operating systems and contains the non portable system calls to load libraries.
 */ 


class LoadableLibrary {
public:
    HANDLE module;

    friend bool LoadableLibraryOpen(LoadableLibrary &ll, const char *dllName);
    friend void LoadableLibraryClose(LoadableLibrary &ll);
    friend void *LoadableLibraryFunction(LoadableLibrary &ll, const char *name);

public:
    /** @brief Constructor.*/
    LoadableLibrary() {
        module = NULL;
    }

    /** @brief Destructor.*/
    ~LoadableLibrary() {
    }

    /**
     * @brief Open dinamically a library.
     * The function uses OS functions to load a dynamic loadable library.
     * The VxWorks implementation however, does not provide a reliable method
     * to unload a module so this function is implemented in a different way.
     * @see LoadableLibraryOpen(). */
    bool Open(const char *dllName) {
        return LoadableLibraryOpen(*this, dllName);
    }

    /**
     * @brief Close a library previously opened dinamically.
     * The function uses OS functions to unload a dynamic loadable library.
     * @see LoadableLibraryClose(). */
    void Close() {
        return LoadableLibraryClose(*this);
    }

    /**
     * @brief Returns the requested function. 
     * Returns the entry point of the module function specified by name.
     * @see LoadableLibraryFunction(). */
    void *Function(const char *name) {
        if (name == NULL) {
            return NULL;
        }
        return LoadableLibraryFunction(*this, name);
    }

    /** 
     * @brief Operator to make easier the function calls.
     * Returns the entry point of the module using the [] syntax.
     * @see LoadableLibraryFunction(). */
    void *operator[](const char *name) {
        return Function(name);
    }

};

#endif
