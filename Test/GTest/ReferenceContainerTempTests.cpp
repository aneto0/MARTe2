/**
 * @file ReferenceContainerTempTests.cpp
 * @brief Source file for class ReferenceContainerTempTests
 * @date 12 Aug 2015
 * @author andre
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
 * the class ReferenceContainerTempTests (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"

void PrintTree(ReferenceT<ReferenceContainer> rc, uint32 m){
    uint32 i=0;
    //printf("[%p]    ", rc.operator ->());
    printf("\n");
    for(i=0; i<rc->Size(); i++){
        uint32 j;
        for(j=0; j<m; j++){
            printf("               ");
        }
        printf("[%p]    ", rc->Get(i).operator ->());
    }
    for(i=0; i<rc->Size(); i++){
        ReferenceT<ReferenceContainer> rcc = rc->Get(i);
        if(rcc.IsValid()){
            PrintTree(rcc, i);
        }
    }

}

int main(int argc,
         char **argv) {
    printf("STARTING!!!\n");
    Heap h;
    ReferenceT<ReferenceContainer> A("ReferenceContainer", h);
    ReferenceT<Object> B("Object", h);
    ReferenceT<ReferenceContainer> C("ReferenceContainer", h);
    ReferenceT<ReferenceContainer> D("ReferenceContainer", h);
    ReferenceT<Object> E("Object", h);
    ReferenceT<ReferenceContainer> F("ReferenceContainer", h);
    ReferenceT<ReferenceContainer> G("ReferenceContainer", h);
    ReferenceT<Object> H("Object", h);
    ReferenceT<Object> I("Object", h);
    ReferenceT<Object> J("Object", h);

    F->Insert(G);
    F->Insert(H);
    F->Insert(I);

    D->Insert(F);
    D->Insert(E);

    C->Insert(E);

    A->Insert(B);
    A->Insert(C);
    A->Insert(D);

    printf("A @ %p\n", A.operator ->());
    printf("B @ %p\n", B.operator ->());
    printf("C @ %p\n", C.operator ->());
    printf("D @ %p\n", D.operator ->());
    printf("E @ %p\n", E.operator ->());
    printf("F @ %p\n", F.operator ->());
    printf("G @ %p\n", G.operator ->());
    printf("H @ %p\n", H.operator ->());
    printf("I @ %p\n", I.operator ->());

    PrintTree(A, 0);
    ReferenceContainer results;
    ReferenceContainerFilterReferences referencesFilter(E);
    ReferenceContainerSearchMode mode(2, ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::RECURSIVE);
    A->Find(results, referencesFilter, mode);
    printf("results.Size = %d\n", results.Size());
    uint32 i = 0;

    for (i = 0; i < results.Size(); i++) {
        printf("Found : %p\n", results.Get(i).operator ->());
    }

    PrintTree(A, 0);

    return 0;
}

