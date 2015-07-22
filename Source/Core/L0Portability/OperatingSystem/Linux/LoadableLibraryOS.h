/**
 * @file LoadableLibraryOS.h
 * @brief Header file for class LoadableLibraryOS
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

 * @details This header file contains the declaration of the class LoadableLibraryOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOADABLE_LIBRARY_OS_H
#define LOADABLE_LIBRARY_OS_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <dlfcn.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief System dependent implementation of functions for loading dynamic libraries.
 */
class LoadableLibraryOS {

public:

    /**
     * @see LoadableLibrary::Close
     * @brief Close a library previously opened dinamically.
     * @param ll is the library to close.
     */
    static void Close(LoadableLibrary &ll) {
        HANDLE m = ll.GetModule();
        if (m != 0) {
            dlclose(m);
        }
    }

    /**
     * @see LoadableLibrary::Open
     * @brief Open a library dinamically.
     * @param ll is the library in return.
     * @param dllName is the name of the library.
     * @return true if the requested library is found and opened.
     */
    static bool Open(LoadableLibrary &ll,
                               char8 const * const dllName) {
        HANDLE m = ll.GetModule();
        if (m != 0) {
            Close(ll);
        }

        m = dlopen(dllName, RTLD_NOW | RTLD_GLOBAL);
        if (m == NULL) {
            return false;
        }
        ll.SetModule(m);
        return true;
    }

    /**
     * @see LoadableLibrary::Function.
     * @brief Load a function of a library dinamically.
     * @param ll is the library where the requested function is.
     * @param name is the name of the function.
     * @return a pointer to the requested function.
     */
    static void *Function(LoadableLibrary &ll,
                                    char8 const * const name) {
        HANDLE m = ll.GetModule();
        if ((m == NULL) || (name == NULL)) {
            return NULL;
        }
        return dlsym(m, name);
    }

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif
