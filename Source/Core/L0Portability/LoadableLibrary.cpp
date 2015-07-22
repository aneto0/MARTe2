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
#include "LoadableLibrary.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,LoadableLibraryOS.h)

HANDLE LoadableLibrary::GetModule( void ) {
    return module;
}

void LoadableLibrary::SetModule( HANDLE const m ) {
    module = m;
}

bool LoadableLibrary::Open(
                         char8 const * const dllName) {
    return LoadableLibraryOSOpen(*this, dllName);
}

void LoadableLibrary::Close(void) {
    LoadableLibraryOSClose(*this);
}

void *LoadableLibrary::Function(
                              char8 const * const name) {
    void* ret = static_cast<void*>(NULL);

    if (name != NULL) {
        ret = LoadableLibraryOSFunction(*this, name);
    }
    return ret;}
